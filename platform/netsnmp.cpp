
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <arpa/inet.h>
#include <QDebug>
#include "../lld/device.h"
#include "devicemanager.h"
using namespace JK;
#include <QNetworkInterface>

#define TimeOutSecond 3

void findAgent(addDeviceHandler handler,void* pData ,char* broadcast);
void findAgentV6(addDeviceHandler ,void*);
void snmpSearchDevices(addDeviceHandler handler,void* pData)
{
    QHostAddress broadcast;
    QList<QNetworkInterface> ilist = QNetworkInterface::allInterfaces();
    foreach (QNetworkInterface interface, ilist) {
        QList<QNetworkAddressEntry> entrylist = interface.addressEntries();
        foreach (QNetworkAddressEntry entry, entrylist) {
            QHostAddress address = entry.ip();
            if(address.protocol() == QAbstractSocket::IPv4Protocol){
                if(!address.isLoopback() && entry.netmask().isEqual(QHostAddress("255.255.255.0"))){
                    broadcast = entry.broadcast();
                    qDebug()<<"search:"<<broadcast;
                    findAgent(handler ,pData ,broadcast.toString().toLatin1().data());
                }
            }
        }
    }

    qDebug()<<"search: ipv6";
    findAgentV6(handler ,pData);
}


#define SYSOBJECTID_OID               "1.3.6.1.2.1.1.2.0"
#define SYSNAME_OID                      "1.3.6.1.2.1.1.5.0"
#define PVT_DEVICEID_OID   "1.3.6.1.4.1.26266.86.10.1.1.1.1.0"
static const oid sysObjectID[] = {1,3,6,1,2,1,1,2,0};
static const oid sysNameID[] = {1,3,6,1,2,1,1,5,0};
static const oid deviceID[] = {1,3,6,1,4,1,26266,86,10,1,1,1,1,0};
static const oid oidName[] = {1,3,6,1,2,1,1,2,0};
static const oid objectID[] = {1,3,6,1,4,1,26266,86,10,2,1};
//static const oid oidName[] = {1,3,6,1,4,1,2699,1,2,1,2,1,1,2,1};
static const oid oidDeviceID[]={1,3,6,1,4,1,2699,1,2,1,2,1,1,3,1};
static const size_t sizeofOidName = sizeof(oidName)/sizeof(oidName[0]);

struct My_synch_state{
    struct synch_state state;
    addDeviceHandler addDevice;
    void* pData;
};

static void handlerData(netsnmp_pdu *pdu, void *magic)
{
    {
        struct sockaddr_in *to = NULL;
        to = (struct sockaddr_in *)pdu->transport_data;
        if(to->sin_family == AF_INET){
            qDebug()<<"find ip:"<<inet_ntoa(to->sin_addr);
        }
    }
    {
        struct sockaddr_in6 *to = NULL;
        to = (struct sockaddr_in6 *)pdu->transport_data;
        if(to->sin6_family == AF_INET6){
            char ipv6[256];
            inet_ntop(AF_INET6 ,&to->sin6_addr ,ipv6 ,256);
            qDebug()<<"find ip:"<<ipv6;
        }
    }
    bool found = false;
    struct variable_list *vars;
    for(vars = pdu->variables; vars; vars = vars->next_variable){
//        int i;
//        for(i=0;i<vars->name_length;i++){
//            if(i == 0)
//                printf("oid name:");
//            printf("%d ",vars->name[i]);
//        }
//        printf("\n");
//        memcpy(str ,vars->val.string ,vars->val_len);
        if(ASN_OBJECT_ID == vars->type){
            if(!memcmp(vars->val.objid ,objectID ,vars->val_len)){
                found = true;
                break;
            }
        }
    }

    if(!found)
        return;
    struct synch_state *state = (struct synch_state *) magic;
    struct My_synch_state *mystate;
    mystate = (struct My_synch_state*)magic;
    DeviceInfo deviceInfo;
    memset((void*)&deviceInfo ,0 ,sizeof(deviceInfo));

    state->waiting = 1;
    {
        struct sockaddr_in *to = NULL;
        to = (struct sockaddr_in *)pdu->transport_data;
        if(to->sin_family == AF_INET){
            deviceInfo.type = DeviceInfo::Type_ipv4;
            strcpy(deviceInfo.address ,inet_ntoa(to->sin_addr));
        }
    }
    {
        struct sockaddr_in6 *to = NULL;
        to = (struct sockaddr_in6 *)pdu->transport_data;
        if(to->sin6_family == AF_INET6){
            deviceInfo.type = DeviceInfo::Type_ipv6;
            inet_ntop(AF_INET6 ,&to->sin6_addr ,deviceInfo.address ,256);
        }
    }

    mystate->addDevice(&deviceInfo ,mystate->pData);
    qDebug()<<"device info:"<<deviceInfo.name<<"("<<deviceInfo.address<<")";
}

