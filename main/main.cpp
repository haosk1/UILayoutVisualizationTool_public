#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include "mainctrl/CMainCtrlProxy.h"
#include "mainctrl/CMainCtrl_Interface.h"

class Test
{
private:
    int m_Window_Width = 0;
    int m_Window_Height = 0;

public:
    explicit Test() = default;
    ~Test() = default;

    bool init();
    void run();
};

bool Test::init()
{
    std::ifstream file("./config.json");
    if (!file.is_open()) {
        return false;
    }

    std::map<std::string, std::string> parsedJson;
    std::string line;
    while (std::getline(file, line)) {
        size_t key_start = line.find('"');
        if (key_start != std::string::npos) {
            size_t key_end = line.find('"', key_start + 1);
            std::string key = line.substr(key_start + 1, key_end - key_start - 1);

            size_t value_start = line.find(':', key_end);
            if (value_start != std::string::npos) {
                size_t value_end = line.find_last_of(',');
                if (value_end == std::string::npos) {
                    value_end = line.size();
                }
                std::string value = line.substr(value_start + 1, value_end - value_start - 1);
                parsedJson[key] = value;
            }
        }
    }

    file.close();

    std::map<std::string, std::string>::iterator iterWidth= parsedJson.find("Window_Width");
    if (iterWidth != parsedJson.end())
    {
        m_Window_Width = atoi(iterWidth->second.c_str());
    }
    std::map<std::string, std::string>::iterator  iterHeight = parsedJson.find("Window_Height");
    if (iterHeight != parsedJson.end())
    {
        m_Window_Height = atoi(iterHeight->second.c_str());
    }
    if(0 != m_Window_Width && 0 != m_Window_Height){
        return true;
    }
    return false;
}

void Test::run()
{
    if (frame::CMainCtrlProxy::getMainCtrl(frame::MainCtrlType::MAINCTRL_TYPE_LINUXPC)->Init(m_Window_Width, m_Window_Height,"haosk"))
    {
        frame::CMainCtrlProxy::getMainCtrl(frame::MainCtrlType::MAINCTRL_TYPE_LINUXPC)->LoopMsg();
    }
    frame::CMainCtrlProxy::getMainCtrl(frame::MainCtrlType::MAINCTRL_TYPE_LINUXPC)->UnInit();
}

int main(int argc, char** argv){
    Test test;
    if(test.init()){
        test.run();
    }
    return 1;
}