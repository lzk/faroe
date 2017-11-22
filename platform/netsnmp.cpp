
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include <arpa/inet.h>
#include <QDebug>
#include "../lld/device.h"
#include "devicemanager.h"
using namespace JK;

void findAgent(addDeviceHandler ,void*);
void findAgentV6(addDeviceHandler ,void*);
void snmpSearchDevices(addDeviceHandler handler,void* pData)
{
    qDebug()<<"begin searching";
    findAgent(handler ,pData);
    findAgentV6(handler ,pData);
}


static const oid oidName[] = {1,3,6,1,4,1,2699,1,2,1,2,1,1,2,1};
static const oid oidDeviceID[]={1,3,6,1,4,1,2699,1,2,1,2,1,1,3,1};
static const size_t sizeofOidName = sizeof(oidName)/sizeof(oidName[0]);

struct My_synch_state{
    struct synch_state state;
    addDeviceHandler addDevice;
    void* pData;
};

static void handlerData(netsnmp_pdu *pdu, void *magic)
{
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

    struct variable_list *vars;
    for(vars = pdu->variables; vars; vars = vars->next_variable){
//        int i;
//        for(i=0;i<vars->name_length;i++){
//            printf("%d ",vars->name[i]);
//        }
//        print_value(vars->name, vars->name_length, vars);
//        char str[1024];
//        memset(str ,0 ,1024);
        memcpy(deviceInfo.name ,vars->val.string ,vars->val_len);
        break;
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

    state->waiting = 0;
    qDebug( "\nResponse (ReqID: %d - Cmd %d)",
                               reqid, (pdu ? pdu->command : -1));

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
    fd_set fdset;
    struct timeval timeout ,*tvp;
    int block;
    int numfds ,count;
    state->reqid = snmp_send(ss ,pdu);
    state->waiting = 1;
    if(state->reqid == 0){
        snmp_free_pdu(pdu);
        state->status = STAT_ERROR;
    }else{
        while(state->waiting & !((DeviceManager*)mystate->pData)->isCancelSearch()){
            numfds = 0;
            FD_ZERO(&fdset);
            block = NETSNMP_SNMPBLOCK;
            tvp=&timeout;
            timerclear(tvp);
            snmp_sess_select_info_flags(0 ,&numfds ,&fdset ,tvp ,&block ,NETSNMP_SELECT_NOALARMS);
            qDebug()<<"timeout:"<<tvp->tv_sec<<":"<<tvp->tv_usec;
            qDebug()<<"block?"<<block;
            if(block == 1)
                tvp=NULL;
            count = select(numfds ,&fdset ,NULL ,NULL ,tvp);
            if(count > 0)
                snmp_read(&fdset);
            else{
                switch (count) {
                case 0:
                    snmp_timeout();
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
    }

//    ss->callback = cbsav;
//    ss->callback_magic = cbmagsav;
    snmp_close (ss);
    return 0;

}

void findAgent(addDeviceHandler handler,void* pData)
{
    netsnmp_session session;
    init_snmp ("snmp");
    snmp_sess_init (&session);
    session.version = SNMP_VERSION_2c;
    session.community = (u_char*)"public";
    session.community_len = strlen ((const char*)session.community);
//    session.peername = (char*)"255.255.255.255";//被监控主机的IP地址
    session.peername = (char*)"192.168.6.255";//被监控主机的IP地址
    session.flags |= SNMP_FLAGS_UDP_BROADCAST;
    session.timeout =  5 *1000 * 1000;

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
    session.timeout =  5 *1000 * 1000;

    struct My_synch_state lstate;
    memset((void*)&lstate ,0 ,sizeof(lstate));
    lstate.addDevice = handler;
    lstate.pData = pData;
    netsnmpBroadcast(session ,(void*)&lstate);
}
