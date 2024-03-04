#include "FormFactory.h"

CFormFactory *CFormFactory::m_Instance = new CFormFactory();
CFormFactory *CFormFactory::Instance()
{
    return m_Instance;
}

CFormFactory::CFormFactory()
{
    CUIController::Instance()->AddUIInitCallback(this);
}

void CFormFactory::onUIInit()
{
    Init();
}

CFormFactory::~CFormFactory()
{
    m_Exit = true;
    inotify_rm_watch(m_fd, m_wd);
    close(m_fd);
    if (nullptr != m_Thread && m_Thread->joinable())
    {
        m_Thread->join();
        delete m_Thread;
        m_Thread = nullptr;
    }

    for (int index = FORM_INVALID; index <= m_FormID; index++)
    {
        UnloadForm((FormID)index);
    }
}

void CFormFactory::Init()
{
    m_Path = "frame/UI/res/Form/";
    if (nullptr == m_Thread)
    {
        m_Thread = new std::thread([this]
        {
            char buffer[BUF_LEN];
            m_fd = inotify_init();
            m_wd = inotify_add_watch(m_fd, m_Path.c_str(), IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVED_TO | IN_MOVED_FROM);//创建、删除、修改、移入、移出
            while(!m_Exit){
                if(!m_bIsInited){
                    DIR *pDir = nullptr;   //  是头文件<dirent.h>的类型
                    struct dirent *ptr = nullptr;  // opendir、readdir这些都是头文件dirent.h
                    if (!(pDir = opendir(m_Path.c_str()))){
                        printf("m_Path:%p \n",pDir);
                        return;
                    } 
                    while ((ptr = readdir(pDir)) != 0) {
                        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
                            m_File = m_Path + ptr->d_name;  
                            break;
                        }
                    }
                    closedir(pDir);
                    //获取SC背景图
                    std::string scPath = "frame/UI/res/SC/";
                    if (!(pDir = opendir(scPath.c_str()))){
                        return;
                    } 
                    while ((ptr = readdir(pDir)) != 0) {
                        if (strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0) {
                            m_SCFilePath = scPath + ptr->d_name;  
                            break;
                        }
                    }
                    closedir(pDir);

                    //获取Font数据
                    std::string Path = "";
                    CUILib::Instance()->getFilePath("Font.xml", Path);
                    std::map<std::string,Font> fontMap;
                    CUILib::Instance()->parseFontXml(Path.c_str(),fontMap);
                    CUILib::Instance()->SetFontMap(fontMap);
                    //获取String数据
                    CUILib::Instance()->getFilePath("String.xml", Path);
                    std::map<std::string,String> stringMap;
                    CUILib::Instance()->parseStringXml(Path.c_str(),stringMap);
                    CUILib::Instance()->SetStringMap(stringMap);
                    //获取Color数据
                    CUILib::Instance()->getFilePath("color.xml", Path);
                    std::map<std::string,Color> colorMap;
                    CUILib::Instance()->parseColorXml(Path.c_str(),colorMap);
                    CUILib::Instance()->SetColorMap(colorMap);
                    m_bIsInited = true;
                }
 
                //获取form数据
                CEventManager::Instance()->SendEvent([this]{
                    for(int index = FORM_INVALID; index <= m_FormID; index++){
                        UnloadForm((FormID)index);
                    }
                    CForm* rootForm = nullptr;
                    bool res = CUILib::Instance()->parseFormXml(m_File.c_str(), &rootForm);
                    if(res && nullptr != rootForm){
                        rootForm->SetDrawablePath(m_SCFilePath);
                        LoadForm(rootForm);
                    }
                });

                int length = read(m_fd, buffer, BUF_LEN); //阻塞
                if ((ulong)length < EVENT_SIZE) {
                    continue;
                }  
            } 
        });
    }
}

void CFormFactory::LoadForm(FormID formID)
{
    CForm *pForm = CFormFactory::GetForm(formID);
    CUIController::Instance()->GetUIPhysicalLayer()->LoadForm(pForm, formID);
}

void CFormFactory::LoadForm(CForm *pForm)
{
    m_FormID++;
    CUIController::Instance()->GetUIPhysicalLayer()->LoadForm(pForm, m_FormID);
}

void CFormFactory::UnloadForm(FormID formID)
{
    CUIController::Instance()->GetUIPhysicalLayer()->UnloadForm(formID);
}

void CFormFactory::UnloadForm(CForm *pForm)
{
    CUIController::Instance()->GetUIPhysicalLayer()->UnloadForm(pForm);
}

long CFormFactory::getFormID()
{
    return m_FormID;
}
