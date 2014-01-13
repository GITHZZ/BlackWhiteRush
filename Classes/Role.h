//
//  Role.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-30.
//
//

#ifndef __BlackWhiteRush__Role__
#define __BlackWhiteRush__Role__

#include "cocos2d.h"
#include "GameObject.h"

using namespace cocos2d;

enum RoleState{
    Role_Move,
    Role_Death,
    Role_JumpOnce,
    Role_JumpSecond,
    Role_JumpingOnce,
    Role_JumpingSecond
};

enum SkillState{
    Skill_None,
    Skill_Sprint,
    Skill_Blood,
    Skill_Wave
};

class Role : public GameObject{
public:
    static Role* instance(const char* file);
    virtual bool inits(const char* file);
    virtual void onCollisionEnter(GameObject* collision);
    
    void update(float dt);
    void jumpAction();
private:
    CC_SYNTHESIZE(RoleState, _rState, State);
    CC_SYNTHESIZE(SkillState, _skState, SkState);
    CC_SYNTHESIZE(CCPoint, _standardPoint,StandardPos);
};

#endif /* defined(__BlackWhiteRush__Role__) */
