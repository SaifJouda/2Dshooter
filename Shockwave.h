#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "physics.h"



using namespace sf;
using namespace std;

struct Shockwave : public CircleShape {
    Shockwave(b2World &world, float x, float y, float r) {
        b2BodyDef bodyDef;
        bodyDef.position.Set(x/pixels_per_meter, y/pixels_per_meter);
        bodyDef.type = b2_dynamicBody;
        bodyDef.linearDamping = 0.0;
        b2CircleShape b2shape;

        b2shape.m_radius = r/pixels_per_meter;

        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0;
        fixtureDef.friction = 0.1;
        fixtureDef.restitution = 1.0;
        fixtureDef.shape = &b2shape;



        res = world.CreateBody(&bodyDef);
        res->CreateFixture(&fixtureDef);
        res->GetFixtureList()->SetSensor(true);

        this->setRadius(r);
        this->setOrigin(r,r);
        this->setPosition(x, y);
        this->setFillColor(Color::White);

        res->SetUserData(this);

    }


    void radius(float r)
    {
        res->GetFixtureList()->GetShape()->m_radius = r/pixels_per_meter;
        this->setRadius(r);
        this->setOrigin(r,r);
    }

    //member functions
    bool checkCollision()
    {
        for (b2ContactEdge* edge = res->GetContactList(); edge; edge = edge->next) {
            if (edge->contact->IsTouching()) return true;
        }
        return false;
    }

    bool checkCollision(Block b)
    {
        for (b2ContactEdge* edge = res->GetContactList(); edge; edge = edge->next) {
            if (edge->other == b) {
                if (edge->contact->IsTouching()) {
                   return true;
                }
            }
        }
        return false;
    }

    void deleteBlock(b2World &world,Block b) {
        world.DestroyBody(b);
        b->SetUserData(0);
    }

    void removeBody()
    {
        res->GetWorld()->DestroyBody(res);
        res->SetUserData(0);
    }



    void updatePosition()
    {
        this->setPosition(res->GetPosition().x*pixels_per_meter, res->GetPosition().y*pixels_per_meter);
        this->setRotation(res->GetAngle()*deg_per_rad);
    }



    bool isDestroyed(Block b) {
        if (b->GetUserData() == 0) return true;
        else return false;
    }


    //member variable
    b2Body* res;
    float speed;
    float angle;

};

