#include "netio.h"
#include <QTcpSocket>
#include <QHostInfo>
#include <QUrl>
#include "log.h"
QHostAddress get_ip_address(const QString& host)
{
    QHostAddress hostAddress;

    QHostInfo info;
    info = QHostInfo::fromName(host);
    if (!info.addresses().isEmpty()) {
        bool found = false;
        foreach (QHostAddress address, info.addresses()) {
            if(QAbstractSocket::IPv4Protocol == address.protocol()){
                hostAddress = address;
                found = true;
                break;
            }
        }
        if(!found)
            hostAddress = info.addresses().first();
        qDebug()<<"host name:" << host << "addresses:" <<info.addresses();
    }
    return hostAddress;
}

NetIO::NetIO():
    tcpSocket(NULL)
{

}
NetIO::~NetIO()
{
    if(tcpSocket)
        delete tcpSocket;
}

int NetIO::resolveUrl(const char* _url)
{
    QUrl url(_url);
    QString host = url.host();//resolve_uri(url);
    if(host.isEmpty())
        return -1;
    hostAddress = get_ip_address(host);
    return 0;
}

int NetIO::type()
{
    return DeviceIO::Type_net;
}

int NetIO::open(int para)
{
    int port;
    switch (para) {
    case 0:
        port = 23010;
        break;
    case 1:
        port = 9100;
        break;
    case 2:
        port = 23011;
        break;
    default:
        port = 23010;
        break;
    }
    return openPort(port);
}

int NetIO::openPort(int port)
{    
    if(!tcpSocket)
        tcpSocket = new QTcpSocket;
    tcpSocket->connectToHost(hostAddress, port);
    if (!tcpSocket->waitForConnected(5000)) {
//        emit error(tcp_socket.error(), tcp_socket.errorString());
//        LOG_PARA("err:%d",tcpSocket->error());
//        LOG_NOPARA("err:" + tcpSocket->errorString());
        qDebug()<<"tcp error:"<< tcpSocket->error();
        qDebug()<< tcpSocket->errorString();
        return -1;
    }
    return 0;

}

int NetIO::close(void)
{
    if(!tcpSocket)
        return -1;
//    tcpSocket->disconnectFromHost();
//    tcpSocket->waitForDisconnected();
//    tcpSocket->abort();
    tcpSocket->close();
    return 0;
}

int NetIO::write(char *buffer, int bufsize)
{
    if(!tcpSocket)
        return -1;
//    int state = tcpSocket->state();
//    LOG_PARA("tcp_socket state:%d" ,state);
    int ret;
    ret = tcpSocket->write(buffer ,bufsize);

    if(!tcpSocket->waitForBytesWritten(15000)){
        LOG_NOPARA("tcp_socket write timeout");
        return -1;
    }
    return ret;
}

int NetIO::read(char *buffer, int bufsize)
{
    if(!tcpSocket)
        return -1;
    int bytesAvailable;
#if 0
    do{
        bytesAvailable = tcpSocket->bytesAvailable();
        if(bytesAvailable < bufsize){
//            LOG_PARA("not enough bytesAvailable:%d,buffer size:%d",bytesAvailable ,bufsize);
//            if(!tcpSocket->waitForReadyRead(10)){
            if(!tcpSocket->waitForReadyRead(15000)){
                return -1;
            }
        }else{
            break;
        }
    }
    while(1);
//    LOG_PARA("bytesAvailable:%d,buffer size:%d",bytesAvailable ,bufsize);
    return tcpSocket->read(buffer ,bufsize);
#else
    int numRead = 0, numReadTotal = 0;
    do{
        if (!tcpSocket->waitForReadyRead(15000)){
            LOG_NOPARA("tcp_socket read timeout");
            break;
        }
        bytesAvailable = tcpSocket->bytesAvailable();
        if(bytesAvailable > bufsize - numReadTotal)
            bytesAvailable = bufsize - numReadTotal;
        if(bytesAvailable > 0){
            numRead = tcpSocket->read(buffer + numReadTotal ,bytesAvailable);
            if(numRead <= 0){
                break;
            }else{
                numReadTotal += numRead;
            }
        }else{
            break;
        }
    }while(numReadTotal < bufsize);
    if(numReadTotal != bufsize)
        return -1;
    return numReadTotal;
#endif
}

int NetIO::writeCommand(char *buffer, int bufsize)
{
    return write(buffer ,bufsize);
}

int NetIO::readCommand(char *buffer, int bufsize)
{
    return read(buffer ,bufsize);
}

bool NetIO::isConnected()
{
    if(openPort(23011) >= 0){
        close();
        return true;
    }
    return false;
}
