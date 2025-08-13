#include "game.h"
#include "gameOver.h"


Game::Game(sf::RenderWindow& window, sf::RenderTexture& canvas, sf::Font& textFont) : mGameMap("assets\\maps\\map.txt"){
    
    mWindowsWidth = sf::VideoMode::getDesktopMode().width;
    mWindowsHeight = sf::VideoMode::getDesktopMode().height;

    if (!mGameMap.readMapFile()) {
        throw std::runtime_error("Couldn't load map.");
    }
    if (!loadFiles()) {
        throw std::runtime_error("Couldn't load files.");
    }

    mZBuffer = std::make_unique<double[]>(mWindowsWidth);
    if (!mZBuffer) {
        throw std::runtime_error("Allocation error.");
    }

    mFont = &textFont;
    mCanvas = &canvas;
    mWindow = &window;

    mWindow->setMouseCursorVisible(0);
    mPlayer.position = mGameMap.playerStartPosition;
    mPlayer.setSounds(mSounds);

    mEnemy.position = mGameMap.enemyStartPosition;
    mEnemy.setSounds(mSounds);

    
}

Game::~Game() {

    mWindow->setMouseCursorVisible(1);
    mPlayer.gameSound.stop();
    mPlayer.walkSound.stop();
    mEnemy.behindWarning.stop();
}

int Game::loadFiles() {
    if (!mTextures[0].loadFromFile("assets\\sprites\\stone.png")) {
        return 0;
    }
    if (!mTextures[1].loadFromFile("assets\\sprites\\brick.png")) {
        return 0;
    }
    if (!mTextures[2].loadFromFile("assets\\sprites\\bloodystone.png")) {
        return 0;
    }
    if (!mTextures[3].loadFromFile("assets\\sprites\\bloodystone2.png")) {
        return 0;
    }
    if (!mTextures[4].loadFromFile("assets\\sprites\\door.png")) {
        return 0;
    }
    if (!mTextures[5].loadFromFile("assets\\sprites\\enemy.png")) {
        return 0;
    }

    if (!mSounds[0].loadFromFile("assets\\sounds\\walk.wav")) {
        return 0;
    }
    if (!mSounds[1].loadFromFile("assets\\sounds\\run.wav")) {
        return 0;
    }
    if (!mSounds[2].loadFromFile("assets\\sounds\\gameSound.wav")) {
        return 0;
    }
    if (!mSounds[3].loadFromFile("assets\\sounds\\behind.wav")) {
        return 0;
    }
    if (!mSounds[4].loadFromFile("assets\\sounds\\front.wav")) {
        return 0;
    }
    return 1;
}

