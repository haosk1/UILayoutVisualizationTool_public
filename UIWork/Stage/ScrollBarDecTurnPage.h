/**
 * frame
 *
 * 可视化元器件之ScrollBarDecTurnPage
 **/
#ifndef __FRAME__SCROLLBARDECTURNPAGE_H__
#define __FRAME__SCROLLBARDECTURNPAGE_H__

#include "FrameLayout.h"

class CScrollBarDecTurnPage : public CFrameLayout
{
public:
  explicit CScrollBarDecTurnPage(){};
  virtual ~CScrollBarDecTurnPage(){};

  virtual void SetActorAttribute(const char *attributeName, const char *attributeValue) override;
};
#endif // __FRAME__SCROLLBARDECTURNPAGE_H__