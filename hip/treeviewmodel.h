#ifndef TREEVIEWMODEL_H
#define TREEVIEWMODEL_H

#include <QStandardItemModel>
#include <QItemSelectionModel>
#include <QDebug>

#include "account.h"

class TreeViewModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TreeViewModel(QObject *parent = 0);
    QStandardItem *getItem(const QModelIndex &index) const;
    QStandardItem *accountsItem;
    QStandardItem *budgetsItem;
    QStandardItem *reportsItem;

private:
    QMap<int, QStandardItem*> accounts;

signals:

public slots:
  void accountsChanged(Account *account);

};

#endif // TREEVIEWMODEL_H
