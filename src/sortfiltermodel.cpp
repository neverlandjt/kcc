#include "../inc/sortfiltermodel.h"


SortFilterModel::SortFilterModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

bool SortFilterModel::lessThan(const QModelIndex &left,
                                      const QModelIndex &right) const
{
    QVariant leftData = sourceModel()->data(left);
    QVariant rightData = sourceModel()->data(right);

    return QString::localeAwareCompare(leftData.toString(), rightData.toString()) < 0;


}


