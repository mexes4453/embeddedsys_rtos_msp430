
#ifndef OS_EVT_H
#define OS_EVT_H


typedef enum
{
    OS__enEvtSigTimer=0,
}   tenOsEvtSig;


typedef struct s_osEvt
{
    tenOsEvtSig evtSig;
}   t_osEvt;







#endif  /*   OS_EVT_H   */