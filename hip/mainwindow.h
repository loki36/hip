#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRelationalTableModel>
#include <QSqlRelationalDelegate>
#include <QFile>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QCloseEvent>

#include "bank.h"
#include "treeviewmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void resizeEvent(QResizeEvent *event);

public slots:
    bool saveFile();
    void openFile();
    void newFile();
    bool closeFile();
    bool equalFiles(QString f1,QString f2);
    void connectDb(QString fileNameDb,bool newFile=false);
    void upgradeDb();
    void executeQueriesFromFile(QFile *file, QSqlQuery *query);
    void selectionChangedSlot(const QItemSelection & /*newSelection*/, const QItemSelection & /*oldSelection*/);
    void closeEvent(QCloseEvent *event);
    bool saveAsFile();

private:
    Ui::MainWindow *ui;
    Bank *bank;
    TreeViewModel *treeViewModel;
    QSqlDatabase db;
    QString workinDb;
    QString fileNameDb;

};

#endif // MAINWINDOW_H
