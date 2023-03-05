#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include "level.h"
using namespace std;
using namespace sf;

// Global Variable Declaration
short bgX = 10;
short bgY = 16;
short bgSize = 32;
int width = bgSize * bgX;
int height = bgSize * bgY;
short mapOffsetX = bgSize * 1;
short mapOffsetY = bgSize * 6;
short positionX;
short positionY;
short optSelect;
short lvPlayed; // to check the player plays which level
stringstream lvP; // for display lvPlayed
short lvComplete; // For ending
short resetCount;
short dialogIndex;
short pageSection;
// Page section desc :
// 1 = main menu
// 2 = level selection
// 3 = gameplay
short FadeIn, lvlSize = 30; //Level Text Animate
short FadeIn1; // Fade In for Moves Text
float temp1, temp2; //Level Text Animate
short w=0; // Variabel untuk For, kenapa menggunakan w? agar terlihat ribet. (fatra kont-)
short MillSec, Sec, Min, Hour, DisplaySec, DisplayMin, DisplayHours; // For Timer
stringstream ss, sm, sh; // For timer display
int movesCount; // Variable to Count Moves
stringstream mStr; // For moves display
bool displayFSTClick;
bool firstClick;
bool press;
bool gameOver;
bool transition;
bool pause;
bool isPaused; // Auto pause for ending
bool snd; // Bool to setting sound

// Renderwindow object declaration
RenderWindow window(VideoMode(width, height), "Push the Box!", Style::Close | Style::Titlebar);

// Map active array
short mapActive[10][8] = {{}};

// Function prototype / declaration
void updateLevel(short from[10][8], short to[10][8]);

// Custom button class
class CustomButton {
public:
    // Atribute
    float btnWidth;
    float btnHeight;
    // Constructor
    CustomButton(Vector2f size) {
        btnWidth = size.x;
        btnHeight = size.y;
        obj.setSize(size);
    }
    // Methods
    void setTexture(string link) {
        BtnTexture.loadFromFile(link);
        obj.setTexture(&BtnTexture);
    }
    void changeSize(Vector2f size) {
        btnWidth = size.x;
        btnHeight = size.y;
        obj.setSize(size);
    }
    void objectPosition(Vector2f pos) {
        obj.setPosition(pos);
    }
    // Check the cursor are hovering the object or not
    bool mouseOver() {
        float mouseX = Mouse::getPosition(window).x;
        float mouseY = Mouse::getPosition(window).y;

        float BtnPosX = obj.getPosition().x;
        float BtnPosY = obj.getPosition().y;

        float BtnXPosWidth = obj.getPosition().x + btnWidth;
        float BtnYPosHeight = obj.getPosition().y + btnHeight;

        if (mouseX < BtnXPosWidth && mouseX > BtnPosX && mouseY < BtnYPosHeight && mouseY > BtnPosY)
            return true;
        else
            return false;
    }
    void draw() {
        window.draw(obj);
    }
private:
    // Atribute
    RectangleShape obj;
    Texture BtnTexture;
};
// End of class

