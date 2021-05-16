#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "viewr.cpp"
#include <sstream>
#include "mission.cpp"
#include <iostream>
#include "map.cpp"
#include <cmath>


using namespace sf;

class Entity {
public:
    float dx, dy, x, y, speed, moveTimer;
    int w, h, HP;
    bool life, isMove, onGround, Fire;
    Texture texture;
    Sprite sprite;


        Entity(Image &image, float X, float Y, int W, int H, String Name) {
        x = X;
        y = Y;
        w = W;
        h = H;
        name = Name;
        moveTimer = 0;
        speed = 0;
        HP = 100;
        dx = 0;
        dy = 0;
        life = true;
        onGround = false;
        isMove = false;
        texture.loadFromImage(image);
        sprite.setTexture(texture);
        sprite.setOrigin(w / 2, h / 2);
    }
};

class Player : public Entity {
public:
    enum {
        left, right, stayleft, stayright, spawn, deadright, deadleft
    } state;
    int playerScore, Cadr;

    Player(Image &image, float X, float Y, int W, int H, String Name) : Entity(image, X, Y, W, H, Name) {
        playerScore = 0;
        state = spawn;
        Fire = false;
        if (name == "Player")
            sprite.setTextureRect(IntRect(0, 0, 60, 80));
    }

    void control() {
        speed = 0;
        Fire = false;
        if (state == right)
            state = stayright;
        if (state == left)
            state = stayleft;
        if (Keyboard::isKeyPressed) {
            if (Mouse::isButtonPressed(Mouse::Left))
                Fire = true;
            if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
                state = left;
                speed = 0.1;
            }
            if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
                state = right;
                speed = 0.1;
            }
            if (((Keyboard::isKeyPressed(Keyboard::Up)) || (Keyboard::isKeyPressed(Keyboard::Space))) && (onGround)) {
                dy = -0.4;
                onGround = false;
                speed = 0.1;
            }
            if (Keyboard::isKeyPressed(Keyboard::K)) {
                HP = 0;
                if ((state == right) || (state == stayright))
                    state = deadright;
                if ((state == left) || (state == stayleft))
                    state = deadleft;
            }
        }
    }

    void checkCollisionWithMap(float Dx, float Dy) {
        for (int i = y / 32; i < (y + h) / 32; i++)
            for (int j = x / 32; j < (x + w) / 32; j++)
                if (TileMap[i][j] == '0') {
                    if (Dy > 0) {
                        y = i * 32 - h;
                        dy = 0;
                        onGround = true;
                    }
                    if (Dy < 0) {
                        y = i * 32 + 32;
                        dy = 0;
                    }
                    if (Dx > 0) { x = j * 32 - w; }
                    if (Dx < 0) { x = j * 32 + 32; }
                }
    }

    void update(float time, float T) {
        control();
        switch (state) {
            case right: {
                dx = speed;
                sprite = TextureShiftRight(T, sprite, onGround, Fire);
                break;
            }
            case left: {
                dx = -speed;
                sprite = TextureShiftLeft(T, sprite, onGround, Fire);
                break;
            }
            case stayright: {
                dx = 0;
                sprite = TextureShiftStayRight(T, sprite, onGround, Fire);
                break;
            }
            case stayleft: {
                dx = 0;
                sprite = TextureShiftStayLeft(T, sprite, onGround, Fire);
                break;
            }
            case spawn: {
                dx = 0;
                sprite.setTextureRect(IntRect(0, 0, 60, 80));
                break;
            }
        }
        x += dx * time;
        checkCollisionWithMap(dx, 0);
        y += dy * time;
        checkCollisionWithMap(0, dy);
        sprite.setPosition(x + w / 2, y + h / 2);
        if (HP <= 0)
            life = false;
        if (life) { setPlayerCoordinateForView(getPlayerCoordinateX(), getPlayerCoordinateY()); }
        dy = dy + 0.0015 * time;
    }

    float getPlayerCoordinateX() {
        return x;
    }

    float getPlayerCoordinateY() {
        return y;
    }

    Sprite BOOM(float time, Sprite bum) {
        float Cadr1 = 0;
        float Cadr2 = 0;
        Cadr1 += 0.00005 * time;
        if ((Cadr1 > 9) && (Cadr2 == 0)) {
            Cadr1 -= 8;
            Cadr2++;
        }
        if ((Cadr1 > 9) && (Cadr2 == 1)) {
            Cadr1 -= 8;
            Cadr2++;
        }
        if ((Cadr1 > 9) && (Cadr2 == 2)) {
            Cadr1 -= 8;
            Cadr2++;
        }
        if ((Cadr1 > 9) && (Cadr2 == 3)) {
            Cadr1 -= 8;
            Cadr2++;
        }
        if ((Cadr1 > 9) && (Cadr2 == 4)) {
            Cadr1 -= 8;
            Cadr2++;
        }
        if ((Cadr1 > 9) && (Cadr2 == 5)) {
            Cadr1 -= 8;
            Cadr2++;
        }
        if ((Cadr1 > 9) && (Cadr2 == 6)) {
            Cadr1 -= 8;
            Cadr2++;
        }
        if ((Cadr1 > 9) && (Cadr2 == 7)) {
            Cadr1 -= 8;
            Cadr2++;
        }
        if ((Cadr1 > 9) && (Cadr2 == 8)) {
            Cadr1 -= 8;
            Cadr2++;
        }
        if ((Cadr1 > 9) && (Cadr2 == 9)) {
            Cadr1 -= 8;
            Cadr2++;
        }
        if ((Cadr1 > 100) && (Cadr2 == 10)) {
            Cadr1 = 0;
            Cadr2 = 0;
        }
        bum.setTextureRect(IntRect(100 * int(Cadr1), 100 * int(Cadr2), 100, 100));
        bum.setPosition(getPlayerCoordinateX(), getPlayerCoordinateY());
        return bum;
    }

    Sprite TextureShiftRight(float time, Sprite hero, bool onGround, bool Fire) {
        if ((onGround == true) && (Fire == false)) {
            float Cadr = 0;
            Cadr += 0.000006 * time;
            if (Cadr > 4) Cadr -= 4;
            hero.setTextureRect(IntRect(60 * int(Cadr), 0, 60, 80));
        }
        if ((onGround == false) && (Fire == false))
            hero.setTextureRect(IntRect(0, 480, 60, 80));
        if ((onGround == true) && (Fire == true)) {
            float Cadr = 0;
            Cadr += 0.000006 * time;
            if (Cadr > 4) Cadr -= 4;
            hero.setTextureRect(IntRect(60 * int(Cadr), 80, 60, 80));
        }
        if ((onGround == false) && (Fire == true)) {
            float Cadr = 0;
            Cadr += 0.000003 * time;
            if (Cadr > 2) Cadr -= 2;
            hero.setTextureRect(IntRect(60 * int(Cadr), 480, 60, 80));
        }
        return hero;
    }

    Sprite TextureShiftLeft(float time, Sprite hero, bool onGround, bool Fire) {
        if ((onGround == true) && (Fire == false)) {
            float Cadr = 0;
            Cadr += 0.000006 * time;
            if (Cadr > 4) Cadr -= 4;
            hero.setTextureRect(IntRect(60 * int(Cadr), 160, 60, 80));
        }
        if ((onGround == false) && (Fire == false))
            hero.setTextureRect(IntRect(0, 560, 60, 80));
        if ((onGround == true) && (Fire == true)) {
            float Cadr = 0;
            Cadr += 0.000006 * time;
            if (Cadr > 4) Cadr -= 4;
            hero.setTextureRect(IntRect(60 * int(Cadr), 240, 60, 80));
        }
        if ((onGround == false) && (Fire == true)) {
            float Cadr = 0;
            Cadr += 0.000003 * time;
            if (Cadr > 2) Cadr -= 2;
            hero.setTextureRect(IntRect(60 * int(Cadr), 560, 60, 80));
        }
        return hero;
    }

    Sprite TextureShiftStayRight(float time, Sprite hero, bool onGround, bool Fire) {
        if ((onGround == true) && (Fire == false))
            hero.setTextureRect(IntRect(0, 0, 60, 80));
        if ((onGround == true) && (Fire == true)) {
            float Cadr = 0;
            Cadr += 0.000003 * time;
            if (Cadr > 2) Cadr -= 2;
            hero.setTextureRect(IntRect(0, 80 * int(Cadr), 60, 80));
        }
        if ((onGround == false) && (Fire == false))
            hero.setTextureRect(IntRect(0, 480, 60, 80));
        if ((onGround == false) && (Fire == true)) {
            float Cadr = 0;
            Cadr += 0.000003 * time;
            if (Cadr > 2) Cadr -= 2;
            hero.setTextureRect(IntRect(60 * int(Cadr), 480, 60, 80));
        }
        return hero;
    }

    Sprite TextureShiftStayLeft(float time, Sprite hero, bool onGround, bool Fire) {
        if ((onGround == true) && (Fire == false))
            hero.setTextureRect(IntRect(60, 160, 60, 80));
        if ((onGround == true) && (Fire == true)) {
            float Cadr = 0;
            Cadr += 0.000003 * time;
            if (Cadr > 2) Cadr -= 2;
            hero.setTextureRect(IntRect(60, 160 + 80 * int(Cadr), 60, 80));
        }
        if ((onGround == false) && (Fire == false))
            hero.setTextureRect(IntRect(0, 560, 60, 80));
        if ((onGround == false) && (Fire == true)) {
            float Cadr = 0;
            Cadr += 0.000003 * time;
            if (Cadr > 2) Cadr -= 2;
            hero.setTextureRect(IntRect(60 * int(Cadr), 560, 60, 80));
        }
        return hero;
    }

    void Dead(float time, float T) {
        switch (state) {
            case deadright: {
                sprite = TextureShiftDeadRight(T, sprite);
                break;
            }
            case deadleft: {
                sprite = TextureShiftDeadLeft(T, sprite);
                break;
            }
        }
    }

    Sprite TextureShiftDeadRight(float time, Sprite hero) {
        float Cadr = 0;
        Cadr += 0.000003 * time;
        if (Cadr > 2) Cadr -= 2;
        hero.setTextureRect(IntRect(60 * int(Cadr), 320, 60, 80));
        return hero;
    }

    Sprite TextureShiftDeadLeft(float time, Sprite hero) {
        float Cadr = 0;
        Cadr += 0.000003 * time;
        if (Cadr > 2) Cadr -= 2;
        hero.setTextureRect(IntRect(60 * int(Cadr), 400, 60, 80));
        return hero;
    }

};


