//
//  Army.h
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-25.
//
//

#ifndef __ChineseCabbageTree__Army__
#define __ChineseCabbageTree__Army__

#include "Unit.h"

USING_NS_CC;


class LightFootman : public Unit {
    
public:
    virtual ~LightFootman();
    
    virtual bool init(const char* resource, UnitTeam team, int level);
    UNIT_CREATE(LightFootman, "lightfootman");
    
};

class Counsellor : public Unit {
public:
    virtual ~Counsellor();
    virtual bool init(const char* resource, UnitTeam team, int level);
    virtual void makeDamage();  // 造成伤害
    UNIT_CREATE(Counsellor, "counsellor");
    virtual CCRect getDamageArea();
};

#endif /* defined(__ChineseCabbageTree__Army__) */
