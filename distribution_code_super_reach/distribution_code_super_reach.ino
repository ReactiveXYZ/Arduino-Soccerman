#include <gamma.h> 
#include <RGBmatrixPanel.h> 
#include <Adafruit_GFX.h> 
#include <EEPROM.h>

// define the wiring of the LED screen
const uint8_t CLK = 8;
const uint8_t LAT = A3;
const uint8_t OE = 9;
const uint8_t A = A0;
const uint8_t B = A1;
const uint8_t C = A2;

// define the wiring of the inputs
const int POTENTIOMETER_PIN_NUMBER = 5;
const int BUTTON_PIN_NUMBER = 10;
const int FINISH_PIN_NUMBER = 11;

// define the number of defenders in a game
const int NUM_DEFENDERS = 4;

// define the number of cannons that defendes can shoot in total
const int NUM_CANNONS = 2;

// define the matrix that represents the LED screen
RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// define Color classes and common colors
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

// Message printer class
class Printer {

    public:

        void level(int level) {
            // L
            matrix.setCursor(1, 1);
            matrix.setTextColor(AQUA.to_333());
            matrix.print('L');
            // E
            matrix.setCursor(7, 1);
            matrix.setTextColor(AQUA.to_333());
            matrix.print('E');
            // V
            matrix.setCursor(13, 1);
            matrix.setTextColor(AQUA.to_333());
            matrix.print('V');
            // E
            matrix.setCursor(19, 1);
            matrix.setTextColor(AQUA.to_333());
            matrix.print('E');
            // L
            matrix.setCursor(25, 1);
            matrix.setTextColor(AQUA.to_333());
            matrix.print('L');
            // :
            matrix.setCursor(29, 1);
            matrix.setTextColor(AQUA.to_333());
            matrix.print(':');
            // level
            matrix.setCursor(13, 9);
            matrix.setTextColor(PURPLE.to_333());
            matrix.print(level);
        }

    void game_over() {
        // G
        matrix.setCursor(4, 0);
        matrix.setTextColor(RED.to_333());
        matrix.print('G');
        // A
        matrix.setCursor(10, 0);
        matrix.setTextColor(RED.to_333());
        matrix.print('A');
        // M
        matrix.setCursor(16, 0);
        matrix.setTextColor(RED.to_333());
        matrix.print('M');
        // E
        matrix.setCursor(22, 0);
        matrix.setTextColor(RED.to_333());
        matrix.print('E');
        // O
        matrix.setCursor(4, 9);
        matrix.setTextColor(RED.to_333());
        matrix.print('O');
        // V
        matrix.setCursor(10, 9);
        matrix.setTextColor(RED.to_333());
        matrix.print('V');
        // E
        matrix.setCursor(16, 9);
        matrix.setTextColor(RED.to_333());
        matrix.print('E');
        // R
        matrix.setCursor(22, 9);
        matrix.setTextColor(RED.to_333());
        matrix.print('R');
    }

