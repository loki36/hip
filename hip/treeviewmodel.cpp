#include "treeviewmodel.h"

TreeViewModel::TreeViewModel(QObject *parent) :
    QStandardItemModel(parent)
{
    QStandardItem *rootNode = this->invisibleRootItem();

    //defining a couple of items
    accountsItem = new QStandardItem(tr("Accounts"));
    budgetsItem = new QStandardItem(tr("Budgets"));
    reportsItem = new QStandardItem(tr("Reports"));
    accountsItem->setEditable(false);
    budgetsItem->setEditable(false);
    reportsItem->setEditable(false);

    //building up the hierarchy
    rootNode->appendRow(accountsItem);
    rootNode->appendRow(budgetsItem);
    rootNode->appendRow(reportsItem);
//    rootNode->insertRow(0,accountsItem);
//    rootNode->insertRow(1,budgetsItem);
//    rootNode->insertRow(2,reportsItem);
}

void TreeViewModel::accountsChanged(Account *account)
{
    QStandardItem *accountItem;
    if(!accounts.value(account->getKey(),0)){
        accountItem = new QStandardItem(account->getName());
        accountItem->setData(account->getKey());
        accounts.insert(account->getKey(),accountItem);
        this->accountsItem->appendRow(accountItem);
    }else{
        accountItem = accounts.value(account->getKey(),0);
        accountItem->setText(account->getName());
    }
}

QStandardItem *TreeViewModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        QStandardItem *item = static_cast<QStandardItem*>(index.internalPointer())->child(index.row());
        if (item)
            qDebug() << "item found";
            return item;
    }
    return this->invisibleRootItem();
}
