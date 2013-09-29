//
//  GameSprite.cpp
//  Air Hockey
//
//  Created by hanks on 2013/08/21.
//
//

#include "BaseSprite.h"

BaseSprite::BaseSprite(GameLayer * game, int type) {
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    this->reset();
}

BaseSprite::~BaseSprite(void) {
    
}

void BaseSprite::update(float dt) {
    /*
     Let sprite to control its status, like postion, rotation, vector, self
     */
    if (_body && this->isVisible()) {
        this->setPositionX(_body->GetPosition().x * PTM_RATIO);
        this->setPositionY(_body->GetPosition().y * PTM_RATIO);
        this->setRotation(CC_RADIANS_TO_DEGREES(-1 * _body->GetAngle()));
    }
}

void BaseSprite::reset() {
    /*
     Reset all status for the sprite.
     */
    
}

void BaseSprite::setSpritePosition(cocos2d::CCPoint position) {
    this->setPosition(position);
    
    if (_body) {
        _body->SetTransform(b2Vec2(position.x / PTM_RATIO,
                                   position.y / PTM_RATIO),
                                   _body->GetAngle());
    }
}

void BaseSprite::hide(void) {
    if (_body) {
        _body->SetLinearVelocity(b2Vec2_zero);
        _body->SetAngularVelocity(0);
        _body->SetTransform(b2Vec2_zero, 0.0);
        _body->SetAwake(false);
    }
}

float BaseSprite::mag(void) {
    if (_body) {
        return pow(_body->GetLinearVelocity().x, 2) + pow(_body->GetLinearVelocity().y, 2);
    }
    return 0.0;
}
