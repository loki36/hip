#include "transactionitemmodel.h"
#include <QDebug>

//TransactionItemModel::TransactionItemModel(QObject *parent) :
//    QAbstractTableModel(parent)
//{
//}

//int TransactionItemModel::rowCount(const QModelIndex & /*parent*/) const
// {
//    return transactions.size();
// }

// int TransactionItemModel::columnCount(const QModelIndex & /*parent*/) const
// {
//     return 10;
// }

// QVariant TransactionItemModel::data(const QModelIndex &index, int role) const
// {
//     if (role == Qt::DisplayRole)
//     {
////         return QString("Row%1, Column%2")
////                 .arg(index.row() + 1)
////                 .arg(index.column() +1);
//         switch(index.column()){
//           case 0:
//             return transactions.value(index.row()+1).getDate();
//             break;
//         }
//     }
//     return QVariant();
// }
TransactionItemModel::TransactionItemModel(QObject *parent) :
    QStandardItemModel(parent)
{
}

void TransactionItemModel::setBank(Bank *bank)
{
   this->bank = bank;
}

void TransactionItemModel::updateTable(qint32 keyAccount)
{
    QStandardItem *newItem = 0;
    QMapIterator<int, Transaction> i(*bank->getTransactions());
    this->setHorizontalHeaderItem(0,new QStandardItem(tr("Date")));
    this->setHorizontalHeaderItem(1,new QStandardItem(tr("Info")));
    this->setHorizontalHeaderItem(2,new QStandardItem(tr("Description")));
    this->setHorizontalHeaderItem(3,new QStandardItem(tr("Category")));
    this->setHorizontalHeaderItem(4,new QStandardItem(tr("Pay")));
    this->setHorizontalHeaderItem(5,new QStandardItem(tr("Amount")));
    int row=0;
    while (i.hasNext()) {
        i.next();
        if(i.value().getKeyAccount()!=keyAccount){
            //qDebug() << i.key() << " : " << i.value().getKeyAccount() << " " << keyAccount;
            continue;
        }

        //qDebug() << i.key() << " : " << i.value().getKeyAccount() << " " << keyAccount;
         //qDebug() << i.key() << " : " << i.value().getDate().toString("yyyy/MM/dd");
        newItem = new QStandardItem(i.value().getDate().toString("yyyy/MM/dd"));
        this->setItem(row,0,newItem);
        newItem = new QStandardItem(i.value().getInfo());
        this->setItem(row,1,newItem);
        newItem = new QStandardItem(i.value().getDescription());
        this->setItem(row,2,newItem);
        QString subcat = bank->getSubCategory(i.value().getKeySubcategory()).getName();
        QString cat = bank->getCategory(bank->getSubCategory(i.value().getKeySubcategory()).getParent()).getName();
        QString catsubcat = cat + (subcat!=""?":"+ subcat:"");
        newItem = new QStandardItem(catsubcat);
        this->setItem(row,3,newItem);
        newItem = new QStandardItem(bank->getPay(i.value().getKeyPay()).getName());
        this->setItem(row,4,newItem);
        newItem = new QStandardItem(QString::number(i.value().getAmount()));
        newItem->setData(i.value().getAmount(),Qt::DisplayRole);
        this->setItem(row,5,newItem);
//        newItem = new QStandardItem(QString::number(i.value().getKeyAccount()));
//        this->setItem(row,6,newItem);
//        newItem = new QStandardItem(i.value().getDate().toString("yyyy/MM/dd"));
//        this->setItem(row,7,newItem);
//        newItem = new QStandardItem(i.value().getDate().toString("yyyy/MM/dd"));
//        this->setItem(row,8,newItem);
//        newItem = new QStandardItem(i.value().getDate().toString("yyyy/MM/dd"));
//        this->setItem(row,9,newItem);
//        newItem = new QStandardItem(i.value().getDate().toString("yyyy/MM/dd"));
//        this->setItem(row,10,newItem);
        row++;
        //}
    }

//    for(int i = 0; i < this->transactions.count();++i ){
//      newItem = new QStandardItem(this->transactions.value(i).getDate().toString("yyyy/MM/dd"));
//      this->setItem(i-1,0,newItem);
//    }
}

