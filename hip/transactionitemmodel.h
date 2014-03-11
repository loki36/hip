#ifndef TRANSACTIONITEMMODEL_H
#define TRANSACTIONITEMMODEL_H

//#include "transaction.h"

//#include <QAbstractTableModel>

//class TransactionItemModel : public QAbstractTableModel
//{
//    Q_OBJECT
//public:
//    explicit TransactionItemModel(QObject *parent = 0);
//    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
//    int columnCount(const QModelIndex &parent = QModelIndex()) const;
//    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
//    QMap<int,Transaction> transactions;

//signals:

//public slots:


//};
#include "bank.h"

#include <QStandardItemModel>

class TransactionItemModel : public QStandardItemModel
{
    Q_OBJECT
public:
    explicit TransactionItemModel(QObject *parent = 0);
    void setBank(Bank *bank);
    void updateTable(qint32 keyAccount);

signals:

public slots:

private:
    Bank *bank;


};

#endif // TRANSACTIONITEMMODEL_H
