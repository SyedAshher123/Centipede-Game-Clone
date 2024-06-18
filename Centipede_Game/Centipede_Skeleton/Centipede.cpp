#include <iostream>
#include <cstdlib>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


using namespace std;

const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX;
const int gameColumns = resolutionY / boxPixelsY;

int gameGrid[gameRows][gameColumns] = {};


/////////////////////////////////////////////////////////////////////////////
//                                                                         //
//                  Functions declarations here                            //
//                                                                         //
/////////////////////////////////////////////////////////////////////////////

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void moveBullet(float bullet[], sf::Clock& bulletClock, bool& spacePressed);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void initializeMushrooms(int maxMushrooms, int destory_mushroom, int double_check, float mushroom[][4]);
void drawMushrooms(sf::RenderWindow& window, sf::Sprite& mushroomSprite, sf::Sprite& mushroom2Sprite, sf::Sprite& mushroomwSprite, sf::Sprite& mushroomhwSprite, int maxMushrooms, int destory_mushroom, int double_check, float mushroom[][4], float centipede[][7], int& check, float whitemushroom[][2]);
void handlePlayerMovement(float player[], sf::Event e);

int checkCollisionWithMushroom(float objX, float objY, float objWidth, float objHeight, int maxMushrooms, int destory_mushroom, int double_check, float mushroom[][4]);
bool checkCollisionofcentipedeWithMushroom(float objX, float objY, float objWidth, float objHeight, int destory_mushroom, float mushroom[][4]);
int checkCollisionWithCentipede(float objX, float objY, float objWidth, float objHeight, int MAX_CENTIPEDE_SEGMENTS, float centipede[][7]);
bool collisionofCentipedeWithPlayer(float objX, float objY, float objWidth, float objHeight, int MAX_CENTIPEDE_SEGMENTS, float centipede[][7]);//gameoverlogic
bool collisionofwhitemushroomWithPlayer(float objX, float objY, float objWidth, float objHeight, int MAX_CENTIPEDE_SEGMENTS, float whitemushroom[][2], float mushroom[][4]);//gameoverlogic


void initializeCentipede(int MAX_CENTIPEDE_SEGMENTS, float& CENTIPEDE_MOVE_SPEED, float centipede[][7]);
void drawCentipede(sf::RenderWindow& window, sf::Sprite& centipedeHeadSprite, sf::Sprite& centipedeBodySprite, int MAX_CENTIPEDE_SEGMENTS, float centipede[][7]);
void handleCentipedeMovement(int maxMushrooms, int MAX_CENTIPEDE_SEGMENTS, int destory_mushroom, float centipede[][7], float mushroom[][4], int& check);
void splitCentipede(int splitIndex, int MAX_CENTIPEDE_SEGMENTS, int& score, float centipede[][7], int& deadSegmentCount, int& level, float mushroom[][4]);
void destoryMushroom(int destoryIndex, int destory_mushroom, int& score, float mushroom[][4]);

void level1(int& score, bool& isGameOverC, bool& isGameOverWM, int maxMushrooms, int destory_mushroom, int double_check, float mushroom[][4],int MAX_CENTIPEDE_SEGMENTS,float& CENTIPEDE_MOVE_SPEED, float centipede[][7], int& deadSegmentCount, int& level );

void level2(int& score, bool& isGameOverC, bool& isGameOverWM, int maxMushrooms, int destory_mushroom, int double_check, float mushroom[][4],int MAX_CENTIPEDE_SEGMENTS,float& CENTIPEDE_MOVE_SPEED, float centipede[][7], int& deadSegmentCount, int& level );


