#ifndef SORTFILTERMODEL_H
#define SORTFILTERMODEL_H

#include<QSortFilterProxyModel>
#include <QTableView>

class KCCFilesystemModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    SortFilterModel(QObject *parent = 0);

//    QDate filterMinimumFile() const { return minDate; }
    void setFilterMinimumFile(const QDate &date);

//    QDate filterMaximumDate() const { return maxDate; }
    void setFilterMaximumDate(const QDate &date);

protected:
    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    bool lessThan(const QModelIndex &left, const QModelIndex &right) const;

private:
    QString minFile;
    QString maxFile;
};


#endif // SORTFILTERMODEL_H
