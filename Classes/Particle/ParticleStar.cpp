//
//  ParticleStar.cpp
//  ChineseCabbageTree
//
//  Created by DLL on 14-9-19.
//
//

#include "ParticleStar.h"

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


ParticleStar* ParticleStar::create()
{
    ParticleStar* ret = new ParticleStar();
    if (ret && ret->initWithTotalParticles(100))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

ParticleStar* ParticleStar::createWithTotalParticles(int numberOfParticles)
{
    ParticleStar* ret = new ParticleStar();
    if (ret && ret->initWithTotalParticles(numberOfParticles))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool ParticleStar::initWithTotalParticles(int numberOfParticles)
{
    if( ParticleSystemQuad::initWithTotalParticles(numberOfParticles) )
    {
        // duration
        _duration = 3;
        _positionType = PositionType::GROUPED;
        // Gravity Mode
        this->_emitterMode = Mode::GRAVITY;
        
        // Gravity Mode: gravity
        this->modeA.gravity = Vec2(0, 0);
        
        // Gravity Mode: radial acceleration
        this->modeA.radialAccel = 0;
        this->modeA.radialAccelVar = 0;
        // Gravity Mode: speed of particles
        this->modeA.speed = 60;
        this->modeA.speedVar = 20;
        
        // starting angle
        _angle = 270;
        _angleVar = 20;
        
        _startSpin = 0;
        _startSpinVar = 45;

        
        // emitter position
        this->_posVar = Vec2(50, 50);
        
        // life of particles
        _life = 1;
        _lifeVar = 0.2f;
        
        
        // size, in pixels
        _startSize = 15.0f;
        _startSizeVar = 5.0f;
        _endSize = START_SIZE_EQUAL_TO_END_SIZE;
        
        // emits per frame
        _emissionRate = _totalParticles/_life;
        
        // color of particles
        _startColor.r = 0.5f;
        _startColor.g = 0.5f;
        _startColor.b = 0.5f;
        _startColor.a = 1.0f;
        _startColorVar.r = 0.5f;
        _startColorVar.g = 0.5f;
        _startColorVar.b = 0.5f;
        _startColorVar.a = 0.0f;
        _endColor.r = 0.5f;
        _endColor.g = 0.5f;
        _endColor.b = 0.5f;
        _endColor.a = 0.0f;
        _endColorVar.r = 0.5f;
        _endColorVar.g = 0.5f;
        _endColorVar.b = 0.5f;
        _endColorVar.a = 0.0f;
        
        Texture2D* texture = getStarTexture();
        if (texture != nullptr)
        {
            setTexture(texture);
        }
        
        // additive
        this->setBlendAdditive(true);
        return true;
    }
    return false;
}
