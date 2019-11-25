#ifndef FILETRANSER_H
#define FILETRANSER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDataStream>
#include <QFile>

enum MsgType
{
    MsgTypeInvalid = 0,
    MsgTypeFile,
    MsgTypeDel,
    MsgTypeRename
};

class filetranser : public QObject
{
    Q_OBJECT
public:
    explicit filetranser(QObject *parent = nullptr);
    void sendFile(const QString& file);
    void sendDeleteFile(const QString& file);
    void sendRenameFile(const QString& file1, const QString& file2);

signals:


private slots:
    void onConnected();
    void onDisConnected();
    void onError(QAbstractSocket::SocketError socketError);
    void onReadReady();


private:
    QTcpSocket* socket;
    quint32 LastsizePack = 0;
};

#endif // FILETRANSER_H
