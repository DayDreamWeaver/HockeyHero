#include "GameLayer.h"
#include "../utils/SoundManager.h"
#include "../utils/CCShake.h"
#include "../utils/CollisionListener.h"
#include "AppDelegate.h"

using namespace cocos2d;
using namespace CocosDenshion;

CCScene* GameLayer::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameLayer *layer = GameLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
GameLayer::GameLayer()
{
    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(GameLayer::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);
    
    // player background
    SoundManager::playBGM();
    
    _player1Score = 0;
    _player2Score = 0;
    _screenSize = CCDirector::sharedDirector()->getWinSize();
    
    _court = CCSprite::create("court.png");
    _court->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5));
    // set Z value to -1, to set background to the bottom
    _court->setZOrder(-1);
    this->addChild(_court);
    
    // init physics
    this->initPhysics();
    
    // label
    _player1ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player1ScoreLabel->setPosition(ccp(_screenSize.width - 60, _screenSize.height * 0.5 - 80));
    _player1ScoreLabel->setRotation(90);
    this->addChild(_player1ScoreLabel);
    
    _player2ScoreLabel = CCLabelTTF::create("0", "Arial", 60);
    _player2ScoreLabel->setPosition(ccp(_screenSize.width - 60, _screenSize.height * 0.5 + 80));
    _player2ScoreLabel->setRotation(90);
    this->addChild(_player2ScoreLabel);
    
    _attackRangeDegree = 10;
    
    // listen to touch
    this->setTouchEnabled(true);
    this->schedule(schedule_selector(GameLayer::update));
}

void GameLayer::draw() {
    //test to draw line of player's fixed track
        
    int blod_value_1 = 0.5 * MAX_BOLD;
    int blod_value_2 = 0.5 * MAX_BOLD;
    
    drawLine(ccp(0, _player1->getStartPoint().y), ccp(_screenSize.width, _player1->getStartPoint().y), RED, blod_value_1);
    drawLine(ccp(0, _player2->getStartPoint().y), ccp(_screenSize.width, _player2->getStartPoint().y), RED, blod_value_2);
    
    drawAngleCheckLine();
    
    drawLine(ccp(0, _player1->getStartPoint().y), ccp(_screenSize.width, _player1->getStartPoint().y), RED, blod_value_1);
    drawLine(ccp(0, _player2->getStartPoint().y), ccp(_screenSize.width, _player2->getStartPoint().y), RED, blod_value_2);
    
    // box2d debug mode
    ccGLEnableVertexAttribs( kCCVertexAttribFlag_Position );
    
    kmGLPushMatrix();
    
    _world->DrawDebugData();
    
    kmGLPopMatrix();
    
}

void GameLayer::drawLine(CCPoint start, CCPoint end, int color, int bold) {
    // select color
    switch (color) {
        case RED:
            //red line from bottom left to top right corner
            ccDrawColor4F(1.0f, 0.0f, 0.0f, 1.0f);
            break;
    }
    
    // set bold value of line

    glLineWidth(bold);

    // draw line
    ccDrawLine(start, end);
}
void GameLayer::drawAngleCheckLine() {
    CCPoint startAttackPoint1 = ccp(_player1->getAttackPoint().x, _player1->getAttackPoint().y);
    //CCPoint startAttackPoint2 = ccp(_player2->getAttackPoint().x, _player2->getAttackPoint().y);
    CCPoint leftAttackRangeVector1 = ccpForAngle(CC_DEGREES_TO_RADIANS(-_attackRangeDegree));
    CCPoint rightAttackRangeVector1 = ccpForAngle(CC_DEGREES_TO_RADIANS(_attackRangeDegree - 180));
    float leftEnd1X = _screenSize.width;
    float leftEnd1Y = startAttackPoint1.y + (_screenSize.width - startAttackPoint1.x) * leftAttackRangeVector1.y / leftAttackRangeVector1.x;
    float rightEnd1X = 0;
    float rightEnd1Y = startAttackPoint1.y - (startAttackPoint1.x - 0) * rightAttackRangeVector1.y / rightAttackRangeVector1.x;
    CCPoint leftEnd1 = ccp(leftEnd1X, leftEnd1Y);
    CCPoint rightEnd2 = ccp(rightEnd1X,rightEnd1Y);
    drawLine(startAttackPoint1, leftEnd1, RED, 2);
    drawLine(startAttackPoint1, rightEnd2, RED, 2);
}

