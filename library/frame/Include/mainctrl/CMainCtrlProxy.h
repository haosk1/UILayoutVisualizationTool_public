/**
 * frame
 *
 * 入口类代理
**/

#ifndef __FRAME__CMAINCTRL_PROXY_H__
#define __FRAME__CMAINCTRL_PROXY_H__

#include "CMainCtrl_Interface.h"

namespace frame {

typedef enum
{
    MAINCTRL_TYPE_LINUXPC,//LINUX PC
} MainCtrlType;

class CMainCtrlProxy
{
public:
    static CMainCtrlInerface* getMainCtrl(MainCtrlType type);
private:
    explicit CMainCtrlProxy(){};
    ~CMainCtrlProxy(){};
};

} // namespace frame 

#endif // __FRAME__CMAINCTRL_PROXY_H__
