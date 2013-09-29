//
//  BallSprite.cpp
//  HockeyHero
//
//  Created by hanks on 2013/09/07.
//
//

#include "BallSprite.h"
#include "../utils/SoundManager.h"
#include "../utils/CCShake.h"

BallSprite::BallSprite(GameLayer * game, int type, CCPoint position) : BaseSprite(game, type) {
    _type= type;
    _startPosition = position;
    
    this->reset();
}

BallSprite::~BallSprite() {
    
}

BallSprite* BallSprite::create(GameLayer * game, int type, CCPoint position) {
    /*
     Create sprite with image file name.
     
     Args:
       pszFileName: char *, string of file name
     
     Returns:
       BallSprite *
     */
    BallSprite *sprite = new BallSprite(game, type, position);
    if (sprite) {
        sprite->initBall();
        sprite->autorelease();
        return sprite;
    }
    CC_SAFE_DELETE(sprite);
    return NULL;
}

void BallSprite::initBall() {
    _jet = CCParticleSystemQuad::create("cool.plist");
    
    // create box2d body
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    
    _body = _game->getWorld()->CreateBody(&bodyDef);
    _body->SetSleepingAllowed(true);
    _body->SetLinearDamping(1.2);
    _body->SetAngularDamping(0.8);
    
    // create circle shape
    b2CircleShape circle;
    circle.m_radius = BALL_RADIUS / PTM_RATIO;
    
    // define fixture
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.density = 5;
    fixtureDef.restitution = 0.7;
    
    // add collision filter so only ball can be hit by player
    fixtureDef.filter.categoryBits = 0x0010;
    
    // set sprite texture
    this->initWithFile("puck.png");
    
    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
    
    setSpritePosition(_startPosition);
}

void BallSprite::update(float dt) {
    /*
     Update ball sprite status
     */
    if (_body && isVisible()) {
        this->setPositionX(_body->GetPosition().x * PTM_RATIO);
        this->setPositionY(_body->GetPosition().y * PTM_RATIO);
    }
    
    // update jet partical position
    if (!this->_jet->isActive()) {
        this->_jet->resetSystem();
    }
    
    this->_jet->setPosition(this->getPosition());
}


CCParticleSystem * BallSprite::getParticle() {
    return this->_jet;
}

void BallSprite::reset() {
    _jet->setPosition(this->getPosition());
    _jet->setAngle(270);
    _jet->stopSystem();
    
    if (_body) {
        _body->SetLinearVelocity(b2Vec2_zero);
        _body->SetAngularVelocity(0);
        _body->SetAwake(true);
    }
    setSpritePosition(_startPosition);
    setVisible(true);
}