void GameLayer::update(float dt) {
    // update player's position
    
    
    _ball->update(dt);
    
    // check for goals
    
    
}

void GameLayer::initPhysics() {
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    _world = new b2World(gravity);

    _world->SetAllowSleeping(true);
    _world->SetContinuousPhysics(true);
    _collisionListener = new CollisionListener();
    _world->SetContactListener(_collisionListener);
    
    m_debugDraw = new GLESDebugDraw(PTM_RATIO);
    _world->SetDebugDraw(m_debugDraw);
                         
    uint32 flags = 0;
    flags += b2Draw::e_shapeBit;
    m_debugDraw->SetFlags(flags);
    
    b2FixtureDef fixtureDef;
    b2BodyDef bodyDef;
    
    // create edges
    b2BodyDef courtBodyDef;
    courtBodyDef.position.Set(0, 0);
    b2Body *courtBody = _world->CreateBody(&courtBodyDef);
    
    // define edges
    b2EdgeShape courtBox;
    
    // bottom
    courtBox.Set(b2Vec2(0, 0), b2Vec2(_screenSize.width / PTM_RATIO, 0));
    courtBody->CreateFixture(&courtBox, 0);
    
    // top
    courtBox.Set(b2Vec2(0, _screenSize.height / PTM_RATIO), b2Vec2(_screenSize.width / PTM_RATIO, _screenSize.height / PTM_RATIO));
    courtBody->CreateFixture(&courtBox, 0);
    
    // left
    courtBox.Set(b2Vec2(0, 0), b2Vec2(0, _screenSize.height / PTM_RATIO));
    courtBody->CreateFixture(&courtBox, 0);
    
    // right
    courtBox.Set(b2Vec2(_screenSize.width / PTM_RATIO, 0), b2Vec2(_screenSize.width / PTM_RATIO, _screenSize.height / PTM_RATIO));
    courtBody->CreateFixture(&courtBox, 0);
    
    // init sprite
    CCPoint startPoint = ccp(_screenSize.width * 0.5, _screenSize.height / 4);
    // create player sprite
    _player1 = PlayerSprite::create(this, P1, startPoint);
    _player1->setAttackPoint(_player1->getStartPoint());
    this->addChild(_player1->getArrow());
    this->addChild(_player1);
    
    startPoint = ccp(_screenSize.width * 0.5, _screenSize.height - _screenSize.height / 4);
    _player2 = PlayerSprite::create(this, P2, startPoint);
    _player2->setAttackPoint(_player2->getStartPoint());
    this->addChild(_player2->getArrow());
    this->addChild(_player2);
    
    // keep player objects
    _players = CCArray::create(_player1, _player2, NULL);
    _players->retain();
    
    ////////////////////////
    // init ball sprite //
    ////////////////////////
    startPoint = ccp(_screenSize.width * 0.5, _screenSize.height * 0.5 - 20);
    _ball = BallSprite::create(this, kSpriteBall, startPoint);
    this->addChild(this->_ball->getParticle());
    this->addChild(_ball);
}

int GameLayer::getGestureDicrection(cocos2d::CCPoint start, cocos2d::CCPoint end, int playerIndex) {
    CCAssert(playerIndex == 0 or playerIndex == 1, "player index is fiexed as 0 or 1");
    int result = -1;
    switch (playerIndex) {
        case P1:
            // player 1
            if (end.y >= start.y) {
                result = UP;
            } else {
                //check the attack angle is greater than 10 degree.
                if (getAcuteAngleOfAttack(start, end) < _attackRangeDegree) {
                    result = UP;
                } else {
                    result = DOWN;
                }
                printf("acute angle:%lf",getAcuteAngleOfAttack(start, end));
            }
            break;
        case P2:
            // player 2
            if (end.y >= start.y) {
                result = DOWN;
            } else {
                //check the attack angle is greater than 10 degree.
                if (getAcuteAngleOfAttack(start, end) < _attackRangeDegree) {
                    result = DOWN;
                } else {
                    result = UP;
                }
            }
            break;
        default:
            break;
    }
    return result;
}