int main()
{
    // These variables are changed from global variables
    int check;
    int level = 1;
    int deadSegmentCount=0;
    const int x = 0;
    const int y = 1;
    const int exists = 2;
    
    const int borderRow = 25; // cuz it is given that border should be 5 rows from the bottom
    
    const int MAX_CENTIPEDE_SEGMENTS = 12;
    float CENTIPEDE_MOVE_SPEED = 0.2f;
    
    const int mushroomX=0;
    const int mushroomY=1;
    const int destory_mushroom=2;
    const int double_check=3;
    
    const int CENTIPEDE_X = 0;
    const int CENTIPEDE_Y = 1;
    const int CENTIPEDE_SPEED = 2;
    const int CENTIPEDE_IS_DEAD = 3;
    const int CENTIPEDE_IS_HEAD = 4;
    const int CENTIPEDE_IS_MOVING_RIGHT = 5;
    const int REVERSE_MOMENT = 6;
    
    int score = 0; // Initialize score
    const int scoreX = 10; // X-coordinate for displaying the score
    const int scoreY = 10; // Y-coordinate for displaying the score
    
    
    // 2D array to store info about each segment of the centipede
    // x, y, speed, isDead, isHead, isMovingRight, reverse direction
    float centipede[12][7];
    float mushroom[42][4];
    float whitemushroom[12][2];
    
    bool isGameOverC = false;
    bool isGameOverWM = false;
    
    srand(time(0));
    
    //Testing
    int random = rand()%11+20;
    int maxMushrooms = 12 + random;
    
    sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);
    window.setSize(sf::Vector2u(740, 680));
    window.setPosition(sf::Vector2i(100, 0));

    sf::Music bgMusic;
    bgMusic.openFromFile("Music/field_of_hopes.ogg");
    bgMusic.play();
    bgMusic.setVolume(50);

    sf::Texture backgroundTexture;
    sf::Sprite backgroundSprite;
    backgroundTexture.loadFromFile("Textures/background.jpg");
    backgroundSprite.setTexture(backgroundTexture);
    backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.30));
    
    
    //Picture for GAME OVER 
    sf::Texture gameOverTexture;
    gameOverTexture.loadFromFile("Textures/Gameover.png");
    sf::Sprite gameOverSprite;
    gameOverSprite.setTexture(gameOverTexture);
   // gameOverSprite.setPosition(0, 0);


    float player[2] = {};
    player[x] = (gameColumns / 2) * boxPixelsX;
    player[y] = (borderRow ) * boxPixelsY; //This will start player at the border row at row 25th from top means 5th row from bottom
    sf::Texture playerTexture;
    sf::Sprite playerSprite;
    playerTexture.loadFromFile("Textures/player.png");
    playerSprite.setTexture(playerTexture);
    playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    float bullet[3] = {};
    bullet[x] = player[x];
    bullet[y] = player[y] - boxPixelsY;
    bullet[exists] = false; // Set to false initially
    sf::Clock bulletClock;
    sf::Texture bulletTexture;
    sf::Sprite bulletSprite;
    bulletTexture.loadFromFile("Textures/bullet.png");
    bulletSprite.setTexture(bulletTexture);
    bulletSprite.setColor(sf::Color(255, 255, 255));
    bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    bool spacePressed = false;
    
    //Normal mushroom sprite 
    sf::Texture mushroomTexture;
    mushroomTexture.loadFromFile("Textures/mushroom.png");
    sf::Sprite mushroomSprite;
    mushroomSprite.setTexture(mushroomTexture);
    mushroomSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));
    
    //Half destroyed mushroom sprite
    sf::Texture mushroom2Texture;
    mushroom2Texture.loadFromFile("Textures/mushroom.png");
    sf::Sprite mushroom2Sprite;
    mushroom2Sprite.setTexture(mushroom2Texture);
    mushroom2Sprite.setTextureRect(sf::IntRect(65, 0, boxPixelsX, boxPixelsY));
    
    //white mushroom sprite
    sf::Texture mushroomwTexture;
    mushroomwTexture.loadFromFile("Textures/mushroom.png");
    sf::Sprite mushroomwSprite;
    mushroomwSprite.setTexture(mushroomwTexture);
    mushroomwSprite.setTextureRect(sf::IntRect(0, 32, boxPixelsX, boxPixelsY));
    
    //white half mushroom sprite
    sf::Texture mushroomhwTexture;
    mushroomhwTexture.loadFromFile("Textures/mushroom.png");
    sf::Sprite mushroomhwSprite;
    mushroomhwSprite.setTexture(mushroomhwTexture);
    mushroomhwSprite.setTextureRect(sf::IntRect(32, 32, boxPixelsX, boxPixelsY));
    

    sf::Texture centipedeHeadTexture;
    centipedeHeadTexture.loadFromFile("Textures/c_head_left_walk.png");

    sf::Sprite centipedeHeadSprite;
    centipedeHeadSprite.setTexture(centipedeHeadTexture);
    centipedeHeadSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    sf::Texture centipedeBodyTexture;
    centipedeBodyTexture.loadFromFile("Textures/c_body_left_walk.png");

    sf::Sprite centipedeBodySprite;
    centipedeBodySprite.setTexture(centipedeBodyTexture);
    centipedeBodySprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

    // Create a sound buffer for the bullet sound
    sf::SoundBuffer bulletBuffer;
    bulletBuffer.loadFromFile("Sound Effects/fire1.wav");

    // Create a sound for the bullet using the loaded sound buffer
    sf::Sound bulletSound;
    bulletSound.setBuffer(bulletBuffer);
    bulletSound.setVolume(50); 
    
    // Create a sound buffer for the Game Over
    sf::SoundBuffer overBuffer;
    overBuffer.loadFromFile("Sound Effects/death.wav");

    // Create a sound for the Game Over using the loaded sound buffer
    sf::Sound overSound;
    overSound.setBuffer(overBuffer);
    overSound.setVolume(100); 
    
    // Create a sound buffer for the level up
    sf::SoundBuffer upBuffer;
    upBuffer.loadFromFile("Sound Effects/1up.wav");

    // Create a sound for the level up using the loaded sound buffer
    sf::Sound upSound;
    upSound.setBuffer(upBuffer);
    upSound.setVolume(80); 
    
    // Create a sound buffer for the level up
    sf::SoundBuffer killBuffer;
    killBuffer.loadFromFile("Sound Effects/kill.wav");

    // Create a sound for the level up using the loaded sound buffer
    sf::Sound killSound;
    killSound.setBuffer(killBuffer);
    killSound.setVolume(120); 
    
    sf::Font font;
    font.loadFromFile("Textures/Bellano.ttf");
    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    
   /* initializeMushrooms(maxMushrooms, destory_mushroom,double_check, mushroom); // Generate random mushroom positions
    initializeCentipede(MAX_CENTIPEDE_SEGMENTS, CENTIPEDE_MOVE_SPEED, centipede); // Generate centipede*/
    

    
     level1(score, isGameOverC, isGameOverWM, maxMushrooms, destory_mushroom, double_check, mushroom, MAX_CENTIPEDE_SEGMENTS, CENTIPEDE_MOVE_SPEED , centipede, deadSegmentCount, level  );
     
     sf::Texture startTexture;
    sf::Sprite startSprite;

    if (!startTexture.loadFromFile("Textures/start.jpg")) {
        // Handle error loading start image
        return 1;
    }

    startSprite.setTexture(startTexture);
    startSprite.setPosition(0, 0);

    bool gameStarted = false;

    // Display start image until Enter key is pressed
    while (window.isOpen() && !gameStarted) {
        window.draw(startSprite);
        window.display();
        window.clear();
        
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                return 0;
            } else if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
                gameStarted = true;
            }
        }
    }
     

    while (window.isOpen() ) {
       if (isGameOverC == true || isGameOverWM == true) {
            window.draw(gameOverSprite);
            window.display(); 
        }
         else{
         
           window.draw(backgroundSprite);
         
    //Checking if player collide with centipede then GAME OVER
    isGameOverC = collisionofCentipedeWithPlayer(player[x], player[y], boxPixelsX, boxPixelsY, MAX_CENTIPEDE_SEGMENTS, centipede); 
    isGameOverWM = collisionofwhitemushroomWithPlayer(player[x], player[y], boxPixelsX, boxPixelsY, MAX_CENTIPEDE_SEGMENTS, whitemushroom, mushroom); 
    
    if(isGameOverC || isGameOverWM ){
    overSound.play();
    }
           
          // Draw the player
          drawPlayer(window, player, playerSprite);
        
        

        // Handle bullets
        if (bullet[exists] == true) {
            moveBullet(bullet, bulletClock, spacePressed);
            drawBullet(window, bullet, bulletSprite);

            int collisionIndexC = checkCollisionWithCentipede(bullet[x], bullet[y], boxPixelsX, boxPixelsY, MAX_CENTIPEDE_SEGMENTS,centipede);
            int collisionIndexM = checkCollisionWithMushroom(bullet[x], bullet[y], boxPixelsX, boxPixelsY, maxMushrooms, destory_mushroom,double_check, mushroom);
           
            
            // If bullet hits a mushroom. 
            if (collisionIndexM >= 0) {
                //killSound.play();
                bullet[exists] = false;
                
                //@ Did this to insure that the mushroom kill when bullet hit it twice
                if( mushroom[collisionIndexM][double_check] >=2){
                destoryMushroom(collisionIndexM, destory_mushroom, score, mushroom);
                }
                
            }
            // If bullet collides with centipede, split the centipede
            if (collisionIndexC >= 0) { 
                killSound.play();
                splitCentipede(collisionIndexC, MAX_CENTIPEDE_SEGMENTS, score, centipede, deadSegmentCount, level,mushroom);
                bullet[exists] = false;
                 
            }
            
             
            
        }
            
              
         // Draw the score on the screen
            scoreText.setString("Score: " + std::to_string(score));
            scoreText.setPosition(scoreX, scoreY);
            window.draw(scoreText);
     
        
        
        
        // Draw mushrooms on the screen
        drawMushrooms(window, mushroomSprite, mushroom2Sprite, mushroomwSprite, mushroomhwSprite, maxMushrooms, destory_mushroom, double_check, mushroom, centipede, check, whitemushroom);
        
        // Draw centipede on the screen
        drawCentipede(window, centipedeHeadSprite, centipedeBodySprite, MAX_CENTIPEDE_SEGMENTS, centipede);
        
        // Handle Cnntipede Movement
        handleCentipedeMovement(maxMushrooms, MAX_CENTIPEDE_SEGMENTS, destory_mushroom, centipede, mushroom, check);
       }
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                return 0;
            }

            // Handle Player Movement
            handlePlayerMovement(player, e);

            // Shoot bullet when Space key is pressed
            if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Space) {
                
                if (!bullet[exists]) {
                    bullet[x] = player[x];
                    bullet[y] = player[y] - boxPixelsY;
                    bullet[exists] = true;
                    spacePressed = true; // Set spacePressed to true when Space key is pressed
                    bulletSound.play();
                }
            }
        }
        
        if (deadSegmentCount == MAX_CENTIPEDE_SEGMENTS) {
            upSound.play();
            level2(score, isGameOverC, isGameOverWM, maxMushrooms, destory_mushroom, double_check, mushroom, MAX_CENTIPEDE_SEGMENTS, CENTIPEDE_MOVE_SPEED , centipede, deadSegmentCount, level);
            continue;
        }

        window.display();
        window.clear();
    }

    return 0;
}

