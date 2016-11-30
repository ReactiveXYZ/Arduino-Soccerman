#include <gamma.h>
#include <RGBmatrixPanel.h>
#include <Adafruit_GFX.h>
#include <EEPROM.h>

// define the wiring of the LED screen
const uint8_t CLK  = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;
const int RESTART_PIN_NUMBER = 11;

// global constant for the number of Invaders in the game
const int NUM_ENEMIES = 16;

// global constant for number of Invaders per row
const int NUM_ENEMIES_PER_ROW = 8;

// global constant for number of rows of invaders
const int NUM_ROWS = 2;

// a global variable that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// the following functions are for printing messages
void print_level(int level);
void print_lives(int lives);
void game_over();

class Color {
  public:
    int red;
    int green;
    int blue;
    Color() {
      red = 0;
      green = 0;
      blue = 0;
    }
    
    Color(int r, int g, int b) {
      red = r;
      green = g;
      blue = b;
    }
    
    uint16_t to_333() const {
      return matrix.Color333(red, green, blue);
    }
};

const Color BLACK(0, 0, 0);
const Color RED(7, 0, 0);
const Color ORANGE(7, 4, 0);
const Color YELLOW(7, 7, 0);
const Color GREEN(0, 7, 0);
const Color BLUE(0, 0, 7);
const Color PURPLE(7, 0, 7);
const Color WHITE(7, 7, 7);
const Color LIME(4, 7, 0);
const Color AQUA(0, 7, 7);

class Invader {
  public:
    // Constructors
    Invader() {
      x = 0;
      y = 0;
      strength = 0;
    }
    
    // sets values for private data members x and y
    Invader(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }
   
    // sets values for private data members
    Invader(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }
    
    // sets values for private data members
    void initialize(int x_arg, int y_arg, int strength_arg) {
      x = x_arg;
      y = y_arg;
      strength = strength_arg;
    }

    // getters
    int get_x() const {
      return x;
    }
    
    int get_y() const {
      return y;
    }
    
    int get_strength() const {
      return strength;
    }

    // Moves the Invader down the screen by one row
    // Modifies: y
    void move() {
      // only move when the invader does not reach the bottom and its strength is greater than 0
      if (y <= 14 && strength >= 1) {
        y = y + 1; 
      }
    }

    // draws the Invader if its strength is greater than 0
    // calls: draw_with_rgb
    void draw() {
      if (strength > 0) {
        // the body color of the invader depends on its strength
        switch (strength) {
          case 1:
            draw_with_rgb(RED, BLUE);
            break;
          case 2:
            draw_with_rgb(ORANGE, BLUE);
            break;
          case 3:
            draw_with_rgb(YELLOW, BLUE);
            break;
          case 4:
            draw_with_rgb(GREEN, BLUE);
            break;
          case 5:
            draw_with_rgb(BLUE, BLUE);
            break;
          case 6:
            draw_with_rgb(PURPLE, BLUE);
            break;
          case 7:
            draw_with_rgb(WHITE, BLUE);
            break;
        }
      }
    }

    // draws black where the Invader used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK, BLACK);
    }

    // Invader is hit by a Cannonball.
    // Modifies: strength
    // calls: draw, erase
    void hit() {
      strength --;
      // change the body color of the invader
      if (strength >= 1) {
        draw();
      } 
      // the invader disappears when it has no strength
      else {
        erase();
      }
    }



  private:
    int x;
    int y;
    int strength;

    // draws the Invader
    void draw_with_rgb(Color body_color, Color eye_color) {
      // (x, y) represents the bottom-left pixel of the invader
      matrix.drawPixel(x, y, body_color.to_333());
      matrix.drawPixel(x + 3, y, body_color.to_333());
      for (int i = 0; i < 4; ++i) {
        matrix. drawPixel(x + i, y - 1, body_color.to_333());
      }
      matrix.drawPixel(x, y - 2, body_color.to_333());
      matrix.drawPixel(x + 3, y - 2, body_color.to_333());
      matrix.drawPixel(x + 1, y - 3, body_color.to_333());
      matrix.drawPixel(x + 2, y - 3, body_color.to_333());
      matrix.drawPixel(x + 1, y - 2, eye_color.to_333());
      matrix.drawPixel(x + 2, y - 2, eye_color.to_333());
    }
};



