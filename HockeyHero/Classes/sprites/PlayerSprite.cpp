//
//  PlayerSprite.cpp
//  HockeyHero
//
//  Created by hanks on 2013/09/07.
//
//

#include "PlayerSprite.h"
#include "../utils/SoundManager.h"
#include "../layers/GameLayer.h"

PlayerSprite::PlayerSprite(GameLayer * game, int type, CCPoint position) : BaseSprite(game, type){
    _startPosition = position;
    _type = type;
    this->reset();
}

PlayerSprite::~PlayerSprite() {
    
}

void PlayerSprite::initPlayer() {
    this->_arrow = ArrowSprite::create("arrow_8.png");
    
    // create body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    
    _body = _game->getWorld()->CreateBody(&bodyDef);
    _body->SetSleepingAllowed(true);
    _body->SetLinearDamping(8);
    _body->SetAngularDamping(5);
    
    // Define shape
    b2CircleShape circle;
    circle.m_radius = BALL_RADIUS/PTM_RATIO;
    
    //define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 5;
    fixtureDef.restitution = 0.7;
    
    fixtureDef.filter.categoryBits = 0x0100;
    
    this->initWithFile("mallet");
    
    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
    
    setSpritePosition(_startPosition);

}

ArrowSprite* PlayerSprite::getArrow() {
    return this->_arrow;
}

PlayerSprite* PlayerSprite::create(GameLayer * game, int type, CCPoint position) {
    PlayerSprite *sprite = new PlayerSprite(game, type, position);
    if (sprite) {
        sprite->initPlayer();
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void PlayerSprite::update(float dt) {
    /*
      Update player status

      Args:
        dt: float, interval between frame

      Returns:
        void
     */
    if (_body && isVisible()) {
        setPositionX(_body->GetPosition().x * PTM_RATIO);
        setPositionY(_body->GetPosition().y * PTM_RATIO);
    }
    
    // update arrow
    this->transferArrow();
}

void PlayerSprite::doSpringEffect(cocos2d::CCPoint start, cocos2d::CCPoint end) {
    float targetX = (end.x - start.x) * 2.5 + start.x;
    float targetY = (end.y - start.y) * 2.5 + start.y;
    
    CCActionInterval * actionTo = CCMoveTo::create(0.4, ccp(targetX, targetY));
    CCActionInterval * actionBack = CCMoveTo::create(0.15, end);
    
    this->runAction(CCSequence::create(actionTo, actionBack, NULL));
}

void PlayerSprite::transferArrow() {
    CCPoint start = this->getAttackPoint();
    CCPoint end = this->getPosition();
    // adjust scale
    if (_arrow->isVisible()) {
        float distance = ccpDistance(start, end);
        CCSize size = _arrow->boundingBox().size;
        float scale = 0;
        
        if (distance > 0) {
            scale = distance / _screenSize.height / 2 * MAX_SCALE;
        }
        
        _arrow->setScaleX(scale);
        
        // adjust angle
        float diffx = end.x - start.x;
        float diffy = end.y - start.y;
        
        float radian = -atan2(diffy, diffx);
        float angle = CC_RADIANS_TO_DEGREES(radian);
        
        _arrow->setRotation(angle);
        
        // adjust position
        _arrow->setPosition(ccpMidpoint(start, end));
    }

}

void PlayerSprite::reset() {
    this->setPosition(_startPosition);
    this->_arrow->setVisible(false);
    this->_arrow->setPosition(_startPosition);
}

