//
//  coordinates.cpp
//  DragMeAway
//
//  Created by Syeda Sara Furqan on 30/10/2014.
//
//

#include "coordinates.h"

coordinates::coordinates()
{
    
}

coordinates::~coordinates()
{
    // release our sprite and layer so that it gets dealloced
//    this->autorelease();
}

Vec2 coordinates::getPosition(){
    return Vec2(_width, _height);
}

void coordinates::setPositon(float width, float height){
    _height = height;
    _width = width;
}