class Cannonball {
  public:
    Cannonball() {
      x = 0;
      // make y out of bound
      y = 30; 
      fired = false;
    }

    // resets private data members to initial values
    void reset() {
      x = 0;
      y = 30;
      fired = false;
    }

    // getters
    int get_x() const {
      return x;
    }
    
    int get_y() const {
      return y;
    }
    
    bool has_been_fired() const {
      return fired;
    }

    // sets private data members
    void fire(int x_arg, int y_arg) {
      fired = true;
      x = x_arg;
      y = y_arg;
    }

    // moves the Cannonball and detects if it goes off the screen
    // Modifies: y, fired
    void move() {
      if (y > 0) {
        y = y - 1;
      }
      else {
        fired = false;
      }
    }
    
    // resets private data members to initial values
    void hit() {
      reset();
    }

    // draws the Cannonball, if it is fired
    void draw() {
      // (x, y) represents the upper pixel of the cannonball
      matrix.drawPixel(x, y, ORANGE.to_333());
      matrix.drawPixel(x, y + 1, ORANGE.to_333());
    }

    // draws black where the Cannonball used to be
    void erase() {
      matrix.drawPixel(x, y, BLACK.to_333());
      matrix.drawPixel(x, y + 1, BLACK.to_333());
    }



  private:
    int x;
    int y;
    bool fired;
};



class Player {
  public:
    Player() {
      x = 0;
      y = 0;
      lives = 3;
    }

    // getters
    int get_x() const {
      return x;
    }
    
    int get_y() const {
      return y;
    }
    
    int get_lives() const {
      return lives;
    }

    // setter
    void set_x(int x_arg) {
      // the y-coordinate of the player is always 15
      initialize(x_arg, 15);
    }

    // Modifies: lives
    void die() {
      lives = lives - 1;
    }

    // draws the Player
    // calls: draw_with_rgb
    void draw() {
      draw_with_rgb(AQUA);
    }

    // draws black where the Player used to be
    // calls: draw_with_rgb
    void erase() {
      draw_with_rgb(BLACK);
    }



  private:
    int x;
    int y;
    int lives;

    // sets private data members x and y to initial values
    void initialize(int x_arg, int y_arg) {
      x = x_arg;
      y = y_arg;
    }

    // draws the player
    void draw_with_rgb(Color color) {
      // (x, y) represents the bottom-middle pixel of the player
      matrix.drawPixel(x, y, color.to_333());
      matrix.drawPixel(x - 1, y, color.to_333());
      matrix.drawPixel(x + 1, y, color.to_333());
      matrix.drawPixel(x, y - 1, color.to_333());
    }
};



class Game {
  public:
    Game() {
      level = 0;
      time = 0;
    }

    // returns enemy at specified index, if in bounds
    Invader get_enemy(int index) {
      return enemies[index];
     }

    // sets up a new game of Space Invaders
    // Modifies: global variable matrix
    // see http://arduino.cc/en/Reference/Setup
    void setup() {
      // set entire screen to black
      matrix.fillScreen(BLACK.to_333());
      
      // go to next level (which is 1st level)
      next_level();

      // log time
      time = millis();
      invaderTime = time;
      cannonballTime = time;
      collisionTime = time;
    }

