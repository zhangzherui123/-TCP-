#ifndef FILEWATCH_H
#define FILEWATCH_H

#include <QObject>
#include <QFileSystemWatcher>
#include <qdebug.h>
#include <QDir>
#include "filetranser.h"

class FileWatch : public QObject
{
    Q_OBJECT
public:
    explicit FileWatch(QObject *parent = 0);
    void setWatchPath(const QString &path);
signals:
    void doFileAdded(const QString &file);
    void doFileRemoved(const QString &file);
    void doFileChanged(const QString &file);
    void doFileRename(const QString &file, const QString &file1);

private slots:
    void onDicChange(const QString &path);
    void onFileChange(const QString &path);

private:
    QFileSystemWatcher* watcher;
    QStringList files();   // 获取该目录下的文件列表
    QString pathWatched; // 被检测目录
    QStringList filesold;    // 已经被监听的文件对象
};

#endif // FILEWATCH_H
