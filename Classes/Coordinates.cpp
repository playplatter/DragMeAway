//
//  Coordinates.cpp
//  DragMeAway
//
//  Created by Syeda Sara Furqan on 30/10/2014.
//
//

#include "Coordinates.h"

Coordinates::Coordinates()
{}

Coordinates::~Coordinates()
{
    // release our sprite and layer so that it gets dealloced
//    this->autorelease();
}

Vec2 Coordinates::getPosition(){
    return Vec2(_width, _height);
}

void Coordinates::setPositon(float width, float height){
    _height = height;
    _width = width;
}