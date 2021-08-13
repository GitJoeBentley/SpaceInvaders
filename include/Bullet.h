#ifndef BULLET_H
#define BULLET_H
#include <SFML/Graphics.hpp>

class Bullet : public sf::CircleShape
{
    public:
        Bullet(const sf::Vector2f& pos);
        bool move();
};

#endif // BULLET_H
