#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "Constants.h"
#include "Saucer.h"
#include "Invaders.h"

Saucer::Saucer()
    : direction(static_cast<Direction>(rand()%2))
{
    texture.loadFromFile(SaucerImageFile);
    setTexture(texture);
    setScale(sf::Vector2f(1.0f,1.0f));
    setOrigin(getLocalBounds().width/2.0f,getLocalBounds().height / 2.0f);
    setPosition((direction == East) ? sf::Vector2f(0.0f,75.0f): sf::Vector2f(static_cast<float>(MainWindowWidth),75.0f));
}

bool Saucer::move()
{
    if (direction == East)
    {
        setPosition(getPosition().x + 0.2f, getPosition().y);
        if (getPosition().x > static_cast<float>(MainWindowWidth))
            return false;
    }
    else
    {
        setPosition(getPosition().x - 0.2f, getPosition().y);
        if (getPosition().x< 0.0f)
            return false;
    }
    return true;
}

bool Saucer::hitByBullet(const Bullet& bullet) const
{
    bool hit;
    // if bullet is below Saucer, there is no hit
    if ((getPosition().y - bullet.getPosition().y) > size().y / 2.0f)
        return false;

    float diffX = fabs(getPosition().x - bullet.getPosition().x);
    float diffY = fabs(getPosition().y - bullet.getPosition().y);
    hit = std::sqrt(diffX * diffX + diffY * diffY) < size().x / 2.0f;
    return hit;
}

