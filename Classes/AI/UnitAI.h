//
//  UnitAI.h
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-18.
//
//

#ifndef __ChineseCabbageTree__UnitAI__
#define __ChineseCabbageTree__UnitAI__

#include <cocos2d.h>

#include "../Units/Unit.h"


USING_NS_CC;

class UnitAI : public CCObject {
protected:
    
public:
    static float gCurrentTime;
    static Unit* getContactUnit(Unit* self, CCArray *units);
    virtual ~UnitAI();
    virtual Unit* findTarget(Unit* self, CCArray* targets) = 0;
    virtual void update(Unit* self, float deltaTime, CCArray* units) = 0;
    CC_SYNTHESIZE(Unit*, mTarget, Target);
    
    virtual float getDistance(Unit* self, Unit* target);
    virtual CCPoint getDirectionUnitVector(CCPoint originVector); // 获取位移法向量
    virtual CCPoint getDirectionVector(Unit* self, float deltaX, float deltaY) = 0; // 获取位移向量
    virtual bool canAttackTarget(int attackRange, float absDeltaX, float absDeltaY) = 0;
};

#endif /* defined(__ChineseCabbageTree__UnitAI__) */
