//
//  CollisionListener.cpp
//  HockeyHero
//
//  Created by hanks on 2013/09/30.
//
//

#include "CollisionListener.h"
#include "../layers/GameLayer.h"
#include "SoundManager.h"

void CollisionListener::BeginContact(b2Contact *contact) {
    b2Body * bodyA = contact->GetFixtureA()->GetBody();
    b2Body * bodyB = contact->GetFixtureB()->GetBody();
    
    BaseSprite * spriteA = (BaseSprite *)bodyA->GetUserData();
    BaseSprite * spriteB = (BaseSprite *)bodyB->GetUserData();
    
    if (spriteA && spriteB) {
        if (spriteA->mag() > 10 || spriteB->mag() > 10) {
            SoundManager::playSE(HIT_SE);
        }
    }
}

void CollisionListener::PreSolve(b2Contact *contact, const b2Manifold *oldManifold) {
    b2Body * bodyA = contact->GetFixtureA()->GetBody();
    b2Body * bodyB = contact->GetFixtureB()->GetBody();
    
    BaseSprite * spriteA = (BaseSprite *)bodyA->GetUserData();
    BaseSprite * spriteB = (BaseSprite *)bodyB->GetUserData();
    
    if (spriteA && spriteB) {
        
    }
}


