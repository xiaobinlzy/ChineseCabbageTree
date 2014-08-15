//
//  UnitAI.cpp
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-18.
//
//

#include "UnitAI.h"

UnitAI::~UnitAI()
{
    
}

float UnitAI::gCurrentTime = 0;

float UnitAI::getDistance(Unit* self, Unit* target)
{
    if (!target || !self) {
        return 0;
    }
    CCPoint pointA = self->getPosition();
    CCPoint pointB = target->getPosition();
    int deltaX = pointA.x - pointB.x;
    int deltaY = (pointA.y - pointB.y) * 2;
    return sqrtf(deltaX * deltaX + deltaY * deltaY);
}

CCPoint UnitAI::getDirectionUnitVector(CCPoint originVector)
{
    float x;
    float y;
    if (originVector.x > 0) {
        if (originVector.y == 0) {
            x = 1;
            y = 0;
        } else {
            x = 0.7;
            y = originVector.y > 0 ? 0.35 : -0.35;
        }
    } else if (originVector.x < 0) {
        if (originVector.y == 0) {
            x = -1;
            y = 0;
        } else {
            x = -0.7;
            y = originVector.y > 0 ? 0.35 : -0.35;
        }
    } else {
        x = 0;
        y = originVector.y > 0 ? 0.5 : -0.5;
    }
    return ccp(x, y);
}


Unit* UnitAI::getContactUnit(Unit* self, CCArray* units)
{
    Unit* result = NULL;
    Unit* unit = NULL;
    for (int i = 0; i < units->count(); i++) {
        unit = (Unit*)units->objectAtIndex(i);
        if (unit != self && unit->isAlive() && self->getBoundingRect().intersectsRect(unit->getBoundingRect())) {
            result = unit;
            break;
        }
    }
    return result;
}