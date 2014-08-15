//
//  General.cpp
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-25.
//
//

#include "General.h"
#include "../AI/AI.h"

General::~General()
{

}

bool General::init(const char *name, UnitTeam team, int level)
{
    if (!Unit::init(name, team, level)) {
        return false;
    }
    mMaxHealthPoint = 500;
    mHealthPoint = 500;
    mBaseDamagePoint = 45;
    mRandomDamagePoint = 10;
    mMoveSpeed = 80;
    mAttackRange = 40;
    mHurtRecoverInterval = 0.2;
    mAttackInterval = 1.0;
    mAttackPrefixInterval = 0.25;
    mDefendRate = 0.4;
    mAttackCanBeDefended = true;
    mAttackCanHurt = true;
    mUnitAI = DefaultAI::sharedAI();
    this->makeActions();
    
    
    return true;
}

Guanyu::~Guanyu()
{

}

Lvbu::~Lvbu()
{

}

