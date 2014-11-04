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

//class CollisionDetection {
//public:
//    //Handle for getting the Singleton Object
//    static CollisionDetection* GetInstance();
//    //Function signature for checking for collision detection spr1, spr2 are the concerned sprites
//    //pp is bool, set to true if Pixel Perfection Collision is required. Else set to false
//    //_rt is the secondary buffer used in our system
//    bool areTheSpritesColliding(Sprite* spr1, Sprite* spr2, bool pp, RenderTexture* _rt);
//private:
//    static CollisionDetection* instance;
//    CollisionDetection();
//    
//    // Values below are all required for openGL shading
//    GLProgram *glProgram;
//    Color4B *buffer;
//    int uniformColorRed;
//    int uniformColorBlue;
//    
//};
class CollisionDetection {
public:
    static CollisionDetection *getInstance(void);
    static void destroyInstance(void);
    
    bool collidesWithSprite(Sprite *sprite1, Sprite *sprite2, bool pp);
    bool collidesWithSprite(Sprite *sprite1, Sprite *sprite2);
    bool collidesWithPoint(Sprite *sprite, const Point &point);
    bool collidesWithSpriteHavingRadius(Sprite *sprite1, float radius1, Sprite *sprite2, float radius2);
    
private:
    class PixelReaderNode : public Node {
    public:
        static PixelReaderNode* create(const Point &readPoint);
        
        PixelReaderNode(const Point &readPoint);
        virtual ~PixelReaderNode(void);
        
        virtual void draw(cocos2d::Renderer *renderer, const cocos2d::Mat4& transform, uint32_t flags) override;
        
        inline const Point &getReadPoint(void) const;
        inline void setReadPoint(const Point &readPoint);
        
        inline const Size &getReadSize(void) const;
        inline void setReadSize(const Size &readPoint);
        
        inline GLubyte *getBuffer(void);
        
    private:
        void onDraw(void);
        
        CustomCommand _readPixelCommand;
        Point _readPoint;
        Size _readSize;
        GLubyte *_buffer;
    };
    
    CollisionDetection(void);
    virtual ~CollisionDetection(void);
    
    Rect getIntersection(const Rect &r1, const Rect &r2) const;
    Point renderSprite(Sprite *sprite, CustomCommand &command, bool red);
    void resetSprite(Sprite *sprite, const Point &oldPosition);
    
    // Singleton
    static CollisionDetection *s_instance;
    
    GLProgram *_glProgram;
    RenderTexture *_rt;
    PixelReaderNode *_pixelReader;
};

// Inline methods
inline const Point &CollisionDetection::PixelReaderNode::getReadPoint(void) const {
    return _readPoint;
}

inline void CollisionDetection::PixelReaderNode::setReadPoint(const Point &readPoint) {
    _readPoint = readPoint;
}

inline const Size &CollisionDetection::PixelReaderNode::getReadSize(void) const {
    return _readSize;
}

inline void CollisionDetection::PixelReaderNode::setReadSize(const Size &readSize) {
    if (_readSize.width * _readSize.height < readSize.width * readSize.height) {
        free(_buffer);
        _buffer = (GLubyte*) malloc(4 * readSize.width * readSize.height);
    }
    _readSize = readSize;
}

inline GLubyte *CollisionDetection::PixelReaderNode::getBuffer(void) {
    return _buffer;
}
#endif /* defined(__DragMeAway__CollisionDetection__) */
