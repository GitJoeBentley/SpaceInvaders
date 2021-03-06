#ifndef GUN_H
#define GUN_H

#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "Invaders.h"
#include "Bullet.h"
#include "Bomb.h"

class Gun
{
public:
    enum Direction {Right, Left};
    static sf::Vector2f GunStartPosition;
    Gun();
    sf::Sprite getGun() const
    {
        return gun;
    }
    sf::Vector2f getPosition() const
    {
        return position;
    }
    void move(Direction dir);
    void moveToStartPosition();
    sf::Vector2f size() const
    {
        return sf::Vector2f(gun.getLocalBounds().width * gun.getScale().x, gun.getLocalBounds().height * gun.getScale().y);
    }
    Bullet* shoot() const;
    bool hitByBomb(Bomb& bomb);
    bool isVisible() const { return visible; }
    void setInvisible() { visible = false; }
    void setVisible() { visible = true; }
    void moveToPosition(sf::Vector2f pos = sf::Vector2f(GunStartPosition));

private:
    sf::Sprite gun;
    sf::Texture texture;
    sf::Vector2f position;
    float gunSpeed;
    bool visible;
};

#endif // GUN_H
