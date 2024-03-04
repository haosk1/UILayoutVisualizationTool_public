#ifndef ISHADERFACTORY_H__
#define ISHADERFACTORY_H__

#include "Shader.h"

class IShaderFactory
{
  public:
  typedef enum
  {
      SHADER_TYPE_NORMAL= 0,
      SHADER_TYPE_INVALID = 0x7FFFFFFF,
  } ShaderType;
  virtual ~IShaderFactory(){};
  virtual  CShader CreateShader(ShaderType eShaderType) = 0;
};

#endif