////////////////////////////////////////////////////////////////////////////
//                                                                        //
// Write your functions definitions here. Some have been written for you. //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

/**
 * @brief Draws the player on the given SFML RenderWindow.
 * 
 * This function sets the position of the player sprite based on the provided
 * player array, which contains the x and y coordinates. It then draws the
 * player sprite on the specified RenderWindow.
 * 
 * @param window The SFML RenderWindow on which to draw the player.
 * @param player An array containing the x and y coordinates of the player.
 * @param playerSprite The SFML Sprite representing the player.
 */
void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) {
    playerSprite.setPosition(player[0], player[1]); // 0 is x , 1 is y
    window.draw(playerSprite);
}

/**
 * @brief Moves the bullet based on elapsed time and space bar input.
 * 
 * This function updates the position of the bullet based on the elapsed time
 * since the last movement. The bullet speed is increased when the space bar
 * is pressed. If the bullet goes out of the screen, its existence flag is set
 * to false, and the spacePressed flag is reset.
 * 
 * @param bullet An array containing the x, y coordinates, and existence flag of the bullet.
 * @param bulletClock The SFML Clock used to measure elapsed time for bullet movement.
 * @param spacePressed A boolean flag indicating whether the space bar is currently pressed.
 */
void moveBullet(float bullet[], sf::Clock& bulletClock, bool& spacePressed) {
    if (bulletClock.getElapsedTime().asMilliseconds() < 20)
        return;

    // Increase bullet speed when space bar is pressed
    int speed = spacePressed ? 20 : 10; // Increase the speed here
    bulletClock.restart();
    bullet[1] -= speed; // 1 is y
    if (bullet[1] < -32) {  // 1 is y
        bullet[2] = false; // 2 is exists
        spacePressed = false; // Reset spacePressed when bullet goes out of the screen
    }
}

/**
 * @brief Draws the bullet on the given SFML RenderWindow.
 * 
 * This function sets the position of the bullet sprite based on the provided
 * bullet array, which contains the x and y coordinates. It then draws the
 * bullet sprite on the specified RenderWindow.
 * 
 * @param window The SFML RenderWindow on which to draw the bullet.
 * @param bullet An array containing the x and y coordinates of the bullet.
 * @param bulletSprite The SFML Sprite representing the bullet.
 */
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) {
    bulletSprite.setPosition(bullet[0], bullet[1]); // 0 is x , 1 is y
    window.draw(bulletSprite);
}

/**
 * @brief Handles player movement based on keyboard input.
 * 
 * This function updates the position of the player based on the keyboard
 * input. The player can move left, right, up, or down within the specified
 * boundaries. The movement is determined by the box size and resolution.
 * 
 * @param player An array containing the x and y coordinates of the player.
 * @param e The SFML Event object to check for keyboard input.
 */