class Enemy : public Entity {
public:
    Enemy(Image &image, float X, float Y, int W, int H, String Name) : Entity(image, X, Y, W, H, Name) {
        if (name == "Enemy") {
            sprite.setTextureRect(IntRect(0, 0, w, h));
            dx = 0.1;
        }
    }

    void checkCollisionWithMap(float Dx, float Dy) {
        for (int i = y / 32; i < (y + h) / 32; i++)
            for (int j = x / 32; j < (x + w) / 32; j++) {
                if (TileMap[i][j] == '0') {
                    if (Dy > 0) { y = i * 32 - h; }
                    if (Dy < 0) { y = i * 32 + 32; }
                    if (Dx > 0) {
                        x = j * 32 - w;
                        dx = -0.1;
                        sprite.scale(-1, 1);
                    }
                    if (Dx < 0) {
                        x = j * 32 + 32;
                        dx = 0.1;
                        sprite.scale(-1, 1);
                    }
                }
            }
    }

    void update(float time) {
        if (name == "Enemy") {

            moveTimer += time;
            if (moveTimer > 3000) {
                dx *= -1;
                moveTimer = 0;
            }
            checkCollisionWithMap(dx, 0);
            x += dx * time;
            dy = dy + 0.0015 * time;
            sprite.setPosition(x + w / 2, y + h / 2);
            if (HP <= 0) { life = false; }
        }
    }
};