    // displays and runs Space Invaders
    // see spec for details of game
    // Modifies: global variable matrix
    void loop(int potentiometer_value, bool button_pressed) {
      // check if restart button (yellow button) is pressed
      bool restart_pressed = (digitalRead(RESTART_PIN_NUMBER) == HIGH);
      if (restart_pressed) {
        restart_game();
        return;
      }
      // check if the player has lost all lives
      if (player.get_lives() < 1 ) {
        end_game();
        return;
       }

      // check if this level is cleared
      if (!level_cleared()) {
        // save time for calc interval
        int currentTime = millis();

        // detect player movement
        int player_center = potentiometer_value / 32;

        player.erase();
        player.set_x(player_center);
        player.draw();

        // detect button press
        if (button_pressed && !ball.has_been_fired()) {
            // fire cannonball with respect to player's current position
            ball.erase();
            ball.fire(player.get_x(), player.get_y());
        }

        // Detect invader collisions
        // frequency: 0.05s/check
        if (currentTime - collisionTime > 50) {
          for (int i = 0; i < get_num_enemies_for_level(); i++ ) {
            // check collision between invader and player || invader touches bound
            if (player_has_collided(enemies[i]) || invader_has_bounded(enemies[i])) {
              // decrease player's life
              player.die();
              // reset level
              reset_level();
              break;
            }

            // check collision between ball and invader
            if (ball_has_hit(enemies[i])) {
              // downgrade/erase enemies
              enemies[i].hit();
              if (enemies[i].get_strength() < 1) {
                enemies[i].erase();
              }

              // remove the cannnonball
              ball.erase();
              // indicate ball is hit
              ball.hit();

              // redraw enemy if clashed
              if (enemies[i].get_strength() > 0) {
                enemies[i].draw();
              }
              
            }
          }
          collisionTime = currentTime;
        }
        
        // Move cannonballs
        // frequency: 0.1s/move
        if (currentTime - cannonballTime > 100) {
          // check the cannonball is fired or not
          if (ball.has_been_fired()) {
            ball.erase();
            ball.move();
            ball.draw();
          } 
          else {
            ball.erase();
          }
          // After the cannonball being moved, set cannonballTime to currentTime
          cannonballTime = currentTime;
        }
          
        // Move invaders
        // frequency: 1.0s/move
        if (currentTime - invaderTime > 1000) {
          // draw invaders for certain level
          for (int i = 0; i < get_num_enemies_for_level(); i++) {
            // move invader when it is still able to fight
            if (enemies[i].get_strength() > 0 && !waiting_for_dispatch(i)) {
              enemies[i].erase();
              enemies[i].move();
              enemies[i].draw();
            }
          }
          // save time for future comparison
          invaderTime = currentTime;
        }
        
        
        
      }
      else {
        // go to next level
        next_level();
      }
    }


  private:
    int level;
    int time;
    int invaderTime;
    int cannonballTime;
    int collisionTime; 
    
    Player player;
    Cannonball ball;
    Invader enemies[NUM_ENEMIES];

    void seedRandom() {

      static const uint32_t salt = 937;
      union
      {
        uint32_t i;
        uint8_t b[4];
      }
      raw;

      int8_t i;
      unsigned int seed;

      for (i = 0; i < sizeof(raw.b); ++i) {
        raw.b[i] = EEPROM.read(i);
      }

      do {
        raw.i += salt;
        seed = raw.i & 0x7FFFFFFF;
      }
      while ((seed < 1) || (seed > 2147483646));

      randomSeed(seed);

      for (i = 0; i < sizeof(raw.b); ++i) {
        EEPROM.write(i, raw.b[i]);
      }

    }

    // generate random strengthes for level 5+
    void random_strengthes(int board[NUM_ROWS][NUM_ENEMIES_PER_ROW]) {
      // generate a random seed
      seedRandom();

      // loop through all rows
      for (int i = 0; i < NUM_ROWS; i++) {

        // loop through all columns
        for (int j = 0; j < NUM_ENEMIES_PER_ROW; j++) {

          int strength = random(1, 6);
          board[i][j] = strength;

        }

      }

    }

    // copy board
    void copy_board(int to[NUM_ROWS][NUM_ENEMIES_PER_ROW], int from[NUM_ROWS][NUM_ENEMIES_PER_ROW]) {

      for (int i = 0; i < NUM_ROWS; i++) {

        for (int j = 0; j < NUM_ENEMIES_PER_ROW; j++) {

          to[i][j] = from[i][j];

        }

      }

    }

