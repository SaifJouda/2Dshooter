#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "physics.h"


using namespace sf;
using namespace std;

struct Wall : public RectangleShape { //inheriting all of RectangleShape
    //Constuctor
        Wall(b2World &world, float x, float y, float width, float height) {
        b2BodyDef bodyDef;
        bodyDef.position.Set((x + width/2.0)/pixels_per_meter, (y + height/2.0)/pixels_per_meter);
        bodyDef.type = b2_staticBody;
        bodyDef.linearDamping = 0.0;
        b2PolygonShape b2shape;
        b2shape.SetAsBox(width/pixels_per_meter/2.0, height/pixels_per_meter/2.0);
        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0;
        fixtureDef.friction = 0.001;
        fixtureDef.restitution = 0.0;
        fixtureDef.shape = &b2shape;

        res = world.CreateBody(&bodyDef);
        res->CreateFixture(&fixtureDef);

        this->setSize(Vector2f(width, height));
        this->setOrigin(width/2.0, height/2.0);
        this->setPosition(x + width/2.0, y + height/2.0);
        this->setFillColor(sf::Color::White);

        res->SetUserData(this);
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

    void removeBody()
    {
        res->GetWorld()->DestroyBody(res);
        res->SetUserData(0);
    }


    //member variable
    b2Body* res;


};

