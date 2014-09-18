//
//  Units.cpp
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-17.
//
//

#include "Unit.h"
#include "../AI/UnitAI.h"

const static CCSize unitSize = CCSizeMake(32, 16);

#pragma mark - 生命周期
void Unit::cleanup()
{
    CC_SAFE_RELEASE(mMoveAction);
    CC_SAFE_RELEASE(mAttackActioin);
    CC_SAFE_RELEASE(mIdleAction);
    CC_SAFE_RELEASE(mSpellAction);
    CC_SAFE_RELEASE(mHurtAction);
    CC_SAFE_RELEASE(mDieAction);
    CC_SAFE_RELEASE(mUnitName);
    CC_SAFE_RELEASE(mDefendAction);
    CCSprite::cleanup();
}


Unit::~Unit()
{
    
}


bool Unit::init(const char* name, UnitTeam team, int level)
{
    if (!CCSprite::init()) {
        return false;
    }
    mAIDirection = rand() % 2;
    mState = UnitStateNone;
    mTeam = team;
    
    mUnitName = CCString::create(name);
    mUnitName->retain();
    this->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_stand.png", name)->getCString()));
    
    return true;
}


#pragma mark - 动画初始化
void Unit::makeActions()
{
    mDamage.canDefend = mAttackCanBeDefended;
    mDamage.canHurt = mAttackCanHurt;
    mDamage.fromUnit = this;
    this->initAttackAction();
    this->initMoveAction();
    this->initIdleAction();
    this->initHurtAction();
    this->initDieAction();
    this->initDefendAction();
    this->idle();
    ccp(1, 2);
    this->makeDirection(0);
}

void Unit::initAttackAction()
{
    // 攻击动画
    Vector<SpriteFrame *> *attackFrames = new Vector<SpriteFrame *>(4);
    for (int i = 0; i < 4; i++) {
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_attack_%02d.png", mUnitName->getCString(), i)->getCString());
//        attackFrames->addObject(frame);
        attackFrames->pushBack(frame);
    }
    CCAnimate *attackAnimate = CCAnimate::create(CCAnimation::createWithSpriteFrames(*attackFrames, 0.1));
    CCDelayTime *damageDelay = CCDelayTime::create(mAttackPrefixInterval);
    CCCallFunc *attackMakeDamageCall = CCCallFunc::create(this, callfunc_selector(Unit::makeDamage));
    CCSpawn *attackSpawn = CCSpawn::create(attackAnimate, CCSequence::create(damageDelay, attackMakeDamageCall, NULL), NULL);
    CCActionInterval *attackDelay = CCDelayTime::create(mAttackInterval - MAX(0.4, mAttackPrefixInterval));
    CCCallFunc *attackCall = CCCallFunc::create(this, callfunc_selector(Unit::idle));
    this->setAttackAction(CCSpeed::create(CCSequence::create(attackSpawn, attackDelay, attackCall, NULL), 1.0f));
    delete attackFrames;
}

void Unit::initMoveAction()
{
    // 移动动画
    Vector<SpriteFrame *> *moveFrames = new Vector<SpriteFrame *>(2);
    for (int i = 0; i < 2; i++) {
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_move_%02d.png", mUnitName->getCString(), i)->getCString());
        moveFrames->pushBack(frame);
    }
    this->setMoveAction(CCSpeed::create(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(*moveFrames, 0.2f))), 1.0f));
    delete moveFrames;
}

void Unit::initIdleAction()
{
    // 待命动画
    Vector<SpriteFrame *> *idleFrames = new Vector<SpriteFrame *>(1);
    CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_stand.png", mUnitName->getCString())->getCString());
    idleFrames->pushBack(frame);
    this->setIdleAction(CCSpeed::create(CCRepeatForever::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(*idleFrames, 0.8f))), 1.0f));
    delete idleFrames;
}

void Unit::initDieAction()
{
    // 死亡动画
    Vector<SpriteFrame *> *dieFrames = new Vector<SpriteFrame *>(2);
    for (int i = 0; i < 2; i++) {
        CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_die_%02d.png", mUnitName->getCString(), i)->getCString());
        dieFrames->pushBack(frame);
    }
    CCRepeat *dieAnimate = CCRepeat::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(*dieFrames, 0.4f)), -1);
    this->setDieAction(CCSpeed::create(CCSpawn::create(dieAnimate, CCSequence::create(CCDelayTime::create(2.4), CCBlink::create(2, 8), CCHide::create(), NULL), NULL), 1.0f));
    delete dieFrames;
}

void Unit::initHurtAction()
{
    // 受伤动画
    Vector<SpriteFrame *> *hurtFrames = new Vector<SpriteFrame *>(1);
    CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_hurt.png", mUnitName->getCString())->getCString());
    hurtFrames->pushBack(frame);
    this->setHurtAction(CCSpeed::create(CCSequence::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(*hurtFrames, mHurtRecoverInterval)), CCCallFunc::create(this, callfunc_selector(Unit::idle)), NULL), 1.0f));
    delete hurtFrames;
}