void handlePlayerMovement(float player[], sf::Event e) {

    const int X = 0;
    const int Y = 1;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && player[X] > 0) {
        player[X] -= boxPixelsX;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && player[X] < resolutionX - boxPixelsX) {
        player[X] += boxPixelsX;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && player[Y] > 25 * boxPixelsY ) {  // 25 is border row
        player[Y] -= boxPixelsY;
    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && player[Y] < resolutionY - boxPixelsY) {
        player[Y] += boxPixelsY;
    }
}


/**
 * @brief Initializes the positions of mushrooms within the game boundaries.
 * 
 * This function populates the mushroom array with random x and y coordinates,
 * ensuring that mushrooms are placed within the valid game area. The positions
 * are calculated based on box size, resolution, and a margin of 3 times the
 * box size on each side.
 */
void initializeMushrooms(int maxMushrooms, int destory_mushroom, int double_check, float mushroom[][4]) {

    int mushroomX=0;
    int mushroomY=1;
    
   for (int i = 0; i < 12; ++i) {
        // Initialize mushrooms with positions outside the visible screen
        mushroom[i][mushroomX] = -1.0;  // Set x position to a value outside the screen
        mushroom[i][mushroomY] = -1.0;  // Set y position to a value outside the screen

        // Initialize other information
         mushroom[i][destory_mushroom]=0;
         mushroom[i][double_check]=0;
       
    }
    
    
    
    for (int i = 12; i < maxMushrooms; ++i) {
        // Calculate the maximum valid x and y positions considering the sprite size
        int maxXPosition = resolutionX - 3 * boxPixelsX; // 3 times on each side
        //int maxYPosition = resolutionY - 3 * boxPixelsY; // 3 times on each side

        // Random x position as a multiple of boxPixelX with a margin of 3 times boxPixelsX on each side
        mushroom[i][mushroomX] = (rand() % (maxXPosition / boxPixelsX - 6) + 3) * boxPixelsX;

        // Random y position above borderRow as a multiple of boxPixelY with a margin of 3 times boxPixelsY on each side
        mushroom[i][mushroomY] = rand() % (832 - 64 + 1) + 64;;
        mushroom[i][destory_mushroom]=0;
        mushroom[i][double_check]=0;
    }
}

/**
 * @brief Draws the mushrooms on the given SFML RenderWindow.
 * 
 * This function iterates through the mushroom array and sets the position
 * of the mushroom sprite based on the x and y coordinates in the array. It
 * then draws each mushroom sprite on the specified RenderWindow.
 * It checks that if check is 0 means centipede is above players boundry then
   it draws the simple mushrooms.
 * Then it checks that if check is 1 means the centipede is blow tha players
   boundry then it draws white mushrooms  for the mushrooms blow the player 
   boundry  
 * 
 * @param window The SFML RenderWindow on which to draw the mushrooms.
 * @param mushroomSprite The SFML Sprite representing the mushroom.
 */
void drawMushrooms(sf::RenderWindow& window, sf::Sprite& mushroomSprite, sf::Sprite& mushroom2Sprite, sf::Sprite& mushroomwSprite, sf::Sprite& mushroomhwSprite, int maxMushrooms, int    destory_mushroom, int double_check, float mushroom[][4], float centipede[][7], int& check, float whitemushroom[][2] ) {

  if(check == 0){
    for (int i = 0; i < maxMushrooms; ++i) {
    
        if(!mushroom[i][destory_mushroom] && mushroom[i][double_check] ==0 && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){
        mushroomSprite.setPosition(mushroom[i][0], mushroom[i][1]); // 0 is x , 1 is y
        window.draw(mushroomSprite);
        }
        
        if(!mushroom[i][destory_mushroom] && mushroom[i][double_check] ==1 && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){
        mushroom2Sprite.setPosition(mushroom[i][0], mushroom[i][1]); // 0  is x , 1 is y
        window.draw(mushroom2Sprite);
        }      
     }
    
    
  }
  
  if(check == 1){
  
    for (int i = 0; i < 12; ++i) {
    
      if (mushroom[i][1] + boxPixelsY > 810){
          
           whitemushroom[i][0] = mushroom[i][0];
           whitemushroom[i][1] = mushroom[i][1];
           if(!mushroom[i][destory_mushroom] && mushroom[i][double_check] ==0 && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){
           mushroomwSprite.setPosition(mushroom[i][0], mushroom[i][1]); // 0 is x , 1 is y
           window.draw(mushroomwSprite);
           }
        
           if(!mushroom[i][destory_mushroom] && mushroom[i][double_check] ==1 && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){
           mushroomhwSprite.setPosition(mushroom[i][0], mushroom[i][1]); // 0  is x , 1 is y
           window.draw(mushroomhwSprite);
           } 
        
      }else{
      
      
           if(!mushroom[i][destory_mushroom] && mushroom[i][double_check] ==0 && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){
           mushroomSprite.setPosition(mushroom[i][0], mushroom[i][1]); // 0 is x , 1 is y
           window.draw(mushroomSprite);
           }
        
           if(!mushroom[i][destory_mushroom] && mushroom[i][double_check] ==1 && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){
           mushroom2Sprite.setPosition(mushroom[i][0], mushroom[i][1]); // 0  is x , 1 is y
           window.draw(mushroom2Sprite);
          }   
      
      }           
   }
     
     for (int i = 12; i < maxMushrooms; ++i) {
    
        if(!mushroom[i][destory_mushroom] && mushroom[i][double_check] ==0 && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){
        mushroomSprite.setPosition(mushroom[i][0], mushroom[i][1]); // 0 is x , 1 is y
        window.draw(mushroomSprite);
        }
        
        if(!mushroom[i][destory_mushroom] && mushroom[i][double_check] ==1 && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){
        mushroom2Sprite.setPosition(mushroom[i][0], mushroom[i][1]); // 0  is x , 1 is y
        window.draw(mushroom2Sprite);
        }      
     }
  
  
  
  }//@ Bracket of check  
    
}

