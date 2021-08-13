#include "Constants.h"
#include "Bullet.h"

Bullet::Bullet(const sf::Vector2f& pos)
    : sf::CircleShape(BulletRadius)
{
    setOrigin(getLocalBounds().width / 2.0f, getLocalBounds().height / 2.0f);
    setPosition(pos);
    setFillColor(sf::Color::Yellow);
}

bool Bullet::move()
{
    setPosition(getPosition().x, getPosition().y - BulletSpeed);
    if (getPosition().y < 40.0f)
        return false;
    return true;
}

