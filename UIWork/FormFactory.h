/**
 * frame
 *
 * UIForm工厂
 **/
#ifndef __FRAME__CFORMFACTORY_H__
#define __FRAME__CFORMFACTORY_H__

#include <dirent.h>
#include <sys/inotify.h>

#include "UIFW/CUIPhysicalLayer.h"
#include "UIFW/UIController.h"
#include "event/CEventManager.h"
#include "UILib.h"
#include "Form/Form_Boot.h"

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN (1024 * (EVENT_SIZE + 16))

#define BEGIN_FORM_REGISTER                   \
public:                                       \
    virtual CForm *GetForm(FormID emunFormID) \
    {                                         \
        switch (emunFormID)                   \
        {

#define FORM_REGISTER(_FormID_, _FormClass_) \
    case (_FormID_):                         \
    {                                        \
        CForm *pForm = new _FormClass_();    \
        return pForm;                        \
    }
#define END_FORM_REGISTER                     \
    default:                                  \
        break;                                \
        }                                     \
        return NULL;                          \
        }                                     \
        virtual bool DeleteForm(CForm *pForm) \
        {                                     \
            delete pForm;                     \
            return true;                      \
        }

typedef enum
{
    FORM_INVALID, // 无效Form
    FORM_BOOT,
    FORM_MAIN,
} FormID;

class UIController;
class CFormFactory : public UIInitCallback
{
public:
    BEGIN_FORM_REGISTER
    FORM_REGISTER(FORM_BOOT, CForm_Boot)
    // FORM_REGISTER(CELLID_FU_MAIN, CForm_Main)
    END_FORM_REGISTER

    static CFormFactory *Instance();
    virtual ~CFormFactory();
    virtual void onUIInit();

    void LoadForm(FormID formID);
    void LoadForm(CForm *pForm);
    void UnloadForm(FormID formID);
    void UnloadForm(CForm *pForm);
    long getFormID();

private:
    void Init();
    explicit CFormFactory();
    static CFormFactory *m_Instance;

    long m_FormID = FORM_BOOT;
    std::thread *m_Thread = nullptr;
    std::string m_Path = "";
    std::string m_File = "";
    std::string m_SCFilePath = "";
    bool m_Exit = false;
    int m_fd = -1;
    int m_wd = -1;
    bool m_bIsInited = false;
};
#endif // __FRAME__CFORMFACTORY_H__