/**
 * @brief Checks for collision between an object(bullet) and mushrooms.
 * 
 * This function checks for collision between the specified object (I used this function for bullet)
 * by its x, y coordinates, width, and height) and each mushroom in the array.
 * Collision is detected using bounding boxes.
 * 
 * @param objX The x-coordinate of the object.
 * @param objY The y-coordinate of the object.
 * @param objWidth The width of the object.
 * @param objHeight The height of the object.
 * @return the index if a collision is detected, -2 otherwise.
 */
int checkCollisionWithMushroom(float objX, float objY, float objWidth, float objHeight, int maxMushrooms, int destory_mushroom, int double_check, float mushroom[][4]) {
    for (int i = 0; i < maxMushrooms; ++i) {
        float mushroomX = mushroom[i][0];
        float mushroomY = mushroom[i][1];

        // Check for collision using bounding boxes
        
       /*@ In almost all functions of collision i have applied this condition that if that mushroom
        or the segment of centipede is destroyed then do not check it for collision , pourpose of
        this is because in functions (split centipede,destory mushroom) i have used a technique
        that moves the destroyed mushroom and segment to (0,0) axis which is not visiable in 
        screen but it is present there, so i did this so that the destroyred mushrooms or
        segment of centipede does not check for collision*/
        
      if (!mushroom[i][destory_mushroom] && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){  
        if (objX < (mushroomX + objWidth) &&
            (objX + objWidth) > mushroomX &&
            objY < (mushroomY + objHeight) &&
            (objY + objHeight) > mushroomY) {
            mushroom[i][double_check] +=1;
            // Collision detected
            return i;
        }
      }  
    }

    // No collision detected
    return -2;
}

/**
 * @brief Checks for collision between an object(centipede and mushrooms.
 * 
 * This function checks for collision between the specified object ( I used this function to check
   collison with centipede only)

 * by its x, y coordinates, width, and height) and each mushroom in the array.
 * Collision is detected using bounding boxes.
 * 
 * @param objX The x-coordinate of the object.
 * @param objY The y-coordinate of the object.
 * @param objWidth The width of the object.
 * @param objHeight The height of the object.
 * @return True if a collision is detected, false otherwise.
 */
bool checkCollisionofcentipedeWithMushroom(float objX, float objY, float objWidth, float objHeight, int maxMushrooms,int destory_mushroom, float mushroom[][4]){
	
	for (int i = 0; i < maxMushrooms; ++i) {
        float mushroomX = mushroom[i][0];
        float mushroomY = mushroom[i][1];

        // Check for collision using bounding boxes
       if (!mushroom[i][destory_mushroom] && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){ 
        if (objX < (mushroomX + objWidth) &&
            (objX + objWidth) > mushroomX &&
            objY < (mushroomY + objHeight) &&
            (objY + objHeight) > mushroomY) {
            // Collision detected
            return true;
        }
       } 
    }

    // No collision detected
    return false;


}

/**
 * @brief Checks for collision between an object(bullet) and centipedes .
 * 
 * This function checks for collision between the specified object (bullet
 * by its x, y coordinates, width, and height) and each centipede segment.
 * 
 * If a collision with a centipede segment is detected, the index
 * of that segment is returned.  
 * If no collision is detected, -2 is returned.
 * 
 * @param objX The x-coordinate of the object.
 * @param objY The y-coordinate of the object.
 * @param objWidth The width of the object.
 * @param objHeight The height of the object.
 * @return The index of the collided centipede segment or -2 for no collision.
 */
int checkCollisionWithCentipede(float objX, float objY, float objWidth, float objHeight, int MAX_CENTIPEDE_SEGMENTS, float centipede[][7]) {
    int CENTIPEDE_X = 0;
    int CENTIPEDE_Y = 1;
    int CENTIPEDE_IS_DEAD = 3;
    int CENTIPEDE_IS_MOVING_RIGHT = 5;
    
    for (int i = 0; i < MAX_CENTIPEDE_SEGMENTS; ++i) {
        float centipedeX = centipede[i][CENTIPEDE_X];
        float centipedeY = centipede[i][CENTIPEDE_Y];

        // Check for collision using bounding boxes
        if (!centipede[i][CENTIPEDE_IS_DEAD]){
         if (objX < (centipedeX + objWidth) &&
            (objX + objWidth) > centipedeX &&
            objY < (centipedeY + objHeight) &&
            (objY + objHeight) > centipedeY) {
            // Collision detected
            return i;
         }
        } 
    }

    /*if (checkCollisionWithMushroom(objX, objY, objWidth, objHeight)) {
        return -1;
    }*/

    // No collision detected
    return -2;
}
 //*GAME OVER FUNCTIONS**
 /*@This function checks collision between (player) and any segment of (centipede)
 * This function checks for collision between the specified object (player
 * by its x, y coordinates, width, and height) and each centipede segment.
 * 
 * If a collision with a centipede segment is detected, true is retured 
 * If no collision is detected, false is returned.
 * 
 * @param objX The x-coordinate of the player.
 * @param objY The y-coordinate of the player.
 * @param objWidth The width of the player.
 * @param objHeight The height of the player.
 */

