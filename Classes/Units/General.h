//
//  General.h
//  ChineseCabbageTree
//
//  Created by DLL on 14-3-25.
//
//

#ifndef __ChineseCabbageTree__General__
#define __ChineseCabbageTree__General__

#include "Unit.h"

USING_NS_CC;

#define GENERAL_CREATE(__TYPE__,__NAME__) \
static __TYPE__* create(const char* name, UnitTeam team, int level) \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet && pRet->init(name, team, level)) \
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


class General : public Unit
{
public:
    virtual ~General();
    virtual bool init(const char* name, UnitTeam team, int level);
};


class Guanyu : public General
{
public:
    virtual ~Guanyu();
    GENERAL_CREATE(Guanyu, "guanyu");
};

class Lvbu : public General
{
public:
    virtual ~Lvbu();
    GENERAL_CREATE(Lvbu, "lvbu");
};


#endif /* defined(__ChineseCabbageTree__General__) */
