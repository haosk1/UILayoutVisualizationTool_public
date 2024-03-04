/**
 * frame
 *
 * 入口类接口
**/

#ifndef __FRAME__CMAINCTRL_INTERFACE_H__
#define __FRAME__CMAINCTRL_INTERFACE_H__

#include <string>

namespace frame {

class CMainCtrlInerface
{
public:
    virtual ~CMainCtrlInerface(){};
    virtual bool Init(int width, int height, std::string name) = 0;
    virtual bool UnInit()  = 0;
    virtual void LoopMsg() = 0;
};

} // namespace frame 

#endif // __FRAME__CMAINCTRL_INTERFACE_H__