static int  callback(int op,
              netsnmp_session * session,
              int reqid, netsnmp_pdu *pdu, void *magic)
{
    struct synch_state *state = (struct synch_state *) magic;
    int             rpt_type;

    if (reqid != state->reqid && pdu && pdu->command != SNMP_MSG_REPORT) {
        qDebug("\nUnexpected response (ReqID: %d,%d - Cmd %d)",
                                   reqid, state->reqid, pdu->command );
        return 0;
    }

    state->waiting = 1;
//    qDebug( "\nResponse (ReqID: %d - Cmd %d)",
//                               reqid, (pdu ? pdu->command : -1));

    if (op == NETSNMP_CALLBACK_OP_RECEIVED_MESSAGE && pdu) {
        if (pdu->command == SNMP_MSG_REPORT) {
            rpt_type = snmpv3_get_report_type(pdu);
            if (SNMPV3_IGNORE_UNAUTH_REPORTS ||
                rpt_type == SNMPERR_NOT_IN_TIME_WINDOW) {
                state->waiting = 1;
            }
            state->pdu = NULL;
            state->status = STAT_ERROR;
            session->s_snmp_errno = rpt_type;
            SET_SNMP_ERROR(rpt_type);
        } else if (pdu->command == SNMP_MSG_RESPONSE) {
            /*
             * clone the pdu to return to snmp_synch_response
             */
//            state->pdu = snmp_clone_pdu(pdu);
//            state->status = STAT_SUCCESS;
//            session->s_snmp_errno = SNMPERR_SUCCESS;
            //add by jacky
            handlerData(pdu ,magic);
            return 0;
        }
        else {
            char msg_buf[50];
            state->status = STAT_ERROR;
            session->s_snmp_errno = SNMPERR_PROTOCOL;
            SET_SNMP_ERROR(SNMPERR_PROTOCOL);
            snprintf(msg_buf, sizeof(msg_buf), "Expected RESPONSE-PDU but got %s-PDU",
                     snmp_pdu_type(pdu->command));
            snmp_set_detail(msg_buf);
            return 0;
        }
    } else if (op == NETSNMP_CALLBACK_OP_TIMED_OUT) {
        qDebug()<<"timeout";
        state->pdu = NULL;
        state->status = STAT_TIMEOUT;
        session->s_snmp_errno = SNMPERR_TIMEOUT;
        SET_SNMP_ERROR(SNMPERR_TIMEOUT);
    } else if (op == NETSNMP_CALLBACK_OP_DISCONNECT) {
        state->pdu = NULL;
        state->status = STAT_ERROR;
        session->s_snmp_errno = SNMPERR_ABORT;
        SET_SNMP_ERROR(SNMPERR_ABORT);
    }

    return 1;
}

