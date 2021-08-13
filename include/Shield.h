#ifndef SHIELD_H
#define SHIELD_H

#include <bitset>
#include <stdint.h>
#include <SFML/Graphics.hpp>

#include "Constants.h"
#include "Invaders.h"
#include "Bomb.h"

using std::bitset;

const unsigned ShieldHeight = 45;
const sf::Vector2f ShieldSize{64.0f,static_cast<float>(ShieldHeight)};


class Shield
{
public:
    Shield();
    const sf::Sprite& getShield() const
    {
        return shieldSprite;
    }
    float topOfShield() const
    {
        return shieldSprite.getPosition().y - ShieldHeight / 2.0f;
    }
    float bottomOfShield() const
    {
        return shieldSprite.getPosition().y + ShieldHeight / 2.0f;
    }
    float leftSidePosition() const
    {
        return shieldSprite.getPosition().x - 32.0f;
    }
   float rightSidePosition() const
    {
        return shieldSprite.getPosition().x + 32.0f;
    }
    bool isAlignedWithBomb(const Bomb& bomb) const;
    bool isAlignedWithBullet(const Bullet& bullet) const;
    bool hitByBomb(sf::Vector2f shieldPosition);
    bool hitByBullet(sf::Vector2f shieldPosition);

private:
    uint64_t shieldData[ShieldHeight];
    sf::Image shieldImage;
    sf::Texture shieldTexture;
    sf::Sprite shieldSprite;

    void getShieldDataFromFile(uint64_t* InitialShieldData);

};

#endif // SHIELD_H
