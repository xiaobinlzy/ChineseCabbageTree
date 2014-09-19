//
//  BattleScene.cpp
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-21.
//
//

#include "BattleScene.h"

#include "../AI/DefaultAI.h"
#include "../Units/Soldier.h"
#include "../Units/General.h"
#include "../Particle/ParticleStar.h"
#include "../base/FirePngData.h"

static Texture2D* getStarTexture()
{
    Texture2D* texture = nullptr;
    Image* image = nullptr;
    unsigned char *buffer = nullptr;
    unsigned char *deflated = nullptr;
    do {
        const std::string key = "/__starPngData";
        texture = Director::getInstance()->getTextureCache()->getTextureForKey(key);
        CC_BREAK_IF(texture != nullptr);
        std::string textureData = __starPngData;
        CCASSERT(!textureData.empty(), "");
        
        auto dataLen = textureData.size();
            // if it fails, try to get it from the base64-gzipped data
            int decodeLen = base64Decode((unsigned char*)textureData.c_str(), (unsigned int)dataLen, &buffer);
            CCASSERT( buffer != nullptr, "CCParticleSystem: error decoding textureImageData");
            CC_BREAK_IF(!buffer);
            
            ssize_t deflatedLen = ZipUtils::inflateMemory(buffer, decodeLen, &deflated);
            CCASSERT( deflated != nullptr, "CCParticleSystem: error ungzipping textureImageData");
            CC_BREAK_IF(!deflated);
            
            // For android, we should retain it in VolatileTexture::addImage which invoked in Director::getInstance()->getTextureCache()->addUIImage()
            image = new Image();
            bool isOK = image->initWithImageData(deflated, deflatedLen);
            CCASSERT(isOK, "CCParticleSystem: error init image with Data");
            CC_BREAK_IF(!isOK);
            
            texture = Director::getInstance()->getTextureCache()->addImage(image, key);
    } while (0);
    CC_SAFE_RELEASE(image);
    return texture;
}

static Texture2D* getDefaultTexture()
{
    Texture2D* texture = nullptr;
    Image* image = nullptr;
    do
    {
        bool ret = false;
        const std::string key = "/__firePngData";
        texture = Director::getInstance()->getTextureCache()->getTextureForKey(key);
        CC_BREAK_IF(texture != nullptr);
        
        image = new Image();
        CC_BREAK_IF(nullptr == image);
        ret = image->initWithImageData(__firePngData, sizeof(__firePngData));
        CC_BREAK_IF(!ret);
        
        texture = Director::getInstance()->getTextureCache()->addImage(image, key);
    } while (0);
    
    CC_SAFE_RELEASE(image);
    
    return texture;
}


BattleScene::~BattleScene() {
	CC_SAFE_RELEASE(mUnits);
    
}

void BattleScene::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACK) {
        CCDirector::sharedDirector()->end();
        
        exit(0);
    }
}

bool BattleScene::init() {
	if (!CCLayer::init()) {
		return false;
	}
	setKeypadEnabled(true);
	CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFrames();
	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("unit.plist");
	mBatchNode = CCSpriteBatchNode::create("unit.png");
	this->addChild(mBatchNode);
    
    mParticleBatchNode = CCParticleBatchNode::createWithTexture(getStarTexture());
    this->addChild(mParticleBatchNode, 1);
    
	mUnits = CCArray::create();
    CC_SAFE_RETAIN(mUnits);
    
	Unit *guanYu = Guanyu::create("guanyu", UnitTeamBlue, 1);
    guanYu->setDelegate(this);
	guanYu->setPosition(ccp(50, 160));
	mBatchNode->addChild(guanYu);
	mUnits->addObject(guanYu);
    
	Unit *lvBu = Lvbu::create("lvbu", UnitTeamRed, 1);
	lvBu->setDelegate(this);
	lvBu->setPosition(ccp(430, 160));
	mBatchNode->addChild(lvBu);
	mUnits->addObject(lvBu);
    
	for (int i = 0; i < 8; i++) {
		Unit* rUnit = LightFootman::create(UnitTeamRed, 1);
        do {
            rUnit->setPosition(ccp(rand() % 200 + 240, rand() % 240 + 40));
        } while (UnitAI::getContactUnit(rUnit, mUnits) != NULL);
        mBatchNode->addChild(rUnit);
        rUnit->setDelegate(this);
        mUnits->addObject(rUnit);
	}
	for (int i = 0; i < 8; i++) {
		Unit* bUnit = LightFootman::create(UnitTeamBlue, 1);
        do {
            bUnit->setPosition(ccp(rand() % 200 + 40, rand() % 240 + 40));
        } while (UnitAI::getContactUnit(bUnit, mUnits) != NULL);
        mBatchNode->addChild(bUnit);
        bUnit->setDelegate(this);
        mUnits->addObject(bUnit);
	}
    for (int i = 0; i < 2; i++) {
        Unit *mage = Counsellor::create(UnitTeamBlue, 1);
        do {
            mage->setPosition(ccp(rand() % 200 + 40, rand() % 240 + 40));
        } while (UnitAI::getContactUnit(mage, mUnits) != NULL);
        mBatchNode->addChild(mage);
        mUnits->addObject(mage);
        mage->setDelegate(this);
    }
    for (int i = 0; i < 2; i++) {
        Unit *mage = Counsellor::create(UnitTeamRed, 1);
        do {
            mage->setPosition(ccp(rand() % 200 + 240, rand() % 240 + 40));
        } while (UnitAI::getContactUnit(mage, mUnits) != NULL);
        mBatchNode->addChild(mage);
        mUnits->addObject(mage);
        mage->setDelegate(this);
    }
	return true;
}

CCParticleBatchNode* BattleScene::getParticleBatchNode()
{
    return mParticleBatchNode;
}


CCScene* BattleScene::scene() {
	CCScene *scene = CCScene::create();
	BattleScene *layer = BattleScene::create();
	scene->addChild(layer);
	return scene;
}

void BattleScene::onExit() {
	unscheduleUpdate();
	CCLayer::onExit();
}

CCArray* BattleScene::getUnits()
{
    return mUnits;
}

bool BattleScene::onTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(1, BattleScene::scene()));
	return true;
}

void BattleScene::onEnterTransitionDidFinish() {
	CCLayer::onEnterTransitionDidFinish();
	UnitAI::gCurrentTime = 0;
	scheduleUpdate();
	this->setTouchEnabled(true);
	this->setTouchMode(kCCTouchesOneByOne);
}

void BattleScene::update(float deltaTime) {
	UnitAI::gCurrentTime += deltaTime;
	CCLayer::update(deltaTime);
	Unit* unit = NULL;
	for (int i = 0; i < mUnits->count(); i++) {
		unit = (Unit*) mUnits->objectAtIndex(i);
		unit->findTarget(mUnits, UnitAI::gCurrentTime);
		unit->updateUnit(deltaTime);
	}
	for (int i = 0; i < mUnits->count(); i++) {
		unit = (Unit*) mUnits->objectAtIndex(i);
		unit->doAction();
		mBatchNode->reorderChild(unit, -unit->getPosition().y);
	}
}
