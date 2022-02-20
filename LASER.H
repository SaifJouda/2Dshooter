#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include "physics.h"
using namespace sf;
using namespace std;
struct Laser : public Sprite{
    Laser(b2World &world, float x, float y, string triangle_file, const sf::Texture &t) {
        size_t n;
        float vx , vy;
        this->setTexture(t);
        b2Vec2 vertices[3];
        b2PolygonShape polygonShape;
        b2BodyDef bodyDef;
        bodyDef.position.Set(x/pixels_per_meter, y/pixels_per_meter);
        bodyDef.type = b2_dynamicBody;
        bodyDef.linearDamping = 0.05;
        b2FixtureDef fixtureDef;
        fixtureDef.density = 1.0;
        fixtureDef.friction = 0.4;
        fixtureDef.restitution = 0.5;


        res = world.CreateBody(&bodyDef);
        ifstream myfile;
        myfile.open(triangle_file);
        if (!myfile.is_open()) {
            cout<<"No Vertex file located"<<endl;
        }
        else {
            cout<<"Vertex file opened successfully"<<endl;
        }
        myfile>>n;
        cout<<n<<endl;
        for (size_t i= 0; i < n; i++ ) {
            for (size_t j = 0; j < 3; j++) {
                myfile>>vx;
                myfile>>vy;
                vertices[j].Set(vx/pixels_per_meter,vy/pixels_per_meter);
            }
            polygonShape.Set(vertices, 3);
            fixtureDef.shape = &polygonShape;
            res->CreateFixture(&fixtureDef);
        }
        this->setPosition(x,y);

        myfile.close();
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

        void updatePosition()
    {

        this->setPosition(res->GetPosition().x*pixels_per_meter, res->GetPosition().y*pixels_per_meter);
        this->setRotation(res->GetAngle()*deg_per_rad+90.0);
    }

    b2Body* res;

};

