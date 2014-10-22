//
//  CollisionDetection.cpp
//  DragMeAway
//
//  Created by Sarah Furqan on 20/10/2014.
//
//

#include "CollisionDetection.h"
// Singleton Instance set to NULL initially
CollisionDetection* CollisionDetection::instance = NULL;

// Handle to get Singleton Instance
CollisionDetection* CollisionDetection::GetInstance() {
    if (instance == NULL) {
        instance = new CollisionDetection();
    }
    return instance;
}

// Private Constructor being called from within the GetInstance handle
CollisionDetection::CollisionDetection() {
    // Code below to setup shaders for use in Cocos2d-x
    glProgram = new GLProgram();
    glProgram->retain();
    glProgram->initWithFilenames("SolidVertexShader.vsh", "SolidColorShader.fsh");
    glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_POSITION, GLProgram::VERTEX_ATTRIB_POSITION);
    glProgram->bindAttribLocation(GLProgram::ATTRIBUTE_NAME_TEX_COORD, GLProgram::VERTEX_ATTRIB_TEX_COORD);
    glProgram->link();
    glProgram->updateUniforms();
    glProgram->use();
    
    uniformColorRed = glGetUniformLocation(glProgram->getProgram(), "u_color_red");
    uniformColorBlue = glGetUniformLocation(glProgram->getProgram(), "u_color_blue");
    
    // A large buffer created and re-used again and again to store glReadPixels data
    buffer = (Color4B *)malloc( sizeof(Color4B) * 10000 );
}

bool CollisionDetection::areTheSpritesColliding(cocos2d::Sprite* spr1, cocos2d::Sprite* spr2, bool pp, RenderTexture* _rt) {
    bool isColliding = false;
    
    // Rectangle of the intersecting area if the sprites are colliding according to Bounding Box collision
    Rect intersection;
    
    // Bounding box of the Two concerned sprites being saved
    Rect r1 = spr1->boundingBox();
    Rect r2 = spr2->boundingBox();
    
    // Look for simple bounding box collision
    if (r1.intersectsRect(r2)) {
        // If we're not checking for pixel perfect collisions, return true
        if (!pp) {
            return true;
        }
        
        float tempX;
        float tempY;
        float tempWidth;
        float tempHeight;
        
        //OPTIMIZE FURTHER
        //CONSIDER THE CASE WHEN ONE BOUDNING BOX IS COMPLETELY INSIDE ANOTHER BOUNDING BOX!
        if (r1.getMaxX() > r2.getMinX()) {
            tempX = r2.getMinX();
            tempWidth = r1.getMaxX() - r2.getMinX();
        } else {
            tempX = r1.getMinX();
            tempWidth = r2.getMaxX() - r1.getMinX();
        }
        
        if (r2.getMaxY() < r1.getMaxY()) {
            tempY = r1.getMinY();
            tempHeight = r2.getMaxY() - r1.getMinY();
        } else {
            tempY = r2.getMinY();
            tempHeight = r1.getMaxY() - r2.getMinY();
        }
        
        // We make the rectangle for the intersection area
//        intersection = Rect(tempX, tempY, tempWidth, tempHeight);
        intersection = Rect(tempX * CC_CONTENT_SCALE_FACTOR(), tempY * CC_CONTENT_SCALE_FACTOR(), tempWidth * CC_CONTENT_SCALE_FACTOR(), tempHeight * CC_CONTENT_SCALE_FACTOR());

        unsigned int x = intersection.origin.x;
        unsigned int y = intersection.origin.y;
        unsigned int w = intersection.size.width;
        unsigned int h = intersection.size.height;
        
        // Total pixels whose values we will get using glReadPixels depends on the Height and Width of the intersection area
        unsigned int numPixels = w * h;
        
        // Setting the custom shader to be used
        spr1->setGLProgram(glProgram);
        spr2->setGLProgram(glProgram);
        glProgram->use();
        
        // Clearing the Secondary Draw buffer of all previous values
        _rt->beginWithClear( 0, 0, 0, 0);
        
        // The below two values are being used in the custom shaders to set the value of RED and BLUE colors to be used
        glUniform1i(uniformColorRed, 255);
        glUniform1i(uniformColorBlue, 0);
        
        // The blend function is important we don't want the pixel value of the RED color being over-written by the BLUE color.
        // We want both the colors at a single pixel and hence get a PINK color (so that we have both the RED and BLUE pixels)
        spr1->setBlendFunc((BlendFunc){GL_SRC_ALPHA, GL_ONE});
        
        // The visit() function draws the sprite in the _rt draw buffer its a Cocos2d-x function
        spr1->visit();
        
        // Setting the shader program back to the default shader being used by our game
        spr1->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
        // Setting the default blender function being used by the game
        spr1->setBlendFunc((BlendFunc){CC_BLEND_SRC, CC_BLEND_DST});
        
        // Setting new values for the same shader but for our second sprite as this time we want to have an all BLUE sprite
        glUniform1i(uniformColorRed, 0);
        glUniform1i(uniformColorBlue, 255);
        spr2->setBlendFunc((BlendFunc){GL_SRC_ALPHA, GL_ONE});
        
        spr2->visit();
        
        spr2->setGLProgram(ShaderCache::getInstance()->getGLProgram(GLProgram::SHADER_NAME_POSITION_TEXTURE_COLOR));
        spr2->setBlendFunc((BlendFunc){CC_BLEND_SRC, CC_BLEND_DST});
        
        // Get color values of intersection area
        glReadPixels(x, y, w, h, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        
        _rt->end();
        
        // Read buffer
        unsigned int step = 1;
        for(unsigned int i=0; i<numPixels; i+=step) {
            Color4B color = buffer[i];
            // Here we check if a single pixel has both RED and BLUE pixels
            if (color.r > 0 && color.b > 0) {
                isColliding = true;
                break;
            }
        }
    }
    return isColliding;
}