bool collisionofCentipedeWithPlayer(float objX, float objY, float objWidth, float objHeight, int MAX_CENTIPEDE_SEGMENTS, float centipede[][7]) {
    
    int CENTIPEDE_X = 0;
    int CENTIPEDE_Y = 1;
    int CENTIPEDE_IS_DEAD = 3;
    
    for (int i = 0; i < MAX_CENTIPEDE_SEGMENTS; ++i) {
        float centipedeX = centipede[i][CENTIPEDE_X];
        float centipedeY = centipede[i][CENTIPEDE_Y];

        // Check for collision using bounding boxes
        
        //@ Reason of doing this is that sometimes collision between player and centipede is 
    //dectected even centipede is far away from player, so now collision will not check for 
    //the segment which is destroyed. 
       if (!centipede[i][CENTIPEDE_IS_DEAD]){  
        if (objX < (centipedeX + objWidth) &&
            (objX + objWidth) > centipedeX &&
            objY < (centipedeY + objHeight) &&
            (objY + objHeight) > centipedeY) {
            // Collision detected
            return true;
           
        }
       } 
    }


    // No collision detected
    return false;
}

/*@ *This function chesks the collision of player with object(white mushroom).
    *This function checks that if white mushroom collides with the player than
    the game is over.
    *And white mushroom appears only in the case when bullet fires the centipede
    segment and that segment is in player area.*/
    
bool collisionofwhitemushroomWithPlayer(float objX, float objY, float objWidth, float objHeight, int MAX_CENTIPEDE_SEGMENTS, float whitemushroom[][2], float mushroom[][4]) {
    
    
    int CENTIPEDE_IS_DEAD = 3;
    
    for (int i = 0; i < MAX_CENTIPEDE_SEGMENTS; ++i) {
        float whitemushroomX = whitemushroom[i][0];
        float whitemushroomY = whitemushroom[i][1];

        // Check for collision using bounding boxes
        
        //@ Reason of doing this is that is that collision should only be
        // detect for the white mushrooms that are not destroyed and are
        // present in the grid
       if (!mushroom[i][2] && mushroom[i][0] >= 0 && mushroom[i][1] >= 0){  
        if (objX < (whitemushroomX + objWidth) &&
            (objX + objWidth) > whitemushroomX &&
            objY < (whitemushroomY + objHeight) &&
            (objY + objHeight) > whitemushroomY) {
            // Collision detected
            return true;
           
        }
       } 
    }


    // No collision detected
    return false;
}

/**
 * @brief Initializes the centipede segments with default values.
 * 
 * This function initializes the centipede array, setting the x and y
 * coordinates, speed, and flags for each segment. The head flag is set to 1
 * for the last segment and 0 for others. The isDead flag is initially set
 * to 0 for all segments. The isMovingRight flag is set to 1 for all segments
 * to start the movement in the right direction.
 */
void initializeCentipede(int MAX_CENTIPEDE_SEGMENTS, float& CENTIPEDE_MOVE_SPEED, float centipede[][7]) {
    int CENTIPEDE_X = 0;
    int CENTIPEDE_Y = 1;
    int CENTIPEDE_SPEED = 2;
    int CENTIPEDE_IS_DEAD = 3;
    int CENTIPEDE_IS_HEAD = 4;
    int CENTIPEDE_IS_MOVING_RIGHT = 5;
    int REVERSE_MOMENT = 6;
    
    srand(static_cast<unsigned int>(time(0)));
    int randomX = rand() % (960 - 550 + 1) + 550;
    
    for (int i = 0; i < MAX_CENTIPEDE_SEGMENTS; i++) {
        centipede[i][CENTIPEDE_X] = randomX - boxPixelsX * (i + 1);
        centipede[i][CENTIPEDE_Y] = 0;
        centipede[i][CENTIPEDE_SPEED] = CENTIPEDE_MOVE_SPEED;
        centipede[i][CENTIPEDE_IS_DEAD] = 0;
        centipede[i][REVERSE_MOMENT] = 0;

        // Set the head flag to 1 for the last segment, 0 for others
       centipede[i][CENTIPEDE_IS_HEAD] = (i == MAX_CENTIPEDE_SEGMENTS - 1) ? 1 : 0;
        centipede[i][CENTIPEDE_IS_MOVING_RIGHT] = 0;
    }
}

/**
 * @brief Draws the centipede segments on the given SFML RenderWindow.
 * 
 * This function iterates through the centipede array and draws each segment
 * on the specified RenderWindow using the provided head and body sprites.
 * The position of each segment is determined by the x and y coordinates in
 * the centipede array. Head and body sprites are drawn accordingly.
 * 
 * @param window The SFML RenderWindow on which to draw the centipede.
 * @param centipedeHeadSprite The SFML Sprite representing the centipede head.
 * @param centipedeBodySprite The SFML Sprite representing the centipede body.
 */
void drawCentipede(sf::RenderWindow& window, sf::Sprite& centipedeHeadSprite, sf::Sprite& centipedeBodySprite, int MAX_CENTIPEDE_SEGMENTS, float centipede[][7]) {
    int CENTIPEDE_X = 0;
    int CENTIPEDE_Y = 1;
    int CENTIPEDE_IS_DEAD = 3;
     int CENTIPEDE_IS_HEAD = 4;

    for (int i = 0; i < MAX_CENTIPEDE_SEGMENTS ; ++i) {
        
        if (!centipede[i][CENTIPEDE_IS_DEAD]) {
            if (!centipede[i][CENTIPEDE_IS_HEAD]) {
             centipedeBodySprite.setPosition(centipede[i][CENTIPEDE_X], centipede[i][CENTIPEDE_Y]);
            window.draw(centipedeBodySprite);
            
            } else {
            centipedeHeadSprite.setPosition(centipede[i][CENTIPEDE_X], centipede[i][CENTIPEDE_Y]);
            window.draw(centipedeHeadSprite);
            }
        }
    }
}

/**
 * @brief Handles the movement of centipede segments.
 * 
 * This function updates the position of each centipede segment based on its
 * current direction and speed. It also checks for collisions with the right
 * and left boundaries, changing the direction and moving down one row if needed.
 * Additionally, it checks for collisions with mushrooms, and if a collision is
 * detected, it changes the direction and moves one row down.
 */
