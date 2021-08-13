#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>

#include "Proto.h"
#include "Constants.h"

using std::cout;
using std::endl;

int main()
{
    sf::RenderWindow window(sf::VideoMode(MainWindowWidth, MainWindowHeight), "Space Invaders", sf::Style::Close);
    bool pauseFlag = false;
    bool gameOver = false;
    bool bulletHitsShield;
    int invaderHit = 0;
    Score score;
    int gunInPlay = 0;
    unsigned loopCount = 0;
    HighScores highScores;

    srand(static_cast<unsigned>(time(0)));

    /////// Background //////
    sf::RectangleShape background(sf::Vector2f(static_cast<float>(MainWindowWidth), static_cast<float>(MainWindowHeight)));
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile(BackgroundFile);
    background.setTexture(&backgroundTexture);

    // Declare window objects
    Invaders invaders;
    Gun gun[numGuns];
    Explosion explosion(ExplosionImageFile);
    std::list<Bullet*> bulletsInFlight;
    Bomb* bombPtr = nullptr;
    Saucer* saucerPtr = nullptr;
    Shield shields[3];

    //////////// Sound effects /////////////
    sf::SoundBuffer invaderBuffer;
    invaderBuffer.loadFromFile(InvadersSoundFile);
    sf::Sound invaderSound;
    invaderSound.setBuffer(invaderBuffer);
    invaderSound.setLoop(true);

    sf::SoundBuffer bulletBuffer;
    bulletBuffer.loadFromFile(BulletSoundFile);
    sf::Sound bulletSound;
    bulletSound.setBuffer(bulletBuffer);
    bulletSound.setVolume(25.0f);
    ;
    sf::SoundBuffer explosionBuffer;
    explosionBuffer.loadFromFile(ExplosionSoundFile);
    sf::Sound explosionSound;
    explosionSound.setBuffer(explosionBuffer);

    sf::SoundBuffer saucerBuffer;
    saucerBuffer.loadFromFile(SaucerSoundFile);
    sf::Sound saucerSound;
    saucerSound.setBuffer(saucerBuffer);
    saucerSound.setLoop(true);
    saucerSound.setVolume(10.0f);

    sf::SoundBuffer bombBuffer;
    bombBuffer.loadFromFile(BombSoundFile);
    sf::Sound bombSound;
    bombSound.setBuffer(bombBuffer);
    bombSound.setVolume(8.0f);

    sf::SoundBuffer bombExplosionBuffer;
    bombExplosionBuffer.loadFromFile(BombExplosionSoundFile);
    sf::Sound bombExplosionSound;
    bombExplosionSound.setBuffer(bombExplosionBuffer);

    sf::SoundBuffer applauseBuffer;
    applauseBuffer.loadFromFile(ApplauseSoundFile);
    sf::Sound applauseSound;
    applauseSound.setBuffer(applauseBuffer);

    // Declare and load a font
    sf::Font font;
    font.loadFromFile(ArielFontFile);

    // Create text
    sf::Text text(sf::String(""), font);
    text.setCharacterSize(24);
    text.setStyle(sf::Text::Bold);
    text.setFillColor(sf::Color(10,250,50));
    text.setPosition(80.0f,20.0f);
    sf::Text gameOverText(sf::String(""), font);
    gameOverText.setCharacterSize(32);
    gameOverText.setFillColor(sf::Color(210,250,20));
    gameOverText.setPosition(412.0f,550.0f);

    // Game Speed controls
    float deltaTime = .2f;
    float totalTime = 0.0f;
    sf::Clock clock;

    // Move the second and third gun to spare location
    gun[1].moveToPosition(sf::Vector2f(760.0f+75.0f,30.0f));
    gun[2].moveToPosition(sf::Vector2f(760.0f+150.0f,30.0f));

    std::string name = welcome(window, highScores, invaders);
    window.setTitle(sf::String(name + "\'s Space Invaders"));
    sf::Event evnt;
    invaders.moveInvadersToStartPosition();

    while (window.isOpen())
    {
        while (gunInPlay < numGuns && !gameOver)
        {
            while (window.pollEvent(evnt))
            {
                switch (evnt.type)
                {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {  // pause
                         stopSound(saucerSound);
                        pauseFlag = !pauseFlag;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
                        gameOver = true;
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))   // move to the right
                        gun[0].move(Gun::Right);
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))    // move to the left
                        gun[0].move(Gun::Left);
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))   // shoot the gun
                    {
                        // Limit the number of bullets that can be fired at one time
                        if (bulletsInFlight.size() < 2)
                        {
                            bulletsInFlight.push_back(gun[0].shoot());
                            score += -1;                                   // subtract one point for each bullet
                            startSound(bulletSound);
                        }
                    }
                default:
                    ;
                }
            }
            if (!pauseFlag)
            {
                // Drop a bomb at a random interval if no bomb is active
                if (rand() % BombFrequency == 0 && bombPtr == nullptr)
                {
                    startSound(bombSound);
                    // Every other bomb is a "targeted bomb"
                    if (rand()%2)
                    {
                        bombPtr = new Bomb;
                        bombPtr -> moveBombToStartPosition(invaders);
                    }
                    else  // drop a targeted bomb
                    {
                        bombPtr = new Bomb(gun[0].getPosition().x);
                        bombPtr->bottomPosition();
                    }
                }
                if (rand()%SaucerFrequency == 0 && saucerPtr == nullptr)    // saucer frequency = 1/8000
                {
                    saucerPtr = new Saucer;
                    stopSound(invaderSound);
                    startSound(saucerSound);
                }

                deltaTime = clock.restart().asSeconds();

                totalTime += deltaTime;
                if(totalTime >= SwitchTime)
                {
                    totalTime -= SwitchTime;
                    /// Move the invaders
                    invaders.move();
                    if (invaders.getPositionOfBottomVisibleInvader() >= ShieldYPosition)
                    {
                        gameOver = true;
                        //break;
                    }
                }

                // Move the saucer
                if(saucerPtr)
                {
                    if (!saucerPtr->move())
                    {
                        stopSound(saucerSound);
                        startSound(invaderSound);
                        delete saucerPtr;
                        saucerPtr = nullptr;
                    }
                }

                // Move the bomb
                if (bombPtr)
                {
                    if (!bombPtr->move())
                    {
                        stopSound(bombSound);
                        startSound(invaderSound);
                        delete bombPtr;
                        bombPtr = nullptr;
                    }
                    else
                    {
                        /// Bomb hits gun?
                        if (gun[0].hitByBomb(*bombPtr))   // gun hit by bomb?
                        {
                            stopSound(bombSound);
                            startSound(bombExplosionSound);

                            delete bombPtr;
                            bombPtr = nullptr;
                            explosion.startExplosion(gun[0].getPosition());
                            if (gunInPlay == 2)
                                gameOver = true;
                        }
                        /// Bomb hits shield?
                        else if (bombPtr && bombPtr->bottomPosition() >= 0.9f * MainWindowHeight - ShieldHeight / 2.0f)
                        {
                            for (int i = 0; i < 3; ++i)
                            {
                                if (shields[i].isAlignedWithBomb(*bombPtr))
                                {
                                    sf::Vector2f shieldPositionToExamine = sf::Vector2f(32 + (bombPtr->nosePosition().x-shields[i].getShield().getPosition().x),22.5f - (shields[i].getShield().getPosition().y - bombPtr->nosePosition().y));

                                    if (shields[i].hitByBomb(shieldPositionToExamine))
                                    {
                                        startSound(explosionSound);
                                        // Get rid of the bomb
                                        delete bombPtr;
                                        bombPtr = nullptr;
                                        break;
                                    }
                                    else
                                        break;
                                }
                            }
                        }
                    }
                }
                // move and display the bullets in flight
                for (auto it = bulletsInFlight.begin(); it!= bulletsInFlight.end(); ++it)
                {
                    bulletHitsShield = false;

                    // Bullet moved off the top of the window
                    if (!(*it)->move())
                    {
                        delete (*it);
                        *it = nullptr;
                        bulletsInFlight.erase(it);
                        break;
                    }
                    /// Bullet hits shield?
                    for (int i = 0; i < 3; ++i)
                    {
                        if (shields[i].isAlignedWithBullet(**it))
                        {
                            sf::Vector2f shieldPositionToExamine = sf::Vector2f(32 + ((*it)->getPosition().x-shields[i].getShield().getPosition().x),22.5f + ((*it)->getPosition().y - shields[i].getShield().getPosition().y));
                            if (shields[i].hitByBullet(shieldPositionToExamine))
                            {
                                // remove bullet
                                delete (*it);
                                *it = nullptr;
                                bulletsInFlight.erase(it);
                                bulletHitsShield = true;
                                break;
                            }
                        }
                    }
                    if (bulletHitsShield)
                        break;
                    /// Bullet hits invader?
                    invaderHit = invaders.invaderHitByBullet(**it);
                    if (invaderHit)
                    {
                        score += invaderHit;
                        startSound(explosionSound);
                        delete (*it);
                        *it = nullptr;
                        bulletsInFlight.erase(it);
                        if (invaders.getCountVisible() == 0)
                            gameOver = true;
                        // add a random invader, maybe
                        else if (!gameOver && invaders.getCountVisible() < 10 && rand() % 2  == 0)
                        {
                            invaders.addRandomInvader();
                        }
                        else {};
                        break;
                    }

                    /// Bullet hits bomb?
                    if (bombPtr && bombPtr->hitByBullet(**it))
                    {
                        score += 20;  // 20 points for hitting a bomb
                        stopSound(bombSound);
                        startSound(explosionSound);
                        explosion.startExplosion(bombPtr->getPosition());

                        // remove bullet
                        delete (*it);
                        *it = nullptr;
                        bulletsInFlight.erase(it);

                        // remove the bomb
                        delete bombPtr;
                        bombPtr = nullptr;
                        break;
                    }
                    /// Bullet hits saucer?
                    else if (saucerPtr && saucerPtr->hitByBullet(**it))
                    {
                        score += 10;  // 10 points for hitting a saucer
                        stopSound(saucerSound);
                        startSound(explosionSound);
                        explosion.startExplosion(saucerPtr->getPosition());

                        // remove bullet
                        delete (*it);
                        *it = nullptr;
                        bulletsInFlight.erase(it);

                        // remove the saucer
                        delete saucerPtr;
                        saucerPtr = nullptr;
                        break;
                    }
                    else
                    {
                        window.draw(**it);
                    }
                }

                // check for explosion
                if (explosion.isExploding())
                {
                    explosion.update();
                    // explosion end
                    if (!explosion.isExploding())
                    {
                        stopSound(explosionSound);
                        startSound(invaderSound);

                        // is it a gun explosion
                        if (fabs(explosion.getPosition().y - gun[0].getPosition().y) < 20.0f)
                        {
                            if (gunInPlay == 2)
                            {
                                gameOver = true;
                                break;
                            }
                            gun[2-gunInPlay].setInvisible();
                            gun[0].setVisible();
                            gun[0].moveToPosition();
                            gunInPlay++;
                            sf::sleep(sf::Time(sf::seconds(2.0f)));
                        }
                    }
                }
            }

            invaders.updateStatus();

            text.setString(sf::String("Score ")+std::to_string(score)+ std::string(66,' ') +"Lives");
            displayWindowObjects(window, background, text, gun, invaders, explosion, bombPtr, saucerPtr, bulletsInFlight, gameOverText, shields);
            loopCount++;
            if (gameOver)
            {
                // stop sounds
                stopSound(invaderSound);
                stopSound(saucerSound);
                stopSound(explosionSound);
                startSound(applauseSound);
                gameOverText.setString(sf::String("Game Over"));

                displayWindowObjects(window, background, text, gun, invaders, explosion, bombPtr, saucerPtr, bulletsInFlight, gameOverText, shields);

                highScores.updateHighScores(Score(name.c_str(),score,time(0)));
                highScores.WriteHighScoresFile();
                sf::sleep(sf::Time(sf::seconds(5.0f)));   // stall
                window.close();
            }
        }                          // while (gunInPlay < numGuns)
    }                              // while (window.isOpen())
    return 0;
}

