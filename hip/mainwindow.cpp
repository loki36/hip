#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "transactionwidget.h"
#include "transactionitemmodel.h"

#include <QDebug>
#include <QFileDialog>
#include "xhbreader.h"
#include <QtWebKitWidgets/QWebView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->connect(ui->actionOpen,SIGNAL(triggered()),this, SLOT(openFile()));
    this->connect(ui->actionClose,SIGNAL(triggered()),this, SLOT(closeFile()));
    this->connect(ui->actionNew,SIGNAL(triggered()),this, SLOT(newFile()));
    this->connect(ui->actionSave,SIGNAL(triggered()),this, SLOT(saveFile()));
    this->connect(ui->actionSave_as,SIGNAL(triggered()),this, SLOT(saveAsFile()));

    QWebView *view = new QWebView(this);
    view->load(QUrl("https://www.google.fr/"));
    view->show();
    //ui->tabWidget->addTab(view,"test");
    ui->verticalLayout->addWidget(view);

    // create Model
    treeViewModel = new TreeViewModel();
    //register the model
    ui->treeView->setModel(treeViewModel);
    //ui->treeView->expandAll();
    ui->treeView->expand(treeViewModel->accountsItem->index());
    bank = new Bank(this);
    QObject::connect(bank,&Bank::accountsChanged,treeViewModel,&TreeViewModel::accountsChanged);
    //treeViewModel->accountItem->appendRow(new QStandardItem("Verona"));
    //selection changes shall trigger a slot
         QItemSelectionModel *selectionModel= ui->treeView->selectionModel();
         connect(selectionModel, SIGNAL(selectionChanged (const QItemSelection &, const QItemSelection &)),
                 this, SLOT(selectionChangedSlot(const QItemSelection &, const QItemSelection &)));

    // for test
//         this->db = QSqlDatabase::addDatabase("QSQLITE");
//         this->db.setDatabaseName("../Frank 2013.sqlite");
//         db.open();

//         QSqlQuery query;
//         query.exec("SELECT * FROM hip_account");

//         while (query.next()) {
//                 Account *account = new Account();
//                 account->setKey(query.value(0).toInt());
//                 account->setName(query.value(2).toString());
//                 bank->insertAccount(account);
//                 qDebug() << account->getName();
//         }

}

void MainWindow::closeEvent(QCloseEvent *event)
 {
//     if (maybeSave()) {
//         writeSettings();
//         event->accept();
//     } else {
//         event->ignore();
//     }
    if(this->closeFile()){
        event->accept();
    } else {
        event->ignore();
    }
 }

MainWindow::~MainWindow()
{
    this->closeFile();
    delete ui;
}

//void MainWindow::selectionChangedSlot(const QItemSelection & /*newSelection*/, const QItemSelection & /*oldSelection*/)
// {
////     //get the text of the selected item
////     const QModelIndex index = ui->treeView->selectionModel()->currentIndex();
////     qDebug() << index.row();
////     qDebug() << treeViewModel->item(index.row())->data().toString();
//    //get the text of the selected item
//         const QModelIndex index = ui->treeView->selectionModel()->currentIndex();

////         QString selectedText = index.data(Qt::DisplayRole).toString();
////         //find out the hierarchy level of the selected item
////         int hierarchyLevel=1;
////         QModelIndex seekRoot = index;
////         while(seekRoot.parent() != QModelIndex())
////         {
////             seekRoot = seekRoot.parent();
////             hierarchyLevel++;
////         }
////         QString showString = QString("%1, Level %2").arg(selectedText)
////                              .arg(hierarchyLevel);
////         setWindowTitle(showString);

