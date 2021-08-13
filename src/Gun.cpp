#include <cmath>
#include "Gun.h"

sf::Vector2f Gun::GunStartPosition(MainWindowWidth / 2.0f, 0.97f * MainWindowHeight);

Gun::Gun()
    : position(sf::Vector2f(MainWindowWidth / 2.0f, 0.96f * MainWindowHeight)),
      gunSpeed(13.0f),
      visible(true)
{
    texture.loadFromFile(GunImageFile);
    gun.setTexture(texture);
    gun.setPosition(position);
    gun.setOrigin(gun.getLocalBounds().width/2.0f,gun.getLocalBounds().height / 2.0f);
}

void Gun::moveToPosition(sf::Vector2f pos)
{
    position = pos;
    gun.setPosition(pos);
}

void Gun::move(Direction dir)
{
    if (dir == Right)
    {
        // if the gun is on the right edge, disallow move
        if (position.x < MainWindowWidth - size().x)
            position.x += gunSpeed;
    }
    if (dir == Left)
    {
        // if the gun is on the left edge, disallow move
        if (position.x > size().x)

            position.x -= gunSpeed;
    }
    gun.setPosition(position);
}

Bullet* Gun::shoot() const
{
    // Get gun location
    sf::Vector2f gunBarrelEnd = sf::Vector2f(position.x, position.y - (size().y / 1.5f));
    return new Bullet(gunBarrelEnd);
}

bool Gun::hitByBomb(Bomb& bomb)
{
    bool hit;
    if ((position.y - bomb.getPosition().y) > ((size().y + BombSize.y) / 2.0f))
        return false;
    float diffX = fabs(position.x - bomb.getPosition().x);
    float diffY = fabs(position.y - bomb.getPosition().y);
    hit = std::sqrt(diffX * diffX + diffY * diffY) < (size().x + BombSize.x) / 2.0f;
    if (hit) visible = false;
    return hit;
}

