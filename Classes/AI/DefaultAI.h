//
//  DefaultAI.h
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-20.
//
//

#ifndef __ChineseCabbageTree__DefaultAI__
#define __ChineseCabbageTree__DefaultAI__

#include <cocos2d.h>
#include "UnitAI.h"

USING_NS_CC;

class DefaultAI : public UnitAI {
private:
public:
    static DefaultAI* sharedAI();
    DefaultAI();
    virtual ~DefaultAI();
    virtual Unit* findTarget(Unit* self, CCArray* targets);
    virtual void update(Unit* self, float deltaTime, CCArray* units);
    virtual CCPoint getMoveTargetPoint(Unit* self, CCPoint directionVector, float deltaTime);
    virtual CCPoint getTurnedDirectionVector(Unit* self, CCPoint originVector, float deltaTime, CCArray* units);
    virtual CCPoint getDirectionVector(Unit* self, float deltaX, float deltaY);
    virtual bool canAttackTarget(int attackRange, float absDeltaX, float absDeltaY);
};

#endif /* defined(__ChineseCabbageTree__DefaultAI__) */