int main()
{
    // Framerate limit
    window.setFramerateLimit(60);

    // Set icon of window
    Image icon;
    icon.loadFromFile("assets/icon.png");
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    // Music declaration
    Music backsound;
    backsound.openFromFile("assets/sound/backsound.ogg");
    backsound.setLoop(true);
    backsound.setVolume(50.0f);

    // Sfx declaration
    SoundBuffer clickLow, clickHight;
    clickLow.loadFromFile("assets/sound/click_low.ogg");
    clickHight.loadFromFile("assets/sound/click_hight.ogg");
    Sound tgFilled, tgUnfilled;
    tgFilled.setBuffer(clickHight);
    tgUnfilled.setBuffer(clickLow);

    // General texture declaration
    Texture bg1, bg2, bg3, pt, wl, bx, bv, tg, lg, lv;
    bg1.loadFromFile("assets/img/background1.jpeg");
    bg2.loadFromFile("assets/img/background2.png");
    bg3.loadFromFile("assets/img/background3.png");
    pt.loadFromFile("assets/img/object/platform.png");
    wl.loadFromFile("assets/img/object/wall.png");
    bx.loadFromFile("assets/img/object/box.png");
    bv.loadFromFile("assets/img/object/boxV.png");
    tg.loadFromFile("assets/img/object/target.png");
    lg.loadFromFile("assets/img/logo.png");

    // Level 1 button object declaration
    CustomButton lvl1(Vector2f(40.0f, 40.0f));
    lvl1.setTexture("assets/img/button/lvl1.png");
    lvl1.objectPosition(Vector2f(width/4 - lvl1.btnWidth+10, 250));

    // Level 2 button object declaration
    CustomButton lvl2(Vector2f(40.0f, 40.0f));
    lvl2.setTexture("assets/img/button/lvl2.png");
    lvl2.objectPosition(Vector2f(width/4 + lvl2.btnWidth/2+10, 250));

    // Level 3 button object declaration
    CustomButton lvl3(Vector2f(40.0f, 40.0f));
    lvl3.setTexture("assets/img/button/lvl3.png");
    lvl3.objectPosition(Vector2f(width/2+10, 250));

    // Level 4 button object declaration
    CustomButton lvl4(Vector2f(40.0f, 40.0f));
    lvl4.setTexture("assets/img/button/lvl4.png");
    lvl4.objectPosition(Vector2f(width/2 + (lvl4.btnWidth+lvl4.btnWidth/2)+10, 250));

    // Level 5 button object declaration
    CustomButton lvl5(Vector2f(40.0f, 40.0f));
    lvl5.setTexture("assets/img/button/lvl5.png");
    lvl5.objectPosition(Vector2f(width/4 - lvl5.btnWidth+10, 320));

    // Level 6 button object declaration
    CustomButton lvl6(Vector2f(40.0f, 40.0f));
    lvl6.setTexture("assets/img/button/lvl6.png");
    lvl6.objectPosition(Vector2f(width/4 + lvl6.btnWidth/2+10, 320));

    // Level 7 button object declaration
    CustomButton lvl7(Vector2f(40.0f, 40.0f));
    lvl7.setTexture("assets/img/button/lvl7.png");
    lvl7.objectPosition(Vector2f(width/2+10, 320));

    // Level 8 button object declaration
    CustomButton lvl8(Vector2f(40.0f, 40.0f));
    lvl8.setTexture("assets/img/button/lvl8.png");
    lvl8.objectPosition(Vector2f(width/2 + (lvl8.btnWidth + lvl8.btnWidth/2)+10, 320));

    // Play button
    CustomButton playBtn(Vector2f(130.0f, 65.0f));
    playBtn.setTexture("assets/img/button/play.png");
    playBtn.objectPosition(Vector2f(width/2-playBtn.btnWidth/2, 350));

    // Pause button
    CustomButton PauseBtn(Vector2f(30.0f,30.0f));
    PauseBtn.setTexture("assets/img/TopBar/PAUSE besar.png");
    PauseBtn.objectPosition(Vector2f(10,15));

    // Sound On Button
    CustomButton sound(Vector2f(30.0f, 30.0f));
    sound.setTexture("assets/img/button/soundOn.png");
    sound.objectPosition(Vector2f(width-(sound.btnWidth+10), height-(sound.btnHeight+10)));

    // Next Level Button
    CustomButton nextBtn(Vector2f(120.0f, 60.0f));
    nextBtn.setTexture("assets/img/button/next.png");
    nextBtn.objectPosition(Vector2f(width/3 - nextBtn.btnWidth/2, height - height/3));

    // Back Home Button
    CustomButton homeBtn(Vector2f(60.0f, 60.0f)); //!! Home Button
    homeBtn.setTexture("assets/img/button/home.png");
    homeBtn.objectPosition(Vector2f(width/2 - homeBtn.btnWidth/2 + nextBtn.btnWidth/2 + 20, height - height/3));

    // Back Button
    CustomButton backBtn(Vector2f(40.0f, 35.0f));
    backBtn.setTexture("assets/img/button/Back.png");
    backBtn.objectPosition(Vector2f(10,10));

    // Player texture declaration
    Texture pBack, pFront, pLeft, pRight;
    pBack.loadFromFile("assets/img/player/back.png");
    pFront.loadFromFile("assets/img/player/front.png");
    pLeft.loadFromFile("assets/img/player/left.png");
    pRight.loadFromFile("assets/img/player/right.png");

    // Background and another object declaration
    Sprite background;
    Sprite platform(pt);
    Sprite wall(wl);
    Sprite box(bx);
    Sprite boxV(bv);
    Sprite target(tg);
    Sprite logo(lg);
    Sprite level(lv);

    // Top bar object declaration
    Texture tBar;
    tBar.loadFromFile("assets/img/TopBar/TopBar.png");
    Sprite bgBar;
    bgBar.setTexture(tBar);

    // Font Declaration
    Font font;
    font.loadFromFile("assets/font/8-BIT WONDER.TTF");
    Font font2;
    font2.loadFromFile("assets/font/Early GameBoy.ttf");

    // Text object declaration
    Text p; // Pause
    p.setFont(font);
    p.setString("Paused");
    p.setCharacterSize(40);
    p.setPosition(width/2 - p.getGlobalBounds().width/2, 100);

    Text timer;
    timer.setCharacterSize(15);
    timer.setFont(font2);
    timer.setFillColor(Color::White);

    Text text;
    text.setFont(font);
    text.setFillColor(Color::White);

    Text dialog;
    dialog.setFont(font2);
    dialog.setFillColor(Color::White);

    Text text1; // Aku nemu dek jero looping isOpen(), mbo gawe opo hmmmmm
    text1.setFont(font);

    Text txtLvl;
    txtLvl.setFont(font);
    txtLvl.setColor(Color(255,255,255,0));

    Text moves;
    moves.setFont(font2);
    moves.setPosition(width/2 - moves.getGlobalBounds().width/2, bgBar.getGlobalBounds().height+5);
    moves.setFillColor(Color(255,255,255,0));

    Text timeRecord;
    timeRecord.setFont(font2);
    timeRecord.setCharacterSize(15);
    timeRecord.setFillColor(Color::White);

    // Background menu object declaration
    RectangleShape bgMenu(Vector2f(width, height));
    bgMenu.setTexture(&bg1);
    bgMenu.setPosition(0, 0);

    // Player object declaration
    RectangleShape player(Vector2f(32.0f, 32.0f));
    player.setTexture(&pFront);

    // Backdrop overlay object declaration
    RectangleShape backdrop(Vector2f(width, height));
    backdrop.setFillColor(Color(0, 0, 0, 135));
    backdrop.setPosition(0, 0);

    // Transition overlay object declaration
    RectangleShape overlay(Vector2f(width, height));
    int opacity = 0;
    short animVelocity = 4;
    overlay.setPosition(0, 0);

    // Set the default data
    gameOver = false;
    pause = false;
    pageSection = 1;
    lvPlayed = 1;
    backsound.play();

	// Start the game loop
    while (window.isOpen()) {
        // Process events
        Event e;
        while (window.pollEvent(e)) {
            // Close Event
            if (e.type == Event::Closed)
                window.close();
            if (Keyboard::isKeyPressed(Keyboard::Escape) && pageSection == 1) // escape only close if on main menu
                window.close();

            // Mouse Event
            if(e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                // Play Button
                if (playBtn.mouseOver() && pageSection == 1) {
                    optSelect = 2;
                    transition = true;
                }

                // Pause Button
                if (PauseBtn.mouseOver() && !pause && !gameOver && firstClick && pageSection == 3) {
                    pause = true;
                    PauseBtn.setTexture("assets/img/TopBar/PlayIcon.png");
                } else if (PauseBtn.mouseOver() && pause && !gameOver && firstClick && pageSection == 3) {
                    pause = false;
                    PauseBtn.setTexture("assets/img/TopBar/PAUSE besar.png");
                }

                // Sound On Off Button
                if (sound.mouseOver() && !snd) {
                    backsound.pause();
                    sound.setTexture("assets/img/button/soundOff.png");
                    snd = true;
                } else if (sound.mouseOver() && snd) {
                    backsound.play();
                    sound.setTexture("assets/img/button/soundOn.png");
                    snd = false;
                }

                // Next Button
                if (nextBtn.mouseOver() && gameOver && pageSection == 3) {
                    if (lvPlayed < 8) {
                        lvPlayed++;
                        lvComplete++;
                    } else // If you completed level 8 (last level), then it will direct you to select level
                        optSelect = 2;
                    transition = true;
                }

                // Home Button
                if (homeBtn.mouseOver() && bool(gameOver || pause) && pageSection == 3) {
                    optSelect = 2;
                    transition = true;
                }

                // Back to Menu Button
                if (backBtn.mouseOver() && pageSection == 2) {
                    optSelect = 1;
                    transition = true;
                }
            }

            // Keyboard Event
            if (e.type == Event::KeyPressed) {
                if (Keyboard::isKeyPressed(Keyboard::M)) {
                    if (!snd) {
                        backsound.pause();
                        sound.setTexture("assets/img/button/soundOff.png");
                        snd = true;
                    } else if (snd) {
                        backsound.play();
                        sound.setTexture("assets/img/button/soundOn.png");
                        snd = false;
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::Space) && isPaused) {
                    dialogIndex++;
                    press = true;
                }
            }
        }

        // Clear screen
        window.clear();

        // Render the background
        if (pageSection == 1) {
            window.draw(bgMenu);
        } else {
            for (short i=0; i<bgY; i++) {
                for (short j=0; j<bgX; j++) {
                    if (pageSection == 2) {
                        background.setTexture(bg2);
                        background.setPosition(j*bgSize, i*bgSize);
                        window.draw(background);
                    } else if (pageSection == 3) {
                        background.setTexture(bg3);
                        background.setPosition(j*bgSize, i*bgSize);
                        window.draw(background);
                    }
                }
            }
        }

        // Main menu section
        if (pageSection == 1) {
            // Render the title
            logo.setPosition(width/2-96, 80);
            window.draw(logo);
            // Render the text
            playBtn.draw();

        }

        // Level selection section
        if (pageSection == 2) {
            // Reset value
            lvComplete = 0;
            resetCount = 0;
            pause = false;
            PauseBtn.setTexture("assets/img/TopBar/PAUSE besar.png");

            // Render the title
            text.setString("Select Level");
            text.setCharacterSize(20);
            text.setPosition(width/2-text.getGlobalBounds().width/2, 100);
            window.draw(text);
            text.setString("choose the level by pressing this button");
            text.setCharacterSize(8);
            text.setPosition(width/2-text.getGlobalBounds().width/2, 130);
            window.draw(text);

            backBtn.draw();
            // Render the level button
            lvl1.draw();
            lvl2.draw();
            lvl3.draw();
            lvl4.draw();
            lvl5.draw();
            lvl6.draw();
            lvl7.draw();
            lvl8.draw();

            // Select level trigger
            if(e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left && !transition) {
                if (lvl1.mouseOver()) {
                    optSelect = 3;
                    lvPlayed = 1;
                    transition = true;
                }
                if (lvl2.mouseOver()) {
                    optSelect = 3;
                    lvPlayed = 2;
                    transition = true;
                }
                if (lvl3.mouseOver()) {
                    optSelect = 3;
                    lvPlayed = 3;
                    transition = true;
                }
                if (lvl4.mouseOver()) {
                    optSelect = 3;
                    lvPlayed = 4;
                    transition = true;
                }
                if (lvl5.mouseOver()) {
                    optSelect = 3;
                    lvPlayed = 5;
                    transition = true;
                }
                if (lvl6.mouseOver()) {
                    optSelect = 3;
                    lvPlayed = 6;
                    transition = true;
                }
                if (lvl7.mouseOver()) {
                    optSelect = 3;
                    lvPlayed = 7;
                    transition = true;
                }
                if (lvl8.mouseOver()) {
                    optSelect = 3;
                    lvPlayed = 8;
                    transition = true;
                }
            }

            // Back to level selection trigger (Keyboard)
            if (Keyboard::isKeyPressed(Keyboard::Escape) && !transition) {
                optSelect = 1;
                transition = true;
            }
        }

        // Gameplay section
        if (pageSection == 3) {
            // Reset Value
            if (opacity >= 240) {
                movesCount = 0;
                Sec = 0;
                Min = 0;
                Hour = 0;
                FadeIn = 0;
                FadeIn1 = 0;
                lvlSize = 30;
                moves.setColor(Color(255,255,255,0));
                txtLvl.setCharacterSize(lvlSize);
                temp1 = 0;
                temp2 = 0;
                txtLvl.setPosition(width/2 - txtLvl.getGlobalBounds().width/2, txtLvl.getGlobalBounds().height*3);
            }
            // Render the level
            for (short y=0; y<9; y++) {
                for (short x=0; x<8; x++) {
                    switch (mapActive[y][x]) {
                        case 1:
                            platform.setPosition(x*bgSize+mapOffsetX, y*bgSize+mapOffsetY);
                            window.draw(platform);
                            break;
                        case 2:
                            wall.setPosition(x*bgSize+mapOffsetX, y*bgSize+mapOffsetY);
                            window.draw(wall);
                            break;
                        case 3:
                            box.setPosition(x*bgSize+mapOffsetX, y*bgSize+mapOffsetY);
                            window.draw(box);
                            break;
                        case 4:
                            target.setPosition(x*bgSize+mapOffsetX, y*bgSize+mapOffsetY);
                            window.draw(target);
                            break;
                        case 5:
                            boxV.setPosition(x*bgSize+mapOffsetX, y*bgSize+mapOffsetY);
                            window.draw(boxV);
                    }
                }
            }
            // Add the control and update the position of player
            if (e.type != Event::KeyPressed && !isPaused) {
                press = true;
            } else if (press && !gameOver && !pause && !isPaused) {
                if (Keyboard::isKeyPressed(Keyboard::Left)) {
                    firstClick = true;
                    player.setTexture(&pLeft);
                    if (mapActive[positionY][positionX-1] == 1 || mapActive[positionY][positionX-1] == 4) {
                        movesCount += 1;
                        positionX -= 1;
                    } else if (mapActive[positionY][positionX-1] == 3 && mapActive[positionY][positionX-2] == 1) {
                        movesCount += 1;
                        mapActive[positionY][positionX-2] = 3;
                        mapActive[positionY][positionX-1] = 1;
                        positionX -= 1;
                    } else if (mapActive[positionY][positionX-1] == 3 && mapActive[positionY][positionX-2] == 4) {
                        movesCount += 1;
                        mapActive[positionY][positionX-2] = 5;
                        mapActive[positionY][positionX-1] = 1;
                        positionX -= 1;
                        tgFilled.play();
                    } else if (mapActive[positionY][positionX-1] == 5 && mapActive[positionY][positionX-2] == 4) {
                        movesCount += 1;
                        mapActive[positionY][positionX-2] = 5;
                        mapActive[positionY][positionX-1] = 4;
                        positionX -= 1;
                        tgFilled.play();
                    } else if (mapActive[positionY][positionX-1] == 5 && mapActive[positionY][positionX-2] == 1) {
                        mapActive[positionY][positionX-2] = 3;
                        mapActive[positionY][positionX-1] = 4;
                        positionX -= 1;
                        tgUnfilled.play();
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    firstClick = true;
                    player.setTexture(&pRight);
                    if (mapActive[positionY][positionX+1] == 1 || mapActive[positionY][positionX+1] == 4) {
                        movesCount += 1;
                        positionX += 1;
                    } else if (mapActive[positionY][positionX+1] == 3 && mapActive[positionY][positionX+2] == 1) {
                        movesCount += 1;
                        mapActive[positionY][positionX+2] = 3;
                        mapActive[positionY][positionX+1] = 1;
                        positionX += 1;
                    } else if (mapActive[positionY][positionX+1] == 3 && mapActive[positionY][positionX+2] == 4) {
                        movesCount += 1;
                        mapActive[positionY][positionX+2] = 5;
                        mapActive[positionY][positionX+1] = 1;
                        positionX += 1;
                        tgFilled.play();
                    } else if (mapActive[positionY][positionX+1] == 5 && mapActive[positionY][positionX+2] == 4) {
                        movesCount += 1;
                        mapActive[positionY][positionX+2] = 5;
                        mapActive[positionY][positionX+1] = 4;
                        positionX += 1;
                        tgFilled.play();
                    } else if (mapActive[positionY][positionX+1] == 5 && mapActive[positionY][positionX+2] == 1) {
                        movesCount += 1;
                        mapActive[positionY][positionX+2] = 3;
                        mapActive[positionY][positionX+1] = 4;
                        positionX += 1;
                        tgUnfilled.play();
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::Up)) {
                    firstClick = true;
                    player.setTexture(&pBack);
                    if (mapActive[positionY-1][positionX] == 1 || mapActive[positionY-1][positionX] == 4) {
                        movesCount += 1;
                        positionY -= 1;
                    } else if (mapActive[positionY-1][positionX] == 3 && mapActive[positionY-2][positionX] == 1) {
                        movesCount += 1;
                        mapActive[positionY-2][positionX] = 3;
                        mapActive[positionY-1][positionX] = 1;
                        positionY -= 1;
                    } else if (mapActive[positionY-1][positionX] == 3 && mapActive[positionY-2][positionX] == 4) {
                        movesCount += 1;
                        mapActive[positionY-2][positionX] = 5;
                        mapActive[positionY-1][positionX] = 1;
                        positionY -= 1;
                        tgFilled.play();
                    } else if (mapActive[positionY-1][positionX] == 5 && mapActive[positionY-2][positionX] == 4) {
                        movesCount += 1;
                        mapActive[positionY-2][positionX] = 5;
                        mapActive[positionY-1][positionX] = 4;
                        positionY -= 1;
                        tgFilled.play();
                    } else if (mapActive[positionY-1][positionX] == 5 && mapActive[positionY-2][positionX] == 1) {
                        movesCount += 1;
                        mapActive[positionY-2][positionX] = 3;
                        mapActive[positionY-1][positionX] = 4;
                        positionY -= 1;
                        tgUnfilled.play();
                    }
                }
                if (Keyboard::isKeyPressed(Keyboard::Down)) {
                    firstClick = true;
                    player.setTexture(&pFront);
                    if (mapActive[positionY+1][positionX] == 1 || mapActive[positionY+1][positionX] == 4) {
                        movesCount += 1;
                        positionY += 1;
                    } else if (mapActive[positionY+1][positionX] == 3 && mapActive[positionY+2][positionX] == 1) {
                        movesCount += 1;
                        mapActive[positionY+2][positionX] = 3;
                        mapActive[positionY+1][positionX] = 1;
                        positionY += 1;
                    } else if (mapActive[positionY+1][positionX] == 3 && mapActive[positionY+2][positionX] == 4) {
                        movesCount += 1;
                        mapActive[positionY+2][positionX] = 5;
                        mapActive[positionY+1][positionX] = 1;
                        positionY += 1;
                        tgFilled.play();
                    } else if (mapActive[positionY+1][positionX] == 5 && mapActive[positionY+2][positionX] == 4) {
                        movesCount += 1;
                        mapActive[positionY+2][positionX] = 5;
                        mapActive[positionY+1][positionX] = 4;
                        positionY += 1;
                        tgFilled.play();
                    } else if (mapActive[positionY+1][positionX] == 5 && mapActive[positionY+2][positionX] == 1) {
                        movesCount += 1;
                        mapActive[positionY+2][positionX] = 3;
                        mapActive[positionY+1][positionX] = 4;
                        positionY += 1;
                        tgUnfilled.play();
                    }
                }
                press = false;
            }
            // Render the player
            player.setPosition(positionX*bgSize+mapOffsetX, positionY*bgSize+mapOffsetY);
            window.draw(player);
            // Check win
            for (short y=0; y<9; y++) {
                for (short x=0; x<8; x++) {
                    if (mapActive[y][x] == 4) {
                        gameOver = false;
                        x = 8;
                        y = 9;
                    } else {
                        gameOver = true;
                    }
                }
            }
            // Display
            if (!gameOver)
                PauseBtn.draw();
            window.draw(bgBar);
            window.draw(timer);
            if (firstClick || opacity <= 250 || gameOver)
                window.draw(txtLvl);

            // Display Before Player Click Keyboard
            if (opacity >= 250) {
                MillSec = 0;
                displayFSTClick = false;
            }
            if (!firstClick && !gameOver && !displayFSTClick) {
                window.draw(backdrop);
                text.setString("Tap any arrow keys to start");
                text.setPosition(width/2 - text.getGlobalBounds().width/2, height/2 - text.getGlobalBounds().height/2);
                text.setCharacterSize(10);
                window.draw(text);
                PauseBtn.draw();
            }

            // Display when win
            if (gameOver && !isPaused) {
                window.draw(backdrop);
                text.setString("Level Complete");
                text.setCharacterSize(20);
                text.setPosition(width/2-text.getGlobalBounds().width/2, 100);
                window.draw(text);

                timeRecord.setString("Time Record");
                timeRecord.setFillColor(Color::Green);
                timeRecord.setPosition(width/2-timeRecord.getGlobalBounds().width/2, text.getGlobalBounds().height*10);

                timer.setCharacterSize(18);
                timer.setFillColor(Color::Green);
                timer.setPosition(width/2-timer.getGlobalBounds().width/2, text.getGlobalBounds().height*10 + 17);

                moves.setCharacterSize(18);
                moves.setFillColor(Color::Green);
                moves.setPosition(width/2-moves.getGlobalBounds().width/2, text.getGlobalBounds().height*14 + 17);

                homeBtn.changeSize(Vector2f(60.0f, 60.0f));
                homeBtn.objectPosition(Vector2f(width/2 - homeBtn.btnWidth/2 + nextBtn.btnWidth/2 + 20, height - height/3));

                window.draw(timeRecord);
                window.draw(moves);
                window.draw(timer);
                nextBtn.draw();
                homeBtn.draw();
                firstClick = false;
            }

            // Timer
            if (!pause && !isPaused && !gameOver) {
                // Increase Timer
                if (firstClick)
                    MillSec++;

                // Timer Algorithm
                if (MillSec == 60) {
                    MillSec = 0;
                    Sec++;
                }
                if (Sec == 60) {
                    Sec = 0;
                    Min++;
                }
                if (Min == 60) {
                    Min = 0;
                    Hour++;
                }

                DisplaySec = Sec;
                DisplayMin = Min;
                DisplayHours = Hour;

                // Reset stringstream
                ss.str("");
                sm.str("");
                ss.str("");
                // Add stringstream
                ss << DisplaySec;
                sm << DisplayMin;
                sh << DisplayHours;

                string zero = "0";
                if (Sec < 10 && Min < 10 && Hour == 0)
                    timer.setString(zero + sm.str() + ':' + zero + ss.str());
                else if (Min < 10 && Sec >= 10 && Hour == 0)
                    timer.setString(zero + sm.str() + ':' + ss.str());
                else if (Min >=10 && Sec >= 10 && DisplaySec != 0 && Hour == 0)
                    timer.setString(sm.str() + ':' + ss.str());
                else if (Min >=10 && Sec < 10 && DisplaySec >= 0 && Hour == 0 || Sec >=10 && DisplaySec >=0 && Hour == 0)
                    timer.setString(sm.str() + ':' + zero + ss.str());
                else if (Hour >= 1 && Sec < 10 && Min < 10)
                    timer.setString(sh.str() + ':' + zero + sm.str() + ':' + zero + ss.str());
                else if (Hour >= 1 && Min < 10 && Sec >= 10)
                    timer.setString(sh.str() + ':' + zero + sm.str() + ':' + ss.str());
                else if (Hour >= 1 && Min >=10 && Sec >= 10)
                    timer.setString(sh.str() + ':' + sm.str() + ':' + ss.str());
                else if (Hour >= 1 && Min >=10 && Sec < 10)
                    timer.setString(sh.str() + ':' + sm.str() + ':' + zero + ss.str());
                timer.setCharacterSize(15);
                timer.setPosition(width/2 - timer.getGlobalBounds().width/2 - 1,15);
                timer.setFillColor(Color::White);
            }

            // Level Text Animate
            lvP.str(""); // Reset stringstream
            lvP << lvPlayed; // Add stringstream
            txtLvl.setString("Level " + lvP.str());

            for (w; w<3; w++)
                if (w==1 || w==1 && opacity >= 240)
                    txtLvl.setPosition(width/2 - txtLvl.getGlobalBounds().width/2, txtLvl.getGlobalBounds().height*3);

            if (FadeIn <= 244 && firstClick) {
                FadeIn = FadeIn+5;
                txtLvl.setFillColor(Color(255, 255, 255, FadeIn));
            } else if (FadeIn >= 244 && lvlSize >= 14 && Sec == 1) {
                lvlSize -= 1;
                temp1 += 6.30;
                temp2 += .8;
                txtLvl.setCharacterSize(lvlSize);
                txtLvl.setPosition(width/2 - txtLvl.getGlobalBounds().width/2 + temp1 , txtLvl.getGlobalBounds().height*3 - temp2);
            }

            // Moves Text
            mStr.str(""); // Reset stringstream
            mStr << movesCount; // Add stringstream

            moves.setCharacterSize(13);
            moves.setString("Moves : " + mStr.str());
            moves.setPosition(width/2 - moves.getGlobalBounds().width/2, bgBar.getGlobalBounds().height+8);

            if (FadeIn1 <= 244 && !pause && temp2 >= 13.6) {
                FadeIn1 += 5;
                moves.setFillColor(Color(255, 255, 255, FadeIn1 ));
            }

            // Another display
            if (!gameOver)
                window.draw(moves);
            if (pause) {
                window.draw(backdrop);
                window.draw(p);

                homeBtn.changeSize(Vector2f(80.0f, 80.0f));
                homeBtn.objectPosition(Vector2f(width/2 - homeBtn.btnWidth/2, 300));
                homeBtn.draw();
            }

            // The Endinggg
            if (isPaused) {
                window.draw(backdrop);
                // First dialog
                if (lvComplete == 7 && resetCount == 5) {
                    if (dialogIndex == 1 && press) {
                        dialog.setString("What's wrong?");
                        dialog.setCharacterSize(12);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 2 && press) {
                        dialog.setString("You're just keep resetings \nfor about 5 times..");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                        updateLevel(mapActive, lv8);
                        player.setTexture(&pFront);
                    }
                    if (dialogIndex == 3 && press) {
                        dialog.setString("Is this level \nimposible to solve?");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 4 && press) {
                        dialog.setString("or, are you just too dumb?");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 5 && press) {
                        dialog.setString("Fine. Now how about this?");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                        updateLevel(mapActive, lv8_2);
                        tgFilled.play();
                    }
                    if (dialogIndex == 6 && press) {
                        dialog.setString("Seems easy to you, right?");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }

                    window.draw(dialog);

                    if (dialogIndex > 6) {
                        movesCount = 0;
                        isPaused = false;
                    }
                }
                // Second dialog
                if (lvComplete == 7 && resetCount == 6) {
                    if (dialogIndex == 1 && press) {
                        dialog.setString("Still can't make it?");
                        dialog.setCharacterSize(12);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 2 && press) {
                        dialog.setString("Cmon, that isn't enough for you?");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                        updateLevel(mapActive, lv8);
                        player.setTexture(&pFront);
                    }
                    if (dialogIndex == 3 && press) {
                        dialog.setString("I just helped the two, \nso the rest you have to solve..");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 4 && press) {
                        dialog.setString("What? the boxes is \nblocking your way?");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 5 && press) {
                        dialog.setString("Okay... \nI'll just remove the boxes");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                        updateLevel(mapActive, lv8_3);
                        tgUnfilled.play();
                    }
                    if (dialogIndex == 6 && press) {
                        dialog.setString("Now you can just solve this..");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }

                    window.draw(dialog);

                    if (dialogIndex > 6) {
                        movesCount = 0;
                        resetCount++;
                        isPaused = false;
                    }
                }
                // Last Dialog
                if (lvComplete == 7 && resetCount == 7) {
                    if (dialogIndex == 1) {
                        dialog.setString("Oh, you didn't have boxes \nto push huh?");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 2 && press) {
                        dialog.setString("Well, it's not Push the Box \nif it doesn't have boxes...");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                        updateLevel(mapActive, lv8_3);
                        player.setTexture(&pFront);
                    }
                    if (dialogIndex == 3 && press) {
                        dialog.setString("Nevermind, neither you won't \nbe able to end this game");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 4 && press) {
                        dialog.setString("I'll end it for you..");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                        updateLevel(mapActive, lv8_end);
                        tgFilled.play();
                    }
                    if (dialogIndex == 5 && press) {
                        dialog.setString("Why?");
                        dialog.setCharacterSize(12);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 6 && press) {
                        dialog.setString("Because the developers are \ntoo lazy to fix that..");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 7 && press) {
                        dialog.setString("Now get away");
                        dialog.setCharacterSize(12);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }
                    if (dialogIndex == 8 && press) {
                        dialog.setString("Oh, and congrats for you \nfrom X RPL C");
                        dialog.setCharacterSize(10);
                        dialog.setPosition(width/2-dialog.getGlobalBounds().width/2, 130);
                    }

                    window.draw(dialog);

                    if (dialogIndex > 8) {
                        isPaused = false;
                    }
                }

                // Reset the press value, so it will not spamming
                if (press)
                    press = false;
            }

            // Ending trigger
            if (lvComplete == 7 && !isPaused && !gameOver && resetCount == 7 && movesCount == 7) {
                dialogIndex = 1;
                isPaused = true;
            }

            // Next level keyboard trigger
            if (Keyboard::isKeyPressed(Keyboard::Space) && gameOver && !isPaused && !transition) {
                if (lvPlayed < 8) {
                    lvPlayed++;
                    lvComplete++;
                } else // If you completed level 8 (last level), then it will direct you to select level
                    optSelect = 2;
                transition = true;
            }

            // Reset keyboard trigger (this is the key of the actual ending)
            if (Keyboard::isKeyPressed(Keyboard::R) && !isPaused && !transition && firstClick) {
                // Adding reset count after completing 7 levels
                if (lvComplete == 7 && resetCount < 6)
                    resetCount++;

                // Ending / Reset event
                if (lvComplete == 7 && bool(resetCount == 5 || resetCount == 6)) {
                    dialogIndex = 1;
                    isPaused = true;
                } else {
                    displayFSTClick = true;
                    transition = true;
                    firstClick = false;
                }
            }

            // Back to level selection keyboard trigger
            if (Keyboard::isKeyPressed(Keyboard::Escape) && !isPaused && !transition) {
                displayFSTClick = true;
                optSelect = 2;
                transition = true;
                firstClick = false;
            }
        }

        sound.draw(); // sound button appears in every pages

        // Transition
        if (transition) {
            if (opacity == 0 && animVelocity < 0) {
                animVelocity *= -1;
                transition = false;
            } else if (opacity > 255-animVelocity) {
                animVelocity *= -1;
                if (optSelect != 0) {
                    switch (optSelect) {
                        case 1:
                            pageSection = 1;
                            break;
                        case 2:
                            pageSection = 2;
                            break;
                        case 3:
                            pageSection = 3;
                            break;
                    }
                    optSelect = 0;
                }
                if (pageSection == 3) {
                    switch (lvPlayed) {
                        case 1:
                            updateLevel(mapActive, lv1);
                            player.setTexture(&pFront);
                            break;
                        case 2:
                            updateLevel(mapActive, lv2);
                            player.setTexture(&pFront);
                            break;
                        case 3:
                            updateLevel(mapActive, lv3);
                            player.setTexture(&pFront);
                            break;
                        case 4:
                            updateLevel(mapActive, lv4);
                            player.setTexture(&pFront);
                            break;
                        case 5:
                            updateLevel(mapActive, lv5);
                            player.setTexture(&pFront);
                            break;
                        case 6:
                            updateLevel(mapActive, lv6);
                            player.setTexture(&pFront);
                            break;
                        case 7:
                            updateLevel(mapActive, lv7);
                            player.setTexture(&pFront);
                            break;
                        case 8:
                            if (lvComplete == 7 && resetCount == 7)
                                updateLevel(mapActive, lv8_3);
                            else
                                updateLevel(mapActive, lv8);
                            player.setTexture(&pFront);
                            break;
                    }
                }
            }
            opacity += animVelocity;
            overlay.setFillColor(Color(0, 0, 0, opacity));
            window.draw(overlay);

            if(opacity >= 250) {
            txtLvl.setColor(Color(255,255,255,0));
            FadeIn = 0;
            }
        }
        // Update the window
        window.display();
    }


    return EXIT_SUCCESS;
}

void updateLevel(short from[10][8], short to[10][8]) {
    for (short y=0; y<9; y++) {
        for (short x=0; x<8; x++) {
            from[y][x] = to[y][x];
        }
    }
    // This is for default position of player
    positionX = to[9][0];
    positionY = to[9][1];
}
