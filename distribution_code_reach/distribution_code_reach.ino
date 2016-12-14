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
const int RESTART_PIN_NUMBER = 11;

// define the number of defenders 
const int NUM_DEFENDERS = 4;

// define the number of cannons that defenders can shoot in total
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

        void greeting() {
            matrix.drawCircle(6, 7, 5, WHITE.to_333());
            matrix.fillCircle(6, 7, 5, WHITE.to_333());

            matrix.drawRect(5, 2, 1, 1, BLUE.to_333());
            matrix.drawRect(6, 2, 1, 1, BLUE.to_333());
            matrix.drawRect(7, 3, 1, 1, BLUE.to_333());
            matrix.drawRect(6, 3, 1, 1, BLUE.to_333());
            matrix.drawRect(4, 3, 1, 1, BLUE.to_333());
            matrix.drawRect(5, 3, 1, 1, BLUE.to_333());
            matrix.drawRect(5, 4, 1, 1, BLUE.to_333());
            matrix.drawRect(6, 4, 1, 1, BLUE.to_333());
   
            matrix.drawRect(5, 10, 1, 1, BLUE.to_333());
            matrix.drawRect(6, 10, 1, 1, BLUE.to_333());
            matrix.drawRect(5, 11, 1, 1, BLUE.to_333());
            matrix.drawRect(6, 11, 1, 1, BLUE.to_333());
            matrix.drawRect(4, 11, 1, 1, BLUE.to_333());
            matrix.drawRect(7, 11, 1, 1, BLUE.to_333());
            matrix.drawRect(5, 12, 1, 1, BLUE.to_333());
            matrix.drawRect(6, 12, 1, 1, BLUE.to_333());
   
            matrix.drawRect(2, 6, 1, 1, BLUE.to_333());
            matrix.drawRect(3, 6, 1, 1, BLUE.to_333());
            matrix.drawRect(2, 8, 1, 1, BLUE.to_333());
            matrix.drawRect(3, 8, 1, 1, BLUE.to_333());
            matrix.drawRect(1, 7, 1, 1, BLUE.to_333());
            matrix.drawRect(2, 7, 1, 1, BLUE.to_333());
            matrix.drawRect(3, 7, 1, 1, BLUE.to_333());
            matrix.drawRect(4, 7, 1, 1, BLUE.to_333());

            matrix.drawRect(9, 6, 1, 1, BLUE.to_333());
            matrix.drawRect(10, 6, 1, 1, BLUE.to_333());
            matrix.drawRect(9, 8, 1, 1, BLUE.to_333());
            matrix.drawRect(10, 8, 1, 1, BLUE.to_333());
            matrix.drawRect(8, 7, 1, 1, BLUE.to_333());
            matrix.drawRect(9, 7, 1, 1, BLUE.to_333());
            matrix.drawRect(10, 7, 1, 1, BLUE.to_333());
            matrix.drawRect(11, 7, 1, 1, BLUE.to_333());
   
            matrix.setCursor(14, 4);
            matrix.setTextColor(LIME.to_333());
            matrix.print('M');

            matrix.setCursor(20, 4);
            matrix.setTextColor(LIME.to_333());
            matrix.print('A');
      
            matrix.setCursor(26, 5);
            matrix.setTextColor(LIME.to_333());
            matrix.print('N');       
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

        Net(bool move = false): Moveable(move), Drawable() {
            // set initial location of the net
            initialize(11, 0);
            // net size random generator
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
              
                if ((x >= 0) && (x <= 31 - (width + 1))) {
                    if (move_right) {
                        x++;
                       
                        if (x == 31 - (width + 1)) {
                            move_right = false;
                        }
                    } 
                    else {
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
            union {
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

// Soccer ball shot from the player
class SoccerBall: public Moveable, public Drawable {
    public:

        SoccerBall(bool move = true): Moveable(move), Drawable() {
          // set initial location of the being fired soccerball
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
            } 
            else {
                return false;
            }
        }

        bool has_hit_net(Net & net) {
            if ((y == 1) && (x >= net.get_x() + 1) && (x <= net.get_x() + net.get_width())) {
                shot = false;
                return true;
            } 
            else {
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

        Player(bool move = true): Moveable(move), Drawable() {
            // set coordinates of the upper left pixel of the player
            initialize(14, 14);
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

// Cannonball shot from defenders
class CannonBall: public Moveable, public Drawable {
    public:

        CannonBall(bool move = true): Moveable(move), Drawable() {
        // set initial location of the cannonball
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

        Defender(bool move = false): Moveable(move), Drawable() {
            // set initial location of the current defender
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

                if (x >= 8 * index && x <= 8 * index + 4) {
                    if (move_left) {
                        x--;
                        if (x == 8 * index) {
                            move_left = false;
                        }
                    } 
                    else {
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

// Game
class Game {
    public:

        Game() {
            // initialize level
            level = 0;
            // initialize time
            time = 0;
        }

        void setup() {
            time = millis();
            matrix.fillScreen(BLACK.to_333());
            print.greeting();
            delay(5000);
            next_level();
        }

        void loop(int potentiometer_value, bool button_pressed) {
            // game over when there is no remaining shots 
            if (player.get_num_shots() < 1) {
                if (!net.has_been_goaled() && !ball.has_been_shot()) {
                    game_over();
                    return;
                }
            }

            // game over when the cannonball from the defenders collids with the player
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
                    time = millis();
                }
                //  move soccer ball
                if (ball.ready_to_act(current_time)) {

                    if (ball.has_been_shot()) {
                        // set variable speed
                        ball.set_speed((player.get_y() - ball.get_y()) * 0.5 * ball.get_speed());
                        // move ball                  
                        ball.move();
                        // redraw player to deal with soccer erasing
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
                if ((ball.get_x() == net.get_x() && ball.get_y() == net.get_y()) || 
                    (ball.get_x() == net.get_x() && ball.get_y() == net.get_y() + 1) ||
                    (ball.get_x() == net.get_x() + net.get_width() + 1 && ball.get_y() == net.get_y()) ||
                    (ball.get_x() == net.get_x() + net.get_width() + 1 && ball.get_y() == net.get_y() + 1)) {
                      
                    ball.reset_with_color(WHITE);
                }
    
                // enable defender's shooting behavior
                int num_cannon_to_shoot = 2;

                int attackers[NUM_CANNONS] = {random(0,4), random(0,4)};

                // when two attackers are the same -> enable one
                if (attackers[0] == attackers[1]) {
                    num_cannon_to_shoot = 1;
                }

                for (int i = 0; i < NUM_CANNONS; i++) {
                    // see if which cannon can be shot
                    if (!cannons[i].has_been_shot() && 
                        defenders[attackers[i]].ready_to_shoot(current_time) &&
                        defenders[attackers[i]].able_to_shoot() && num_cannon_to_shoot > 0) {
                        
                        defenders[attackers[i]].shoot(cannons[i]);
                        num_cannon_to_shoot--;
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



                } 
            else {
                next_level();
            }
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
            // set different rules for different levels
            switch (level) {
                case 1:

                    player.allow_unlimited_shots(true);

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

                    ball.set_can_move(true);
                    ball.set_speed(6);

                    net.set_can_move(true);
                    net.set_speed(3);
                    net.allow_net_to_expand(false);
                    
                    for (int i = 0; i < NUM_DEFENDERS; ++i) {
                        defenders[i].set_can_move(true);
                        defenders[i].set_speed(2);
                        defenders[i].allow_shooting(false);
                    }
                    break;

                case 3:

                    player.allow_unlimited_shots(false);
                    player.set_num_shots(5);

                    ball.set_can_move(true);
                    ball.set_speed(6);

                    net.set_can_move(true);
                    net.set_speed(3);
                    net.allow_net_to_expand(true);
                    
                    for (int i = 0; i < NUM_DEFENDERS; ++i) {
                        defenders[i].set_can_move(true);
                        defenders[i].set_speed(3);
                        defenders[i].allow_shooting(false);
                    }
                    break;

                case 4:

                    player.allow_unlimited_shots(false);
                    player.set_num_shots(5);

                    ball.set_can_move(true);
                    ball.set_speed(6);

                    net.set_can_move(true);
                    net.set_speed(3);
                    net.allow_net_to_expand(true);
                    
                    for (int i = 0; i < NUM_CANNONS; ++i) {
                        cannons[i].set_speed(1);                     
                    }

                    for (int i = 0; i < NUM_DEFENDERS; ++i) {
                        defenders[i].set_can_move(true);
                        defenders[i].set_speed(1);
                        defenders[i].allow_shooting(true);
                    }

                    Defender::set_shooting_frequency(0.1);
                    break;

                case 5:

                    player.allow_unlimited_shots(false);
                    player.set_num_shots(5);
                    
                    ball.set_can_move(true);
                    ball.set_speed(6);

                    net.set_can_move(true);
                    net.set_speed(3);
                    net.allow_net_to_expand(true);
                    
                    for (int i = 0; i < NUM_CANNONS; ++i) {
                        cannons[i].set_speed(2);                     
                    }

                    for (int i = 0; i < NUM_DEFENDERS; ++i) {
                        defenders[i].set_can_move(true);
                        defenders[i].set_speed(2);
                        defenders[i].allow_shooting(true);
                    }

                    Defender::set_shooting_frequency(0.1);
                    break;

                case 6:
                    player.allow_unlimited_shots(false);
                    player.set_num_shots(3);
                    
                    ball.set_can_move(true);
                    ball.set_speed(6);

                    net.set_can_move(true);
                    net.set_speed(4);
                    net.allow_net_to_expand(true);
                    
                    for (int i = 0; i < NUM_CANNONS; ++i) {
                        cannons[i].set_speed(3);                     
                    }

                    for (int i = 0; i < NUM_DEFENDERS; ++i) {
                        defenders[i].set_can_move(true);
                        defenders[i].set_speed(3);
                        defenders[i].allow_shooting(true);
                    }

                    Defender::set_shooting_frequency(0.1);
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
            } 
            else if (val >= 29) {
                return 29;
            }
            return val;
        }

        void draw_net() {
            net.draw();
        }

        void draw_defenders() {

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
                player.erase();
                player.set_x(14);
                player.draw();
        }
            
        bool level_cleared() {
            return net.has_been_goaled();
        }

        void reset_level() {
            initialize_time_counter();
            initialize_move_status();

            for (int i = 0; i < NUM_CANNONS; ++i) {
              cannons[i].reset();
            }

            if (level > 6) {
              return;
            }
            
            // clear board
            matrix.fillScreen(BLACK.to_333());
            // print level message
            print.level(level);
            delay(1500);

            // fill the screen with black
            matrix.fillScreen(BLACK.to_333());

            draw_player();
            draw_defenders();
            draw_net();
            net.reset();
        }

        void next_level() {
            level++;
            // clear the board
            matrix.fillScreen(BLACK.to_333());
            // go to the next level
            reset_level();
        }

        void game_over() {
            // clear the board
            matrix.fillScreen(BLACK.to_333());
            // print the message
            print.game_over();
            delay(40000);
        }
};

// Default constructor
Game game;

// Arduino's native setup function
// runs only once
void setup() {

    Serial.begin(9600);
    pinMode(BUTTON_PIN_NUMBER, INPUT);
    matrix.begin();

    game.setup();
}

// Arduino's native loop function
// runs millions of times per second
void loop() {

    int potentiometer_value = analogRead(POTENTIOMETER_PIN_NUMBER);

    bool button_pressed = (digitalRead(BUTTON_PIN_NUMBER) == HIGH);

    game.loop(potentiometer_value, button_pressed);
}