void Game::raycasting() {

    for (int i = 0; i < mWindowsWidth; i += 2) {

        double cameraX = 2 * i / float(mWindowsWidth) - 1;
        double rayDirX = mPlayer.direction.x + mPlayer.plane.x * cameraX;
        double rayDirY = mPlayer.direction.y + mPlayer.plane.y * cameraX;

        int mapX = mPlayer.position.x;
        int mapY = mPlayer.position.y;
        int stepX;
        int stepY;
        bool hit = 0;
        bool side;
        double perpWallDistance;

        double unitXSize = (rayDirX == 0) ? DBL_MAX : abs(1 / rayDirX);
        double unitYSize = (rayDirY == 0) ? DBL_MAX : abs(1 / rayDirY);

        double rayLengthX = 0;
        double rayLengthY = 0;
        double distance;

        if (rayDirX < 0) {
            rayLengthX = (mPlayer.position.x - mapX) * unitXSize;
            stepX = -1;
        }
        else {
            rayLengthX = (mapX + 1 - mPlayer.position.x) * unitXSize;
            stepX = 1;
        }
        if (rayDirY < 0) {
            rayLengthY = (mPlayer.position.y - mapY) * unitYSize;
            stepY = -1;
        }
        else {
            rayLengthY = (mapY + 1 - mPlayer.position.y) * unitYSize;
            stepY = 1;
        }

        while (!hit) {
            if (rayLengthX < rayLengthY) {
                mapX += stepX;
                distance = rayLengthX;
                rayLengthX += unitXSize;
                side = 0;
            }
            else {
                mapY += stepY;
                distance = rayLengthY;
                rayLengthY += unitYSize;
                side = 1;
            }

            if (mGameMap.mapData[mapY][mapX] != 0) {
                hit = 1;
            }
        }

        //count exact point where ray hited wall
        double intersectionX = (mPlayer.position.x) + rayDirX * distance;
        double intersectionY = (mPlayer.position.y) + rayDirY * distance;


        if (side == 0) perpWallDistance = (rayLengthX - unitXSize);
        else           perpWallDistance = (rayLengthY - unitYSize);

        int lineHeight = 1000 / perpWallDistance;

        int drawStart = -lineHeight / 2 + (mWindowsHeight / 2);

        float point;
        if (side == 0) {
            point = (intersectionY - mapY) * mTEXTURE_SIZE;
        }
        else {
            point = (intersectionX - mapX) * mTEXTURE_SIZE;
        }

        //need it to display sprites properly
        mZBuffer[i] = perpWallDistance;
        mZBuffer[i + 1] = perpWallDistance;

        sf::RectangleShape wall(sf::Vector2f(2, lineHeight));
        int transparency = (0 > 255 - (perpWallDistance * 20)) ? 0 : 255 - (perpWallDistance * 20);

        wall.setPosition(i, drawStart);
        wall.setTexture(&mTextures[mGameMap.mapData[mapY][mapX] - 1]);
        wall.setFillColor(sf::Color(255, 255, 255, transparency));
        wall.setTextureRect(sf::IntRect(point, 0, 1, mTEXTURE_SIZE));

        mCanvas->draw(wall);
    }

}

void Game::drawSprites() {

    sf::RectangleShape sprite;
    sprite.setTexture(&mTextures[5]);

    //sprite position relative to player
    double spritePosX = mEnemy.position.x - mPlayer.position.x;
    double spritePosY = mEnemy.position.y - mPlayer.position.y;
    double distance = sqrt(spritePosX * spritePosX + spritePosY * spritePosY);  //distance between player and enemy

    double invDet = 1.0 / (mPlayer.plane.x * mPlayer.direction.y - mPlayer.direction.x * mPlayer.plane.y);

    double transformX = invDet * (mPlayer.direction.y * spritePosX - mPlayer.direction.x * spritePosY);
    double transformY = invDet * (-mPlayer.plane.y * spritePosX + mPlayer.plane.x * spritePosY);

    //if enemy is behind camera play warning then return
    if (transformY < 0) {
        //if distance between player and enemy is lower than 10 units then play warning
        if (distance < 10) {
            if (mEnemy.encounterWarningClock.getElapsedTime().asSeconds() > 8) {
                mEnemy.behindWarning.play();
                mEnemy.encounterWarningClock.restart();
            }
        }
        return;
    }

    if (distance < 10) {
        if (mEnemy.encounterWarningClock.getElapsedTime().asSeconds() > 8) {
            mEnemy.frontWarning.play();
            mEnemy.encounterWarningClock.restart();
        }
    }

    int spriteScreenX = int((mWindowsWidth / 2) * (1 + transformX / transformY));

    //calculate height of the sprite on screen
    int spriteHeight = abs(int(mWindowsHeight / (transformY)));
    int drawStartY = -spriteHeight / 2 + mWindowsHeight / 2;
    int drawEndY = spriteHeight / 2 + mWindowsHeight / 2;

    //calculate width of the sprite on screen
    int spriteWidth = abs(int(mWindowsHeight / (transformY)));
    int drawStartX = -spriteWidth / 2 + spriteScreenX;
    int drawEndX = spriteWidth / 2 + spriteScreenX;

    double texWidth = mTEXTURE_SIZE / (double)spriteWidth;
    int orgStartX = drawStartX;
    int orgWidth = spriteWidth;

    //if sprite is not on screen than return
    if (drawStartX >= mWindowsWidth || drawEndX < 0) {
        return;
    }

    //these need to be checked to avoid 'out of range' error in next two loops
    if (drawEndX >= mWindowsWidth) {
        drawEndX = mWindowsWidth - 1;
    }
    if (drawStartX < 0) {
        spriteWidth -= 0 - drawStartX;
        drawStartX = 0;
    }

    for (int i = drawStartX; i <= drawStartX; i++) {
        if (mZBuffer[i] < transformY) {
            drawStartX++;
            spriteWidth--;
        }
    }
    for (int i = drawEndX; i > drawStartX; i--) {
        if (mZBuffer[i] < transformY) {
            drawEndX--;
            spriteWidth--;
        }
    }
    if (spriteWidth <= 0) {
        return;
    }

    sprite.setSize(sf::Vector2f(spriteWidth, spriteHeight));
    sprite.setTextureRect(sf::IntRect((drawStartX - orgStartX) * texWidth, 0, texWidth * spriteWidth, mTEXTURE_SIZE));
    sprite.setPosition(sf::Vector2f(drawStartX, drawStartY));
    mCanvas->draw(sprite);
}