//         //qDebug() << treeViewModel->item(index.parent().row())->child(index.row())->data();
//         QStandardItem *item = treeViewModel->getItem(index);
//         if(item->parent()){
//           switch(item->parent()->row()){
//             case 0:
//               //qDebug() << item->parent()->data(Qt::DisplayRole).toString();
////               QLayoutItem *layoutItem = NULL;
////                  while ((layoutItem = ui->centralWidget->layout()->takeAt(0)) != 0) {
////                      ui->centralWidget->layout()->removeItem(layoutItem);
////                      delete layoutItem;
////                  }
//               QLayout *layout = ui->horizontalLayout;
//               while(!layout->isEmpty()) {
//                   delete layout->itemAt(0)->widget();
//               }

//               TransactionWidget *transactionWidget = new TransactionWidget(this);
//               layout->addWidget(transactionWidget);
//               TransactionItemModel *transacionModel = new TransactionItemModel(this);
//               transacionModel->setBank(bank);
//               //qDebug() << treeViewModel->item(index.parent().row())->child(index.row())->data().toInt();
//               transacionModel->updateTable(treeViewModel->item(index.parent().row())->child(index.row())->data().toInt());
//               transacionModel->sort(0,Qt::DescendingOrder);
//               transactionWidget->getTableView()->setModel(transacionModel);
//               transactionWidget->getTableView()->resizeColumnsToContents();
//               //transactionWidget->getTableView()->horizontalHeader()->setStretchLastSection(true);
//               break;
//           }
//         }else{

//         }
//         //qDebug() << index.row();
//         //qDebug() << item->data(Qt::DisplayRole).toString();
// }

void MainWindow::selectionChangedSlot(const QItemSelection & /*newSelection*/, const QItemSelection & /*oldSelection*/)
 {
         const QModelIndex index = ui->treeView->selectionModel()->currentIndex();

         QStandardItem *item = treeViewModel->getItem(index);
         if(item->parent()){
           switch(item->parent()->row()){
             case 0:
               QLayout *layout = ui->horizontalLayout;
               while(!layout->isEmpty()) {
                   delete layout->itemAt(0)->widget();
               }

               // creation du model
               QSqlRelationalTableModel *transacionModel = new QSqlRelationalTableModel(this,this->db);
               transacionModel->setTable("hip_transaction");
               transacionModel->setFilter(
                           QString("hip_transaction.account_s = '%1'").arg(
                               treeViewModel->item(index.parent().row())->child(index.row())->data().toString()
                           )
               );
               //transacionModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
               transacionModel->setEditStrategy(QSqlTableModel::OnFieldChange);

               // set relation
               //transacionModel->setRelation(1, QSqlRelation("hip_account", "account_s", "name"));
               transacionModel->setRelation(
                           transacionModel->fieldIndex("third_s"),
                           QSqlRelation("hip_third", "third_s", "name")
               );
               transacionModel->setRelation(
                           transacionModel->fieldIndex("subcategory_s"),
                           QSqlRelation("v_hip_subcategory", "subcategory_s", "name")
               );

               transacionModel->sort(3,Qt::DescendingOrder);
               transacionModel->select();
               //qDebug() << transacionModel->query().lastQuery();

               //prepare dipslay
//               transacionModel->removeColumn(0); // don't show the ID
//               transacionModel->removeColumn(1); // don't show account name
//               transacionModel->removeColumn(2); // don't show ofxid
               transacionModel->setHeaderData(6, Qt::Horizontal, tr("third")); // rename third field
               transacionModel->setHeaderData(8, Qt::Horizontal, tr("category")); // rename third field

               TransactionWidget *transactionWidget = new TransactionWidget(this);
               layout->addWidget(transactionWidget);
               transactionWidget->getTableView()->setModel(transacionModel);
               transactionWidget->getTableView()->setItemDelegate(new QSqlRelationalDelegate(transactionWidget->getTableView()));

               //hide column
               transactionWidget->getTableView()->setColumnHidden(0,true);
               transactionWidget->getTableView()->setColumnHidden(1,true);
               transactionWidget->getTableView()->setColumnHidden(2,true);
               transactionWidget->getTableView()->setColumnHidden(11,true);
               transactionWidget->getTableView()->setColumnHidden(12,true);
               transactionWidget->getTableView()->setColumnHidden(13,true);
               transactionWidget->getTableView()->setAlternatingRowColors(true);
               //transactionWidget->getTableView()->setStyleSheet("alternate-background-color: yellow;background-color: red;");

               transactionWidget->getTableView()->horizontalHeader()->setSectionsMovable(true);
               transactionWidget->getTableView()->horizontalHeader()->setDragEnabled(true);
               transactionWidget->getTableView()->horizontalHeader()->setDragDropMode(QAbstractItemView::InternalMove);

               transactionWidget->getTableView()->show();
               transactionWidget->getTableView()->resizeColumnsToContents();
               transactionWidget->getTableView()->horizontalHeader()->setStretchLastSection(true);
               break;
           }
         }else{

         }
         //qDebug() << index.row();
         //qDebug() << item->data(Qt::DisplayRole).toString();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
   QMainWindow::resizeEvent(event);
   // Your code here.
   QList<int> list= ui->splitter->sizes();
   list.replace(0,this->height()*0.1);
   list.replace(1,this->height()*0.9);
   ui->splitter->setSizes(list);

}

