#include "filetranser.h"

filetranser::filetranser(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket();
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadReady()));
    connect(socket, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(onDisConnected()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(onError(QAbstractSocket::SocketError)));
    socket->connectToHost("192.168.81.1",6000);
}

void filetranser::sendFile(const QString &filename)
{
    // 读取到的文件
    QByteArray datafileContent;

    QFile file(filename);
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "文件打开失败";
        return;
    }
    datafileContent = file.readAll();
    file.close();

    // 要发送的数据包
    QByteArray dataSend;

    QDataStream stream(&dataSend, QIODevice::WriteOnly);
    stream <<quint32() << (quint32)MsgTypeFile << filename << datafileContent;
    stream.device()->seek(0);
    stream << dataSend.size();

    socket->write(dataSend);
}

void filetranser::sendDeleteFile(const QString &file)
{
    // 要发送的数据包
    QByteArray dataSend;

    QDataStream stream(&dataSend, QIODevice::WriteOnly);
    stream <<quint32() << (quint32)MsgTypeDel<< file;
    stream.device()->seek(0);
    stream << dataSend.size();

    socket->write(dataSend);
}

void filetranser::sendRenameFile(const QString &file1, const QString &file2)
{
    QByteArray dataSend;

    QDataStream stream(&dataSend, QIODevice::WriteOnly);
    stream <<quint32() << (quint32)MsgTypeRename<< file1 << file2;
    stream.device()->seek(0);
    stream << dataSend.size();

    socket->write(dataSend);
}

void filetranser::onConnected()
{
    qDebug() << "客户端连接服务器成功.";
}

void filetranser::onDisConnected()
{
    qDebug() << "服务器端连接断开.";
    if(socket == 0)
        return;
    socket->close();
}

void filetranser::onError(QAbstractSocket::SocketError socketError)
{
    qDebug() << "服务器端连接发生错误.";
}

void filetranser::onReadReady()
{
    // 可以得到是谁触发了这个槽函数
    QObject* obj = this->sender();
    QTcpSocket* socket = qobject_cast<QTcpSocket*>(obj);
    qint64 sizeNow = 0;
    do
    {
        sizeNow = socket->bytesAvailable();  // 这个函数可以获取当前数据包的大小
        QDataStream stream(socket);
        if(LastsizePack == 0)
        {
            if(sizeNow < sizeof(quint32))
            {
                // 如果还没有包头长的话直接返回
                return;
            }

            stream >> LastsizePack;  // 当前包的大小, 注意这里不一定等于缓存区中数据的大小

        }



        if(sizeNow >= LastsizePack - 4)  // 可以读了
        {
            LastsizePack = 0;
            quint32 msg;
            stream >> msg;
            sizeNow = socket->bytesAvailable();

            switch (msg)
            {
                case MsgTypeFile:
                {
                    QByteArray datafilecontent;
                    stream >> datafilecontent;
                    break;
                }
                case MsgTypeDel:
                {
                    QString filedel;
                    stream >> filedel;
                    break;
                }
                case MsgTypeRename:
                {
                    QString file1;
                    QString file2;
                    stream >> file1 >> file2;
                    break;
                }

                default:
                    break;
            }
        }
        else
            return;

    }
    while(sizeNow > 0);
}
