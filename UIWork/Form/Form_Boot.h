
/**
 * frame
 *
 * UIForm基类
**/

#ifndef FORM_BOOT_H
#define FORM_BOOT_H

#include <vector>

#include "UIFW/Form.h"
#include "Actor/LeaderActor.h"

class CForm_Boot : public CForm
{
public:
    explicit CForm_Boot(){};
    virtual ~CForm_Boot(){};

    virtual void Load(void);

protected:
    virtual void DispatchDraw(CUICanvasInterface &stCanvas);

private:
    CForm_Boot(const CForm_Boot &);
    CForm_Boot &operator=(const CForm_Boot &);

    CLeaderActor* CreatLeader(CLeaderActor::LeaderIndex index);
    CActor* CreatAirWall();
    CActor* CreatSoldier();

    bool CheckIsCollision(CActor* pActor);
};

#endif /*#ifndef FORM_BOOT_H*/
