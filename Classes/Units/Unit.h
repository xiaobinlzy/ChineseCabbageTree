//
//  Units.h
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-17.
//
//

#ifndef __ChineseCabbageTree__Unit__
#define __ChineseCabbageTree__Unit__

#include <cocos2d.h>
#include "UnitDelegate.h"
#include <particle_nodes/CCParticleSystem.h>
#include <particle_nodes/firePngData.h>



#define UNIT_CREATE(__TYPE__,__NAME__) \
static __TYPE__* create(UnitTeam team, int level) \
{ \
__TYPE__ *pRet = new __TYPE__(); \
const char* resource = NULL; \
switch (team) { \
case UnitTeamBlue: \
resource = CCString::createWithFormat("%s_b", __NAME__)->getCString(); \
break; \
case UnitTeamRed: \
resource = CCString::createWithFormat("%s_r", __NAME__)->getCString(); \
break; \
} \
if (pRet && pRet->init(resource, team, level)) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

USING_NS_CC;

class UnitAI;
// 定义单位状态
typedef enum {
    UnitStateNone,
    UnitStateIdle,  // 待命
    UnitStateMove,  // 移动
    UnitStateAttack,    // 攻击
    UnitStateDie,   // 死亡
    UnitStateHurt,  // 受伤
    UnitStateDefend,    // 防御
    UnitStateSpell,  // 施放技能
    UnitStateException  // 异常
} UnitState;

typedef enum {
    UnitExceptionStateChaos,    // 混乱
    UnitExceptionStateFear, // 恐惧
    UnitExceptionStateStun  // 眩晕
} UnitExceptionState;   // 异常状态

// 定义单位所属的队伍
typedef enum {
    UnitTeamBlue,
    UnitTeamRed
} UnitTeam;

class Unit;


typedef struct {
    int damagePoint;
    Unit* fromUnit;
    bool canDefend;
    bool canHurt;
} UnitDamage;

class UnitAI;




class Unit : public CCSprite {

protected:
    UnitState mState;
    CCAction* mCurrentAction;
    bool mAttackCanBeDefended;
    bool mAttackCanHurt;
    UnitDamage mDamage;
    UnitDamage mTakenDamages[20];
    int mNumberOfDamages;

public:
#pragma mark - 生命周期
    virtual ~Unit();
    virtual bool init(const char* name, UnitTeam team, int level);
    virtual void cleanup(); // 重写函数，释放动作对象
    
#pragma mark - 寻路和碰撞
    virtual void changeAIDirection();
    // AI寻路绕过障碍物的方向
    CC_SYNTHESIZE_READONLY(int, mAIDirection, AIDirection);
    // 移动帧的目标坐标
    CC_SYNTHESIZE(CCPoint, mTargetPosition, TargetPosition);
    virtual void setPosition(CCPoint position);
    virtual CCRect getBoundingRect();
    CC_SYNTHESIZE(CCPoint, mObsoleteDirection, ObsoleteDirection);
    
#pragma mark - 单位属性
    CC_SYNTHESIZE_READONLY(CCString*, mName, Name);
    // 生命值
    CC_SYNTHESIZE(int, mHealthPoint, HealthPoint);
    
    // 基础攻击力
    CC_SYNTHESIZE(int, mBaseDamagePoint, BaseDamagePoint);
    
    // 浮动攻击力
    CC_SYNTHESIZE(int, mRandomDamagePoint, RandomDamagePoint);
    
    // 移动速度
    CC_SYNTHESIZE(int, mMoveSpeed, MoveSpeed);
    
    // 攻击距离
    CC_SYNTHESIZE(int, mAttackRange, AttackRange);
    
    // 攻击目标
    CC_SYNTHESIZE(Unit*, mTarget, Target);
    
    // 所属队伍
    CC_SYNTHESIZE(UnitTeam, mTeam, Team);
        
    // 受到攻击后的恢复时间
    CC_SYNTHESIZE(float, mHurtRecoverInterval, HurtRecoverInterval);
    
    // 攻击间隔时间
    CC_SYNTHESIZE(float, mAttackInterval, AttackInterval);
    
    // 攻击前摇时间
    CC_SYNTHESIZE(float, mAttackPrefixInterval, AttackPrefixInterval);
    
    // 招架概率
    CC_SYNTHESIZE(float, mDefendRate, DefendRate);
    
    // 单位AI
    CC_SYNTHESIZE(UnitAI*, mUnitAI, UnitAI);
    
    // 最大生命值
    CC_SYNTHESIZE(int, mMaxHealthPoint, MaxHealthPoint);
    
#pragma mark - 动作动画
    /* 动作动画 */
    CC_SYNTHESIZE_RETAIN(CCSpeed*, mMoveAction, MoveAction);
    CC_SYNTHESIZE_RETAIN(CCSpeed*, mIdleAction, IdleAction);
    CC_SYNTHESIZE_RETAIN(CCSpeed*, mDieAction, DieAction);
    CC_SYNTHESIZE_RETAIN(CCSpeed*, mHurtAction, HurtAction);
    CC_SYNTHESIZE_RETAIN(CCSpeed*, mSpellAction, SpellAction);
    CC_SYNTHESIZE_RETAIN(CCSpeed*, mAttackActioin, AttackAction);
    CC_SYNTHESIZE_RETAIN(CCSpeed*, mDefendAction, DefendAction);
    virtual void makeActions();
    virtual void idle();    // 待命动作
    virtual void move();    // 移动动作
    virtual void attack();  // 攻击动作
    virtual void die(); // 死亡动作
    virtual void hurt();    // 受伤动作
    virtual void defend();  // 防御
    virtual void runUnitAction(CCAction* action);   // 开始单位新动作动作，会停止之前的动作
    virtual void stopUnitAction();  // 停止单位动作
    
    CC_SYNTHESIZE(UnitDelegate*, mDelegate, Delegate);
    
#pragma mark - 状态
    virtual bool isReady();  // 是否等待命令
    virtual bool isAlive(); // 是否还活着
    virtual bool isMoving();
    
#pragma mark - AI
    virtual void findTarget(CCArray* targets, float currentTime);
    virtual void updateUnit(float deltaTime);  // update回调函数，dt为时间微分
    
    CC_SYNTHESIZE_READONLY(float, mNextAttackTime, NextAttackTime);
    
    virtual void makeDamage();  // 造成伤害
    
    virtual void markDamage(UnitDamage damage); // 标记收到伤害，将在统计之后做出伤害计算。
    virtual void doAction();    // 执行这一帧的动作
    float mNextTargetTime;  // 下一次寻找目标的时间
    
protected:
    virtual void initAttackAction();
    virtual void initMoveAction();
    virtual void initIdleAction();
    virtual void initHurtAction();
    virtual void initDieAction();
    virtual void initDefendAction();
    virtual void makeDirection(float xDirection);    // 调整纹理贴图左右朝向
};

#endif /* defined(__ChineseCabbageTree__Units__) */
