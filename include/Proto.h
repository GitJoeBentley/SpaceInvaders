#ifndef PROTO_H_INCLUDED
#define PROTO_H_INCLUDED

#include <string>
#include <list>

#include <SFML/Audio.hpp>

#include "HighScores.h"
#include "Invaders.h"
#include "Explosion.h"
#include "Gun.h"
#include "Bullet.h"
#include "Saucer.h"
#include "Bomb.h"
#include "Shield.h"

// Function prototypes
std::string welcome(sf::RenderWindow& window, const HighScores&, Invaders& invaders);
std::string getWelcomeText();
char getKey();
void startSound(sf::Sound& sound);
void stopSound(sf::Sound& sound);
void displayWindowObjects(sf::RenderWindow& window, sf::RectangleShape& background, sf::Text& text, Gun* gun, Invaders& invaders, Explosion& explosion, Bomb* bombPtr, Saucer* saucerPtr, std::list<Bullet*>& bulletsInFlight, sf::Text& gameOverText, Shield* shield);


#endif // PROTO_H_INCLUDED