bool MainWindow::saveAsFile(){
    if(this->workinDb==""){
        return false;
    }
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "new.hip",
                               tr("hip (*.hip)"));
    if(fileName==""){
        return false;
    }
    this->fileNameDb = fileName;
    return this->saveFile();
}

bool MainWindow::saveFile(){
    if(this->workinDb==""){
        return false;
    }
    if(this->fileNameDb==""){
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                   "new.hip",
                                   tr("hip (*.hip)"));
        if(fileName==""){
            return false;
        }
        this->fileNameDb = fileName;

    }
    QFile::copy(this->workinDb,this->fileNameDb);
    return true;
}

bool MainWindow::closeFile(){
    if(this->workinDb !=""){
        QLayout *layout = ui->horizontalLayout;
        while(!layout->isEmpty()) {
            delete layout->itemAt(0)->widget();
        }

        if(!this->equalFiles(this->workinDb,this->fileNameDb)){
            qDebug() << "file DB change save it ?";
            QMessageBox msgBox;
            msgBox.setText(tr("The document has been modified."));
            msgBox.setInformativeText(tr("Do you want to save your changes?"));
            msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Save);
            int ret = msgBox.exec();
            switch (ret) {
              case QMessageBox::Save:
                  // Save was clicked
                  if(!this->saveFile()){
                      return false;
                  }
                  break;
              case QMessageBox::Discard:
                  // Don't Save was clicked
                  break;
              case QMessageBox::Cancel:
                  // Cancel was clicked
                return false;
                  break;
              default:
                  // should never be reached
                  break;
            }
        }else{
            qDebug() << "file DB no change";
        }
        this->db.close();
        this->db.removeDatabase("QSQLITE");
        QFile::remove(this->workinDb);
        this->workinDb="";
        this->fileNameDb="";
        return true;
    }
    return true;
}

void MainWindow::openFile()
{
    QString fileName;
    fileName = QFileDialog::getOpenFileName(
        this,
        ////tr("Open File"), fileName, QString(tr("Files ( %1 )").arg("*.xhb"))
        tr("Open File"), fileName, QString(tr("Files ( %1 )").arg("*.sqlite"))
    );

   this->connectDb(fileName);

    ////XhbReader xhbReader(fileName);

    //QObject::connect(bank,&Bank::accountsChanged,treeViewModel,&TreeViewModel::accountsChanged);

    ////bank = xhbReader.parse(bank);

    //foreach(Account account,accounts){
    //    qDebug() << account.getName();
    //}
    //qDebug()<<bank->getTransactions(1).getDate().toString("yyyyMMdd");
}

void MainWindow::newFile()
{
    this->connectDb("new.hip",true);
}