double GameLayer::getAcuteAngleOfAttack(cocos2d::CCPoint attack, cocos2d::CCPoint tap) {
    CCPoint leftHorizon = ccp(-1,0);
    CCPoint rightHorizon = ccp(1,0);
    CCPoint attackDirection = ccpSub(attack, tap);
    double angle1 = CC_RADIANS_TO_DEGREES(ccpAngle(leftHorizon,attackDirection));
    double angle2 = CC_RADIANS_TO_DEGREES(ccpAngle(rightHorizon,attackDirection));
    return angle1 < angle2 ? angle1 : angle2;
}
void GameLayer::updatePlayerScore(int player) {
    SoundManager::playSE(SCORE_SE);
    
    
    // get back to original position
    char score_buffer[10];
    if (player == 1) {
        _player1Score++;
        sprintf(score_buffer, "%i", _player1Score);
        _player1ScoreLabel->setString(score_buffer);
        _ball->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5 + 2 * _ball->getRadius()));
    } else {
        _player2Score++;
        sprintf(score_buffer, "%i", _player2Score);
        _player2ScoreLabel->setString(score_buffer);
        _ball->setPosition(ccp(_screenSize.width * 0.5, _screenSize.height * 0.5 - 2 * _ball->getRadius()));
    }
}

void GameLayer::reset() {
    _ball->reset();
    // clear touch obj and set player to origin position
    _player1->reset();
    _player2->reset();
}

void GameLayer::ccTouchesBegan(CCSet* pTouches, CCEvent* event) {
    CCSetIterator i;
    CCTouch* touch;
    CCPoint tap;
    BaseSprite* player;
    
    for (i = pTouches->begin(); i != pTouches->end(); i++) {
        touch = (CCTouch *)(*i);
        if (touch) {
            tap = touch->getLocation();
            for (int p = 0; p < 2; p++) {
                player = (BaseSprite *)_players->objectAtIndex(p);
                // if touch on player, set touch object onto player
                if (player->boundingBox().containsPoint(tap)) {
                    //player->setTouch(touch);
                }
            }
        }
    }
    
}

void GameLayer::ccTouchesMoved(CCSet* pTouches, CCEvent* event) {
    CCSetIterator i;
    CCTouch* touch;
    CCPoint tap;
    PlayerSprite* player;
    int direction = -1;
    for (i = pTouches->begin(); i != pTouches->end(); i++) {
        touch = (CCTouch *)(*i);
        if (touch) {
            tap = touch->getLocation();
            for (int p = 0; p < _players->count(); p++) {
                player = (PlayerSprite *)_players->objectAtIndex(p);
                // if player contains a touch
//                if (player) {
//                    CCPoint nextPosition = tap;
//                    switch (player->getType()) {
//                        case P1:
//                            direction = this->getGestureDicrection(ccp(player->getStartPoint().x, player->getStartPoint().y - _ball->getRadius()), tap, player->getType());
//                            break;
//                        case P2:
//                            direction = this->getGestureDicrection(ccp(player->getStartPoint().x, player->getStartPoint().y + _ball->getRadius()), tap, player->getType());
//                            break;
//                    }
//                    
//                    // update player 1's position Y
//                    switch (direction) {
//                        case UP:
//                            nextPosition.y = player->getStartPoint().y;
//                            player->getArrow()->setVisible(false);
//                            // update attack start position
//                            player->setAttackPoint(player->getPosition());
//                            printf("UP\n");
//                            break;
//                        case DOWN:
//                            player->getArrow()->setVisible(true);
//                            printf("DOWN\n");
//                            break;
//                        default:
//                            break;
//                    }

                    
                    // update position and vector to player
                    
//                }
            }
        }
    }
}

void GameLayer::ccTouchesEnded(CCSet* pTouches, CCEvent* event) {
    CCSetIterator i;
    CCTouch* touch;
    CCPoint tap;
    PlayerSprite* player;
    
    printf("end");
    
    for (i = pTouches->begin(); i != pTouches->end(); i++) {
        touch = (CCTouch *)(*i);
        if (touch) {
            tap = touch->getLocation();
            for (int p = 0; p < _players->count(); p++) {
                player = (PlayerSprite *)_players->objectAtIndex(p);
                
//                if (player->getTouch() != NULL && player->getTouch() == touch) {
//                    player->setTouch(NULL);
//                    // need to keep its vector, because need to perform spring effect
//                        player->setPosition(player->getAttackPoint());
//                        // show spring effect
//                        if (player->getArrow()->isVisible()) {
//                            player->doSpringEffect(tap, player->getAttackPoint());
//                        }
//                    }
            }
        }
    }

}

void GameLayer::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

GameLayer::~GameLayer() {
    CC_SAFE_RELEASE(_players);
    delete _world;
    _world = NULL;
    
    delete m_debugDraw;
    delete _collisionListener;
}
