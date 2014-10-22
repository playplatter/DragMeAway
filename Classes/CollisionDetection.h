//
//  CollisionDetection.h
//  DragMeAway
//
//  Created by Sarah Furqan on 20/10/2014.
//
//

#ifndef __DragMeAway__CollisionDetection__
#define __DragMeAway__CollisionDetection__

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class CollisionDetection {
public:
    //Handle for getting the Singleton Object
    static CollisionDetection* GetInstance();
    //Function signature for checking for collision detection spr1, spr2 are the concerned sprites
    //pp is bool, set to true if Pixel Perfection Collision is required. Else set to false
    //_rt is the secondary buffer used in our system
    bool areTheSpritesColliding(Sprite* spr1, Sprite* spr2, bool pp, RenderTexture* _rt);
private:
    static CollisionDetection* instance;
    CollisionDetection();
    
    // Values below are all required for openGL shading
    GLProgram *glProgram;
    Color4B *buffer;
    int uniformColorRed;
    int uniformColorBlue;
    
};

#endif /* defined(__DragMeAway__CollisionDetection__) */