int main() {
    long long int TimeForBoom = 0;
    long long int TimeForHero = 0;
    long long int TimeForDead = 0;
    RenderWindow window(VideoMode(1200, 675), "My game");
    view.reset(sf::FloatRect(0, 0, 1200, 675));

    Texture bumtexture;
    bumtexture.loadFromFile("images/BUM.png");

    Image map_image;
    map_image.loadFromFile("images/map.png");
    Texture map;
    map.loadFromImage(map_image);
    Sprite s_map;
    s_map.setTexture(map);

    Sprite bumsprite;
    bumsprite.setTexture(bumtexture);
    bumsprite.setTextureRect(IntRect(0, 0, 100, 100));
    bumsprite.setPosition(250, 250);

    Music music;
    music.openFromFile("music/music.ogg");
    music.play();

    Clock clock;
    Clock clockboom;
    clockboom.restart();

    Font font;
    font.loadFromFile("Fonk.ttf");
    Text text("", font, 40);
    text.setStyle(sf::Text::Bold);

    Image quest_image;
    quest_image.loadFromFile("images/mission.jpg");
    Texture quest_texture;
    quest_texture.loadFromImage(quest_image);
    Sprite s_quest;
    s_quest.setTexture(quest_texture);
    s_quest.setTextureRect(IntRect(0, 0, 340, 510));
    s_quest.setScale(0.6f, 0.6f);


    Image heroImage;
    heroImage.loadFromFile("images/heroes.png");

    Image easyEnemyImage;
    easyEnemyImage.loadFromFile("images/Enamy.png");

    Player p(heroImage, 300, 200, 60, 80, "Player");
    Enemy easyEnemy(easyEnemyImage, 400, 300, 180, 180, "Enemy");

    bool showMissionText = true;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asMicroseconds();
        TimeForHero += time;
        TimeForBoom += time;

        if (TimeForHero > 1300000) TimeForHero = 0;
        if (TimeForBoom > 1600000) TimeForBoom = 0;
        clock.restart();
        time = time / 800;


        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed)
                if ((event.key.code == Keyboard::Tab)) {

                    switch (showMissionText) {

                        case true: {
                            std::ostringstream task;
                            task << getTextMission(getCurrentMission(p.getPlayerCoordinateX()));
                            showMissionText = false;
                            break;
                        }
                        case false: {
                            text.setString("");
                            showMissionText = true;
                            break;
                        }
                    }
                }
        }
        if (Keyboard::isKeyPressed(Keyboard::Escape))
            window.close();

        bumsprite = p.BOOM(TimeForBoom, bumsprite);
        p.update(time, TimeForHero);
        p.Dead(time, TimeForDead);

        easyEnemy.update(time);
        window.setView(view);
        window.clear();


        for (int i = 0; i < HEIGHT_MAP; i++)
            for (int j = 0; j < WIDTH_MAP; j++) {
                if (TileMap[i][j] == ' ') s_map.setTextureRect(IntRect(0, 0, 32, 32));
                if (TileMap[i][j] == 's') s_map.setTextureRect(IntRect(32, 0, 32, 32));
                if (TileMap[i][j] == '0') s_map.setTextureRect(IntRect(64, 0, 32, 32));
                s_map.setPosition(j * 32, i * 32);
                window.draw(s_map);
            }

        std::ostringstream playerHP;
        playerHP << p.HP;
        text.setString("HP " + playerHP.str());
        text.setPosition(view.getCenter().x + 400, view.getCenter().y - 300);
        window.draw(text);

        if (!showMissionText) {
            text.setPosition(view.getCenter().x + 125, view.getCenter().y - 130);
            s_quest.setPosition(view.getCenter().x + 115, view.getCenter().y - 130);
            window.draw(s_quest);
            window.draw(text);
        }
        window.draw(easyEnemy.sprite);
        window.draw(p.sprite);
        window.draw(bumsprite);
        window.display();
    }

    return 0;
}

