//
//  BaseSprite.h
//  HockeyHero
//
//  Created by hanks on 2013/08/21.
//
//

#ifndef __HockeyHero__BaseSprite__
#define __HockeyHero__BaseSprite__

#include "cocos2d.h"
#include "Box2D.h"

#define PTM_RATIO 32.0
#define BALL_RADIUS 8.0

class GameLayer;

using namespace cocos2d;

enum {
    kSpriteBall
};

class BaseSprite : public CCSprite {
protected:
    // size of design resolution size
    CCSize _screenSize;

public:
    BaseSprite(GameLayer * game, int type);
    // need to use virtual deconstruction in the base class
    virtual ~BaseSprite(void);
    
    // box2d definition
    CC_SYNTHESIZE(b2Body *, _body, Body);
    CC_SYNTHESIZE(GameLayer *, _game, Game);
    CC_SYNTHESIZE(int, _type, Type);

    
    // get radius of sprite
    inline float getRadius() {
        return this->getTexture()->getContentSize().width * 0.5f;
    }
    // let sprite to control status of itself
    virtual void update(float dt);
    // reset variables of sprite
    virtual void reset(void);
    virtual void setSpritePosition (CCPoint position);
    virtual void hide(void);
    virtual float mag(void);
};

#endif /* defined(__HockeyHero__GameSprite__) */
