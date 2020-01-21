#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inc/archivehandler.h"


bool isArchive(const QString &filename) {
    QStringList splitted_name = filename.split('.');
    if (splitted_name.length() < 2) {
        return false;
    }
    const QString extension = splitted_name.back();
    QStringList supported_formats = {"tar", "pax", "cpio", "ISO9660", "zip", "mtree",
                                     "shar", "ar", "empty", "raw", "xar", "lha", "lzh",
                                     "rar", "7-Zip", "WARC"
    };
    return supported_formats.contains(extension);
}


int compressFiles(const QList<QPair<QString, QString>>& selectedFiles, const QString& outname, int compress){

    struct archive *a;
    struct archive_entry *entry;
    ssize_t len;
    int fd;

    static char buff[16384];


    const std::string filename = outname.toStdString();

    a = archive_write_new();
    switch (compress) {
    case 'j': case 'y':
        archive_write_add_filter_bzip2(a);
        break;
    case 'Z':
        archive_write_add_filter_compress(a);
        break;
    case 'z':
        archive_write_add_filter_gzip(a);
        break;
    default:
        archive_write_add_filter_none(a);
        break;
    }

    archive_write_set_format_ustar(a);
    archive_write_open_filename(a, filename.c_str());
    std::string curr;
    foreach (auto paths, selectedFiles) {

        curr = paths.first.toStdString();

        struct archive *disk = archive_read_disk_new();
        archive_read_disk_set_standard_lookup(disk);
        int r;

        r = archive_read_disk_open(disk, curr.c_str());
        if (r != ARCHIVE_OK) {
            qDebug() << archive_error_string(disk);
            return 1;
        }

        while(true) {
            entry = archive_entry_new();

            archive_entry_set_pathname(entry, paths.second.toStdString().c_str());

            r = archive_read_next_header2(disk, entry);
            if (r == ARCHIVE_EOF)
                break;
            if (r != ARCHIVE_OK) {
                qDebug() << archive_error_string(disk);
                return 1;
            }
            archive_read_disk_descend(disk);
            archive_entry_set_pathname(entry, paths.second.toStdString().c_str());

            r = archive_write_header(a, entry);
            if (r == ARCHIVE_FATAL)
                return 1;
            if (r > ARCHIVE_FAILED) {
                fd = open(archive_entry_sourcepath(entry), O_RDONLY);
                len = read(fd, buff, sizeof(buff));
                while (len > 0) {
                    archive_write_data(a, buff, len);
                    len = read(fd, buff, sizeof(buff));
                }
                close(fd);
            }
            archive_entry_free(entry);
        }

        archive_read_close(disk);
        archive_read_free(disk);
    }
    archive_write_close(a);
    archive_write_free(a);
    return 0;
}


Archive::Archive(const QString& filename) {
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    if (archive_read_open_filename(a, filename.toStdString().c_str(), BLOCK_SIZE) != ARCHIVE_OK)
        qDebug() << "Can't open the archive";
}

Archive::~Archive() {
    archive_read_close(a);
    archive_read_free(a);

}


void Archive::close() {
    archive_entry_free(entry);
    archive_read_free(a);
}


int Archive::extract(const QString& file_to) {
    struct archive *ext;
    int flags;
    int r;

    flags = ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS;

    ext = archive_write_disk_new();
    archive_write_disk_set_options(ext, flags);
    archive_write_disk_set_standard_lookup(ext);
    std::string file_to_str = file_to.toStdString();
    for (;;) {
        r = archive_read_next_header(a, &entry);
        if (r == ARCHIVE_EOF)
            break;
        if (r < ARCHIVE_OK)
            qDebug() << archive_error_string(a);
        if (r < ARCHIVE_WARN) {
            qDebug() << archive_error_string(a);
            return 1;

        }

        const char *currentFile = archive_entry_pathname(entry);
        const std::string fullOutputPath = file_to_str + '/' + currentFile;
        archive_entry_set_pathname(entry, fullOutputPath.c_str());

        r = archive_write_header(ext, entry);
        if (r < ARCHIVE_OK)
            qDebug() << archive_error_string(ext);
        else if (archive_entry_size(entry) > 0) {
            r = copyData(ext);

            if (r < ARCHIVE_OK)
                qDebug() << archive_error_string(ext);
            if (r < ARCHIVE_WARN) {
                qDebug() << "warn";
                return 1;
            }

        }

        r = archive_write_finish_entry(ext);
        if (r < ARCHIVE_OK)
            qDebug() << archive_error_string(ext);
        if (r < ARCHIVE_WARN) {
            qDebug() << archive_error_string(a);
            return 1;
        }

    }
    archive_write_close(ext);
    archive_write_free(ext);
    return 0;
}


int
Archive::copyData(struct archive *aw) {
    int r;
    const void *buff;
    size_t size;
    la_int64_t offset;

    for (;;) {
        r = archive_read_data_block(a, &buff, &size, &offset);

        if (r == ARCHIVE_EOF) {

            return ARCHIVE_OK;
        }
        if (r < ARCHIVE_OK) {
            qDebug() << archive_error_string(a);

            return r;
        }

        r = archive_write_data_block(aw, buff, size, offset);
        if (r < ARCHIVE_OK) {
            qDebug() << archive_error_string(aw);
            return r;
        }

    }
}
