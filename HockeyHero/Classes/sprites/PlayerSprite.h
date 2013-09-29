//
//  PlayerSprite.h
//  Air_Hockey
//
//  Created by hanks on 2013/09/07.
//
//

#ifndef __HockeyHero__PlayerSprite__
#define __HockeyHero__PlayerSprite__

#include "cocos2d.h"
#include "BaseSprite.h"
#include "ArrowSprite.h"

using namespace cocos2d;

#define MAX_SCALE 20

enum {
    P1,
    P2,
};

class PlayerSprite : public BaseSprite {
private:
    ArrowSprite *_arrow;
    CCPoint _startPosition;
public:
    CC_SYNTHESIZE(CCPoint, _attackPoint, AttackPoint);
    
    ArrowSprite* getArrow(void);
    PlayerSprite(GameLayer * game, int type, CCPoint position);
    ~PlayerSprite(void);
    static PlayerSprite* create(GameLayer * game, int type, CCPoint position);
    void update(float dt);
    void reset();
    void transferArrow();
    void doSpringEffect(cocos2d::CCPoint start, cocos2d::CCPoint end);
    void initPlayer();
};

#endif /* defined(__HockeyHero__PlayerSprite__) */
