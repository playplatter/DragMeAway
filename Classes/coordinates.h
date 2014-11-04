//
//  coordinates.h
//  DragMeAway
//
//  Created by Syeda Sara Furqan on 30/10/2014.
//
//

#ifndef __DragMeAway__coordinates__
#define __DragMeAway__coordinates__

#include <stdio.h>
#include "cocos2d.h"
using namespace cocos2d;

class coordinates : public Ref
{
public:
    
    coordinates();
    ~coordinates();
    
//    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
//    virtual bool init();
//    
//    // implement the "static create()" method manually
//    CREATE_FUNC(coordinates);
    
    void setPositon(float width, float height);
    Vec2 getPosition();
    
private:
    float _width, _height;
    
};


#endif /* defined(__DragMeAway__coordinates__) */