void MainWindow::connectDb(QString fileNameDb,bool newFile){

    if(!newFile){
        this->fileNameDb = fileNameDb;

        //copy file db
        QFileInfo fileInfo(fileNameDb);
        //    qDebug()<<fileNameDb;
        //    qDebug()<<fileInfo.path()+ QDir::separator() +fileInfo.baseName()+".working.hip";
        this->workinDb = fileInfo.path()+ QDir::separator() +fileInfo.baseName()+".working.hip";
        QFile::copy(fileNameDb,workinDb);
    }else{
        this->workinDb=fileNameDb;
        this->fileNameDb = "";
    }

    this->db = QSqlDatabase::addDatabase("QSQLITE");
    this->db.setDatabaseName(workinDb);
    db.open();

    this->upgradeDb();

    QSqlQuery query;
    query.exec("SELECT * FROM hip_account");

    while (query.next()) {
            Account *account = new Account();
            account->setKey(query.value(0).toInt());
            account->setName(query.value(2).toString());
            bank->insertAccount(account);
            //qDebug() << account->getName();
    }
}

void MainWindow::upgradeDb(){
   QApplication::setOverrideCursor(Qt::BusyCursor);
   QSqlQuery query;
   query.exec("PRAGMA user_version");
   query.next();
   int dbVersion = query.value(0).toInt();
   qDebug() << query.value(0).toString();
   switch(dbVersion){
     // Creation de la base
     case 0:
       qDebug() << "Create DB";
       this->executeQueriesFromFile(new QFile("sql/createSchema.sql"),&query);
     // update for version 2 when DB is in version 1
     case 1:
       qDebug() << "DB up to date";
     // update for version 3 when DB is in version 2
     case 2:
       break;

     // Warning the version of the db is too high for the current version of the program
     default:
       break;
   }
   QApplication::restoreOverrideCursor();
}

//https://gist.github.com/savolai/6852986
void MainWindow::executeQueriesFromFile(QFile *file, QSqlQuery *query)
{
    file->open(QFile::ReadOnly);
    while (!file->atEnd()){
        QByteArray readLine="";
        QString cleanedLine;
        QString line="";
        bool finished=false;
        while(!finished && !file->atEnd()){
            readLine = file->readLine();
            cleanedLine=readLine.trimmed();
            //qDebug() << cleanedLine;
            // remove comments at end of line
            QStringList strings=cleanedLine.split("--");
            cleanedLine=strings.at(0);

            // remove lines with only comment, and DROP lines
            if(!cleanedLine.startsWith("--")
                //&& !cleanedLine.startsWith("DROP")
                && !cleanedLine.isEmpty()){
                line+=" "+cleanedLine;
            }
            if(cleanedLine.endsWith(";")){
                finished=true;
                break;
            }
            if(cleanedLine.startsWith("COMMIT")){
                finished=true;
            }
        }

        if(!line.isEmpty()){
            query->exec(line);
        }
        if(!query->isActive()){
            qDebug() << QSqlDatabase::drivers();
            qDebug() << query->lastError();
            qDebug() << "test executed query:"<< query->executedQuery();
            qDebug() << "test last query:"<< query->lastQuery();
        }
    }
}

bool MainWindow::equalFiles(QString f1,QString f2){
    QCryptographicHash hash1( QCryptographicHash::Sha1 );
    QFile file1( f1 );

    if ( file1.open( QIODevice::ReadOnly ) ) {
        hash1.addData( file1.readAll() );
    } else {
        // Handle "cannot open file" error
    }

    // Retrieve the SHA1 signature of the file
    QByteArray sig1 = hash1.result();

    QCryptographicHash hash2( QCryptographicHash::Sha1 );
    QFile file2( f2 );

    if ( file2.open( QIODevice::ReadOnly ) ) {
        hash2.addData( file2.readAll() );
    } else {
        // Handle "cannot open file" error
    }

    // Retrieve the SHA1 signature of the file
    QByteArray sig2 = hash2.result();

//    qDebug() << sig1;
//    qDebug() << sig2;

    if(sig1==sig2){
        return true;
    }else{
        return false;
    }
}
