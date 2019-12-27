#ifndef KCC_ARCHIVEHANDLER_H
#define KCC_ARCHIVEHANDLER_H

#include <archive.h>
#include <archive_entry.h>
#include <QString>
#include <string>

bool isArchive(const QString filename);


class Archive {
private:
    struct archive *a = nullptr;
    struct archive_entry *entry = nullptr;
    static const size_t BLOCK_SIZE = 10240;
public:
    Archive();

    explicit Archive(const QString filename);

    ~Archive();

    void open(const QString filename);

    void close();

    int copyData(struct archive* aw);
    int extract(const QString file_to);


};




#endif //KCC_ARCHIVEHANDLER_H