int netsnmpBroadcast(netsnmp_session& session ,void* magic)
{
    netsnmp_session *ss;
    netsnmp_pdu *pdu;
    ss = snmp_open (&session);
    if (ss == NULL){
        snmp_sess_perror ("snmp_open", &session);
        return (1);
    }

    int new_length=sizeofOidName;
    pdu = snmp_pdu_create (SNMP_MSG_GET);
    snmp_add_null_var (pdu, oidName, new_length);

//    snmp_callback cbsav;
//    void* cbmagsav;
//    cbsav = ss->callback;
//    cbmagsav = ss->callback_magic;
    struct My_synch_state *mystate;
    mystate = (struct My_synch_state*)magic;
    struct synch_state *state;
    state = (struct synch_state*)magic;
    ss->callback = callback;
    ss->callback_magic = (void*) state;
    state->reqid = snmp_send(ss ,pdu);
    state->waiting = 1;
    if(state->reqid == 0){
        snmp_free_pdu(pdu);
        state->status = STAT_ERROR;
    }else{
        fd_set fdset;
        int block;
        int numfds ,count;
        struct timeval timeout ,*tvp;
#if 0

        numfds = 0;
        FD_ZERO(&fdset);
        block = NETSNMP_SNMPBLOCK;
        tvp=&timeout;
        timerclear(tvp);
//            snmp_select_info(&numfds ,&fdset ,tvp ,&block);
        snmp_sess_select_info_flags(0 ,&numfds ,&fdset ,tvp ,&block ,NETSNMP_SELECT_NOALARMS);
//        if(block == 1){
//            tvp=NULL;
//            break;
//        }
        qDebug()<<"timeout time:"<<tvp->tv_sec <<"."<<tvp->tv_usec;

        long tm_val = 1000 * 1000;
        int times = TimeOutSecond * 1;

        struct timeval tpstart,tpend;
        long long tm;
        for(int i = 0 ;i < times ;i++){
            gettimeofday(&tpstart,NULL);
            while(!((DeviceManager*)mystate->pData)->isCancelSearch()){

                gettimeofday(&tpend,NULL);
                tm = tm_val + tpstart.tv_sec*1000*1000+tpstart.tv_usec  - tpend.tv_sec*1000*1000 - tpend.tv_usec;
                if(tm <= 0)
                    break;
                timeout.tv_sec = tm / tm_val;
                timeout.tv_usec = tm % tm_val;
                count = select(numfds ,&fdset ,NULL ,NULL ,tvp);
                if(count > 0){
                    snmp_read(&fdset);
                }
            }
        }
//        for(int i = 0; i < times ;i++){
//            if(((DeviceManager*)mystate->pData)->isCancelSearch()){
//               break;
//            }
//            timeout.tv_sec = 0;//tm / tm_val;
//            timeout.tv_usec = 0;//tm % tm_val;
//            count = select(numfds ,&fdset ,NULL ,NULL ,tvp);
//            if(count > 0){
//                snmp_read(&fdset);
//            }
//            usleep(tm_val);
//        }
        snmp_timeout();
#else
        while(state->waiting & !((DeviceManager*)mystate->pData)->isCancelSearch()){
            numfds = 0;
            FD_ZERO(&fdset);
            block = NETSNMP_SNMPBLOCK;
            tvp=&timeout;
            timerclear(tvp);
            snmp_select_info(&numfds ,&fdset ,tvp ,&block);
//            snmp_sess_select_info_flags(0 ,&numfds ,&fdset ,tvp ,&block ,NETSNMP_SELECT_NOALARMS);
//            if(block == 1)
//                tvp=NULL;
            count = select(numfds ,&fdset ,NULL ,NULL ,tvp);
            if(count > 0){
                snmp_read(&fdset);
            }else{
                switch (count) {
                case 0:
                    snmp_timeout();
                    qDebug()<<"time out";
                    break;
                case -1:
//                    if(errno == EINTR)
//                        continue;
//                    else
//                        snmp_errno = SNMPERR_GENERR;
                default:
                    state->status = STAT_ERROR;
                    break;
                }
                break;
            }
        }
#endif
    }
//    ss->callback = cbsav;
//    ss->callback_magic = cbmagsav;
    snmp_close (ss);
    return 0;

}

void findAgent(addDeviceHandler handler,void* pData ,char* broadcast)
{
    netsnmp_session session;
    init_snmp ("snmp");
    snmp_sess_init (&session);
    session.version = SNMP_VERSION_2c;
    session.community = (u_char*)"public";
    session.community_len = strlen ((const char*)session.community);
    session.peername = (char*)broadcast;//被监控主机的IP地址
    session.flags |= SNMP_FLAGS_UDP_BROADCAST;
    session.timeout =  TimeOutSecond *1000 * 1000;

    struct My_synch_state lstate;
    memset((void*)&lstate ,0 ,sizeof(lstate));
    lstate.addDevice = handler;
    lstate.pData = pData;
    netsnmpBroadcast(session ,(void*)&lstate);
}

void findAgentV6(addDeviceHandler handler,void* pData)
{
    netsnmp_session session;
    init_snmp ("snmp");
    snmp_sess_init (&session);
    session.version = SNMP_VERSION_2c;
    session.community = (u_char*)"public";
    session.community_len = strlen ((const char*)session.community);
//    session.peername = (char*)"udp6:[2001:DB8::6:721A:4FF:FE11:6D58]";//被监控主机的IP地址
    session.peername = (char*)"udp6:[ff08::]";//被监控主机的IP地址
    session.flags |= SNMP_FLAGS_UDP_BROADCAST;
    session.timeout =  TimeOutSecond *1000 * 1000;

    struct My_synch_state lstate;
    memset((void*)&lstate ,0 ,sizeof(lstate));
    lstate.addDevice = handler;
    lstate.pData = pData;
    netsnmpBroadcast(session ,(void*)&lstate);
}

//#include <QUdpSocket>
//QUdpSocket udpSocket;
void snmpCancelSearch()
{
    snmp_close_sessions();

//    system("echo \"cancel search\" >>0 ");
//    qDebug()<<"cancel search";
//    udpSocket.writeDatagram("cancel search",11,QHostAddress::LocalHost, 59230);
}
