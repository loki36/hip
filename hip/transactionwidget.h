#ifndef TRANSACTIONWIDGET_H
#define TRANSACTIONWIDGET_H

#include <QWidget>
#include <QTableView>

namespace Ui {
class TransactionWidget;
}

class TransactionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TransactionWidget(QWidget *parent = 0);
    ~TransactionWidget();
    QTableView * getTableView();

private:
    Ui::TransactionWidget *ui;
};

#endif // TRANSACTIONWIDGET_H
