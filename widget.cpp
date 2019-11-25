#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    fileTransfer = new filetranser();
    connect(&filewatch, SIGNAL(doFileAdded(QString)),
            this, SLOT(doFileAdded(QString)));
    connect(&filewatch, SIGNAL(doFileRemoved(QString)),
            this, SLOT(doFileRemoved(QString)));
    connect(&filewatch, SIGNAL(doFileChanged(QString)),
            this, SLOT(doFileChanged(QString)));
    connect(&filewatch, SIGNAL(doFileRename(QString,QString)),
            this, SLOT(doFileRename(QString,QString)));
    filewatch.setWatchPath("C:Users/Administrator/Desktop/test");
}

Widget::~Widget()
{
    delete ui;
}

void Widget::doFileAdded(const QString &file)
{
    fileTransfer->sendFile(file);
}

void Widget::doFileRemoved(const QString &file)
{
    fileTransfer->sendDeleteFile(file);
}

void Widget::doFileChanged(const QString &file)
{
    fileTransfer->sendFile(file);
}

void Widget::doFileRename(const QString &file, const QString &file1)
{
    fileTransfer->sendRenameFile(file, file1);
}
