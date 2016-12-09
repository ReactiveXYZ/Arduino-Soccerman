#include < gamma.h > 
#include < RGBmatrixPanel.h > 
#include < Adafruit_GFX.h > 
#include < EEPROM.h >

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

// define the number of defenders in a game
const int NUM_DEFENDERS = 4;

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

};

// Abstract class for objects that are moveable
class Moveable {

    public:

        explicit Moveable(bool move): able_to_move(move),
        last_action_time(0) {};

    void set_can_move(bool move) {
        this - > able_to_move = move;
    }

    bool can_move() {
        return this - > able_to_move;
    }

    void set_speed(int speed) {
        if (speed_unit == 0) {
            this - > speed_unit = speed;
        }

        this - > cool_down = 1000 / speed;
    }

    int get_speed() {
        return this - > speed_unit;
    }

    void set_initial_action_time(int time) {
        this - > last_action_time = time;
    }

    bool ready_to_act(int currentTime) const {
        return currentTime - last_action_time > this - > cool_down;
    }

    void timestamp() {
        this - > last_action_time = millis();
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
        this - > x = x;
    }

    int get_x() const {
        return this - > x;
    }

    void set_y(int y) {
        this - > y = y;
    }

    int get_y() const {
        return this - > y;
    }

    void initialize(int x, int y) {
        this - > x = x;
        this - > y = y;
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

// Defender
class Defender: public Moveable, public Drawable {

    public:

        Defender(bool move = false): Moveable(move),
    Drawable() {
        // set coordinates of the most left pixel of the current defender
        initialize(4, 3);
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

    private: bool move_left = false;
    int index;

};

// Net
class Net: public Moveable, public Drawable {

    public:

        Net(bool move = false): Moveable(move),
    Drawable() {
        // set coordinates of the upper left pixel of the net
        initialize(11, 0);
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

    void move() {
        // record timestamp
        Moveable::timestamp();

        erase();

        if (can_move()) {
            if ((x >= 0) && (x <= 25)) {
                if (move_right) {
                    x++;
                    if (x == 25) {
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

    void draw() {
        // (x, y) represents the upper left pixel of the net
        for (int i = 0; i <= 6; ++i) {
            draw_with_color(x + i, y, WHITE);
        }

        draw_with_color(x, y + 1, WHITE);
        draw_with_color(x + 6, y + 1, WHITE);
    }

    void erase() {
        // (x, y) represents the upper left pixel of the net
        for (int i = 0; i <= 6; ++i) {
            draw_with_color(x + i, y, BLACK);
        }

        draw_with_color(x, y + 1, BLACK);
        draw_with_color(x + 6, y + 1, BLACK);
    }

    private:

        bool goaled = false;
    bool move_right = true;

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

    bool has_been_shot() const {
        return shot;
    }

    void shoot(int x_arg, int y_arg) {
        shot = true;
        x = x_arg;
        y = y_arg;
    }

    bool has_hit_defender(Defender & defender) {
        if ((y == 3) && ((x >= defender.get_x() && x <= defender.get_x() + 3) ||
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
        if ((y == 1) && (x >= net.get_x() + 1) && (x <= net.get_x() + 5)) {
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
            } else {
                shot = false;
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
        initialize(14, 14);
    }

    void reset(int xval) {
        erase();
        x = xval;
        draw();
    }

    void allow_unlimited_shots(bool allow) {
        unlimited_shots = allow;
    }

    void shoot(SoccerBall & ball) {
        if (!ball.has_been_shot()) {
            ball.shoot(x + 1, y + 2 - num_shots);
            if (!unlimited_shots) {
                num_shots--;
                Serial.println(num_shots);
                draw();
            }
        }

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
    int num_shots = 5;

};

// Game
class Game {
    public:

        Game() {
            // initialize level
            level = 7;
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
        // set current time
        int current_time = millis();
        // check whether level is cleared
        if (!level_cleared()) {

            // detect the movement of player
            player.reset(parse_potentiometer_value(potentiometer_value));
            // check if button is pressed
            if (button_pressed && (current_time - time > 500) && !ball.has_been_shot()) {
                // erase the previous ball
                ball.reset();
                // shoot the ball after press the button
                player.shoot(ball);
                Serial.println(player.get_num_shots());
                time = millis();
            }
            //  move soccer ball
            if (ball.ready_to_act(current_time)) {

                if (ball.has_been_shot()) {
                    // set variable speed
                    ball.set_speed((player.get_y() - ball.get_y()) * 0.5 * ball.get_speed());
                    // move ball                  
                    ball.move();

                } else {

                    ball.reset();

                }
            }

            // move defenders

            for (int i = 0; i < NUM_DEFENDERS; ++i) {

                if (defenders[i].ready_to_act(current_time)) {

                    defenders[i].move();

                }

                if (ball.has_hit_defender(defenders[i])) {

                    ball.reset();

                    defenders[i].draw();

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

        } else {

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
    int time;

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

    }

    void initialize_move_status() {

        // TODO:
        // change move status and speed
        // of objects based on level
        switch (level) {
        case 1:

            // double check if the speed in proper when get the board
            // allow player to ave unlimited shots
            player.allow_unlimited_shots(true);
            player.set_num_shots(5);
            // move soccer
            ball.set_can_move(true);
            // set the speed of soccer
            ball.set_speed(6);
            net.set_can_move(false);
            // move defenders
            for (int i = 0; i < NUM_DEFENDERS; ++i) {

                defenders[i].set_can_move(true);
                // set speed for defenders
                defenders[i].set_speed(1);

            }
            break;

        case 2:

            player.allow_unlimited_shots(false);
            player.set_num_shots(5);
            // set the speed of soccer
            ball.set_speed(6);
            // move soccer
            ball.set_can_move(true);
            net.set_can_move(false);
            for (int i = 0; i < NUM_DEFENDERS; ++i) {

                defenders[i].set_can_move(true);
                // set speed for defenders
                defenders[i].set_speed(1);

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

            for (int i = 0; i < NUM_DEFENDERS; ++i) {

                defenders[i].set_can_move(true);
                // set speed for defenders
                defenders[i].set_speed(2);

            }
            break;

        case 6:

            player.allow_unlimited_shots(false);
            player.set_num_shots(3);
            // set the speed of soccer
            ball.set_speed(6);
            // move soccer
            ball.set_can_move(true);
            // set the speed of the net
            net.set_speed(4);
            // move the net
            net.set_can_move(true);

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

        // clear board
        matrix.fillScreen(BLACK.to_333());
        // print messages
        print.level(level);
        delay(1500);

        // fill the screen with black
        matrix.fillScreen(BLACK.to_333());

        initialize_time_counter();
        initialize_move_status();

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

// Initialize Game instance
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
