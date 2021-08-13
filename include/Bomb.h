#ifndef BOMB_H
#define BOMB_H

#include <utility>
#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "Invaders.h"

class Invaders;  // forward declaration

const sf::Vector2f BombSize{12.0f,24.0f};
//extern const sf::Vector2u NumInvaders;

class Bomb : public sf::Sprite
{
public:
    Bomb(int imageIndex = 4);
    Bomb(float Xcoordinate);   // bomb targeted at gun location
    void moveBombToStartPosition(const Invaders& invaders);
    bool move();
    bool hitByBullet(Bullet& bullet);
    float bottomPosition() const;
    sf::Vector2f nosePosition() const;
    float leftSidePosition() const;
    float rightSidePosition() const;
protected:

private:
    sf::Texture texture;
    int imageIndex;
    sf::Vector2f position;
};

#endif // BOMB_H
