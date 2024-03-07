
////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include <iostream>

#ifdef SFML_SYSTEM_IOS
#include <SFML/Main.hpp>
#endif

std::string resourcesDir()
{
#ifdef SFML_SYSTEM_IOS
    return "";
#else
    return "resources/";
#endif
}

////////////////////////////////////////////////////////////
/// Entry point of application
///
/// \return Application exit code
///
////////////////////////////////////////////////////////////

sf::CircleShape playerShape() {

sf::CircleShape ball;
    ball.setRadius(5);
    ball.setFillColor(sf::Color::White);
    ball.setOrigin(-100, -100);
    return ball;

}

sf::CircleShape healthShape() {
    sf::CircleShape ball;
    ball.setRadius(10);
    ball.setFillColor(sf::Color::Green);
    ball.setOrigin(-100, -100);
    ball.move(+400, +200);
    return ball;
}

sf::RectangleShape armorShape() {
    
    
    sf::Vector2f squareSize(10, 10);
    sf::RectangleShape square;
    square.setSize(squareSize);
    square.setFillColor(sf::Color::Red);
    square.setOrigin(-100,-100);
    square.move(+200, +100);
    return square;


}



struct spawnPointArmor {

    sf::RectangleShape shape;
    bool show;
    int nextTime;
    
    spawnPointArmor() {
        nextTime = 0;
        show = true;
        shape = armorShape();
    }
};

struct spawnPointHealth {

    sf::CircleShape shape;

    bool show;
    int nextTime;
    spawnPointHealth() {
        shape = healthShape();
        show = true;
        nextTime = 0;
    }

};


struct Player {

    sf::CircleShape shape;

    int playerHealth;
    int playerArmor;

    Player() {
        shape = playerShape();
        playerHealth = 100;
        playerArmor = 0;
    }

};


int play_game() {
    const float gameWidth = 800;
    const float gameHeight = 600;
    sf::RenderWindow window(sf::VideoMode(static_cast<unsigned int>(gameWidth), static_cast<unsigned int>(gameHeight), 32), "SFML Tennis",
                            sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    sf::Clock clock;
    bool isPlaying = false;

    const float speed = 100.f;
    Player player;
    spawnPointArmor armor;
    spawnPointHealth health;

    sf::Clock noReset;
    while (window.isOpen()) 
    {
        sf::Event event;
        while (window.pollEvent(event))
        {

            if ((event.type == sf::Event::Closed) ||
               ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

        }


            float deltaTime = clock.restart().asSeconds();

            // Move the player's paddle
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                player.shape.move(0.f, -speed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                player.shape.move(0.f, speed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                player.shape.move(speed * deltaTime, 0.f);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                player.shape.move(-speed * deltaTime, 0.f);
            }

        if ( (abs(player.shape.getPosition().x - armor.shape.getPosition().x) < 10) &&
            (abs(player.shape.getPosition().y - armor.shape.getPosition().y) < 10) &&
            (armor.show)            
        ) {
            armor.show = false;
            armor.nextTime = noReset.getElapsedTime().asMilliseconds() + 3000;
            player.playerArmor += 100;
        }
        if ( (abs(player.shape.getPosition().x - health.shape.getPosition().x) < 10) &&
            (abs(player.shape.getPosition().y - health.shape.getPosition().y) < 10) &&
            (health.show)            
        ) {
            health.show = false;
            health.nextTime = noReset.getElapsedTime().asMilliseconds() + 3000;
            player.playerHealth += 50;
        }

        std::cout << player.playerHealth << "\n" << player.playerArmor << "\n";

        window.clear(sf::Color(50, 50, 50));
        window.draw(player.shape);

        if (health.show) {
            window.draw(health.shape);
        }
        if (armor.show) {
            window.draw(armor.shape);
        }
        window.display();

        if ((armor.nextTime < noReset.getElapsedTime().asMilliseconds())
            && !(armor.show)
        ) {
            armor.nextTime = 0;
            armor.show = true;
        }

        if ((health.nextTime < noReset.getElapsedTime().asMilliseconds()) && !health.show) {
            health.nextTime = 0;
            health.show = true;
        }


  
    }

    return 1;
}

int main() {
    return play_game();
}