void handleCentipedeMovement(int maxMushrooms, int MAX_CENTIPEDE_SEGMENTS, int destory_mushroom, float centipede[][7], float mushroom[][4], int& check) {

    check = 0;
    int CENTIPEDE_X = 0;
    int CENTIPEDE_Y = 1;
    int CENTIPEDE_SPEED = 2;
    int CENTIPEDE_IS_MOVING_RIGHT = 5;
    int REVERSE_MOMENT = 6;
    
    for (int i = 0; i < MAX_CENTIPEDE_SEGMENTS; i++ ) {
        if (centipede[i][CENTIPEDE_Y] + boxPixelsY > resolutionY - boxPixelsY){
            centipede[i][REVERSE_MOMENT] = 1;
        } 
        else if (centipede[i][CENTIPEDE_Y] + boxPixelsY <= (boxPixelsY) * 26) {
            centipede[i][REVERSE_MOMENT] = 0;
        }

    if(centipede[i][REVERSE_MOMENT] == 0){    
        // Update object position
        if (centipede[i][CENTIPEDE_IS_MOVING_RIGHT] == 1) {
            centipede[i][CENTIPEDE_X] += centipede[i][CENTIPEDE_SPEED];

        } else {
            centipede[i][CENTIPEDE_X] -= centipede[i][CENTIPEDE_SPEED];
        }

        // Check right boundary and switch direction
        if (int(centipede[i][CENTIPEDE_X]) + boxPixelsX >= resolutionX) {
            centipede[i][CENTIPEDE_X] = resolutionX - boxPixelsX;
            centipede[i][CENTIPEDE_IS_MOVING_RIGHT] = 0;
            centipede[i][CENTIPEDE_Y] += boxPixelsY;
                     
        }

        // Check left boundary and switch direction
        if (centipede[i][CENTIPEDE_X] < 0) {
            if (i == MAX_CENTIPEDE_SEGMENTS - 1)
            centipede[i][CENTIPEDE_X] = 0;
            centipede[i][CENTIPEDE_IS_MOVING_RIGHT] = 1;
            centipede[i][CENTIPEDE_Y] += boxPixelsY;
             
            
        }
             
       // Check for collisions with mushrooms
      for (int j = 0; j < maxMushrooms; ++j) {
      if (checkCollisionofcentipedeWithMushroom(centipede[i][CENTIPEDE_X], centipede[i][CENTIPEDE_Y], boxPixelsX,  boxPixelsY,maxMushrooms,destory_mushroom, mushroom)) {
                // Collision detected, change direction and move one row down
                centipede[i][CENTIPEDE_IS_MOVING_RIGHT] = !centipede[i][CENTIPEDE_IS_MOVING_RIGHT];
                centipede[i][CENTIPEDE_Y] += boxPixelsY;
                break; // No need to check for other collisions in the same row
  }
 }
 
 }
 
      if(centipede[i][REVERSE_MOMENT] == 1){    
        // Update object position
        if (centipede[i][CENTIPEDE_IS_MOVING_RIGHT] == 1) {
            centipede[i][CENTIPEDE_X] += centipede[i][CENTIPEDE_SPEED];

        } else {
            centipede[i][CENTIPEDE_X] -= centipede[i][CENTIPEDE_SPEED];
        }
      
      // Check right boundary and switch direction  
      if (centipede[i][CENTIPEDE_Y] + boxPixelsY > resolutionY){
        // Check right boundary and switch direction
        if (centipede[i][CENTIPEDE_X] + boxPixelsX > resolutionX) {
            centipede[i][CENTIPEDE_X] = resolutionX - boxPixelsX;
            centipede[i][CENTIPEDE_IS_MOVING_RIGHT] = 0;
            //centipede[i][CENTIPEDE_Y] -= boxPixelsY;    
        }
     }else{
     
        if (centipede[i][CENTIPEDE_X] + boxPixelsX > resolutionX) {
            centipede[i][CENTIPEDE_X] = resolutionX - boxPixelsX;
            centipede[i][CENTIPEDE_IS_MOVING_RIGHT] = 0;
            centipede[i][CENTIPEDE_Y] -= boxPixelsY;    
        }  
     }   

        // Check left boundary and switch direction
        if (centipede[i][CENTIPEDE_X] < 0) {
            centipede[i][CENTIPEDE_X] = 0;
            centipede[i][CENTIPEDE_IS_MOVING_RIGHT] = 1;
            centipede[i][CENTIPEDE_Y] -= boxPixelsY;
             
            
        }
             
       // Check for collisions with mushrooms
      for (int j = 0; j < maxMushrooms; ++j) {
      if (checkCollisionofcentipedeWithMushroom(centipede[i][CENTIPEDE_X], centipede[i][CENTIPEDE_Y], boxPixelsX,  boxPixelsY,maxMushrooms,destory_mushroom, mushroom)) {
                // Collision detected, change direction and move one row down
                centipede[i][CENTIPEDE_IS_MOVING_RIGHT] = !centipede[i][CENTIPEDE_IS_MOVING_RIGHT];
                centipede[i][CENTIPEDE_Y] -= boxPixelsY;
                break; // No need to check for other collisions in the same row
  }
 }
 
 }   
    if (centipede[i][CENTIPEDE_Y] + boxPixelsY > 810) {
            // Move back to the top of player region
            check = 1;
        }    
        
}
}

/**
 * @brief Splits the centipede into two segments upon a hit.
 * 
 * This function marks the hit segment as dead and determines the head and tail
 * indices of the remaining centipede segments. The head and tail indices are
 * used to identify the new head segment in the centipede array.
 * 
 * @param hitIndex The index of the centipede segment that was hit.
 */
