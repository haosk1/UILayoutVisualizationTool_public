#include "ScrollBarDecTurnPage.h"

void CScrollBarDecTurnPage::SetActorAttribute(const char *attributeName,
                                              const char *attributeValue)
{
  CFrameLayout::SetActorAttribute(attributeName, attributeValue);
  // 处理自己的 SetActorAttribute
  if (!strcmp(attributeName, "ScrollBarLayout"))
  {
    std::string stdScrollBarLayoutPath = "";
    std::string stdtemp = attributeValue;
    stdtemp = stdtemp.substr(1, stdtemp.length() - 1);
    CUILib::Instance()->getFilePath(stdtemp + ".xml", stdScrollBarLayoutPath);
    CUILib::Instance()->parseScrollBarXml(stdScrollBarLayoutPath.c_str(), this);
  }
}