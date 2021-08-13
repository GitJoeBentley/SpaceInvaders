#ifndef INVADER_H
#define INVADER_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class Invader : public sf::RectangleShape
{
    public:
        enum Status {Visible,Hit,Invisible};
        Invader();
        virtual ~Invader();
        Status getStatus() const { return status; }
        void setStatus(Status stat) { this->status = stat;  explosionCount = 1;}
        bool isVisible() const { return status == Visible; }
        void setVisible() { status = Visible; }
        void setHitStatus() { status = Hit; }
        void setExplosionCount() { explosionCount = 1; }
        void resetExplosionCount() { explosionCount = 0u; }
        unsigned getExplosionCount() const { return explosionCount; }
        void incrementExplosionCount() { ++explosionCount; if (explosionCount > 100) status = Invisible; }
    protected:

    private:
        Status status;
        unsigned explosionCount;
};

#endif // INVADER_H
