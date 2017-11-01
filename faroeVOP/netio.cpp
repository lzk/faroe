#include "netio.h"
#include <QTcpSocket>
#include <QHostInfo>
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

#include <QUrl>
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

int NetIO::open(const char* url)
{
    resolveUrl(url);
    return open();
}

int NetIO::open()
{
    return open(23010);
}

int NetIO::open(int port)
{    
    if(!tcpSocket)
        tcpSocket = new QTcpSocket;
    tcpSocket->connectToHost(hostAddress, port);
    if (!tcpSocket->waitForConnected(5000)) {
//        emit error(tcp_socket.error(), tcp_socket.errorString());
//        C_LOG("err:%d",tcpSocket->error());
//        _Q_LOG("err:" + tcpSocket->errorString());
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
    tcpSocket->close();
    return 0;
}

int NetIO::write(char *buffer, int bufsize)
{
    if(!tcpSocket)
        return -1;
//    int state = tcpSocket->state();
//    C_LOG("tcp_socket state:%d" ,state);
    int ret;
    ret = tcpSocket->write(buffer ,bufsize);

    if(!tcpSocket->waitForBytesWritten(5000)){
//        _Q_LOG("tcp_socket write timeout");
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
//            C_LOG("not enough bytesAvailable:%d,buffer size:%d",bytesAvailable ,bufsize);
//            if(!tcpSocket->waitForReadyRead(10)){
            if(!tcpSocket->waitForReadyRead()){
                return -1;
            }
        }else{
            break;
        }
    }
    while(1);
//    C_LOG("bytesAvailable:%d,buffer size:%d",bytesAvailable ,bufsize);
    return tcpSocket->read(buffer ,bufsize);
#else
    int numRead = 0, numReadTotal = 0;
    do{
        bytesAvailable = tcpSocket->bytesAvailable();
        if(bytesAvailable > bufsize - numReadTotal)
            bytesAvailable = bufsize - numReadTotal;
        if(bytesAvailable > 0){
            numRead = tcpSocket->read(buffer + numReadTotal ,bytesAvailable);
            if(numRead < 0){
                break;
            }else{
                numReadTotal += numRead;
                if (numRead == 0 && !tcpSocket->waitForReadyRead())
                    break;
            }
        }else{
            if(!tcpSocket->waitForReadyRead())
                break;
        }

    }while(numReadTotal < bufsize);
    if(numReadTotal != bufsize)
        return -1;
    return numReadTotal;
#endif
}

bool NetIO::isConnected()
{
    if(open(23011) >= 0){
        close();
        return true;
    }
    return false;
}
