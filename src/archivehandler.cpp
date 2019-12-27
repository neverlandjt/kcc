#include "inc/archivehandler.h"
#include <QDebug>

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

Archive::Archive() {
//    a = archive_read_new();
//    archive_read_support_filter_all(a);
//    archive_read_support_format_all(a);
};

Archive::Archive(const QString& filename) {
    a = archive_read_new();
    archive_read_support_filter_all(a);
    archive_read_support_format_all(a);

    if (archive_read_open_filename(a, filename.toStdString().c_str(), BLOCK_SIZE) != ARCHIVE_OK)
        qDebug() << "Can't open the archive";
    qDebug() << "arc name " << filename.toStdString().c_str();
}

Archive::~Archive() {
//    if (archive_read_free(a) != ARCHIVE_OK){
//        exit(1);
//    }
    archive_read_close(a);
    archive_read_free(a);
//    archive_write_close(a);
//    archive_write_free(a);

}

void Archive::open(const QString& filename) {
    if (archive_read_open_filename(a, filename.toStdString().c_str(), BLOCK_SIZE) != ARCHIVE_OK)
        qDebug() << "Can't open the archive";
}

void Archive::close() {
//    archive_entry_free(entry);
//    archive_read_free(a);

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
