#ifndef SHADER_H__
#define SHADER_H__

#include <string>

class CShader
{
public:
    CShader(){};
    virtual ~CShader(){};

    void SetVertexShader(const std::string & shader);
    void SetFragmentShader(const std::string & shader);

    std::string GetVertexShader();
    std::string GetFragmentShader();

private:
    std::string m_VertexShader;
    std::string m_FragmentShader;
};

#endif
