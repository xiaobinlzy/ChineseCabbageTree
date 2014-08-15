//
//  DefaultAI.cpp
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-20.
//
//

#include "DefaultAI.h"



static DefaultAI* gDefaultAI = NULL;

Unit* DefaultAI::findTarget(Unit* self, CCArray* targets)
{
    float minDistance = 99999999;
    Unit* target = NULL;
    for (int i = 0; i < targets->count(); i++) {
        Unit *unit = (Unit*)targets->objectAtIndex(i);
        if (unit->getTeam() == self->getTeam() || !unit->isAlive()) {
            continue;
        }
        float distance = getDistance(self, unit);
        if (distance < minDistance) {
            minDistance = distance;
            target = unit;
        }
    }
    return target;
}

DefaultAI::~DefaultAI()
{
    
}

DefaultAI::DefaultAI()
{
    
}

DefaultAI* DefaultAI::sharedAI()
{
    if (!gDefaultAI) {
        gDefaultAI = new DefaultAI();
    }
    return gDefaultAI;
}

void DefaultAI::update(Unit* self, float deltaTime, CCArray* units)
{
    if (!self->isReady()) {
        return;
    }
    Unit* target = self->getTarget();
    if (!target) {
        self->idle();
        return;
    }
    
    CCPoint selfPosition = self->getPosition();
    CCPoint targetPosition = target->getPosition();
    float deltaX = targetPosition.x - selfPosition.x;
    float deltaY = targetPosition.y - selfPosition.y;
    float absY = abs(deltaY);
    float absX = abs(deltaX);
    if (this->canAttackTarget(self->getAttackRange(), absX, absY)) {
        self->attack();
        self->setTargetPosition(selfPosition);
    } else {
        CCPoint originVector = this->getDirectionUnitVector(this->getDirectionVector(self, deltaX, deltaY));
        this->getTurnedDirectionVector(self, originVector, deltaTime, units);
        self->move();
    }
}

bool DefaultAI::canAttackTarget(int attackRange, float absDeltaX, float absDeltaY)
{
    return absDeltaY <= 20 && absDeltaX <= attackRange;
}

CCPoint DefaultAI::getDirectionVector(Unit* self, float deltaX, float deltaY)
{
    CCPoint selfPosition = self->getPosition();
    float absY = abs(deltaY);
    float absX = abs(deltaX);
    return ccp(absX > self->getAttackRange() ? deltaX : 0, absY > 20 ? deltaY : 0);
}

CCPoint DefaultAI::getTurnedDirectionVector(Unit *self, CCPoint originVector, float deltaTime, CCArray* units)
{
    self->setTargetPosition(this->getMoveTargetPoint(self, originVector, deltaTime));
    int reDirectionTimes = 0;
    CCPoint result = originVector;
    Unit* contactUnit = getContactUnit(self, units);
    Unit* lastContact = NULL;
    while ((contactUnit || (!(self->getObsoleteDirection().equals(CCPointZero)) && self->getObsoleteDirection().equals(result))) && reDirectionTimes < 8) {
        reDirectionTimes++;
        if (contactUnit) {
            lastContact = contactUnit;
        }
        
        if (self->getAIDirection()) {
            if (result.x > 0) {
                if (result. y > 0) {
                    result.x = 0;
                    result.y = 0.5;
                } else if (result.y < 0) {
                    result.x = 1;
                    result.y = 0;
                } else {
                    result.x = 0.7;
                    result.y = 0.35;
                }
                
            } else if (result.x < 0) {
                if (result.y > 0) {
                    result.x = -1;
                    result.y = 0;
                } else if (result.y < 0) {
                    result.x = 0;
                    result.y = -0.5;
                } else {
                    result.x = -0.7;
                    result.y = -0.35;
                }
            } else {
                if (result.y > 0) {
                    result.x = -0.7;
                    result.y = 0.35;
                } else if (result.y < 0) {
                    result.x = 0.7;
                    result.y = -0.35;
                }
            }
        } else {
            if (result.x > 0) {
                if (result. y > 0) {
                    result.x = 1;
                    result.y = 0;
                } else if (result.y < 0) {
                    result.x = 0;
                    result.y = -0.5;
                } else {
                    result.x = 0.7;
                    result.y = -0.35;
                }
                
            } else if (result.x < 0) {
                if (result.y > 0) {
                    result.x = 0;
                    result.y = 0.5;
                } else if (result.y < 0) {
                    result.x = -1;
                    result.y = 0;
                } else {
                    result.x = -0.7;
                    result.y = 0.35;
                }
            } else {
                if (result.y > 0) {
                    result.x = 0.7;
                    result.y = 0.35;
                } else if (result.y < 0) {
                    result.x = -0.7;
                    result.y = -0.35;
                }
            }
        }
        self->setTargetPosition(this->getMoveTargetPoint(self, result, deltaTime));
        contactUnit = getContactUnit(self, units);
    }
    if (reDirectionTimes >= 8) {
        result = ccp(0, 0);
    }
    
    if ((result.x != 0 && result.x == -originVector.x) || (result.y != 0 && result.y == -originVector.y)) {
        if (lastContact && lastContact->isMoving()) {
            self->changeAIDirection();
        }
        self->setObsoleteDirection(originVector);
        
    } else {
        self->setObsoleteDirection(CCPointZero);
    }
    
    return result;
}

CCPoint DefaultAI::getMoveTargetPoint(Unit* self, CCPoint directionVector, float deltaTime)
{
    return ccpAdd(self->getPosition(), ccpMult(directionVector, self->getMoveSpeed() * deltaTime * self->getMoveAction()->getSpeed()));
}
