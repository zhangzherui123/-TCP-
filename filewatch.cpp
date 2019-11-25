#include "filewatch.h"

FileWatch::FileWatch(QObject *parent) : QObject(parent)
{
    watcher = new QFileSystemWatcher();
    connect(watcher,SIGNAL(directoryChanged(QString)),
            this, SLOT(onDicChange(QString)));
    connect(watcher,SIGNAL(fileChanged(QString)),
            this, SLOT(onFileChange(QString)));
}

void FileWatch::setWatchPath(const QString &path)
{
    pathWatched = path;

    QStringList files = this->files();
    watcher->addPaths(files);
    watcher->addPath(path);
    filesold = watcher->files();
}

void FileWatch::onDicChange(const QString &path)
{
    qDebug() << "目录发生变化 ";

    QStringList fileNow = this->files();
    if(fileNow.count() > filesold.count())
    {
        foreach (QString file, filesold) {
           fileNow.removeAll(file);
        }

        // 添加新文件到监听目录中
        watcher->addPaths(fileNow);
        filesold = watcher->files();
        qDebug() << "新创建的文件名是 = " << fileNow;
        emit doFileAdded(fileNow.first());
    }
    qDebug() << endl;
}

void FileWatch::onFileChange(const QString &path)
{
    qDebug() << "文件发生变化 ";
    QStringList fileWatched = watcher->files();

    if(fileWatched.contains(path))
    {
//        qDebug() << path << "文件内容发生改变.";
        emit doFileChanged(path);
    }
    else
    {
        // 当文件被删除之后会自动的从监听列表中删除,所以我们可以通过这种方式
        // 来判断到底是文件信息发生变化还是被删除了
        QStringList fileNow = this->files();
        if(fileNow.count() == filesold.count())
        {
            // 数量没有发生变化,就说明发生了文件重命名
            foreach (QString wifile, fileWatched) {
               fileNow.removeAll(wifile);
            }
            watcher->addPaths(fileNow);
//            qDebug() << path << "重命名为" << fileNow;
            emit doFileRename(path, fileNow.first());
        }
        else   // 数量发生变化,说明文件被删除
//            qDebug() << path << "文件已被删除.";
            emit doFileRemoved(path);
    }
    filesold = watcher->files();
    qDebug() << endl;

}

QStringList FileWatch::files()
{
    QDir dir(pathWatched);
    QFileInfoList infolist = dir.entryInfoList(QDir::Files);
    QStringList ret;
    foreach (QFileInfo info, infolist) {
       ret.append(info.absoluteFilePath());
    }
    return ret;
}
