//
//  Props.h
//  BlackWhiteRush
//
//  Created by 何遵祖 on 13-12-30.
//
//

#ifndef __BlackWhiteRush__Props__
#define __BlackWhiteRush__Props__

#include "cocos2d.h"
#include "GameObject.h"

class Props : public GameObject{
public:
    static Props* instance(const char* file);
    virtual bool inits(const char* file);
private:
    CC_SYNTHESIZE(PropType, _propType, PropType);
};

#endif /* defined(__BlackWhiteRush__Props__) */