    // draw enemies based on level
    void draw_enemies() {

      // do not draw when at level 0
      if (level == 0) {

        return;

      }

      // construct enemy strength map
      int num_rows = NUM_ROWS;

      int enemy_strengthes[num_rows][NUM_ENEMIES_PER_ROW];

      switch (level) {

        case 1: {
          int layout[NUM_ROWS][NUM_ENEMIES_PER_ROW] = {{1, 1, 1, 1, 1, 1, 1, 1}, {}};
          copy_board(enemy_strengthes, layout);
          num_rows = 1;
          }      
          break;

        case 2: {
          int layout[NUM_ROWS][NUM_ENEMIES_PER_ROW] = {{1, 2, 1, 2, 1, 2, 1, 2}, {2, 1, 2, 1, 2, 1, 2, 1}};
          copy_board(enemy_strengthes, layout);
          }
          break;

        case 3: {
          int layout[NUM_ROWS][NUM_ENEMIES_PER_ROW] = {{1, 2, 3, 4, 5, 1, 2, 3}, {4, 5, 1, 2, 3, 4, 5, 1}};
          copy_board(enemy_strengthes, layout);
          }
          break;

        case 4: {
          int layout[num_rows][NUM_ENEMIES_PER_ROW] = {{5, 4, 5, 4, 5, 4, 5, 4}, {2, 3, 2, 3, 2, 3, 2, 3}};
          copy_board(enemy_strengthes, layout);
          }
          break;

        default:
          random_strengthes(enemy_strengthes);
          break;
      }

      // draw enemies
      int index = 0;

      for (int i = 0; i < num_rows; i++) {

        for (int j = 0; j < NUM_ENEMIES_PER_ROW; j++) {
          // erase existing enemies
          enemies[index].erase();

          // draw the invader itself
          Invader invader(4 * j , 4 * (i + 1) - 1, enemy_strengthes[i][j]);
          invader.draw();

          // add to data array
          enemies[index] = invader;
          ++index;
        }

      }

    }

    // get number of enemies for level
    int get_num_enemies_for_level() {

      int num_enemies = 0;

      switch (level) {
        case 1:
          num_enemies = 8;
          break;

        default:
          num_enemies = 16;
          break;
      }

      return num_enemies;
    }

    bool ball_has_hit(Invader& invader) {

      // check if coordinates falls into collision range
      if (ball.get_y() <= invader.get_y() &&
          ball.get_y() >= invader.get_y() - 3 &&
          ball.get_x() >= invader.get_x() &&
          ball.get_x() <= invader.get_x() + 3) {
       

        if (invader.get_strength() > 0) {
          
          return true;

        }

      }

      return false;

    }

    bool player_has_collided(Invader& invader) {

      // when the origin of player touches invader
      if (player.get_y() == invader.get_y() &&
          player.get_x() >= invader.get_x() - 1 &&
          player.get_x() <= invader.get_x() + 4) {

        if (invader.get_strength() > 0) {

          return true;

        }

      } 
      // when the player's top touches the invader
      else if ((player.get_x() == invader.get_x() ||
               player.get_x() == invader.get_x() + 3) &&
               player.get_y() - 1 == invader.get_y()) {

        if (invader.get_strength() > 0) {

          return true;

        }
      }
      

      return false;

    }

    bool invader_has_bounded(Invader& invader) {

      // check if y coordinate has reach bottom
      // which is 31
      
      return invader.get_y() == 15;
    }

    bool waiting_for_dispatch(int index) {

      int batch = index / NUM_ENEMIES_PER_ROW;

      for (int i = (batch + 1) * NUM_ENEMIES_PER_ROW; i < get_num_enemies_for_level(); i++) {

        if (enemies[i].get_strength() > 0) {

          return true;

        }

      }

      return false;

    }

    // check if Player defeated all Invaders in current level
    bool level_cleared() {

      // get num of enemies present
      // depending on the level selected
      int num_enemies = get_num_enemies_for_level();

      // check if all the enemies have no strength
      for (int i = 0; i < num_enemies; i++) {

        if (enemies[i].get_strength() != 0) {

          return false;

        }

      }

      return true;

    }

