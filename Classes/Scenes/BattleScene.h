//
//  BattleScene.h
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-21.
//
//

#ifndef __ChineseCabbageTree__BattleScene__
#define __ChineseCabbageTree__BattleScene__

#include <cocos2d.h>
#include "../Units/Unit.h"

using namespace cocos2d;



class BattleScene : public CCLayer, public UnitDelegate {
private:
    CCSpriteBatchNode *mBatchNode;
    CCParticleBatchNode *mParticleBatchNode;
    CCArray* mUnits;
public:
#pragma mark - life cycle
    virtual bool init();
    virtual ~BattleScene();
    virtual void onEnterTransitionDidFinish();
    virtual void update(float deltaTime);
    virtual void onExit();
    static CCScene* scene();
    CREATE_FUNC(BattleScene);
    
#pragma mark - touch & click event
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
//    virtual void keyBackClicked();
//    virtual void keyMenuClicked();
    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
    
#pragma mark - unit delegate
    virtual CCArray* getUnits();
    virtual CCParticleBatchNode* getParticleBatchNode();
};

#endif /* defined(__ChineseCabbageTree__BattleScene__) */
