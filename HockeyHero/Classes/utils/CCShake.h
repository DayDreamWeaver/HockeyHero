//
//  CCShake.h
//  Air_Hockey
//
//  Created by hanks on 2013/09/23.
//
//

#ifndef __Air_Hockey__CCShake__
#define __Air_Hockey__CCShake__

#include "cocos2d.h"

using namespace cocos2d;

class CCShake : public CCActionInterval
{
    // Code by Francois Guibert
    // Contact: www.frozax.com - http://twitter.com/frozax - www.facebook.com/frozax
public:
    CCShake();
    
    // Create the action with a time and a strength (same in x and y)
    static CCShake* create(float d, float strength );
    // Create the action with a time and strengths (different in x and y)
    static CCShake* createWithStrength(float d, float strength_x, float strength_y );
    bool initWithDuration(float d, float strength_x, float strength_y );
    
protected:
    
    void startWithTarget(cocos2d::CCNode *pTarget);
    void update(float time);
    void stop(void);
    
    
    // Initial position of the shaked node
    float m_initial_x, m_initial_y;
    // Strength of the action
    float m_strength_x, m_strength_y;
};

#endif /* defined(__Air_Hockey__CCShake__) */
