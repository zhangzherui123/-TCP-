#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "filewatch.h"
#include "filetranser.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


private slots:
    void doFileAdded(const QString &file);
    void doFileRemoved(const QString &file);
    void doFileChanged(const QString &file);
    void doFileRename(const QString &file, const QString &file1);


private:
    Ui::Widget *ui;
    FileWatch filewatch;
    filetranser* fileTransfer;
};

#endif // WIDGET_H
