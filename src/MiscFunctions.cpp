#include <iostream>
#include <fstream>
#include <sstream>

#include "Proto.h"
#include "Constants.h"
#include "Gun.h"
#include "Saucer.h"


std::string welcome(sf::RenderWindow& window, const HighScores& highScores, Invaders& invaders)
{
    std::string text, buffer,name;

       // Music
    sf::Music music;
    music.openFromFile(OpeningMusicFile);
    music.setVolume(40);
    music.play();

    // Text
    sf::Font font;
    if (!font.loadFromFile(CourierNewFontFile))
    {
        std::cout << "Can't find font" << std::endl;
    }
    text = getWelcomeText();
    sf::Text instructions;
    instructions.setString(text.c_str());
    instructions.setFont(font);
    instructions.setCharacterSize(20); // in pixels, not points!
    instructions.setFillColor(sf::Color::Cyan);
    instructions.setPosition(90.0f,20.0f);

    // Create High Scores text
    sf::Text highScoresText;
    highScoresText.setFont(font);
    highScoresText.setCharacterSize(20); // in pixels, not points!
    highScoresText.setFillColor(sf::Color::Green);
    highScoresText.setPosition(480.0f,210.0f);

    // Write High Scores
    std::ostringstream sout;
    sout << highScores << std::endl;
    highScoresText.setString(sout.str());

    sf::Event evnt;
    char input = ' ';

    // Create and position a gun
    Gun gun;
    gun.moveToPosition(sf::Vector2f(56.0f,264.0f));

    // Position 5 invaders
    for (auto i = 0; i < 5; i++)
    {
        invaders.getInvader(i,0).setPosition(sf::Vector2f(55.0f,313.0f + i * 45.0f));
    }

    // create and position a saucer
    Saucer* saucerPtr = new Saucer;
    saucerPtr->setPosition(sf::Vector2f(50.0f,544.0f));

    // create and position 3 bombs
    Bomb bomb[3] = {0,1,2};
    for (auto i = 0; i < 3; i++)
    {
        bomb[i].setPosition(40.0f+ i*16.0f, 610.0f);
    }

    while (window.isOpen())
    {
        while (window.pollEvent(evnt))
        {
            switch (evnt.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                input = getKey();
                if ((input >= 'a' && input <= 'z')||(input >= 'A' && input <= 'Z')||input == ' ')
                {
                    name += input;
                }
                if (input == '\b') // backspace
                {
                    name = "";
                    text = getWelcomeText();
                }
                break;
            default:
                ;
            }
        }
        window.clear();
        instructions.setString((text+name+'_').c_str());
        window.draw(instructions);
        window.draw(highScoresText);

        window.draw(gun.getGun());

        for (auto i = 0; i < 5; i++)
        {
            window.draw(invaders.getInvader(i,0));
        }
        window.draw(*saucerPtr);
        for (auto i = 0; i < 3; i++)
        {
            window.draw(bomb[i]);
        }

        window.display();
        if (isspace(name[0]))
            name = name.substr(1);  // remove leading space from name
        name[0] = static_cast<char>(toupper(name[0]));

        if (input == '\n')
        {
            delete saucerPtr;
            window.clear();
            music.stop();
            return name;
        }
    }
    music.stop();
    delete saucerPtr;
    return "";
}

std::string getWelcomeText()
{
    std::ifstream fin(WelcomeFile);
    std::string text, buffer;
    if (!fin)
    {
        std::cout << "Cannot open welcome file." << std::endl;
        return "";
    }

    while (getline(fin, buffer))
    {
        text += buffer += '\n';
    }

    text += "\n          Please type your name and press Enter ===> ";

    fin.close();
    return text;
}

// Determine what key is pressed
char getKey()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        return  'a';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        return  'b';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        return  'c';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        return  'd';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        return  'e';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        return  'f';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        return  'g';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        return  'h';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I))
        return  'i';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J))
        return  'j';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::K))
        return  'k';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L))
        return  'l';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::M))
        return  'm';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
        return  'n';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::O))
        return  'o';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        return  'p';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        return  'q';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
        return  'r';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        return  's';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::T))
        return  't';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::U))
        return  'u';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
        return  'v';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        return  'w';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
        return  'x';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y))
        return  'y';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        return  'z';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        return  ' ';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
        return  '\n';
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Backspace))
        return  '\b';
    return ' ';
}

void startSound(sf::Sound& sound)
{
    if (sound.getStatus() != sf::SoundSource::Playing)
        sound.play();
}

void stopSound(sf::Sound& sound)
{
    if (sound.getStatus() != sf::SoundSource::Stopped)
        sound.stop();
}

void displayWindowObjects(sf::RenderWindow& window, sf::RectangleShape& background, sf::Text& text, Gun* gun, Invaders& invaders, Explosion& explosion, Bomb* bombPtr, Saucer* saucerPtr, std::list<Bullet*>& bulletsInFlight, sf::Text& gameOverText, Shield* shields)
{
    window.clear();
    window.draw(background);
    window.draw(text);
    invaders.draw(window);
    if (gun[0].isVisible())
        window.draw(gun[0].getGun());
    if (gun[2].isVisible())
        window.draw(gun[2].getGun());
    if (gun[1].isVisible())
        window.draw(gun[1].getGun());
    if (explosion.isExploding())
        window.draw(explosion.getExplosion());
    if (bombPtr)
        window.draw(*bombPtr);
    if (saucerPtr)
        window.draw(*saucerPtr);
    // draw bullets
    for (auto it = bulletsInFlight.begin(); it!= bulletsInFlight.end(); ++it)
        window.draw(**it);
    window.draw(gameOverText);
    window.draw(shields[0].getShield());
    window.draw(shields[1].getShield());
    window.draw(shields[2].getShield());
    window.display();
}
