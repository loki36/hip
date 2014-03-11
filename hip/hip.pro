#-------------------------------------------------
#
# Project created by QtCreator 2014-02-02T18:52:05
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webkitwidgets

TARGET = hip
TEMPLATE = app


SOURCES += main.cpp\
    mainwindow.cpp\
    account.cpp \
    xhbreader.cpp \
    bank.cpp \
    pay.cpp \
    category.cpp \
    subcategory.cpp \
    tag.cpp \
    transaction.cpp \
    budget.cpp \
    budgetyear.cpp \
    treeviewmodel.cpp \
    transactionwidget.cpp \
    transactionitemmodel.cpp

HEADERS  += mainwindow.h\
    account.h \
    xhbreader.h \
    bank.h \
    pay.h \
    category.h \
    subcategory.h \
    tag.h \
    transaction.h \
    budget.h \
    budgetyear.h \
    treeviewmodel.h \
    transactionwidget.h \
    transactionitemmodel.h

FORMS    += mainwindow.ui \
    transactionwidget.ui

TRANSLATIONS = l10n/hip_en.ts l10n/hip_fr.ts

translation_file.path = $$OUT_PWD/l10n
translation_file.files = $$PWD/l10n/*.qm

sql_file.path = $$OUT_PWD/sql
sql_file.files += $$PWD/sql/createSchema.sql

INSTALLS += sql_file
INSTALLS += translation_file
