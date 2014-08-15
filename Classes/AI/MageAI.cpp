//
//  MageAI.cpp
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-26.
//
//

#include "MageAI.h"


static MageAI* gMageAI = NULL;

MageAI::~MageAI()
{

}

MageAI* MageAI::sharedAI()
{
    if (!gMageAI) {
        gMageAI = new MageAI();
    }
    return gMageAI;
}

CCPoint MageAI::getDirectionVector(Unit* self, float deltaX, float deltaY)
{
    CCPoint selfPosition = self->getPosition();
    float absY = abs(deltaY);
    float absX = abs(deltaX);
    int attackRange = self->getAttackRange();
    return ccp(absX > attackRange ? deltaX : absX < attackRange >> 1 ? -deltaX : 0, absY > 20 ? deltaY : 0);
}