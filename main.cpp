#include "physics.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <SFML/Audio.hpp>
#include "SFML/Graphics.hpp"
#include "Ship.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Particles.h"
#include "Shockwave.h"
#include "Wall.h"
#include "Laser.h"

using namespace std;
using namespace sf; //for graphics


static RenderWindow window;

class FooDraw: public b2Draw
{
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
    {
        cout<<"hello"<<endl;
        vector<Vertex> v;
        for (int i=0; i < vertexCount; i++)
        {
            v.push_back(Vertex(Vector2f(vertices[i].x*pixels_per_meter, vertices[i].y*pixels_per_meter)));
        }
        window.draw(&v[0], vertexCount, TriangleStrip);
    }
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {}

    /// Draw a solid closed polygon provided in CCW order.


    /// Draw a circle.
    void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) {}

    /// Draw a solid circle.
    void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) {}

    /// Draw a line segment.
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {}

    /// Draw a transform. Choose your own length scale.
    /// @param xf a transform.
    void DrawTransform(const b2Transform& xf) {}

    /// Draw a point.
    void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) {}

};




int main()
{

    Music song1;
    song1.openFromFile("Enter-the-Gungeon-Gungeon-Up-Gungeon-Down-OST.ogg");

    Music song2;
    song2.openFromFile("Enter-The-Gungeon-Secretes.ogg");

    int song1play=1;
    int song2play=0;



    cout<<"Press/Hold [Left Click] to shoot"<<endl;
    cout<<"Press/Hold [Right Click] to use secondary ability"<<endl;
    cout<<"Press [E] to use ultimate ability"<<endl;
    cout<<""<<endl;

    cout<<"Cheats:"<<endl;
    cout<<"Press [U] to add xp"<<endl;
    cout<<"Press [L] to increase level (WARNING DONT HOLD IT DOWN)"<<endl;

    bool restartgame=false;
    bool endgame=false;

    while(endgame==false)
    {
        FooDraw fd;

        song1.play();
        song1.setLoop(true);

        //test
        CircleShape saif(10);
        saif.setFillColor(Color::Green);
        saif.setOrigin(10,10);

        window.create(VideoMode(1200,700),"Omar is Stinky");  //creates a window on the screen that is 800 by 600
        window.setFramerateLimit(60); //sets the game loop to run 60 times per second
        b2World world(b2Vec2(0.0, 0.0));
        window.setMouseCursorVisible(false); ///hides mouse

        ///Font
        Font f1;
        f1.loadFromFile("NovaSquare.ttf");

        ///Border
        vector<Wall> walls;
        walls.push_back(Wall(world,0,0,2,700));
        walls.push_back(Wall(world,0,0,1200,2));
        walls.push_back(Wall(world,1198,0,2,700));
        walls.push_back(Wall(world,0,698,1200,2));


        ///mouse
        Texture mouset;
        mouset.loadFromFile("mouse.png"); //texture
        float mousescale=0.7; //changes size
        int mousescale2=0;
        //int for mouse
        int mx,my;

        ///mouse shape
        RectangleShape mouser;
        mouser.setTexture(&mouset);
        mouser.setSize(Vector2f(22,31));

        ///points and level counter
        Text pointso, levelo;
        pointso.setFont(f1);
        levelo.setFont(f1);
        pointso.setCharacterSize(20);
        levelo.setCharacterSize(20);
        pointso.setPosition(1030,33);
        levelo.setPosition(1041,5);
        ///bullets
        vector<Bullet> bullet;
        Texture bullettexture;
        bullettexture.loadFromFile("bullet1.png");
        int bulletclock=0; //int to control rate of fire
        int bulletrate=10; //controls rate
        int abilitybulletclock=0;
        int abilitycooldown=0;


        ///Particle
        vector<Particle> particles;
        vector<int> particlestimer;
        vector<Particle> gameoverparticles;
        vector<int> gameoverparticlestimer;
        bool particle1=false;
        bool particle2=false;
        bool particle3=false;
        int particlex,particley;


        ///Ship
        Texture RedShip1;
        Texture RedShip2;
        Texture RedShip3;
        Texture GreenShip1;
        Texture GreenShip2;
        Texture GreenShip3;
        Texture BlueShip1;
        Texture BlueShip2;
        Texture BlueShip3;
        RedShip1.loadFromFile("RedShip1.png");
        RedShip2.loadFromFile("RedShip2.png");
        RedShip3.loadFromFile("RedShip3.png");
        GreenShip1.loadFromFile("GreenShip1.png");
        GreenShip2.loadFromFile("GreenShip2.png");
        GreenShip3.loadFromFile("GreenShip3.png");
        BlueShip1.loadFromFile("BlueShip1.png");
        BlueShip2.loadFromFile("BlueShip2.png");
        BlueShip3.loadFromFile("BlueShip3.png");

        vector<Ship> player;
//    player(world,400,300,"ship.txt",shiptexture);
//    player.setOrigin(27,26);
        int invtime=0;
        bool playerdamage=false;
        int health=100;


        ///Enemies
        //enemy vectors
        vector <Enemy> enemy1;
        vector <Enemy> enemy2;
        //Textures
        Texture enemyt1;
        enemyt1.loadFromFile("enemy11.png");
        Texture enemyt2;
        enemyt2.loadFromFile("enemy2.png");
        //int to control spawn rate
        int enemy1spawnrate=20,enemy1spawn=0;
        int enemy2spawnrate=20,enemy2spawn=0;


        ///Shockwave
        vector<Shockwave> shockcircle;


        ///Healthbar
        Texture mbar,hbar,abar,ubar,hearttex;
        mbar.loadFromFile("mainbar.png");
        hbar.loadFromFile("health.png");
        abar.loadFromFile("abilitybar.png");
        ubar.loadFromFile("ultimatebar.png");
        hearttex.loadFromFile("pixelheart.png");

        float hbarx,abarx,ubarx;
        hbarx=62;
        abarx=51;
        ubarx=78;
        int lives=3;
        bool ubardrain=false;
        Text lvnum;
        lvnum.setFont(f1);
        lvnum.setCharacterSize(14);
        lvnum.setPosition(58,51);
        // lvnum.setColor(Color::Black);


        ///points
        int points=0;
        int restartpoints=0;
        int restartlevel=0;

        ///level 0 is starting menu
        int level=0;
        int nospawntime=200;
        int enemyspawns=0;
        int enemyspawnslimit=50;
        float textvel=5;
        RectangleShape levelmover(Vector2f(1,1));
        levelmover.setPosition(500,-30);
        Text leveltext;
        Text levelnum;
        leveltext.setFont(f1);
        levelnum.setFont(f1);
        levelnum.setString(to_string(1));
        leveltext.setCharacterSize(30);
        leveltext.setString("Level");
        levelnum.setCharacterSize(30);
        leveltext.setPosition(500,-30);
        levelnum.setPosition(500,-30);

        bool updatelevel=true;
        int graceperiod=200;

        ///background
        Texture mainbacktexture;
        mainbacktexture.loadFromFile("Background.png");
        int backx=0,backy=0;
        float backf=0;



        vector<RectangleShape> stars;
        vector<int> starsvis;
        vector<bool> starsfade;
        int starsclock=0;

        ///Starting Screen
        Text playbutton;
        playbutton.setFont(f1);
        int playbuttonsize=60;
        int shipchosen=0;
        Text chooseship;
        chooseship.setFont(f1);
        Text spacedestroyer;
        spacedestroyer.setFont(f1);
        spacedestroyer.setCharacterSize(64);
        spacedestroyer.setPosition(320,100);
        spacedestroyer.setString("SPACE DESTROYER");

        ///Rpg points (the +100 that u see when an enemy is destroyed)
        vector <Text> rpgpoints;
        vector <int> rpgpointsy;
        vector <float> rpgvisibility;
        vector <int> rpgpointsx;

        ///black fade in affect during screens
        RectangleShape fade;
        fade.setFillColor(Color(0,0,0,0));
        fade.setSize(Vector2f(1200,700));
        int fadevis=0;
        bool fadein=false;

        ///exp bar and ship upgrade
        int experience=0;
        int explevel=1;
        bool shipup=false;
        bool shipfirstcreate=false;
        int shipuplevel=0;
        Text shiplev;
        shiplev.setCharacterSize(21);
        shiplev.setPosition(700,647);
        shiplev.setString("Level:");
        Text shiplevn;
        shiplevn.setCharacterSize(21);
        shiplevn.setPosition(770,647);
        shiplev.setFont(f1);
        shiplevn.setFont(f1);

        ///Game over and Checkpoints
        bool backtomain=false;
        bool restartatcheck=false;

        int gameoveranimationtime=0;
        bool gameoveranimation=false;
        int checkpointlevel=0;
        int checkpointship=0;
        int checkpointexp=0;
        int checkpointpoints=0;
        bool gameover=false;
        Text gameoverpointstext;
        //Text gameoverpointsnum;
        Text gameoverrestart;
        Text gameovertext;
        Text gameoverlevel;
        Text gameoverlevelcheck;
        //Text gameoverlevelnum;
        Text gameoverreturn;
        float gameovershipx;
        float gameovershipy;
        int gameovertexty=0;
        float gameovertextvelocity;
        float returntomainmenu=false;
        int particlevisibility=255;

        gameoverreturn.setFont(f1);
        gameoverpointstext.setFont(f1);
        //gameoverpointsnum.setFont(f1);
        gameoverrestart.setFont(f1);
        gameovertext.setFont(f1);
        gameoverlevel.setFont(f1);
        gameoverlevelcheck.setFont(f1);
        //gameoverlevelnum.setFont(f1);

        gameoverreturn.setCharacterSize(42);
        gameoverpointstext.setCharacterSize(42);
        //gameoverpointsnum.setCharacterSize(42);
        gameoverrestart.setCharacterSize(42);
        gameovertext.setCharacterSize(64);
        gameoverlevel.setCharacterSize(42);
        gameoverlevelcheck.setCharacterSize(42);
        //gameoverlevelnum.setCharacterSize(42);

        gameoverreturn.setString("Exit Game");


        gameovertext.setString("GAME OVER");

        if (restartgame==true)
        {
            restartgame=false;
        }

//!Keyboard::isKeyPressed(Keyboard::R)
        while (window.isOpen()&& restartgame==false && endgame==false)     //the main game loop, exits if someone closes the window
        {

            Event event; //creates an event object, events include mouse clicks, mouse movement, keyboard presses, etc..
            while (window.pollEvent(event)) //loop that checks for events
            {
                if (event.type == Event::Closed) //checks if window is closed
                    window.close();
            }   //ends the event loop

            //  cout<<fadevis<<endl;
            ///fade effect


//        if(song1play==1)
//        {
//            song1.play();
//            song1.setLoop(true);
//        }else if(song2play==1)
//        {
//            song2.play();
//            song2.setLoop(true);
//        }

            if(Keyboard::isKeyPressed(Keyboard::Escape))
            {
                restartgame=true;
                endgame=true;
            }

            if(gameoveranimation==false)
            {
                if(fadein==true)
                {

                    fadevis+=10;
                    if(fadevis>=250)
                    {
                        fadein=false;
                        if(level<2)
                        {
                            level++;
                        }
                    }
                }
                else if(fadevis>0)
                {
                    fadevis-=10;
                }
            }
            else
            {
                if(fadein==true && fadevis<250)
                {

                    fadevis+=10;
                    if(gameover==true)
                    {
                        fadein=false;
                    }
                }
                else if(fadevis>0 && gameover==true)
                {
                    fadevis-=10;
                }

            }
            fade.setFillColor(Color(0,0,0,fadevis));

            ///point and level tracker
            pointso.setString("Points: "+to_string(points));
            levelo.setString("Level: "+to_string(level-1));

            ///mouse
            mouser.setPosition(Mouse::getPosition(window).x,Mouse::getPosition(window).y);
            mouser.setScale(mousescale,mousescale);
            mousescale2++;
            if (mousescale2 >= 0 && mousescale2 <= 10)
            {
                mousescale+=0.03;
            }
            else if (mousescale2 >= 10 && mousescale2 <= 20)
            {
                mousescale-=0.03;
            }
            if( mousescale2 >20)
            {
                mousescale2=0;
            }

            if(Keyboard::isKeyPressed(Keyboard::L))
            {
                level++;
            }


            ///main menu
            if(level==0)
            {
                sf::Sprite mainbackground(mainbacktexture,sf::IntRect(backx,backy,500,320));
                mainbackground.setScale(2.4,2.4);
                if(backf>=1)
                {

                    if(backx>=3500)
                    {
                        if(backy>=4480)
                        {
                            backx=0;
                            backy=0;
                        }
                        else
                        {
                            backx=0;
                            backy+=320;
                        }
                    }
                    else
                    {
                        backx+=500;
                    }

                    backf=0;
                }
                else
                {
                    backf++;
                }



                playbutton.setCharacterSize(playbuttonsize);
                playbutton.setPosition(540,350);
                playbutton.setString("Play");
                if(545<Mouse::getPosition(window).x && Mouse::getPosition(window).x<655 && 370<Mouse::getPosition(window).y && Mouse::getPosition(window).y<410&& playbuttonsize<70)
                {
                    playbuttonsize++;
                    if(Mouse::isButtonPressed(Mouse::Left))
                    {

                        fadein=true;
                    }
                }
                else if(playbuttonsize>60)
                {
                    playbuttonsize--;
                }



                window.clear(); //clears the screen
                //world.Step(1.0/60, int32(8), int32(3));

                window.draw(mainbackground);

                window.draw(playbutton);
                window.draw(spacedestroyer);

                window.draw(mouser);
                for (auto &i : walls)
                    window.draw(i);
                window.draw(fade);
//            window.display();
            }
            ///choosing screen
            if(level==1)
            {
                starsclock++;
                if(starsclock>20)
                {
                    stars.push_back(RectangleShape(Vector2f(2,2)));
                    stars.back().setFillColor(Color(255,255,255,1));
                    stars.back().setPosition(rand()%1200,rand()%700);
                    starsvis.push_back(1);
                    starsfade.push_back(false);
                    starsclock=0;
                }

                for(int i=0; i<stars.size(); i++)
                {

                    if(starsfade[i]==true)
                    {
                        starsvis[i]-=2;
                    }
                    else
                    {
                        starsvis[i]+=2;
                    }

                    if(starsvis[i]<1)
                    {
                        stars.erase(stars.begin()+i);
                        starsvis.erase(starsvis.begin()+i);
                        starsfade.erase(starsfade.begin()+i);
                    }

                    if(starsvis[i]>=255)
                    {
                        starsfade[i]=true;
                    }

                    stars[i].setFillColor(Color(255,255,255,starsvis[i]));

                }



                ///creates ships to chose from
                sf::Sprite red(RedShip1,sf::IntRect(0,0,50,50));
                sf::Sprite blue(BlueShip1,sf::IntRect(0,0,62,48));
                sf::Sprite green(GreenShip1,sf::IntRect(0,0,64,42));


                ///changes size of ship when hovering over them
                if(315<Mouse::getPosition(window).x && Mouse::getPosition(window).x<435 && 285<Mouse::getPosition(window).y && Mouse::getPosition(window).y<445)
                {
                    red.setScale(3,3);
                    red.setPosition(312,285);
                    if(Mouse::isButtonPressed(Mouse::Left))
                    {
                        fadein=true;
                        shipchosen=1;
                    }
                }
                else
                {
                    red.setScale(2,2);
                    red.setPosition(335,300);

                }

                if(505<Mouse::getPosition(window).x && Mouse::getPosition(window).x<685 && 300<Mouse::getPosition(window).y && Mouse::getPosition(window).y<440)
                {
                    blue.setScale(3,3);
                    blue.setPosition(505,295);
                    if(Mouse::isButtonPressed(Mouse::Left))
                    {
                        fadein=true;
                        shipchosen=2;
                    }

                }
                else
                {
                    blue.setScale(2,2);
                    blue.setPosition(535,310);

                }

                if(725<Mouse::getPosition(window).x && Mouse::getPosition(window).x<935 && 300<Mouse::getPosition(window).y && Mouse::getPosition(window).y<440)
                {
                    green.setScale(3,3);
                    green.setPosition(705,300);
                    if(Mouse::isButtonPressed(Mouse::Left))
                    {
                        fadein=true;
                        shipchosen=3;
                    }
                }
                else
                {
                    green.setScale(2,2);
                    green.setPosition(735,320);

                }

                chooseship.setCharacterSize(50);
                chooseship.setPosition(425,200);
                chooseship.setString("Select A Ship");


                //  red.setPosition(,)
                window.clear(); //clears the screen
                world.Step(1.0/60, int32(8), int32(3));

                for (auto &i : stars)
                    window.draw(i);

                window.draw(red);
                window.draw(blue);
                window.draw(green);
                window.draw(chooseship);

                window.draw(mouser);
                for (auto &i : walls)
                    window.draw(i);
                window.draw(fade);

            }
            ///main game
            if(level>1 && gameover==false)
            {
                ///exp bar
                RectangleShape expback(RectangleShape(Vector2f(400,20)));
                expback.setFillColor(Color(86,94,104,230));
                RectangleShape expred(RectangleShape(Vector2f(experience,20)));
                expback.setPosition(400,650);
                expred.setPosition(400,650);
                expred.setFillColor(Color(255,0,0,230));
                if (experience>=400)
                {
                    shipup=true;
                    shipuplevel++;
                    lives+=1;
                    experience=0;
                }

                if(Keyboard::isKeyPressed(Keyboard::U))
                {
                    experience+=2;
                }

                if(shipup==true)
                {
                    shockcircle.push_back(Shockwave(world,player[0].res->GetPosition().x*pixels_per_meter,player[0].res->GetPosition().y*pixels_per_meter,1));
                    shockcircle.back().setFillColor(Color(0,0,0,0));
                    if(shipchosen==1)
                    {
                        shockcircle.back().setOutlineColor(Color(255,0,0));
                    }
                    if(shipchosen==2)
                    {
                        shockcircle.back().setOutlineColor(Color(0,0,255));
                    }
                    if(shipchosen==3)
                    {
                        shockcircle.back().setOutlineColor(Color(0,255,0));
                    }
                    shockcircle.back().setOutlineThickness(4);

                    int playerx=player[0].getPosition().x;
                    int playery=player[0].getPosition().y;
                    if(shipuplevel==1)
                    {
                        if (shipchosen==1)
                        {
                            player[0].removeBody();
                            player.erase(player.begin());
                            player.push_back(Ship(world,playerx,playery,"RedShip.txt",RedShip2));
                            player.back().setOrigin(27,26);

                        }
                        if (shipchosen==2)
                        {
                            player[0].removeBody();
                            player.erase(player.begin());
                            player.push_back(Ship(world,playerx,playery,"BlueShip.txt",BlueShip2));
                            player.back().setOrigin(33,25);
                        }
                        if (shipchosen==3)
                        {
                            player[0].removeBody();
                            player.erase(player.begin());
                            player.push_back(Ship(world,playerx,playery,"GreenShip.txt",GreenShip2));
                            player.back().setOrigin(30,23.5);
                        }
                    }
                    if(shipuplevel==2)
                    {
                        if (shipchosen==1)
                        {
                            player[0].removeBody();
                            player.erase(player.begin());
                            player.push_back(Ship(world,playerx,playery,"RedShip.txt",RedShip3));
                            player.back().setOrigin(27,32);
                        }
                        if (shipchosen==2)
                        {
                            player[0].removeBody();
                            player.erase(player.begin());
                            player.push_back(Ship(world,playerx,playery,"BlueShip.txt",BlueShip3));
                            player.back().setOrigin(35,26);
                        }
                        if (shipchosen==3)
                        {
                            player[0].removeBody();
                            player.erase(player.begin());
                            player.push_back(Ship(world,playerx,playery,"GreenShip.txt",GreenShip3));
                            player.back().setOrigin(31,23);
                        }

                        if(shipuplevel>=3)
                        {

                        }
                    }

                    shipup=false;

                }

                ///ship level
                shiplevn.setString(to_string(shipuplevel+1));

                ///background

                starsclock++;
                if(starsclock>20)
                {
                    stars.push_back(RectangleShape(Vector2f(2,2)));
                    stars.back().setFillColor(Color(255,255,255,1));
                    stars.back().setPosition(rand()%1200,rand()%700);
                    starsvis.push_back(1);
                    starsfade.push_back(false);
                    starsclock=0;
                }

                for(int i=0; i<stars.size(); i++)
                {

                    if(starsfade[i]==true)
                    {
                        starsvis[i]-=2;
                    }
                    else
                    {
                        starsvis[i]+=2;
                    }

                    if(starsvis[i]<1)
                    {
                        stars.erase(stars.begin()+i);
                        starsvis.erase(starsvis.begin()+i);
                        starsfade.erase(starsfade.begin()+i);
                    }

                    if(starsvis[i]>=255)
                    {
                        starsfade[i]=true;
                    }

                    stars[i].setFillColor(Color(255,255,255,starsvis[i]));

                }


                ///bars
                sf::Sprite mainbar(mbar,sf::IntRect(0,0,120,51));
                mainbar.setScale(1.4,1.4);
                mainbar.setPosition(20,20);

                sf::Sprite healthbar(hbar,sf::IntRect(0,0,hbarx,9));
                healthbar.setScale(1.4,1.4);
                healthbar.setPosition(90,43);

                sf::Sprite abilitybar(abar,sf::IntRect(0,0,abarx,10));
                abilitybar.setScale(1.4,1.4);
                abilitybar.setPosition(90,62);

                sf::Sprite ultimatebar(ubar,sf::IntRect(0,0,ubarx,8));
                ultimatebar.setScale(1.4,1.4);
                ultimatebar.setPosition(53,79);

                sf::Sprite heart (hearttex);
                heart.setScale(0.06,0.06);
                heart.setPosition(42,38);
                lvnum.setString(to_string(lives));


                ///Ship
                if(shipfirstcreate==false)
                {
                    if(shipchosen==1)
                    {
                        player.push_back(Ship(world,400,300,"RedShip.txt",RedShip1));
                        player.back().setOrigin(25,25);

                    }
                    if(shipchosen==2)
                    {
                        player.push_back(Ship(world,400,300,"BlueShip.txt",BlueShip1));
                        player.back().setOrigin(31,24);

                    }
                    if(shipchosen==3)
                    {
                        player.push_back(Ship(world,400,300,"GreenShip.txt",GreenShip1));
                        player.back().setOrigin(32,21);

                    }
//                b2PolygonShape saifisdoedoe = (b2PolygonShape)player[0].res->GetFixtureList().GetShape();
                    //saifisdoedoe.m_vertices
                    shipfirstcreate=true;
                }

                ///Moves Player
                // TODO    physics
                if(Keyboard::isKeyPressed(Keyboard::D)==true)
                {
                    player[0].res->ApplyForceToCenter(b2Vec2(20,0), true);
                }
                else
                {
                    if(player[0].res->GetLinearVelocity().x>0)
                    {
                        player[0].res->ApplyForceToCenter(b2Vec2(-1*(player[0].res->GetLinearVelocity().x),0), true);
                    }
                }

                if(Keyboard::isKeyPressed(Keyboard::A)==true)
                {
                    player[0].res->ApplyForceToCenter(b2Vec2(-20,0), true);
                }
                else
                {
                    if(player[0].res->GetLinearVelocity().x<0)
                    {
                        player[0].res->ApplyForceToCenter(b2Vec2(-1*(player[0].res->GetLinearVelocity().x),0), true);
                    }
                }

                if(Keyboard::isKeyPressed(Keyboard::S)==true)
                {
                    player[0].res->ApplyForceToCenter(b2Vec2(0,20), true);
                }
                else
                {
                    if(player[0].res->GetLinearVelocity().y>0)
                    {
                        player[0].res->ApplyForceToCenter(b2Vec2(0,-1*(player[0].res->GetLinearVelocity().y)), true);
                    }
                }
                if(Keyboard::isKeyPressed(Keyboard::W)==true)
                {
                    player[0].res->ApplyForceToCenter(b2Vec2(0,-20), true);
                }
                else
                {
                    if(player[0].res->GetLinearVelocity().y<0)
                    {
                        player[0].res->ApplyForceToCenter(b2Vec2(0,-1*(player[0].res->GetLinearVelocity().y)), true);
                    }
                }


                ///mouse cords
                mx=(Mouse::getPosition(window).x - player[0].getPosition().x);
                my=(Mouse::getPosition(window).y - player[0].getPosition().y);
                ///points ship at cursor

                float angle_deg = atan2(my, mx);
                float angle_deg2 =angle_deg;
                player[0].res->SetTransform(player[0].res->GetPosition(), angle_deg2);

                //player.setRotation(angle_deg*180.0/3.14153);
                if( (angle_deg) > player[0].res->GetAngle())
                {
                    player[0].res->SetAngularVelocity(10);
                }
                else if( (angle_deg) < player[0].res->GetAngle())
                {
                    player[0].res->SetAngularVelocity(-10);
                }
                else
                {
                    player[0].res->SetAngularVelocity(0);
                }

                ///Bullet
                //saif.setPosition(Vector2f(player[0].res->GetPosition().x*pixels_per_meter,player[0].res->GetPosition().y*pixels_per_meter));
                bulletclock++;
                if (bulletclock>bulletrate && gameoveranimation==false)
                {
                    if (Mouse::isButtonPressed(Mouse::Left))  //creates bullets
                    {


//                bullet.push_back(Bullet(world,player.res->GetPosition().x*pixels_per_meter + 29*cos(angle_deg - 3.14159/4) ,
//                                        player.res->GetPosition().y*pixels_per_meter + 29*sin(angle_deg - 3.14159/4), 28,9));
                        bullet.push_back(Bullet(world,player[0].res->GetPosition().x*pixels_per_meter,
                                                player[0].res->GetPosition().y*pixels_per_meter, 28,9));
                        bullet.back().res->GetFixtureList()->SetSensor(true);
                        bullet.back().res->SetType(b2_dynamicBody);
                        bullet.back().setRotation(angle_deg);
                        bullet.back().res->SetLinearVelocity(b2Vec2(20*cos(angle_deg), 20*sin(angle_deg)));
                        bullet.back().res->SetTransform(bullet.back().res->GetPosition(), angle_deg);
                        //bullet.back().setFillColor(Color(255, 0, 102));
                        bullet.back().setTexture(&bullettexture);
                        bullet.back().setOrigin(21,5);
                        bulletclock=0;

                    }
                }
                //cout<<shipuplevel<<endl;
                bulletrate=10/(shipuplevel+1)+2;






                if(abarx<=0)
                {
                    abilitycooldown=120;
                }

                if(abilitycooldown>0)
                {
                    abilitycooldown--;
                }

                abilitybulletclock++;

                if (Mouse::isButtonPressed(Mouse::Right) && abarx>0 && abilitycooldown<=0 && gameoveranimation==false)  //creates bullets
                {
                    if(abilitybulletclock>10)
                    {


//                bullet.push_back(Bullet(world,player.res->GetPosition().x*pixels_per_meter + 29*cos(angle_deg - 3.14159/4) ,
//                                        player.res->GetPosition().y*pixels_per_meter + 29*sin(angle_deg - 3.14159/4), 28,9));
                        if(shipuplevel>=0)
                        {
                            bullet.push_back(Bullet(world,player[0].res->GetPosition().x*pixels_per_meter,
                                                    player[0].res->GetPosition().y*pixels_per_meter, 28,9));
                            bullet.back().res->GetFixtureList()->SetSensor(true);
                            bullet.back().res->SetType(b2_dynamicBody);
                            bullet.back().setRotation(angle_deg+0.1);
                            bullet.back().res->SetLinearVelocity(b2Vec2(20*cos(angle_deg+0.1), 20*sin(angle_deg+0.1)));
                            bullet.back().res->SetTransform(bullet.back().res->GetPosition(), angle_deg+0.1);
                            bullet.back().setFillColor(Color(10, 10, 255));
                            bullet.back().setTexture(&bullettexture);
                            bullet.back().setOrigin(21,5);

                            bullet.push_back(Bullet(world,player[0].res->GetPosition().x*pixels_per_meter,
                                                    player[0].res->GetPosition().y*pixels_per_meter, 28,9));
                            bullet.back().res->GetFixtureList()->SetSensor(true);
                            bullet.back().res->SetType(b2_dynamicBody);
                            bullet.back().setRotation(angle_deg-0.1);
                            bullet.back().res->SetLinearVelocity(b2Vec2(20*cos(angle_deg-0.1), 20*sin(angle_deg-0.1)));
                            bullet.back().res->SetTransform(bullet.back().res->GetPosition(), angle_deg-0.1);
                            bullet.back().setFillColor(Color(10, 10, 255));
                            bullet.back().setTexture(&bullettexture);
                            bullet.back().setOrigin(21,5);
                        }

                        if(shipuplevel>=1)
                        {
                            bullet.push_back(Bullet(world,player[0].res->GetPosition().x*pixels_per_meter,
                                                    player[0].res->GetPosition().y*pixels_per_meter, 28,9));
                            bullet.back().res->GetFixtureList()->SetSensor(true);
                            bullet.back().res->SetType(b2_dynamicBody);
                            bullet.back().setRotation(angle_deg+0.2);
                            bullet.back().res->SetLinearVelocity(b2Vec2(20*cos(angle_deg+0.2), 20*sin(angle_deg+0.2)));
                            bullet.back().res->SetTransform(bullet.back().res->GetPosition(), angle_deg+0.2);
                            bullet.back().setFillColor(Color(10, 10, 255));
                            bullet.back().setTexture(&bullettexture);
                            bullet.back().setOrigin(21,5);


                            bullet.push_back(Bullet(world,player[0].res->GetPosition().x*pixels_per_meter,
                                                    player[0].res->GetPosition().y*pixels_per_meter, 28,9));
                            bullet.back().res->GetFixtureList()->SetSensor(true);
                            bullet.back().res->SetType(b2_dynamicBody);
                            bullet.back().setRotation(angle_deg-0.2);
                            bullet.back().res->SetLinearVelocity(b2Vec2(20*cos(angle_deg-0.2), 20*sin(angle_deg-0.2)));
                            bullet.back().res->SetTransform(bullet.back().res->GetPosition(), angle_deg-0.2);
                            bullet.back().setFillColor(Color(10, 10, 255));
                            bullet.back().setTexture(&bullettexture);
                            bullet.back().setOrigin(21,5);
                        }

                        if(shipuplevel>=2)
                        {
                            bullet.push_back(Bullet(world,player[0].res->GetPosition().x*pixels_per_meter,
                                                    player[0].res->GetPosition().y*pixels_per_meter, 28,9));
                            bullet.back().res->GetFixtureList()->SetSensor(true);
                            bullet.back().res->SetType(b2_dynamicBody);
                            bullet.back().setRotation(angle_deg+0.1);
                            bullet.back().res->SetLinearVelocity(b2Vec2(20*cos(angle_deg+0.1), 20*sin(angle_deg+0.3)));
                            bullet.back().res->SetTransform(bullet.back().res->GetPosition(), angle_deg+0.3);
                            bullet.back().setFillColor(Color(10, 10, 255));
                            bullet.back().setTexture(&bullettexture);
                            bullet.back().setOrigin(21,5);

                            bullet.push_back(Bullet(world,player[0].res->GetPosition().x*pixels_per_meter,
                                                    player[0].res->GetPosition().y*pixels_per_meter, 28,9));
                            bullet.back().res->GetFixtureList()->SetSensor(true);
                            bullet.back().res->SetType(b2_dynamicBody);
                            bullet.back().setRotation(angle_deg-0.1);
                            bullet.back().res->SetLinearVelocity(b2Vec2(20*cos(angle_deg-0.1), 20*sin(angle_deg-0.3)));
                            bullet.back().res->SetTransform(bullet.back().res->GetPosition(), angle_deg-0.3);
                            bullet.back().setFillColor(Color(10, 10, 255));
                            bullet.back().setTexture(&bullettexture);
                            bullet.back().setOrigin(21,5);
                        }

                        if(shipuplevel>=3)
                        {
                            bullet.push_back(Bullet(world,player[0].res->GetPosition().x*pixels_per_meter,
                                                    player[0].res->GetPosition().y*pixels_per_meter, 28,9));
                            bullet.back().res->GetFixtureList()->SetSensor(true);
                            bullet.back().res->SetType(b2_dynamicBody);
                            bullet.back().setRotation(angle_deg+0.1);
                            bullet.back().res->SetLinearVelocity(b2Vec2(20*cos(angle_deg+0.1), 20*sin(angle_deg+0.4)));
                            bullet.back().res->SetTransform(bullet.back().res->GetPosition(), angle_deg+0.4);
                            bullet.back().setFillColor(Color(10, 10, 255));
                            bullet.back().setTexture(&bullettexture);
                            bullet.back().setOrigin(21,5);

                            bullet.push_back(Bullet(world,player[0].res->GetPosition().x*pixels_per_meter,
                                                    player[0].res->GetPosition().y*pixels_per_meter, 28,9));
                            bullet.back().res->GetFixtureList()->SetSensor(true);
                            bullet.back().res->SetType(b2_dynamicBody);
                            bullet.back().setRotation(angle_deg-0.1);
                            bullet.back().res->SetLinearVelocity(b2Vec2(20*cos(angle_deg-0.1), 20*sin(angle_deg-0.4)));
                            bullet.back().res->SetTransform(bullet.back().res->GetPosition(), angle_deg-0.4);
                            bullet.back().setFillColor(Color(10, 10, 255));
                            bullet.back().setTexture(&bullettexture);
                            bullet.back().setOrigin(21,5);
                        }

                        abilitybulletclock=0;


                    }
                    abarx-=0.4;
                }

                if(abarx<51)
                {
                    abarx+=0.1;
                }




                ///deletes bullets if out of screen
                for (int i=0; i<bullet.size(); i++)
                {
                    if (bullet[i].getPosition().x>1250 || bullet[i].getPosition().x<-50 || bullet[i].getPosition().y>750 || bullet[i].getPosition().y<-50)
                    {
                        bullet[i].removeBody();
                        bullet.erase(bullet.begin() + i);
                    }
                }



                ///Shockwave
                if(ubarx>=78 && ubardrain==false)
                {
                    if (Keyboard::isKeyPressed(Keyboard::E))
                    {
                        shockcircle.push_back(Shockwave(world,player[0].res->GetPosition().x*pixels_per_meter,player[0].res->GetPosition().y*pixels_per_meter,1));
                        //shockcircle.back().setPosition(Vector2f(Vector2f(player.res->GetPosition().x*pixels_per_meter,player.res->GetPosition().y*pixels_per_meter)));
                        shockcircle.back().setFillColor(Color(0,0,0,0));
                        if(shipchosen==1)
                        {
                            shockcircle.back().setOutlineColor(Color(255,0,0));
                        }
                        if(shipchosen==2)
                        {
                            shockcircle.back().setOutlineColor(Color(0,0,255));
                        }
                        if(shipchosen==3)
                        {
                            shockcircle.back().setOutlineColor(Color(0,255,0));
                        }
                        shockcircle.back().setOutlineThickness(3);
                        ubardrain=true;


                    }



                }
                if(ubardrain==true)
                {
                    ubarx-=4;
                    if(ubarx<=0)
                    {
                        ubardrain=false;
                    }
                }
                else if(ubarx<78)
                {
                    ubarx+=0.1;
                }


                for(int i=0; i<shockcircle.size(); i++)
                {
                    shockcircle[i].radius(shockcircle[i].getRadius()+60);
                    if(shockcircle[i].getRadius()>1600)
                    {

                        shockcircle[i].removeBody();
                        shockcircle.erase(shockcircle.begin()+i);

                    }
                }

                /// Enemies


                enemy1spawn++;
                enemy2spawn++;

                if(gameoveranimation==false)
                {
                    if(graceperiod<=0)
                    {
                        if(enemy1spawn>enemy1spawnrate)
                        {
                            enemyspawns++;
                            int randspawn=rand()%4+1;
                            if (randspawn==1)
                            {
                                enemy1.push_back(Enemy(world,-100,rand()%750,40,30));
                            }
                            if (randspawn==2)
                            {
                                enemy1.push_back(Enemy(world,1300,rand()%750,40,30));
                            }
                            if (randspawn==3)
                            {
                                enemy1.push_back(Enemy(world,rand()%1200,-100,40,30));
                            }
                            if (randspawn==4)
                            {
                                enemy1.push_back(Enemy(world,rand()%1200,850,40,30));
                            }


                            enemy1.back().setTexture(&enemyt1);
                            enemy1.back().setOrigin(17,15);
                            enemy1.back().res->GetFixtureList()->SetSensor(true);
                            enemy1.back().res->SetType(b2_dynamicBody);
                            enemy1.back().setFillColor(Color(255, 0, 102));
                            enemy1spawn=0;
                        }

                        if(enemy2spawn>enemy2spawnrate)
                        {
                            enemyspawns++;
                            int randspawn=rand()%4+1;
                            if (randspawn==1)
                            {
                                enemy2.push_back(Enemy(world,-100,rand()%750,30,30));
                            }
                            if (randspawn==2)
                            {
                                enemy2.push_back(Enemy(world,1300,rand()%750,30,30));
                            }
                            if (randspawn==3)
                            {
                                enemy2.push_back(Enemy(world,rand()%1200,-100,30,30));
                            }
                            if (randspawn==4)
                            {
                                enemy2.push_back(Enemy(world,rand()%1200,850,30,30));
                            }

                            enemy2.back().setTexture(&enemyt2);
                            enemy2.back().setOrigin(15,15);
                            enemy2.back().res->GetFixtureList()->SetSensor(true);
                            enemy2.back().res->SetType(b2_dynamicBody);
                            enemy2.back().setFillColor(Color(255, 0, 102));
                            enemy2spawn=0;
                        }
                    }
                }

                enemy1spawnrate=20-level/1.5+2;
                enemy2spawnrate=20-level/1.5+2;

                if(enemyspawns>=enemyspawnslimit && enemy1.size()<1 && enemy2.size()<1)
                {
                    level++;
                    enemyspawns=0;
                    enemyspawnslimit+=50;
                    updatelevel=true;
                    graceperiod=400;
                    levelmover.setPosition(500,-30);
                    textvel=5;
                    levelnum.setString(to_string(level-1));
                }

                if(graceperiod>0 && graceperiod<180)
                {
                    if(updatelevel==true)
                    {
                        levelnum.setPosition(levelmover.getPosition().x+100,levelmover.getPosition().y);
                        leveltext.setPosition(levelmover.getPosition().x,levelmover.getPosition().y);
                        textvel-=0.06;
                        levelmover.setPosition(levelmover.getPosition().x,levelmover.getPosition().y+textvel);
                    }
                }

                if(graceperiod>0)
                {
                    graceperiod--;
                }



                ///gets angle to player
                for(int i=0; i<enemy1.size(); i++)
                {
                    float angle_enm = atan2(enemy1[i].getPosition().y-player[0].getPosition().y, enemy1[i].getPosition().x-player[0].getPosition().x);
                    enemy1[i].res->SetTransform(enemy1[i].res->GetPosition(), angle_enm);
                    enemy1[i].res->SetLinearVelocity(b2Vec2(-5*cos(angle_enm), -5*sin(angle_enm)));         //.setRotation(angle_enm*180.0/3.14153);
                }

                for(int i=0; i<enemy2.size(); i++)
                {
                    float angle_enm = atan2(player[0].getPosition().y-enemy2[i].getPosition().y,player[0].getPosition().x-enemy2[i].getPosition().x);
                    enemy2[i].res->SetLinearVelocity(b2Vec2(5*cos(angle_enm), 5*sin(angle_enm)));

                    enemy2[i].res->SetTransform(enemy2[i].res->GetPosition(), enemy2[i].getRotation()+1);
                }

                ///collision from enemy to bullets
                for(int i=0; i<enemy1.size(); i++)
                {
                    for(int j=0; j<bullet.size(); j++)
                    {
                        if(enemy1[i].checkCollision(bullet[j].res))
                        {
                            particlex=enemy1[i].res->GetPosition().x;
                            particley=enemy1[i].res->GetPosition().y;
                            particle1=true;

                            points+=100;
                            experience+=2;
                            rpgpoints.push_back(Text("+100",f1,20));
                            rpgpointsy.push_back(enemy1[i].getPosition().y);
                            rpgpointsx.push_back(enemy1[i].getPosition().x);
                            rpgvisibility.push_back(255);


                            bullet[j].removeBody();
                            bullet.erase(bullet.begin() + j);
                            enemy1[i].removeBody();
                            enemy1.erase(enemy1.begin() + i);

                        }
                    }
                }

                for(int i=0; i<enemy2.size(); i++)
                {
                    for(int j=0; j<bullet.size(); j++)
                    {
                        if(enemy2[i].checkCollision(bullet[j].res))
                        {
                            particlex=enemy2[i].res->GetPosition().x;
                            particley=enemy2[i].res->GetPosition().y;
                            particle1=true;

                            rpgpoints.push_back(Text("+100",f1,20));
                            rpgpointsy.push_back(enemy2[i].getPosition().y);
                            rpgpointsx.push_back(enemy2[i].getPosition().x);
                            rpgvisibility.push_back(255);
                            points+=100;
                            experience+=2;

                            bullet[j].removeBody();
                            bullet.erase(bullet.begin() + j);
                            enemy2[i].removeBody();
                            enemy2.erase(enemy2.begin() + i);


                        }
                    }
                }

                ///kills enemies from shockwave
                for(int i=0; i<enemy1.size(); i++)
                {
                    for(int j=0; j<shockcircle.size(); j++)
                        if(enemy1[i].checkCollision(shockcircle[j].res))
                        {
                            particlex=enemy1[i].res->GetPosition().x;
                            particley=enemy1[i].res->GetPosition().y;
                            particle1=true;

                            points+=100;
                            experience+=2;
                            rpgpoints.push_back(Text("+100",f1,20));
                            rpgpointsy.push_back(enemy1[i].getPosition().y);
                            rpgpointsx.push_back(enemy1[i].getPosition().x);
                            rpgvisibility.push_back(255);
                            enemy1[i].removeBody();
                            enemy1.erase(enemy1.begin() + i);



                        }

                }

                for(int i=0; i<enemy2.size(); i++)
                {
                    for(int j=0; j<shockcircle.size(); j++)
                    {

                        if(enemy2[i].checkCollision(shockcircle[j].res))
                        {
                            particlex=enemy2[i].res->GetPosition().x;
                            particley=enemy2[i].res->GetPosition().y;
                            particle1=true;

                            points+=100;
                            experience+=2;
                            rpgpoints.push_back(Text("+100",f1,20));
                            rpgpointsy.push_back(enemy2[i].getPosition().y);
                            rpgpointsx.push_back(enemy2[i].getPosition().x);
                            rpgvisibility.push_back(255);
                            enemy2[i].removeBody();
                            enemy2.erase(enemy2.begin() + i);



                        }

                    }
                }

                ///rpg points control (moving them, making them fade and deleting them)
                for (int i=0; i<rpgpoints.size(); i++)
                {
                    rpgpoints[i].setPosition(rpgpointsx[i]-20,rpgpointsy[i]);
                    rpgpoints[i].setFillColor(Color(255,255,255,rpgvisibility[i]));
                }

                for (int i=0; i<rpgpoints.size(); i++)
                {
                    rpgpointsy[i]-=1;

                    rpgvisibility[i]-=4.26;
                    if(rpgvisibility[i]<1)
                    {

                        rpgpoints.erase(rpgpoints.begin() + i);
                        rpgpointsx.erase(rpgpointsx.begin() + i);
                        rpgpointsy.erase(rpgpointsy.begin() + i);
                        rpgvisibility.erase(rpgvisibility.begin()+i);

                    }
                }


                ///collision from player to enemies

                for(int i=0; i<enemy1.size(); i++)
                {
                    if(enemy1[i].checkCollision(player[0].res))
                    {
                        particlex=enemy1[i].res->GetPosition().x;
                        particley=enemy1[i].res->GetPosition().y;
                        particle1=true;
                        playerdamage=true;
                        enemy1[i].removeBody();
                        enemy1.erase(enemy1.begin() + i);


                    }

                }

                for(int i=0; i<enemy2.size(); i++)
                {
                    if(enemy2[i].checkCollision(player[0].res))
                    {
                        particlex=enemy2[i].res->GetPosition().x;
                        particley=enemy2[i].res->GetPosition().y;
                        particle1=true;
                        playerdamage=true;
                        enemy2[i].removeBody();
                        enemy2.erase(enemy2.begin() + i);


                    }

                }

                if(playerdamage==true)
                {
                    health-=4;
                    hbarx=health*0.62;
                    playerdamage=false;
                }


                if (health<=0 && gameover==false)
                {

                    if(lives>0)
                    {
                        health=100;
                        hbarx=health*0.62;
                        lives--;
                    }
                    else
                    {
                        gameoveranimation=true;
                        gameoveranimationtime--;
                    }
                    if (gameoveranimation==true && gameoveranimationtime<1)
                    {
                        fadein=true;
                        shockcircle.push_back(Shockwave(world,player[0].res->GetPosition().x*pixels_per_meter,player[0].res->GetPosition().y*pixels_per_meter,1));
                        shockcircle.back().setFillColor(Color(0,0,0,0));
                        if(shipchosen==1)
                        {
                            shockcircle.back().setOutlineColor(Color(255,0,0));
                        }
                        if(shipchosen==2)
                        {
                            shockcircle.back().setOutlineColor(Color(0,0,255));
                        }
                        if(shipchosen==3)
                        {
                            shockcircle.back().setOutlineColor(Color(0,255,0));
                        }
                        shockcircle.back().setOutlineThickness(4);

                        gameovershipx=player[0].getPosition().x;
                        gameovershipy=player[0].getPosition().y;
                        player[0].setColor(Color(0,0,0,0));
                        particle2=true;
                        gameoveranimationtime=120;

                    }





                    if(gameoveranimation==true && gameoveranimationtime<5)
                    {

                        gameover=true;
                        gameovertextvelocity=60;
                        gameoveranimation=false;


                    }
                }

                if(gameoveranimation==true && particlevisibility>0)
                {
                    particlevisibility-=3;
                }

                for(int i; i<particles.size(); i++)
                {
                    particles[i].setFillColor(Color(255, 0, 102,particlevisibility));
                }

                for(int i; i<gameoverparticles.size(); i++)
                {
                    gameoverparticles[i].setFillColor(Color(255, 0, 102,particlevisibility));
                }
                // cout<<particlevisibility<<endl;


                if(Keyboard::isKeyPressed(Keyboard::H))
                {
                    health-=3;
                }


                ///creates particles
                if(particle1==true)
                {
                    for(int i=0; i<10; i++)
                    {
                        particlestimer.push_back(360);
                        particles.push_back(Particle(world,particlex,particley,5,5));
                        particles.back().res->GetFixtureList()->SetSensor(true);
                        particles.back().res->SetType(b2_dynamicBody);
                        particles.back().res->SetTransform(b2Vec2(particlex,particley),0);
                        particles.back().res->SetLinearVelocity(b2Vec2(rand()%40-20,rand()%40-20));
                        particles.back().setFillColor(Color(255, 0, 102));
                    }
                    particle1=false;
                }

                for(int i=0; i<particlestimer.size(); i++)
                {
                    particlestimer[i]--;
                    if (particlestimer[i]<=0)
                    {
                        particles[i].removeBody();
                        particles.erase(particles.begin() + i);
                        particlestimer.erase(particlestimer.begin() + i);
                    }
                }


                if((level-1)%5==0)
                {
                    restartpoints=points;
                    restartlevel=(level-1);
                    checkpointexp=experience;
                    checkpointship=explevel;
                }


                ///updates position


                player[0].updatePosition();



                for(int i=0; i<particles.size(); i++)
                {
                    particles[i].updatePosition();
                }

                for(int i=0; i<bullet.size(); i++)
                {
                    bullet[i].updatePosition();
                }

                for(int i=0; i<enemy1.size(); i++)
                {
                    enemy1[i].updatePosition();
                }

                for(int i=0; i<enemy2.size(); i++)
                {
                    enemy2[i].updatePosition();
                }


                for(int i=0; i<shockcircle.size(); i++)
                {
                    shockcircle[i].updatePosition();
                }



                window.clear(); //clears the screen
                world.Step(1.0/60, int32(8), int32(3));

                // window.draw(mainbackground);

                for (auto &i : stars)
                    window.draw(i);

                for (auto &i : bullet)
                    window.draw(i);

                for (auto &i : enemy1)
                    window.draw(i);

                for (auto &i : enemy2)
                    window.draw(i);

                for (auto &i : particles)
                    window.draw(i);

                for (auto &i : shockcircle)
                    window.draw(i);

                for (auto &i : rpgpoints)
                    window.draw(i);

                window.draw(player[0]);

                //window.draw(saif);
                window.draw(mainbar);
                window.draw(healthbar);
                window.draw(abilitybar);
                window.draw(ultimatebar);
                window.draw(leveltext);
                window.draw(levelnum);
                window.draw(levelo);
                window.draw(pointso);
                window.draw(heart);
                window.draw(lvnum);
                window.draw(expback);
                window.draw(expred);
                window.draw(shiplev);
                window.draw(shiplevn);


                window.draw(mouser);
                window.draw(fade);
                for (auto &i : walls)
                    window.draw(i);





            }
            else if(level>1 && gameover==true)
            {
                starsclock++;
                if(starsclock>20)
                {
                    stars.push_back(RectangleShape(Vector2f(2,2)));
                    stars.back().setFillColor(Color(255,255,255,1));
                    stars.back().setPosition(rand()%1200,rand()%700);
                    starsvis.push_back(1);
                    starsfade.push_back(false);
                    starsclock=0;
                }

                for(int i=0; i<stars.size(); i++)
                {

                    if(starsfade[i]==true)
                    {
                        starsvis[i]-=2;
                    }
                    else
                    {
                        starsvis[i]+=2;
                    }

                    if(starsvis[i]<1)
                    {
                        stars.erase(stars.begin()+i);
                        starsvis.erase(starsvis.begin()+i);
                        starsfade.erase(starsfade.begin()+i);
                    }

                    if(starsvis[i]>=255)
                    {
                        starsfade[i]=true;
                    }

                    stars[i].setFillColor(Color(255,255,255,starsvis[i]));

                }

                if(particle2==true)
                {
                    for(int i=0; i<20; i++)
                    {
                        gameoverparticlestimer.push_back(360);
                        gameoverparticles.push_back(Particle(world,gameovershipx,gameovershipy,5,5));
                        gameoverparticles.back().res->GetFixtureList()->SetSensor(true);
                        gameoverparticles.back().res->SetType(b2_dynamicBody);
                        gameoverparticles.back().res->SetTransform(b2Vec2(gameovershipx,gameovershipy),0);
                        gameoverparticles.back().res->SetLinearVelocity(b2Vec2(rand()%40-20,rand()%40-20));
                        gameoverparticles.back().setFillColor(Color(255, 0, 102));

                    }
                    particle2=false;
                }

                for(int i=0; i<gameoverparticlestimer.size(); i++)
                {
                    gameoverparticlestimer[i]--;
                    if (gameoverparticlestimer[i]<=0)
                    {
                        gameoverparticles[i].removeBody();
                        gameoverparticles.erase(gameoverparticles.begin() + i);
                        gameoverparticlestimer.erase(gameoverparticlestimer.begin() + i);
                    }
                }


                for(int i=0; i<gameoverparticles.size(); i++)
                {
                    gameoverparticles[i].updatePosition();
                }



                gameoverrestart.setString("Back To Main Menu");
                gameoverlevel.setString("Level: "+to_string(level-1));
                gameoverpointstext.setString("Points: "+to_string(points));

                gameovertext.setPosition(390,gameovertexty+50);
                gameoverpointstext.setPosition(460,gameovertexty+200);

                gameoverlevelcheck.setPosition(850,gameovertexty+400);
                gameoverlevel.setPosition(500,gameovertexty+300);


                if(355<Mouse::getPosition(window).x && Mouse::getPosition(window).x<835&& 510<Mouse::getPosition(window).y && Mouse::getPosition(window).y<545)
                {
                    gameoverreturn.setCharacterSize(52);
                    gameoverreturn.setPosition(470,gameovertexty+500);
                    if(Mouse::isButtonPressed(Mouse::Left))
                    {
                        restartgame=true;
                        endgame=true;
                    }
                }
                else
                {
                    gameoverreturn.setCharacterSize(42);
                    gameoverreturn.setPosition(490,gameovertexty+500);
                }

                if(245<Mouse::getPosition(window).x && Mouse::getPosition(window).x<1000&& 410<Mouse::getPosition(window).y && Mouse::getPosition(window).y<440)
                {
                    gameoverrestart.setCharacterSize(52);
                    gameoverrestart.setPosition(335,gameovertexty+400);
                    if(Mouse::isButtonPressed(Mouse::Left))
                    {
                        restartgame=true;

                    }
                }
                else
                {
                    gameoverrestart.setCharacterSize(42);
                    gameoverrestart.setPosition(380,gameovertexty+400);
                }






                window.clear();
                for (auto &i : stars)
                    window.draw(i);
                for (auto &i : gameoverparticles)
                    window.draw(i);

                window.draw(gameoverpointstext);
                window.draw(gameoverrestart);
                window.draw(gameovertext);
                window.draw(gameoverlevel);
                window.draw(gameoverlevelcheck);
                window.draw(gameoverreturn);

                window.draw(mouser);
                window.draw(fade);
            }


            ////a/d/asd/asd/ad/ad/ad/ad/ad/ad/asd/a/da/sdasd/as/dasd/af/f/a
//        fd.SetFlags(b2Draw::e_shapeBit);
//        world.SetDebugDraw(&fd);
//
//        world.DrawDebugData();


            window.display();   //displays everything on the video card to the monitor
        }   //ends the game loop


        continue;

    }


    return 0;
}
