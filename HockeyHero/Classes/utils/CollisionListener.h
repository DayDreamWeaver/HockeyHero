//
//  CollisionListener.h
//  HockeyHero
//
//  Created by hanks on 2013/09/30.
//
//

#ifndef __HockeyHero__CollisionListener__
#define __HockeyHero__CollisionListener__

#include "Box2D.h"
#include "../sprites/BallSprite.h"
#include "../sprites/PlayerSprite.h"

class CollisionListener : public b2ContactListener {
public:
    virtual void BeginContact(b2Contact * contact);
    virtual void PreSolve(b2Contact * contact, const b2Manifold * oldManifold);
};

#endif /* defined(__HockeyHero__CollisionListener__) */