void splitCentipede(int hitIndex, int MAX_CENTIPEDE_SEGMENTS, int& score, float centipede[][7], int& deadSegmentCount, int& level, float mushroom[][4]) {
    
    int headIndex, tailIndex;
    int CENTIPEDE_IS_DEAD = 3;
    int CENTIPEDE_IS_HEAD = 4;
    
  //level 1******   
  if(level == 1){
   
   //@ logic if the hit segment in not head
   if(centipede[hitIndex][CENTIPEDE_IS_HEAD] == 0){
    
       // Mark the hit segment as dead
       centipede[hitIndex][CENTIPEDE_IS_DEAD] = 1;
       deadSegmentCount+=1;
       
       /*@ I did this so that bullet can pass through the segment when that segment is dead
       @ And second reason of doing this is that sometimes collision between player and centipede is 
       dectected even centipede is far away from player, I did this so that destroyed segments 
       donot effect this.*/
      
      score+=10;
    
      if (hitIndex - 1 >= 0) {
        centipede[hitIndex - 1][CENTIPEDE_IS_HEAD] = 1;
      }
      
      //@This is for sponing of mushroom
      mushroom[hitIndex][0] = centipede[hitIndex][0];  // Set x position of mushroom to x position of centipede
      mushroom[hitIndex][1] = centipede[hitIndex][1];  // Set y position of mushroom to y position of centipede
}

  //@ logic if the hit segment is head
  if(centipede[hitIndex][CENTIPEDE_IS_HEAD] == 1){
     score +=20;
     
     mushroom[hitIndex][0] = centipede[hitIndex][0];  // Set x position of mushroom to x position of centipede
     mushroom[hitIndex][1] = centipede[hitIndex][1];  // Set y position of mushroom to y position of centipede
   
     for (int i = hitIndex; i >= 0; i--) {
     
        if(centipede[i][CENTIPEDE_IS_DEAD] == 1){
         break;
        }
        else{
        centipede[i][CENTIPEDE_IS_DEAD] = 1;
        deadSegmentCount+=1;
        
        }
     } 
      
  }
  
} // closing bracket for level 1
    
  //@ level 2 logic starts here****
  if(level == 2){
  
    // Find the head.
    for (int i = hitIndex; i >= 0; i--) {
        if (centipede[i][CENTIPEDE_IS_DEAD] == 1) {
            headIndex = i + 1;
            break;
        }
        headIndex = i;
    }

    // Find the tail.
    for (int i = hitIndex; i < MAX_CENTIPEDE_SEGMENTS; i++) {
        if (centipede[i][CENTIPEDE_IS_DEAD] == 1) {
            tailIndex = i - 1;
            break;
        }
        tailIndex = i;
    }

    // Mark the hit segment as dead
    centipede[hitIndex][CENTIPEDE_IS_DEAD] = 1;
    deadSegmentCount+=1;
    
    if(centipede[hitIndex][CENTIPEDE_IS_HEAD] == 1){
    score+=20;
    }else{
    score+=10;
    }
    
    
    if (hitIndex - 1 >= 0) {
        centipede[hitIndex - 1][CENTIPEDE_IS_HEAD] = 1;
    }
     //@This is for sponing of mushroom
      mushroom[hitIndex][0] = centipede[hitIndex][0];  // Set x position of mushroom to x position of centipede
      mushroom[hitIndex][1] = centipede[hitIndex][1];  // Set y position of mushroom to y position of centipede
    
    
    }
}

void destoryMushroom(int hitIndex, int destory_mushroom, int& score, float mushroom[][4]){

 mushroom[hitIndex][destory_mushroom] = 1;
 //@ I did this so that bullet can pass through the mushroom when that mushroom is dead
 //mushroom[hitIndex][0]=0;
 //mushroom[hitIndex][1]=0;
 score+=1;
 

}

/*@ This function is for level 1 in level one splitting of centipede is a bit diffrent that why it 
 is necessary to make a seprate function for level 1.
*In level 1 the speed is bit slower and the whole segment before the cetipede will destroy if the
bullet hits the head of centiipede*/

void level1(int& score, bool& isGameOverC, bool& isGameOverWM, int maxMushrooms, int destory_mushroom, int double_check, float mushroom[][4],int MAX_CENTIPEDE_SEGMENTS,float& CENTIPEDE_MOVE_SPEED, float centipede[][7], int& deadSegmentCount, int& level ){
    
    score = 0;
    level = 1;
    isGameOverC = false;
    isGameOverWM = false;
    int borderRow = 25;
    deadSegmentCount = 0;
    CENTIPEDE_MOVE_SPEED = 0.25f;
    
    initializeMushrooms(maxMushrooms, destory_mushroom,double_check, mushroom); // Generate random mushroom positions
    initializeCentipede(MAX_CENTIPEDE_SEGMENTS, CENTIPEDE_MOVE_SPEED, centipede); // Generate centipede

    // Init the player
    float player[2] = {};
    player[0] = (gameColumns / 2) * boxPixelsX;
    player[1] = (borderRow ) * boxPixelsY; //This will start player at the border row at row 25th from top means 5th row from bottom
    
}

/*@ This is a function for other levels this function will be called whenever all the segments of 
centipede are died in the prevoius level.
*In each advancing level the speed of centipede will increase.
And in these levels if the bullet hits the head of centipede the segment before it becomes the head*/

void level2(int& score, bool& isGameOverC, bool& isGameOverWM, int maxMushrooms, int destory_mushroom, int double_check, float mushroom[][4],int MAX_CENTIPEDE_SEGMENTS,float& CENTIPEDE_MOVE_SPEED, float centipede[][7], int& deadSegmentCount, int& level ){
    
    score = score;
    level = 2;
    isGameOverC = false;
    isGameOverWM = false;
    int borderRow=25;
    deadSegmentCount = 0;
    CENTIPEDE_MOVE_SPEED += 0.1f;
    
    initializeMushrooms(maxMushrooms, destory_mushroom,double_check, mushroom); // Generate random mushroom positions
    initializeCentipede(MAX_CENTIPEDE_SEGMENTS, CENTIPEDE_MOVE_SPEED, centipede); // Generate centipede

    // Init the player
    float player[2] = {};
    player[0] = (gameColumns / 2) * boxPixelsX;
    player[1] = (borderRow ) * boxPixelsY; //This will start player at the border row at row 25th from top means 5th row from bottom
    
}
