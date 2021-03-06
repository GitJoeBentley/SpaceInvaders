#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <string>

#include <SFML/Graphics.hpp>


class Explosion
{
public:
    Explosion(const std::string& filename);
    bool isExploding() const
    {
        return exploding;
    }
    sf::Sprite getExplosion() const
    {
        return explosion;
    }
    void startExplosion(sf::Vector2f location)
    {
        explosion.setPosition(location);
        count = 0;
        exploding = true;
    }
    void update();
    sf::Vector2f getPosition() const { return explosion.getPosition(); }

private:
    sf::Texture texture;
    sf::Sprite explosion;
    bool exploding;
    int count;
};

#endif // EXPLOSION_H
