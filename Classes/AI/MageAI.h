//
//  MageAI.h
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-26.
//
//

#ifndef __ChineseCabbageTree__MageAI__
#define __ChineseCabbageTree__MageAI__

#include <cocos2d.h>
#include "DefaultAI.h"

USING_NS_CC;

class MageAI : public DefaultAI {
public:
    virtual ~MageAI();
    static MageAI* sharedAI();
    virtual CCPoint getDirectionVector(Unit* self, float deltaX, float deltaY);
    
};

#endif /* defined(__ChineseCabbageTree__MageAI__) */
