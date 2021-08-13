#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <algorithm>
#include "Shield.h"


Shield::Shield()
{
    static uint64_t InitialShieldData[ShieldHeight] = {0};
    static bool GotShieldData = false;
    static sf::Vector2f ShieldPosition = sf::Vector2f{0.0f, ShieldYPosition};
    ShieldPosition.x += MainWindowWidth / 4.0f;
    if (!GotShieldData)
    {
        getShieldDataFromFile(InitialShieldData);
        GotShieldData = true;
    }
    memcpy(shieldData,InitialShieldData,sizeof(InitialShieldData));
    // create the Shield image
    shieldImage.create(64, 45, sf::Color::Transparent);
    uint64_t mask;
    for (int y = 0; y < 45; y++)
    {
        mask = 1;
        for (int x = 0; x < 64; x++, mask <<= 1)
        {
            if (mask & shieldData[y])
                shieldImage.setPixel(x,y,sf::Color(0,255,0,255));
            else
                shieldImage.setPixel(x,y,sf::Color::Transparent);
        }
    }
    shieldTexture.loadFromImage(shieldImage);
    shieldSprite.setTexture(shieldTexture);
    shieldSprite.setOrigin(shieldSprite.getLocalBounds().width/2.0f,shieldSprite.getLocalBounds().height / 2.0f);
    shieldSprite.setPosition(ShieldPosition);
}


void Shield::getShieldDataFromFile(uint64_t* InitialShieldData)
{
    std::string buffer;
    std::ifstream fin(ShieldDataFile);
    if (!fin)
    {
        std::cerr << "Unable to open file " << ShieldDataFile << std::endl;
        std::exit(23);
    }
    for (auto i = 0u; i < ShieldHeight; i++)
    {
        getline(fin,buffer);
        // remove spaces
        buffer.erase(std::remove(buffer.begin(),buffer.end(),' '),buffer.end());
        // didn't work on Mac g++ InitialShieldData[i] = bitset<64>(buffer).to_ullong();
        InitialShieldData[i] = std::stoull(buffer.c_str(),0,2);
    }
}

// Is the bomb aligned with the shield?
bool Shield::isAlignedWithBomb(const Bomb& bomb) const
{
    if (bomb.rightSidePosition() <= leftSidePosition())
        return false;
    if (bomb.leftSidePosition() >= rightSidePosition())
        return false;
    return true;
}

// Is the bullet aligned with the shield?
bool Shield::isAlignedWithBullet(const Bullet& bullet) const
{
    if (bullet.getPosition().x > rightSidePosition() + BulletRadius)
        return false;
    if (bullet.getPosition().x < leftSidePosition() - BulletRadius)
        return false;
    if (bullet.getPosition().y > bottomOfShield())
        return false;
    return true;
}

bool Shield::hitByBomb(sf::Vector2f position)
{
    bitset<64> bits;
    int minXpos = static_cast<int>(position.x - BombSize.x/2 - 1);
    minXpos = minXpos < 0 ? 0 : minXpos;
    int maxXpos = static_cast<int>(position.x + BombSize.x/2 + 1);
    maxXpos = maxXpos > 63 ? 63 : maxXpos;
    bool itsAHit = false;

    // count the bits in the intersecting rectangle
    for (auto row = 0u; row < static_cast<size_t>(position.y + 1) && row < ShieldHeight; ++row)
    {
        bits.reset();
        bits |= shieldData[row];
        for (auto col = minXpos; col <= maxXpos; ++col)
        {
            if (bits.test(col))
            {
                itsAHit = true;
                break;
            }
        }
    }
    // if it's a hit, remove 5/6 pixels of the pixels in the targeted rectangle (+ a little more)
    if (itsAHit)
    {
        minXpos = minXpos - rand() % 5;
        minXpos = minXpos < 0 ? 0 : minXpos;
        maxXpos = maxXpos + rand() % 5;
        maxXpos = maxXpos > 63 ? 63 : maxXpos;
        unsigned maxYpos = static_cast<unsigned>(position.y + 1u + rand() % 10);

        for (auto row = 0u; row < maxYpos && row < ShieldHeight; ++row)
        {
            bits.reset();
            bits |= shieldData[row];
            for (auto col = minXpos; col <= maxXpos; ++col)
            {
                if (rand() % 6)
                    bits.reset(col);
            }
            shieldData[row] = bits.to_ullong();
        }
        uint64_t mask;
        for (int y = 0; y < 45; y++)
        {
            mask = 1;
            for (int x = 0; x < 64; x++, mask <<= 1)
            {
                if (mask & shieldData[y])
                    shieldImage.setPixel(x,y,GreenPixel);
                else
                    shieldImage.setPixel(x,y,TransparentPixel);
            }
        }
        shieldTexture.loadFromImage(shieldImage);
        shieldSprite.setTexture(shieldTexture);

    }
    return itsAHit;
}

bool Shield::hitByBullet(sf::Vector2f position)
{
    bool itsAHit = false;
    bitset<64> bits;
    int minXpos = static_cast<int>(position.x - BulletRadius - 1);
    minXpos = minXpos < 0 ? 0 : minXpos;
    int maxXpos = static_cast<int>(position.x + BulletRadius + 1);
    maxXpos = maxXpos > 63 ? 63 : maxXpos;
    int minRow = static_cast<unsigned>(position.y) - 1;
      minRow = minRow >= 0 ? minRow : 0;

    // count the bits in the intersecting rectangle
    for (int row = ShieldHeight - 1; row >= minRow; --row)
    {
        bits.reset();
        bits |= shieldData[row];
        for (auto col = minXpos; col <= maxXpos; ++col)
        {
            if (bits.test(col))
            {
                itsAHit = true;
                break;
            }
        }
    }
    // if it's a hit, remove 4/5 pixels of the pixels in the targeted rectangle (+ a little more)
    if (itsAHit)
    {
        minXpos = minXpos - rand() % 2;
        minXpos = minXpos < 0 ? 0 : minXpos;
        maxXpos = maxXpos + rand() % 2;
        maxXpos = maxXpos > 63 ? 63 : maxXpos;

        for (int row = ShieldHeight-1; row >= 0 && row >= minRow; --row)
        {
            bits.reset();
            bits |= shieldData[row];
            for (auto col = minXpos; col <= maxXpos; ++col)
            {
                if (rand() % 8)
                    bits.reset(col);
            }
            shieldData[row] = bits.to_ullong();
        }
        uint64_t mask;
        for (int y = 0; y < 45; y++)
        {
            mask = 1;
            for (int x = 0; x < 64; x++, mask <<= 1)
            {
                if (mask & shieldData[y])
                    shieldImage.setPixel(x,y,sf::Color(0,255,0,255));
                else
                    shieldImage.setPixel(x,y,sf::Color::Transparent);
            }
        }
        shieldTexture.loadFromImage(shieldImage);
        shieldSprite.setTexture(shieldTexture);
    }
    return itsAHit;
}