    void go_blue() {
        // #
        matrix.setCursor(4, 0);
        matrix.setTextColor(YELLOW.to_333());
        matrix.print('*');
        // G
        matrix.setCursor(10, 0);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('G');
        // O
        matrix.setCursor(16, 0);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('O');
        // #
        matrix.setCursor(22, 0);
        matrix.setTextColor(YELLOW.to_333());
        matrix.print('*');
        // B
        matrix.setCursor(4, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('B');
        // L
        matrix.setCursor(10, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('L');
        // U
        matrix.setCursor(16, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('U');
        // E
        matrix.setCursor(22, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('E');
    }

    void waiting() {
      // W
      matrix.setCursor(4, 0);
      matrix.setTextColor(YELLOW.to_333());
      matrix.print('W');
      // A
      matrix.setCursor(10, 0);
      matrix.setTextColor(YELLOW.to_333());
      matrix.print('A');
      // I
      matrix.setCursor(15, 0);
      matrix.setTextColor(YELLOW.to_333());
      matrix.print('I');
      // T
      matrix.setCursor(20, 0);
      matrix.setTextColor(YELLOW.to_333());
      matrix.print('T');
      // I
      matrix.setCursor(9, 9);
      matrix.setTextColor(YELLOW.to_333());
      matrix.print('I');
      // N
      matrix.setCursor(14, 9);
      matrix.setTextColor(YELLOW.to_333());
      matrix.print('N');
      // G
      matrix.setCursor(20, 9);
      matrix.setTextColor(YELLOW.to_333());
      matrix.print('G');
      }
    
    void mode_options() {
        // [
        matrix.setCursor(7, 0);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('[');
        // 1
        matrix.setCursor(13, 0);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('1');
        // P
        matrix.setCursor(20, 0);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('P');
        // ]
        matrix.setCursor(25, 0);
        matrix.setTextColor(BLUE.to_333());
        matrix.print(']');
        // [
        matrix.setCursor(7, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('[');
        // 2
        matrix.setCursor(13, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('2');
        // P
        matrix.setCursor(20, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('P');
        // ]
        matrix.setCursor(25, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print(']');
    }

    void selected(int option) {
      if (option == 1) {
        matrix.setCursor(1, 0);
        matrix.setTextColor(RED.to_333());
        matrix.print('#');
      }
      if (option == 2) {
        matrix.setCursor(1, 9);
        matrix.setTextColor(RED.to_333());
        matrix.print('#');
      }
    }

    void deselected() {

        matrix.setCursor(1, 0);
        matrix.setTextColor(BLACK.to_333());
        matrix.print('#');
        matrix.setCursor(1, 9);
        matrix.setTextColor(BLACK.to_333());
        matrix.print('#');

    }

    void scores(int score1, int score2) {
        // P
        matrix.setCursor(4, 0);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('P');
        // 1
        matrix.setCursor(10, 0);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('1');
        // :
        matrix.setCursor(14, 0);
        matrix.setTextColor(BLUE.to_333());
        matrix.print(':');
        // score1
        matrix.setCursor(18, 0);
        matrix.setTextColor(GREEN.to_333());
        matrix.print(score1);
        // P
        matrix.setCursor(4, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('P');
        // 2
        matrix.setCursor(10, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print('2');
        // :
        matrix.setCursor(14, 9);
        matrix.setTextColor(BLUE.to_333());
        matrix.print(':');
        // score2
        matrix.setCursor(18, 9);
        matrix.setTextColor(GREEN.to_333());
        matrix.print(score2);
    }


      
      
    
    

};

// Abstract class for objects that are moveable
class Moveable {

    public:

        explicit Moveable(bool move): able_to_move(move),
                                      last_action_time(0) {};

      void set_can_move(bool move) {
        this->able_to_move = move;
      }

      bool can_move() {
        return this->able_to_move;
      }

    void set_speed(int speed) {
        if (speed_unit == 0) {
            this->speed_unit = speed;
        }

        this->cool_down = 1000 / speed;
    }

    int get_speed() {
        return this->speed_unit;
    }

    void set_initial_action_time(int time) {
        this->last_action_time = time;
    }

    bool ready_to_act(int currentTime) const {
        return currentTime - last_action_time > this->cool_down;
    }

    void timestamp() {
        this->last_action_time = millis();
    }

    virtual void move() = 0;

    private:

        bool able_to_move;
    int last_action_time;
    int cool_down;
    int speed_unit = 0;
};

// Abstract class for objects that are drawable
class Drawable {

    public:

        explicit Drawable(int origin_x = 0,
            int origin_y = 0): x(origin_x), y(origin_y) {};

    void set_x(int x) {
        this->x = x;
    }

    int get_x() const {
        return this->x;
    }

    void set_y(int y) {
        this->y = y;
    }

    int get_y() const {
        return this->y;
    }

    void initialize(int x, int y) {
        this->x = x;
        this->y = y;
    }

    virtual void draw() = 0;
    virtual void erase() = 0;

    protected:

        int x;
    int y;

    void draw_with_color(int x, int y, Color color) {

        matrix.drawPixel(x, y, color.to_333());

    }

};


// Net
class Net: public Moveable, public Drawable {

    public:

        Net(bool move = false): Moveable(move),Drawable() {
        // set coordinates of the upper left pixel of the net
        initialize(11, 0);
        seedRandom();
    }

    void reset() {
        goaled = false;
        move_right = true;
    }

    void goal() {
        goaled = true;
    }

    bool has_been_goaled() const {
        return goaled;
    }
     
     void allow_net_to_expand (bool expand) {
      expandable = expand;
     }
    void move() {
        // record timestamp
        Moveable::timestamp();

        erase();

        if (can_move()) {
          width = predict_width();
          
            if ((x >= 0) && (x <= 31-(width + 1))) {
                if (move_right) {
                    x++;
                   
                    if (x == 31 - (width + 1)) {
                        move_right = false;
                    }
                } else {
                    x--;
                    if (x == 0) {
                        move_right = true;
                    }
                }
            }
        }

        draw();

    }

    void seedRandom() {

      static const uint32_t salt = 937;
      union{
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

    int predict_width() {
      
     int proposed_width = random(2, 6);

      if (!expandable) {
        return 5;
      }

       if (x + proposed_width + 2  > 31) {
            proposed_width = 31 - x - 2;
            return proposed_width;
          }
          else {
            return proposed_width;
          }
     
    }
      


    int get_width() {
      return width;
    }

    void draw() {
        // (x, y) represents the upper left pixel of the net
        for (int i = 0; i <= (width + 1); ++i) {
            draw_with_color(x + i, y, WHITE);
        }

        draw_with_color(x, y + 1, WHITE);
        draw_with_color(x + width + 1, y + 1, WHITE);
    }

    void erase() {
        // (x, y) represents the upper left pixel of the net
        for (int i = 0; i <= (width + 1); ++i) {
            draw_with_color(x + i, y, BLACK);
        }

        draw_with_color(x, y + 1, BLACK);
        draw_with_color(x + (width + 1) , y + 1, BLACK);
    }

    private:

        bool goaled = false;
    bool move_right = true;
    int width = 5;
    bool expandable = false;

};

// Soccer ball
class SoccerBall: public Moveable, public Drawable {

    public:

        SoccerBall(bool move = true): Moveable(move),
    Drawable() {
        // set coordinates of the being fired soccerball
        initialize(15, 11);
    }

    void reset() {
        erase();
        initialize(99, 99);
        set_speed(get_speed());
        shot = false;
    }

    void reset_with_color(Color color) {
        draw_with_color(x, y, color);
        initialize(99, 99);
        set_speed(get_speed());
        shot = false;
        erase();
    }

    bool has_been_shot() const {
        return shot;
    }

    void shoot(int x_arg, int y_arg) {
        shot = true;
        x = x_arg;
        y = y_arg;
    }

    bool has_hit_defender(Drawable & defender) {
        if ((y == defender.get_y()) && ((x >= defender.get_x() && x <= defender.get_x() + 3) ||
                (x >= defender.get_x() + 8 && x <= defender.get_x() + 11) ||
                (x >= defender.get_x() + 16 && x <= defender.get_x() + 19) ||
                (x >= defender.get_x() + 24 && x <= defender.get_x() + 27))) {
            shot = false;
            return true;
        } else {
            return false;
        }
    }

    bool has_hit_net(Net & net) {
        if ((y == 1) && (x >= net.get_x() + 1) && (x <= net.get_x() + net.get_width())) {
            shot = false;
            return true;
        } else {
            return false;
        }
    }

    void move() {
        // record timestamp
        Moveable::timestamp();

        erase();

        if (can_move()) {
            if (y > 0) {
                y--;
            } 
            else {
                reset();
            }
        }

        draw();
    }

    void erase() {
        // (x, y) represents the pixel of the being fired soccerball
        draw_with_color(x, y, BLACK);
    }

    void draw() {
        // (x, y) represents the pixel of the being fired soccerball
        draw_with_color(x, y, ORANGE);
    }

    private:

        bool shot;

};


// Player
class Player: public Moveable, public Drawable {

    public:

        Player(bool move = true): Moveable(move),
    Drawable() {
        // set coordinates of the upper left pixel of the player
        initialize(2, 14);
    }

    void reset(int x_val) {
        erase();
        x = x_val;
        draw();
    }

    void die() {

        dead = true;

    }

    bool is_alive() {
      return !dead;
    }

    void allow_unlimited_shots(bool allow) {
        unlimited_shots = allow;
    }

    void shoot(SoccerBall & ball) {
        
        ball.shoot(x + 1, y + 2 - num_shots);
        if (!unlimited_shots) {
            num_shots--;
            draw();
        }
        ball.set_initial_action_time(millis());

    }

    int get_num_shots() {
        return num_shots;
    }

    int set_num_shots(int shots) {
        num_shots = shots;
    }

    void move() {
        // record timestamp
        Moveable::timestamp();
        // leave blank temporarily
    }

    void erase() {
        // (x, y) represents the upper left pixel of the player
        draw_with_color(x, y, BLACK);
        draw_with_color(x, y + 1, BLACK);
        draw_with_color(x + 2, y, BLACK);
        draw_with_color(x + 2, y + 1, BLACK);

        int copy_num_shots = num_shots;
        while (copy_num_shots > 0) {
            draw_with_color(x + 1, 15 - copy_num_shots + 1, BLACK);
            copy_num_shots--;
        }
    }

    void draw() {
        // (x, y) represents the upper left pixel of the player
        draw_with_color(x, y, AQUA);
        draw_with_color(x, y + 1, AQUA);
        draw_with_color(x + 2, y, AQUA);
        draw_with_color(x + 2, y + 1, AQUA);

        int copy_num_shots = num_shots;
        while (copy_num_shots > 0) {
            draw_with_color(x + 1, 15 - copy_num_shots + 1, WHITE);
            copy_num_shots--;
        }
    }

    private:

        bool unlimited_shots = false;
        bool dead = false;
        int num_shots = 5;

};

// Cannonball
class CannonBall: public Moveable, public Drawable {


      public:

          CannonBall(bool move = true): Moveable(move), Drawable() {
        // set coordinates of the left pixel of the cannonball
        initialize(99, 99);
      }
      void reset() {
        erase();
        initialize(99, 99);
        shot = false;
        draw();
      }

      bool has_been_shot() const {
        return shot;
      }


      void move() {
        // record timestamp
        Moveable::timestamp();

        erase();

        if (can_move()) {
          if (y < 15) {
            y++;
          }
          else {
            reset();
          }
        }

        draw();
      }

      void draw() {
        draw_with_color(x, y, PURPLE);
        draw_with_color(x, y + 1, PURPLE);
        draw_with_color(x + 1, y, PURPLE);
        draw_with_color(x + 1, y + 1, PURPLE);
      }

      void erase() {
        draw_with_color(x, y, BLACK);
        draw_with_color(x, y + 1, BLACK);
        draw_with_color(x + 1, y, BLACK);
        draw_with_color(x + 1, y + 1, BLACK);
      }

      bool has_hit_player(Player& player) {
        int px = 0;
        int py = 0;
        int ps = 0;
        // px, py represents coordinates of player
        // ps represents remaining number of shots 
        px = player.get_x();
        py = player.get_y();
        ps = player.get_num_shots();

        if ((px == x + 1 || px == x - 2) && (py == y - 1 || py ==y || py == y + 1)) {
          return true;
        }

        switch (ps) {
          case 0:
          case 1:
          case 2:
            if ((px == x || px == x - 1) && (py == y + 1)) {
              return true;
            }
            break;

          case 3:
            if ((px == x || px == x - 1) && (py == y + 1 || py == y + 2)) {
              return true;
            }
            break;

           case 4:
             if ((px == x || px == x - 1) && 
                 (py == y + 1 || py == y + 2 || py == y + 3)) {
              return true;
            }
            break;

           case 5:
             if ((px == x || px == x - 1) && 
                 (py == y + 1 || py == y + 2 || py == y + 3 || py == y + 4)) {
               return true;
            }
            break;         
        }

        return false;
      }


      void shoot(int x_val, int y_val) {
        x = x_val;
        y = y_val;
        shot = true;
      }

      bool has_hit_soccer(Drawable& ball) {
        if ((x == ball.get_x() || x == ball.get_x() - 1) && y == ball.get_y() - 2) {
          return true;
        }
        else {
          return false;
        }
      }


      private:

      bool shot = false;
};

// Defender
class Defender: public Moveable, public Drawable {

    public:
    
    static int shooting_cool_down_time;

    Defender(bool move = false): Moveable(move),
        Drawable() {
        // set coordinates of the most left pixel of the current defender
        initialize(4, 3);
    }

    static void set_shooting_frequency(int freq) {

        shooting_cool_down_time = 1000 / freq;
    }
  
    void allow_shooting(bool allow) {
      can_shoot_cannon = allow;
    }

    bool able_to_shoot() {
      return can_shoot_cannon;
    }

    void set_index(int i) {
        index = i;
        move_left = false;
    }

    void move() {
        // record timestamp
        Moveable::timestamp();
        erase();

        if (can_move()) {

            if ((x >= 8 * index) && (x <= 8 * index + 4)) {
                if (move_left) {
                    x--;
                    if (x == 8 * index) {
                        move_left = false;
                    }
                } else {
                    x++;
                    if (x == 8 * index + 4) {
                        move_left = true;
                    }
                }
            }
        }

        draw();
    }

    void draw() {
        // (x, y) represents the most left pixel of the current defender
        draw_with_color(x, y, RED);
        draw_with_color(x + 1, y, RED);
        draw_with_color(x + 2, y, RED);
        draw_with_color(x + 3, y, RED);
    }

    void erase() {
        // (x, y) represents the most left pixel of the current defender
        draw_with_color(x, y, BLACK);
        draw_with_color(x + 1, y, BLACK);
        draw_with_color(x + 2, y, BLACK);
        draw_with_color(x + 3, y, BLACK);
    }

    bool ready_to_shoot(int current_time) {

        return current_time - last_shooting_time > shooting_cool_down_time;

    }

    void shoot(CannonBall & cannon) {

        if (can_shoot_cannon) {
            // timestamp
            last_shooting_time = millis();
            // shoot the cannon
            cannon.shoot(x + 1, y + 1);
        }

    }

    private:
        int index;
        bool move_left = false;
        bool can_shoot_cannon = false;
        int last_shooting_time;
};

int Defender::shooting_cool_down_time = 0;

// Commander [ADDED IN SUPER REACH]
class Commander {

    public:

        Commander() {}

        void interact_with(Defender& defender, CannonBall& cannon) {

            char action = this->read_message();

            switch (action) {

                case 'L':

                    // move defender one to the left
                    defender.erase();

                    if (defender.get_x() <= 0) {

                        defender.set_x(0);

                    } else {

                        defender.set_x(defender.get_x() - 1);

                    }

                    defender.draw();

                    break;

                case 'R':

                    // move defender one to the right
                    defender.erase();

                    if (defender.get_x() >= 28) {

                        defender.set_x(28);

                    } else {

                        defender.set_x(defender.get_x() + 1);

                    }

                    defender.draw();

                    break;

                case 'S':

                    // let the defender shoot a cannonball
                    if (!cannon.has_been_shot()) {

                        defender.shoot(cannon);

                    }

                    break;

                default:
                    break;

            }

        }

        void start_multi_game() {

            Serial.write("s 1");

        }

        void score(String p){

            if (p == "player") {

                Serial.write("p p1");

            }

            if (p == "defender") {

                Serial.write("p p2");

            }

        }

        void finish_multi_game() {

            Serial.write("f 1");

        }


    private:

        char read_message() {

            char ch = 'x';

            if (Serial.available() > 0) {

                ch = Serial.read();

            }

            return ch;

        } 

};

// Single Game
class SingleGame {
    public:

        SingleGame() {
            // initialize level
            level = 0;
            // initialize time
            time = 0;
        }

    void setup() {
        time = millis();
        next_level();

        
    }

    void loop(int potentiometer_value, bool button_pressed) {

        // TODO:
        // check how many shots player has
        if (player.get_num_shots() < 1) {
            if (!net.has_been_goaled() && !ball.has_been_shot()) {
                game_over();
                return;
            }

        }

        if (!player.is_alive() && !net.has_been_goaled()) {
          game_over();
          return;
        }
        
        // set current time
        int current_time = millis();
        // check whether level is cleared
        if (!level_cleared()) {

            // detect the movement of player
            // remain stable if player is not moving significantly
            if (abs(prev_potentiometer_value - potentiometer_value) > 30) {
              player.reset(parse_potentiometer_value(potentiometer_value));
              prev_potentiometer_value = potentiometer_value;
            }
            
            // check if button is pressed
            if (button_pressed && !ball.has_been_shot()) {
                // erase the previous ball
                ball.reset();
                // shoot the ball after press the button
                player.shoot(ball);
                
            }
            //  move soccer ball
            if (ball.ready_to_act(current_time)) {

                if (ball.has_been_shot()) {
                    // set variable speed
                    ball.set_speed((player.get_y() - ball.get_y()) * 0.5 * ball.get_speed());
                    // move ball                  
                    ball.move();
                    // redraw player
                    player.draw();

                } 
            }

            // move defenders

            for (int i = 0; i < NUM_DEFENDERS; ++i) {

                if (ball.has_hit_defender(defenders[i])) {
                    ball.reset_with_color(RED);
                }
                
                if (defenders[i].ready_to_act(current_time)) {

                    defenders[i].move();

                }

            }

            // move the net
            if (net.ready_to_act(current_time)) {
               
                net.move();

            }

            if (ball.has_hit_net(net)) {

                net.goal();

                ball.reset();

            }

            // reset color when the soccer ball hits the bound of the net
            if ((ball.get_x() == net.get_x() && ball.get_y() == net.get_y())|| 
                (ball.get_x() == net.get_x() && ball.get_y() == net.get_y() + 1) ||
                (ball.get_x() == net.get_x() + net.get_width() + 1 && ball.get_y() == net.get_y()) ||
                (ball.get_x() == net.get_x() + net.get_width() + 1 && ball.get_y() == net.get_y() + 1) ){
                  ball.reset_with_color(WHITE);
                }

            // enable defender's shooting behavior
            int num_cannon_to_shoot = 2;

            int attackers[NUM_CANNONS] = {random(0,4), random(0,4)};

            // when two attackers are the same -> enable one
            if (attackers[0] == attackers[1]) {

                num_cannon_to_shoot = 1;

            }

            for (int i = 0; i < NUM_CANNONS; i ++) {

                // see if which cannon can be shot
                if (!cannons[i].has_been_shot() && defenders[attackers[i]].ready_to_shoot(current_time) 
                     && defenders[attackers[i]].able_to_shoot() && num_cannon_to_shoot > 0) {
                    
                    defenders[attackers[i]].shoot(cannons[i]);
                    num_cannon_to_shoot --;

                }

                // when ball is fired
                if (cannons[i].has_been_shot()) {

                    // move the ball when necessary
                    if (cannons[i].ready_to_act(current_time)) {
  
                        cannons[i].move();

                    }

                    // detected collisions
                    if (cannons[i].has_hit_player(player)) {

                        player.die();

                    }

                    if (cannons[i].has_hit_soccer(ball)) {

                        cannons[i].reset();
                        
                        ball.reset();

                    }

                }

                

            }



        } else {

            next_level();

        }

    }

    bool has_been_selected () const {
        return selected;
    }

    void select() {

        selected = true;
      
    }
    
    private:

        int level = 0;

    Printer print;
    Player player;
    SoccerBall ball;
    Defender defenders[NUM_DEFENDERS];
    Net net;
    CannonBall cannons[NUM_CANNONS];
    int time;
    int prev_potentiometer_value = 512;
    bool selected = false;
    
    void initialize_time_counter() {
        int time = millis();
        // for player
        player.set_initial_action_time(time);
        // for soccer ball
        ball.set_initial_action_time(time);
        
        // for all defenders
        for (int i = 0; i < NUM_DEFENDERS; i++) {

            defenders[i].set_initial_action_time(time);

        }
        // for all cannons
        for (int i = 0; i < NUM_CANNONS; i++) {

            cannons[i].set_initial_action_time(time);

        }

    }

    void initialize_move_status() {

        // TODO:
        // change move status and speed
        // of objects based on level
        switch (level) {
        case 1:

            

                    
                    player.allow_unlimited_shots(true);
                    player.set_num_shots(5);
                    ball.set_can_move(true);
                    ball.set_speed(6);

                    net.set_can_move(false);
                    net.allow_net_to_expand(false);

                    for (int i = 0; i < NUM_DEFENDERS; ++i) {
                        defenders[i].set_can_move(true);
                        defenders[i].set_speed(3);
                        defenders[i].allow_shooting(false);
                    }
                    break;

        case 2:

            player.allow_unlimited_shots(false);
            player.set_num_shots(5);
            // set the speed of soccer
            ball.set_speed(6);
            // move soccer
            ball.set_can_move(true);
            net.set_can_move(true);
            // allow net to expand
            net.allow_net_to_expand(true);
            net.set_speed(3);
            
            for (int i = 0; i < NUM_DEFENDERS; ++i) {

                defenders[i].set_can_move(true);
                // set speed for defenders
                defenders[i].set_speed(1);
                defenders[i].allow_shooting(false);

            }
            for (int i = 0; i < NUM_CANNONS; ++i ){
              // set cannon speed
              cannons[i].set_speed(1);
            }
            break;

        case 3:

            player.allow_unlimited_shots(false);
            player.set_num_shots(5);
            // set the speed of soccer
            ball.set_speed(6);
            // move soccer
            ball.set_can_move(true);
            // set the speed of the net
            net.set_speed(2);
            // move the net
            net.set_can_move(true);
            // allow net to expand
            net.allow_net_to_expand(true);
            

            for (int i = 0; i < NUM_DEFENDERS; ++i) {

                defenders[i].set_can_move(true);
                // set speed for defenders
                defenders[i].set_speed(1);

            }
            break;

        case 4:

            player.allow_unlimited_shots(false);
            player.set_num_shots(5);
            // set the speed of soccer
            ball.set_speed(6);
            // move soccer
            ball.set_can_move(true);
            // set the speed of the net
            net.set_speed(4);
            // move the net
            net.set_can_move(true);
            // allow net to expand
            net.allow_net_to_expand(true);
            
            for (int i = 0; i < NUM_DEFENDERS; ++i) {

                defenders[i].set_can_move(true);
                // set speed for defenders
                defenders[i].set_speed(1);

            }
            break;

        case 5:

            player.allow_unlimited_shots(false);
            player.set_num_shots(5);
            // set the speed of soccer
            ball.set_speed(6);
            // move soccer
            ball.set_can_move(true);
            // set the speed of the net
            net.set_speed(4);
            // move the net
            net.set_can_move(true);
            // allow net to expand
            net.allow_net_to_expand(true);
            

            for (int i = 0; i < NUM_DEFENDERS; ++i) {

                defenders[i].set_can_move(true);
                // set speed for defenders
                defenders[i].set_speed(2);

            }
            break;

        case 6:

            player.allow_unlimited_shots(true);
            player.set_num_shots(3);
            // set the speed of soccer
            ball.set_speed(16);
            // move soccer
            ball.set_can_move(true);
            // set the speed of the net
            net.set_speed(4);
            // move the net
            net.set_can_move(true);
            // allow net to expand
            net.allow_net_to_expand(true);
           

            for (int i = 0; i < NUM_DEFENDERS; ++i) {

                defenders[i].set_can_move(true);
                // set speed for defenders
                defenders[i].set_speed(2);

            }
            break;

        default:
            matrix.fillScreen(BLACK.to_333());
            print.go_blue();
            delay(30000);
        }
    }

    int parse_potentiometer_value(int value) {

        int val = value / 32;

        if (val <= 0) {
            return 0;
        } else if (val >= 29) {
            return 29;
        }

        return val;
    }

    void draw_net() {

        // TODO:
        net.draw();

    }

    void draw_defenders() {

        // TODO:
        // for all levels, defenders act similarly

        for (int i = 0; i < NUM_DEFENDERS; ++i) {
            // erase existing defenders
            defenders[i].erase();
            // draw defenders
            defenders[i].initialize(8 * i, 3);
            defenders[i].set_index(i);
            defenders[i].draw();

        }

    }

    void draw_player() {

            // TODO
            player.erase();
            player.set_x(14);
            player.draw();

        }
        // I think level_cleared should be a boolean
    bool level_cleared() {

        // TODO:
        // check if the net has been goaled
        return net.has_been_goaled();
    }

    void reset_level() {

        // TODO:
        // assume there is getter in player class
        initialize_time_counter();
        initialize_move_status();

        if (level > 6) {
          return;
        }
        
        // clear board
        matrix.fillScreen(BLACK.to_333());
        // print messages
        print.level(level);
        delay(1500);

        // fill the screen with black
        matrix.fillScreen(BLACK.to_333());

        // draw player
        draw_player();
        // draw defenders
        draw_defenders();
        draw_net();
        net.reset();
    }

    void next_level() {

        // TODO:
        // increment level
        level++;
        // clear the board
        matrix.fillScreen(BLACK.to_333());
        // reset level to the next level
        reset_level();

    }

    

    void game_over() {

        // TODO:
        // clear the board
        matrix.fillScreen(BLACK.to_333());
        // print the message
        print.game_over();
        delay(20000);

    }

};

// Multiple player Game [ADDED IN SUPER REACH]
class MultiGame {

    public: 

        MultiGame() {

        }

        void setup() {

            // initialization
            initialize_time_counter();

            initialize_object_configuration();

            // send data over the net
            cmd.start_multi_game();

            // draw
            draw_everything();

        }

        void loop(int potentiometer_value, bool player_button_pressed, bool finish_pressed) {

            // check if server is ready
            /*
            if (!server_ready()) {

                matrix.fillScreen(BLACK.to_333());

                print.waiting();

                return;
            }*/

            // when server is ready ...
            int current_time = millis();
                
            // enable game finish
            if (finish_pressed) {
              
                // finish the game
                finished = true;

            }

            if (finished) {

                // print final score
                if (last_score_time == 0) {

                    print.scores(player_score, defender_score);
                  
                }
                
                if (current_time - last_score_time > 1500) {

                    // print scores
                    matrix.fillScreen(BLACK.to_333());

                    print.scores(player_score, defender_score);

                    last_score_time = current_time;
                }
                
                // send finish info over the wires
                cmd.finish_multi_game();

                return;
              
            }
            
            // enable player control (on board)
            if (abs(potentiometer_value - prev_potentiometer_value) > 30) {

                draw_player(potentiometer_value / 32);

                prev_potentiometer_value = potentiometer_value;

            }

            // enable player shooting
            if (!soccer.has_been_shot() && player_button_pressed) {

                player.shoot(soccer);

                soccer.set_initial_action_time(current_time);

                last_soccer_time = current_time;

             }
              
            

            // enable defender control (remote)
            cmd.interact_with(defender, cannon);

            // detect movements
            // --- soccer
            if (soccer.ready_to_act(current_time)) {

                soccer.move();

                player.draw();

            }

            // --- cannon
            if (cannon.ready_to_act(current_time)) {

                cannon.move();

            }

            // --- net
            if (net.ready_to_act(current_time)) {

                net.move();
              
            }

            // detect collision
            // --- soccer goals net
            if (soccer.has_hit_net(net)) {

                // reset soccerball
                soccer.reset();

                // indicate player scored
                player_score ++;

                // send score info over the wires
                cmd.score("player");

            }

            // --- soccer hits cannon
            if (cannon.has_hit_soccer(soccer)) {

                // reset cannon ball
                cannon.reset();

                // reset soccer ball
                soccer.reset();

            }

            // --- cannon hits player
            if (cannon.has_hit_player(player)) {

                // reset cannon ball
                cannon.reset();

                // indicate defender goaled
                defender_score ++;

                // send score info over the wires
                cmd.score("defender");

            }

        }

        bool has_been_selected () const {
            return selected;
        }

        void select() {

            selected = true;
      
        }
    private:
        // helpers
        Commander cmd;
        Printer print;
        bool selected = false;
        bool finished = false;
        
        // game objects
        CannonBall cannon;
        SoccerBall soccer;
        Net net;
        Defender defender;
        Player player;

        // score
        int defender_score = 0;
        int player_score = 0;

        // timers / pace trackers
        int last_score_time = 0;
        int last_soccer_time = 0;
        int last_button_pressed = 0;
        int prev_potentiometer_value = 512; // middle of screen

        // initialization functions
        void initialize_time_counter() {

            int time = millis();
          
            player.set_initial_action_time(time);

            soccer.set_initial_action_time(time);
            
            defender.set_initial_action_time(time);

            cannon.set_initial_action_time(time);

        }

        void initialize_object_configuration() {
            // config player
            player.allow_unlimited_shots(true);
            
            // config cannon
            cannon.set_speed(6);

            // config soccer
            soccer.set_speed(6);

            // config net
            net.set_can_move(true);
            net.set_speed(4);
            net.allow_net_to_expand(true);

            // config defender
            defender.allow_shooting(true);

        }

        

        // drawing functions
        void draw_net() {

            net.erase();
            net.set_x(14);
            net.draw();

        }

        void draw_defender() {

            defender.erase();
            defender.set_x(14);
            defender.draw();

        }

        void draw_player(int x = 16) {

            player.erase();
            if (x >= 29) {
              x = 29;
            }
            player.set_x(x);
            player.draw();

        }

        void draw_everything() {

            draw_net();

            draw_defender();

        }

        // helper functions
        bool server_ready() {

            if (Serial.available() > 0) {

                return Serial.readStringUntil("\n") == "READY";

            }

            return false;

        }

        int parse_potentiometer_value(int value) {

            int val = value / 32;

            if (val <= 0) {

                return 0;

            } 

            if (val >= 29) {

                return 29;

            }

            return val;
        }

        void reset() {

            draw_everything();

        }


};

// Mode selection [ADDED IN SUPER REACH]
class ModeSelector {

    public: 

        void show_options() {

            if (millis() - last_refreshed_option > 1000 ) {

                matrix.fillScreen(BLACK.to_333());
                
                print.mode_options();

                last_refreshed_option = millis();
              
            }
            
        }

        int choose(int potentiometer_value, bool button_pressed) {

            int current_time = millis();
            
            // detect potentiometer value change
            int option = potentiometer_value / 512 + 1;

            if (current_time - last_refreshed_selection > 500 ) {

                print.deselected();

                print.selected(option);   

                last_refreshed_selection = millis();
            }

            // detected button pressed action
            if ((current_time - time > 500) && button_pressed) {
                
                mode = option;

                time = millis();

            }

            return mode;

        }

        int selected() {

            return mode;

        }

    private:
        // helper class
        Printer print;

        // timer
        int time = 0;
        int last_refreshed_option = 0;
        int last_refreshed_selection = 0;
        // selected mode
        int mode = 0;

};

// Initialize Single Game instance
SingleGame single_game;

// Initialize Multi Game instance
MultiGame multi_game;

// Initialize Mode selector
ModeSelector select;

void setup() {

    Serial.begin(9600);
    pinMode(BUTTON_PIN_NUMBER, INPUT);
    matrix.begin();
}

void loop() {

    // prepare inputs
    int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);

    bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);

    bool finish_pressed = (digitalRead(FINISH_PIN_NUMBER) == HIGH);

    // check game option
    int option = select.selected();

    if (option == 0){

        // load selection screen
        select.show_options();
        
        option = select.choose(potentiometer_value, button_pressed);

    }
    
    switch (option) {

        // single mode selected
        case 1:
            if (!single_game.has_been_selected()) {
              matrix.fillScreen(BLACK.to_333());
              single_game.select();
              single_game.setup();
              button_pressed = false;
            }
            single_game.loop(potentiometer_value, button_pressed);
        break;

        // multi mode selected
        case 2:
            if (!multi_game.has_been_selected()) {
              matrix.fillScreen(BLACK.to_333());
              multi_game.select();
              multi_game.setup();
              button_pressed = false;
            }
            multi_game.loop(potentiometer_value, button_pressed, finish_pressed);
        break;

        default:
        break;
    }

    
}