    // set up a level
    void reset_level() {
      // clear board
      matrix.fillScreen(BLACK.to_333());
      // print messages
      print_lives(player.get_lives());
      delay(1500);
      matrix.fillScreen(BLACK.to_333());
      print_level(level);
      delay(1500);
      matrix.fillScreen(BLACK.to_333());

      // draw the player in the middle
      player.erase();
      player.set_x(16);
      player.draw();

      // draw enemies
      draw_enemies();
    }

    void next_level() {

      // increment level
      ++level;

      // clear board
      matrix.fillScreen(BLACK.to_333());

      // reset player and enemies
      reset_level();

    }

    void restart_game() {

      // reset private members
      level = 0;
      time = 0;
      player = Player();
      ball = Cannonball();
      // resetup
      setup();

    }

    void end_game() {

      // clear board
      matrix.fillScreen(BLACK.to_333());

      // print game over
      game_over();
      delay(1500);
    }
};


// a global variable that represents the game Space Invaders
Game game;

// see http://arduino.cc/en/Reference/Setup
void setup() {

  Serial.begin(9600);
  pinMode(BUTTON_PIN_NUMBER, INPUT);
  matrix.begin();

  game.setup();
}

void loop() {

  int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);

  bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);

  game.loop(potentiometer_value, button_pressed);
}

void print_level(int level) {
  // L
  matrix.setCursor(1,1);
  matrix.setTextColor(AQUA.to_333());
  matrix.print('L');
  // E
  matrix.setCursor(7,1);
  matrix.setTextColor(AQUA.to_333());
  matrix.print('E');
  // V
  matrix.setCursor(13,1);
  matrix.setTextColor(AQUA.to_333());
  matrix.print('V');
  // E
  matrix.setCursor(19,1);
  matrix.setTextColor(AQUA.to_333());
  matrix.print('E');
  // L
  matrix.setCursor(25,1);
  matrix.setTextColor(AQUA.to_333());
  matrix.print('L');
  // :
  matrix.setCursor(29,1);
  matrix.setTextColor(AQUA.to_333());
  matrix.print(':');
  // level
  matrix.setCursor(13, 9);
  matrix.setTextColor(PURPLE.to_333());
  matrix.print(level);
}


// displays number of lives
void print_lives(int lives) {
  // L
  matrix.setCursor(2,1);
  matrix.setTextColor(YELLOW.to_333());
  matrix.print('L');
  // I
  matrix.setCursor(7,1);
  matrix.setTextColor(YELLOW.to_333());
  matrix.print('I');
  // V
  matrix.setCursor(12,1);
  matrix.setTextColor(YELLOW.to_333());
  matrix.print('V');
  // E
  matrix.setCursor(18,1);
  matrix.setTextColor(YELLOW.to_333());
  matrix.print('E');
  // S
  matrix.setCursor(24,1);
  matrix.setTextColor(YELLOW.to_333());
  matrix.print('S');
  // :
  matrix.setCursor(28,1);
  matrix.setTextColor(YELLOW.to_333());
  matrix.print(':');
  // lives
  matrix.setCursor(13, 9);
  matrix.setTextColor(PURPLE.to_333());
  matrix.print(lives);
}


// displays "game over"
void game_over() {
  // G
  matrix.setCursor(4,0);
  matrix.setTextColor(RED.to_333());
  matrix.print('G');
  // A
  matrix.setCursor(10,0);
  matrix.setTextColor(RED.to_333());
  matrix.print('A');
  // M
  matrix.setCursor(16,0);
  matrix.setTextColor(RED.to_333());
  matrix.print('M');
  // E
  matrix.setCursor(22,0);
  matrix.setTextColor(RED.to_333());
  matrix.print('E');
  // O
  matrix.setCursor(4,9);
  matrix.setTextColor(RED.to_333());
  matrix.print('O');
  // V
  matrix.setCursor(10,9);
  matrix.setTextColor(RED.to_333());
  matrix.print('V');
  // E
  matrix.setCursor(16,9);
  matrix.setTextColor(RED.to_333());
  matrix.print('E');
  // R
  matrix.setCursor(22,9);
  matrix.setTextColor(RED.to_333());
  matrix.print('R');
}

