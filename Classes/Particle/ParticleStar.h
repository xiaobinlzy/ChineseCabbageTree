//
//  ParticleStar.h
//  ChineseCabbageTree
//
//  Created by DLL on 14-9-19.
//
//

#ifndef __ChineseCabbageTree__ParticleStar__
#define __ChineseCabbageTree__ParticleStar__

#include <iostream>
#include <cocos2d.h>


static const char* __starPngData = "H4sIAAAAAAAAAwFLAbT+iVBORw0KGgoAAAANSUhEUgAAABkAAAAZCAYAAADE6YVjAAABEklEQVRIDe2TYRGDMAyFuRmYhEpAQiUgAQmTwBxMAhKQUAmTwBwggX3hKNdjkBbY9ovcPRqa9/KOtGTZGUcn0Pe9FRztE9Vj0oA8SjxCwMCA7h9G9VYj+AZUIG3cEA3wcdMmA8mCesRV437UEMnZ+HAkhSeRW/AALZAofW3TirAY5Prj+Nnp/b90OTBxitG+EYXzpPlVMWhD7lp+WSsE+/cgn6ev+cbmd77AKl8xlDY3DQV0yIHcmlhMVzrUR3O6yjm0Y3fHWgIjQlYLGuCjjjacE1CKwRM4YOd1/06tAN2I9D8cgRg4UPpm2govB2JUabypBlEMKlmnzYQEvhi1STpIJqHnIgWtGO3WLzY9N386gTdy7LvukmMkygAAAABJRU5ErkJgghE6j/xLAQAA";

USING_NS_CC;

class ParticleStar : public ParticleSystemQuad
{
public:
    static ParticleStar* create();
    static ParticleStar* createWithTotalParticles(int numberOfParticles);
protected:
    ParticleStar() {};
    virtual ~ParticleStar() {};
    
    bool init(){ return initWithTotalParticles(1500); }
    virtual bool initWithTotalParticles(int numberOfParticles);
    
private:
    CC_DISALLOW_COPY_AND_ASSIGN(ParticleStar);
};



#endif /* defined(__ChineseCabbageTree__ParticleStar__) */
