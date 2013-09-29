//
//  BallSprite.h
//  HockeyHero
//
//  Created by hanks on 2013/09/07.
//
//

#ifndef __HockeyHero__BallSprite__
#define __HockeyHero__BallSprite__

#include "BaseSprite.h"

using namespace cocos2d;

class BallSprite : public BaseSprite {
protected:
    CCParticleSystem * _jet;
    CCPoint _startPosition;
public:
    BallSprite(GameLayer * game, int type, CCPoint position);
    ~BallSprite(void);
    static BallSprite* create(GameLayer * game, int type, CCPoint position);
    void update(float dt);
    void reset(void);
    CCParticleSystem * getParticle();
    void initBall();
};

#endif /* defined(__HockeyHero__BallSprite__) */
