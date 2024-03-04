/**
 * frame
 *
 * UI类型定义
**/
#ifndef __UIDEF_H__
#define __UIDEF_H__

#define ARRAY_MAX(_array_) (sizeof(_array_) / sizeof(_array_[0]))
#define UI_MAX(_arg1_, _arg2_) ((_arg1_) > (_arg2_) ? (_arg1_) : (_arg2_))
#define UI_MIN(_arg1_, _arg2_) ((_arg1_) < (_arg2_) ? (_arg1_) : (_arg2_))

#define REPEAT_ENDLESS (0xFFFFFFFF)

#endif // __UIDEF_H__