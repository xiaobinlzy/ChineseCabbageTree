//
//  Army.cpp
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-25.
//
//

#include "Soldier.h"
#include "../AI/AI.h"

#pragma mark - 轻步兵
LightFootman::~LightFootman()
{
    
}

bool LightFootman::init(const char *resource, UnitTeam team, int level)
{
    if (!Unit::init(resource, team, level)) {
        return false;
    }
    mMaxHealthPoint = 100;
    mHealthPoint = 100;
    mBaseDamagePoint = 18;
    mRandomDamagePoint = 4;
    mMoveSpeed = 40;
    mAttackRange = 35;
    mHurtRecoverInterval = 0.4;
    mAttackInterval = 1.5;
    mAttackPrefixInterval = 0.3;
    mDefendRate = 0.15;
    mAttackCanBeDefended = true;
    mAttackCanHurt = true;
    mUnitAI = DefaultAI::sharedAI();
    this->makeActions();
    
    return true;
}

#pragma mark - 策士
Counsellor::~Counsellor()
{
}


bool Counsellor::init(const char *resource, UnitTeam team, int level)
{
    if (!Unit::init(resource, team, level)) {
        return false;
    }
    
    mMaxHealthPoint = 60;
    mHealthPoint = 60;
    mBaseDamagePoint = 35;
    mRandomDamagePoint = 10;
    mMoveSpeed = 40;
    mAttackRange = 150;
    mHurtRecoverInterval = 0.8;
    mAttackInterval = 3;
    mAttackPrefixInterval = 1.5;
    mDefendRate = 0.05;
    mAttackCanBeDefended = false;
    mAttackCanHurt = true;
    mUnitAI = MageAI::sharedAI();
    this->makeActions();
    
    return true;
}

CCRect Counsellor::getDamageArea()
{
    CCPoint position = mTarget->getPosition();
    return CCRectMake(position.x - 40, position.y - 30, 80, 60);
}

void Counsellor::makeDamage()
{
    if (mTarget && mState == UnitStateAttack && mDelegate) {
        mNextAttackTime = UnitAI::gCurrentTime + (mAttackInterval - MAX(0.4, mAttackPrefixInterval)) * mAttackActioin->getSpeed();
        CCPoint position = mTarget->getPosition();
        CCArray *units = mDelegate->getUnits();
        for (int i = 0; i < units->count(); i++) {
            Unit* unit = (Unit*)units->objectAtIndex(i);
            if (unit->isAlive() && unit->getTeam() != mTeam && this->getDamageArea().containsPoint(unit->getPosition())) {
                mDamage.damagePoint = mBaseDamagePoint + (mRandomDamagePoint == 0 ? 0 : rand() % mRandomDamagePoint);
                unit->markDamage(mDamage);
            }
        }
        CCParticleFire *particle = CCParticleFire::create();
        particle->setEmitterMode(kCCParticleModeGravity);
        particle->setLife(1);
        particle->setDuration(1);
        particle->setAutoRemoveOnFinish(true);
        particle->setPosition(position);
        position.y -= 20;
        if (mTeam == UnitTeamRed) {
            particle->setStartColor(ccc4f(0.24f, 0.75f, 0.88f, 1.0f));
        }
        mDelegate->getParticleBatchNode()->addChild(particle);
    }
    
}
