#ifndef INVADERS_H
#define INVADERS_H
#include <string>
#include "Bullet.h"
#include "Invader.h"

class Invaders
{
public:
    static const sf::Vector2u NumInvaders;
        enum Direction {Right, Left};

    Invaders();
    void moveInvadersToStartPosition();
    void draw(sf::RenderWindow& window);
    bool move();   // return false if unable to move
    Invader& getInvader(int row, int col)
    {
        return invader[row][col];
    }
    int getCountVisible() const
    {
        return countVisible;
    }
    sf::Vector2f getInvaderPosition(int row, int col) const;
    int getMaxIndexOfVisibleInvaderByColumn(int column) const;
    int getMaxIndexOfVisibleInvaders() const;
    int getIndexOfRightmostInvaderColumn() const;
    int getIndexOfLeftmostInvaderColumn() const;
    float getPositionOfBottomVisibleInvader() const;
    int invaderHitByBullet(Bullet& bullet);
    void updateStatus();
    void increaseSpeed()
    {
        speed *= 1.031f;    // 3% increase
    }
    void addRandomInvader();

private:
    Invader invader[5][12];
    sf::Texture InvaderTexture;
    Direction direction;
    float speed;
    int countVisible;
};

#endif // INVADERS_H
