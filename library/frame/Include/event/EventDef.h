/**
 * frame
 *
 * 事件相关类型定义
**/
#ifndef __FRAME__EVENTDEF_H__
#define __FRAME__EVENTDEF_H__
namespace frame {

typedef enum
{
    EVENT_ACTIVE_TYPE_INVALID,//无效消息
    EVENT_ACTIVE_TYPE_VALID,//有效消息
} EvenActiveType;

struct Active_Event
{
    EvenActiveType active_type;
    bool isActive;
};

typedef enum
{
    EVENT_TYPE_INVALID,//无效消息
    EVENT_TYPE_VALID,//有效消息
} EvenType;

typedef enum
{
    EVENT_KEY_TYPE_INVALID,//无效消息
    EVENT_KEY_TYPE_DOWN,//键盘按键压下消息
    EVENT_KEY_TYPE_UP,//键盘按键抬起消息
    EVENT_KEY_TYPE_REPEAT,//键盘按键重复消息
} EventKeyType;

struct Keyboard_Event
{
    EvenType keyboard_type;
    int key;
    EventKeyType key_action;
    int mods;
};

typedef enum
{
    EVENT_MOUSE_BUTTON_INVALID,//无效消息
    EVENT_MOUSE_BUTTON_LEFT,//鼠标左键消息
    EVENT_MOUSE_BUTTON_RIGHT,//鼠标右键消息
    EVENT_MOUSE_BUTTON_MIDDLE,//鼠标中键消息
} EventMouseButton;
typedef enum
{
    EVENT_MOUSE_TYPE_INVALID,//无效消息
    EVENT_MOUSE_TYPE_DOWN,//鼠标压下消息
    EVENT_MOUSE_TYPE_MOVE,//鼠标移动消息
    EVENT_MOUSE_TYPE_UP,//鼠标抬起消息
} EventMouseType;

struct Mouse_Event
{
    EventMouseButton mouse_button;
    EventMouseType mouse_type;
    int iPosX;
    int iPosY;
};

struct Input_Char_Event
{
    EvenType input_char_type;
    unsigned int iChar;
};

union EventBody
{
    Mouse_Event mouse_event;
    Keyboard_Event keyboard_event;
    Active_Event active_event;
    Input_Char_Event input_char_event;
    struct
    {
        void* data;
    } user_event;
};

typedef enum
{
    EVENT_TYPE_MOUSE,//鼠标消息
    EVENT_TYPE_KETBOARD,//键盘消息
    EVENT_TYPE_ACTIVE,//force消息
    EVENT_TYPE_INPUT_CHAR,//字符输入消息
} EventType;

struct Event
{
    EventType type;
    EventBody body;
};

} // namespace frame 

#endif // __FRAME__EVENTDEF_H__