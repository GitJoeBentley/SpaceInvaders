#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Bomb.h"
#include "Constants.h"

using std::cout;
using std::endl;


Bomb::Bomb(int index)
    : imageIndex(index<3?index:rand()%3)
{
    texture.loadFromFile(BombImageFile);
    setTexture(texture);
    setOrigin(getLocalBounds().width/2.0f,getLocalBounds().height / 2.0f);
    sf::IntRect bombImageLocationInTexture;
    bombImageLocationInTexture.height = static_cast<int>(BombSize.y);
    bombImageLocationInTexture.width = static_cast<int>(BombSize.x);
    bombImageLocationInTexture.left = 0;
    bombImageLocationInTexture.top = imageIndex * texture.getSize().y / 3;
    setTextureRect(bombImageLocationInTexture);
}

// A targeted bomb drops just above the gun (slightly offset, left or right))
Bomb::Bomb(float Xcoordinate)
    : imageIndex(0),
      position(sf::Vector2f(rand()%2 ? Xcoordinate + 10.0f : Xcoordinate - 10.0f, 0.5f * MainWindowHeight))
{
    // set texture for bomb
    texture.loadFromFile(BombImageFile);
    setTexture(texture);
    sf::IntRect bombImageLocationInTexture;
    bombImageLocationInTexture.height = static_cast<int>(BombSize.y);
    bombImageLocationInTexture.width = static_cast<int>(BombSize.x);
    bombImageLocationInTexture.left = 0;
    bombImageLocationInTexture.top = 0;
    setTextureRect(bombImageLocationInTexture);
    setOrigin(getLocalBounds().width/2.0f,getLocalBounds().height / 2.0f);
    setPosition(position);
}

void Bomb::moveBombToStartPosition(const Invaders& invaders)
{
    int bomberCol = rand() % Invaders::NumInvaders.y;
    int bomberRow = invaders.getMaxIndexOfVisibleInvaderByColumn(bomberCol);
    if (bomberRow != -1)
        position = sf::Vector2f(invaders.getInvaderPosition(bomberRow,bomberCol));
    else
        position = sf::Vector2f(static_cast<float>(MainWindowWidth), static_cast<float>(MainWindowHeight));
}

bool Bomb::move()
{
    int bombWiggle = rand() % 10;
    if (position == sf::Vector2f(static_cast<float>(MainWindowWidth), static_cast<float>(MainWindowHeight)))
    {
        return false;
    }

    sf::IntRect bombImageLocationInTexture;
    bombImageLocationInTexture = getTextureRect();
    if (bombImageLocationInTexture.left == 0)
        bombImageLocationInTexture.left = 12;
    else
        bombImageLocationInTexture.left = 0;

    setTextureRect(bombImageLocationInTexture);
    position.y += BombSpeed;

    // Wiggle the bomb once in a while
    if (bombWiggle == 1) position.x += BombSpeed;
    if (bombWiggle == 2) position.x -= BombSpeed;

    setPosition(position);
    if (position.y > 0.95f * MainWindowHeight)
        return false;
    return true;
}

bool Bomb::hitByBullet(Bullet& bullet)
{
    bool hit;
    if ((position.y - bullet.getPosition().y) > BombSize.y / 2.0f)
        return false;
    float diffX = fabs(position.x - bullet.getPosition().x);
    float diffY = fabs(position.y - bullet.getPosition().y);
    hit = std::sqrt(diffX * diffX + diffY * diffY) < (BombSize.x / 2.0f + 1.0f);
    return hit;
}

float Bomb::bottomPosition() const
{
    return position.y + BombSize.y / 2.0f;
}

sf::Vector2f Bomb::nosePosition() const
{
    return sf::Vector2f(position.x, position.y + BombSize.y / 2.0f);
}

float Bomb::leftSidePosition() const
{
    return position.x - BombSize.x / 4.0f;
}

float Bomb::rightSidePosition() const
{
    return position.x + BombSize.x / 4.0f;
}
