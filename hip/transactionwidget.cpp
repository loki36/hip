#include "transactionwidget.h"
#include "ui_transactionwidget.h"

TransactionWidget::TransactionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionWidget)
{
    ui->setupUi(this);
}

TransactionWidget::~TransactionWidget()
{
    delete ui;
}

QTableView *TransactionWidget::getTableView()
{
    return this->ui->tableView;
}