void Unit::initDefendAction()
{
    // 防御动画
    Vector<SpriteFrame *> *defendFrames = new Vector<SpriteFrame *>(1);
    CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(CCString::createWithFormat("%s_defend.png", mUnitName->getCString())->getCString());
    defendFrames->pushBack(frame);
    this->setDefendAction(CCSpeed::create(CCSequence::create(CCAnimate::create(CCAnimation::createWithSpriteFrames(*defendFrames, mHurtRecoverInterval)), CCCallFunc::create(this, callfunc_selector(Unit::idle)), NULL), 1.0f));
    delete defendFrames;
}

#pragma mark - 动作

void Unit::defend()
{
    if (mState != UnitStateDie && mState != UnitStateException && mState != UnitStateSpell) {
        mState = UnitStateDefend;
        this->runUnitAction(mDefendAction);
    }
}

void Unit::idle()
{
    if (mState != UnitStateIdle) {
        mState = UnitStateIdle;
        this->runUnitAction(mIdleAction);
    }
}

void Unit::move()
{
    if (mState == UnitStateIdle) {
        mState = UnitStateMove;
        this->runUnitAction(mMoveAction);
    }
    if (mState == UnitStateMove) {
        this->makeDirection((mTarget->getPosition().x - this->getPosition().x));
    }
}


void Unit::attack()
{
    if ((mState == UnitStateIdle || mState == UnitStateMove) && mTarget && UnitAI::gCurrentTime >= mNextAttackTime) {
        mState = UnitStateAttack;
        this->runUnitAction(mAttackActioin);
    }
    if (mState == UnitStateAttack) {
        this->makeDirection((mTarget->getPosition().x - this->getPosition().x));
    }
    this->changeAIDirection();
}

void Unit::die()
{
    if (mState != UnitStateDie) {
        mState = UnitStateDie;
        this->runUnitAction(mDieAction);
    }
}

void Unit::hurt()
{
    if (mState != UnitStateDie) {
        mState = UnitStateHurt;
        this->runUnitAction(mHurtAction);
    }
}


#pragma mark - 状态

bool Unit::isMoving()
{
    return mState == UnitStateMove;
}

bool Unit::isAlive()
{
    return mState != UnitStateDie;
}

bool Unit::isReady()
{
    return isAlive() && (mState == UnitStateIdle || mState == UnitStateMove);
}

#pragma mark - 碰撞和寻路
CCRect Unit::getBoundingRect()
{
    CCPoint point = this->getTargetPosition();
    return CCRectMake(point.x - ((int)unitSize.width >> 1), point.y - ((int)unitSize.height >> 1), unitSize.width, unitSize.height);
}

void Unit::changeAIDirection()
{
    mAIDirection = !mAIDirection;
}


void Unit::makeDirection(float xDirection)
{
    if (xDirection > 0) {
        this->setScaleX(-1);
    } else if (xDirection < 0) {
        this->setScaleX(1);
    } else {
        
        setScaleX(mTeam == UnitTeamBlue ? -1 : 1);
    }
}




void Unit::setPosition(CCPoint position)
{
    CCSprite::setPosition(position);
    this->setTargetPosition(position);
}

#pragma mark - AI

void Unit::updateUnit(float deltaTime)
{
    if (mUnitAI && mDelegate) {
        mUnitAI->update(this, deltaTime, mDelegate->getUnits());
    }
}


void Unit::findTarget(CCArray* targets, float currentTime)
{
    if (mUnitAI && (mState == UnitStateIdle || mState == UnitStateMove)) {
        Unit* target = mUnitAI->findTarget(this, targets);
        this->setTarget(target);
    }
}

#pragma mark - 自定义

void Unit::stopUnitAction()
{
    if (mCurrentAction) {
        this->stopAction(mCurrentAction);
        mCurrentAction = NULL;
    }
}

void Unit::runUnitAction(CCAction* action)
{
    this->stopUnitAction();
    mCurrentAction = action;
    this->runAction(action);
}





void Unit::markDamage(UnitDamage damage)
{
    mTakenDamages[mNumberOfDamages++] = damage;
}






void Unit::makeDamage()
{
    if (mTarget && mTarget->isAlive() && mState == UnitStateAttack) {
        mNextAttackTime = UnitAI::gCurrentTime + (mAttackInterval - MAX(0.4, mAttackPrefixInterval)) * mAttackActioin->getSpeed();
        mDamage.damagePoint = mBaseDamagePoint + (mRandomDamagePoint == 0 ? 0 : rand() % mRandomDamagePoint);
        mTarget->markDamage(mDamage);
    }
}



void Unit::doAction()
{
    bool isDamageTaken = mNumberOfDamages > 0;
    for (int i = 0; i < mNumberOfDamages; i++) {
        if (mTakenDamages[i].canDefend && ((mTakenDamages[i].fromUnit->getPosition().x - getPosition().x) > 0 ? getScaleX() < 0 : getScaleX() > 0) && (rand() % 100) / 100.0f < mDefendRate) {
            this->defend();
        } else {
            mHealthPoint -= mTakenDamages[i].damagePoint;
            if (mHealthPoint > 0) {
                if (mTakenDamages[i].canHurt) {
                    this->hurt();
                }
            } else {
                this->die();
            }
        }
    }
    mNumberOfDamages = 0;
    if (!isDamageTaken) {
        this->setPosition(mTargetPosition);
    }
}