/*
if (p.life)
		{
                float timeboom = clockboom.getElapsedTime().asMicroseconds();
                timeboom = timeboom / 800;

				Cadr1 += 0.0001 * timeboom;
				if ( (Cadr1 > 9) && (Cadr2 == 0) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 1) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 2) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 3) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 4) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 5) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 6) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 7) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 8) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 9) && (Cadr2 == 9) ) {Cadr1 -= 8; Cadr2++;}
				if ( (Cadr1 > 200) && (Cadr2 == 10) ) {Cadr1 = 0; Cadr2 = 0; clockboom.restart();}

               bumsprite.setPosition(p.getPlayerCoordinateX() - 32, p.getPlayerCoordinateY()-44);

				bumsprite.setTextureRect(IntRect(100 * int(Cadr1), 100 * int(Cadr2), 100, 100));

		}




if ((Keyboard::isKeyPressed(Keyboard::Left) || (Keyboard::isKeyPressed(Keyboard::A)))) {
				p.dir = 1; p.speed = 0.1;dir =1 -  , speed =0.1 -  .  -
				Cadr += 0.005 * time;
				if (Cadr > 4) Cadr -= 4;
				p.sprite.setTextureRect(IntRect(60 * int(Cadr), 80, 60, 80));
				getPlayerCoordinateForview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}

			if ((Keyboard::isKeyPressed(Keyboard::Right) || (Keyboard::isKeyPressed(Keyboard::D)))) {
				p.dir = 0; p.speed = 0.1; ,
				Cadr += 0.005 * time;
				if (Cadr > 4) Cadr -= 4;
				p.sprite.setTextureRect(IntRect(60 * int(Cadr), 0, 60, 80));
				getPlayerCoordinateForview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}

			if ((Keyboard::isKeyPressed(Keyboard::Up) || (Keyboard::isKeyPressed(Keyboard::W)))) {
				p.dir = 2; p.speed = 0.1; ,
				Cadr += 0.005 * time;
				if (Cadr > 3) Cadr -= 3;
				p.sprite.setTextureRect(IntRect(96 * int(Cadr), 288, 96, 96));
				getPlayerCoordinateForview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}

			if ((Keyboard::isKeyPressed(Keyboard::Right) && (Keyboard::isKeyPressed(Keyboard::Space)))) {
				p.dir = 0; p.speed = 0.1;dir =1 -  , speed =0.1 -  .  -
				Cadr += 0.005 * time;
				if (Cadr > 4) Cadr -= 4;
				p.sprite.setTextureRect(IntRect(60 * int(Cadr), 160, 60, 80));
				getPlayerCoordinateForview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}


			if ((Keyboard::isKeyPressed(Keyboard::Left) && (Keyboard::isKeyPressed(Keyboard::Space)))) {
				p.dir = 1; p.speed = 0.1;dir =1 -  , speed =0.1 -  .  -
				Cadr += 0.005 * time;
				if (Cadr > 4) Cadr -= 4;
				p.sprite.setTextureRect(IntRect(60 * int(Cadr), 240, 60, 80));
				getPlayerCoordinateForview(p.getPlayerCoordinateX(), p.getPlayerCoordinateY());
			}
			*/
