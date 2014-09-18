//
//  UnitDelegate.h
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-25.
//
//

#ifndef ChineseCabbageTree_UnitDelegate_h
#define ChineseCabbageTree_UnitDelegate_h

#include <cocos2d.h>

USING_NS_CC;



class UnitDelegate {
public:
    virtual CCParticleBatchNode* getParticleBatchNode() = 0;
    virtual CCArray* getUnits() = 0;
};

#endif