void Game::handleKeyboard(sf::Time dt) {
    //keyboard handling
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {

        mPlayer.move(dt, 1, mGameMap);
        if (mPlayer.walkSound.getStatus() != sf::Sound::Playing) {
            mPlayer.walkSound.play();
        }
    }
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {

        mPlayer.move(dt, -1, mGameMap);
        if (mPlayer.walkSound.getStatus() != sf::Sound::Playing) {
            mPlayer.walkSound.play();
        }
    }
}
void Game::handleEvents(std::vector<std::unique_ptr<Screen>> &screens, sf::Time dt) {
    
    sf::Event event;
    while (mWindow->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            mWindow->close();
        }
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.scancode == sf::Keyboard::Scan::Escape) {
                
                screens.erase(screens.begin());
                return;
            }
            if (event.key.scancode == sf::Keyboard::Scan::LShift) {
                //need to check if shift is not already pressed 
                //if not then change move speed and set running sound 
                if (mPlayer.moveSpeed != 6) {
                    mPlayer.moveSpeed = 6;
                    mPlayer.walkSound.setBuffer(mSounds[1]);
                }
            }
            if (event.key.scancode == sf::Keyboard::Scan::E) {
                //if player press E on door game is over
                if (mPlayer.interact(mGameMap)) {
                    screens.insert(screens.begin(), std::make_unique<GameOver>(*mWindow, *mCanvas, *mFont, "You are safe!"));
                    screens.erase(screens.begin() + 1);
                    return;
                }
            }
        }

        if (event.type == sf::Event::KeyReleased) {
            if (event.key.scancode == sf::Keyboard::Scan::LShift) {
                mPlayer.moveSpeed = 3;
                mPlayer.walkSound.setBuffer(mSounds[0]);
            }
            if (event.key.scancode == sf::Keyboard::Scan::W) {
                mPlayer.walkSound.stop();
            }
        }
        if (event.type == sf::Event::MouseMoved) {
            mPlayer.rotate(dt, sf::Mouse::getPosition().x, mWindowsWidth);

        }
    }
}

void Game::update(std::vector<std::unique_ptr<Screen>> &screens, sf::Time dt) {

    mEnemy.update(dt);
    if (mEnemy.seekPath) {
        mEnemy.findPathToPlayer(mPlayer.position, mGameMap);
    }
    if (mEnemy.checkCollision(mPlayer)) {
        //if enemy catch player game is over
        screens.insert(screens.begin(), std::make_unique<GameOver>(*mWindow, *mCanvas, *mFont, "You are dead!"));
        screens.erase(screens.begin() + 1);
        return;
    }

    sf::Sprite canvas_sprite;

    mCanvas->clear(sf::Color(0x12, 0x12, 0x12));
    mWindow->clear(sf::Color::Black);

    raycasting();
    drawSprites();
    mCanvas->display();

    canvas_sprite.setTexture(mCanvas->getTexture());

    mWindow->draw(canvas_sprite);
    mWindow->display();
}