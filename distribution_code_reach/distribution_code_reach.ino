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

		void level() {
			// TODO
		}

		void remaining_shots() {
			// TODO
		}

		void game_over() {
			// TODO
		}

};


// Abstract class for objects that are moveable
class Moveable {

	public:

		explicit Moveable(bool move):can_move(move), 
									last_action_time(0){};

		void set_can_move(bool move) {
			this->can_move = move;
		}

		void can_move() {
			return this->can_move;
		}

		void set_speed(int speed) {
			this->cool_down = 1000 / speed;
		}

		void set_initial_action_time(int time) {
			this->last_action_time = time;
		}

		bool ready_to_act() const {
			return millis() - last_action_time > this->cool_down;
		}

		void timestamp() {
			this->last_action_time = millis();
		}

		virtual void move() = 0;

	private:

		bool can_move;
		int last_action_time;
		int cool_down; 
};

// Abstract class for objects that are drawable
class Drawable {

	public:

		explicit Drawable(int origin_x = 0, 
						  int origin_y = 0):x(origin_x)
										   ,y(origin_y){};

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

		void draw_with_color(int x, int y, Color color) {

			matrix.drawPixel(x, y, color.to_333());

		}

	private:

		int x;
		int y;

};

// Defender
class Defender: public Moveable, public Drawable {

	public:

		Defender(bool move = false): Moveable(move), Drawable() {}

		void move() {
			// record timestamp
			Moveable::timestamp();
			// TODO: 
		}

		void draw() {
			// TODO:
		}

		void erase() {
			// TODO: 
		}

};

// Net
class Net : public Moveable, public Drawable {

	public: 

		Net(bool move = false): Moveable(move), Drawable(move) {}


		void goal() {

			// TODO:
			// make goaled = true
		}

		void has_been_goaled() const {

			// TODO:

		}

		void move() {
			// record timestamp
			Moveable::timestamp();
			// TODO: 
		}

		void draw() {
			// TODO:
		}

		void erase() {
			// TODO:
		}

		private:

			bool goaled = false;
		
};

// Soccer ball
class SoccerBall : public Moveable, public Drawable {

	public:

		SoccerBall(bool move = true): Moveable(move), Drawable() {}

		bool has_been_shot() const {
			// TODO:
		}

		void shoot(int x_arg, int y_arg) {
			// TODO:
		}

		bool has_hit_defender(Defender& defender) {
			// TODO:
		}

		bool has_hit_net(Net& net){
			// TODO:
		}

		void move() {
			// record timestamp
			Moveable::timestamp();
			// TODO:
		}

		void erase() {
			// TODO:
		}

		void draw() {
			// TODO:
		}

	private:

		bool shot;

};

// Player
class Player : public Moveable, public Drawable {

	public:

		Player(bool move = true): Moveable(move), Drawable() {}

		void allow_unlimited_shots() {
			// TODO:
		}

		void fail_to_goal(){
			// TODO
		}

		bool has_collided_with(Defender& defender) {
			// TODO:
		}

		void move() {
			// record timestamp
			Moveable::timestamp();
			// TODO:
		}

		void erase() {
			// TODO:
		}

		void draw() {
			// TODO:
		}

	private:

		bool unlimited_shots = false;
		int num_shots = 3;

};

// Game
class Game {

	public:

		Game() {
			// initialize level
			level = 0;
		}

		void setup() {
			// initialize timestamp for all objects
			this->initialize_time_counter();
			// initialize move status for all objects
			this->initialize_move_status();

			// TODO:
			// initialize matrix etc...
		}

		void loop(int potentiometer_value, bool button_pressed) {

			// TODO:

		}

	private:

		int level = 0;

		Printer print;
		Player player;
		SoccerBall ball;
		Defender defenders[NUM_DEFENDERS];

		void initialize_time_counter() {
			int time = millis();
			// for player
			player.set_initial_action_time(time);
			// for soccer ball
			ball.set_initial_action_time(time);
			// for all defenders
			for (int i = 0; i < NUM_DEFENDERS; i++){
			    defenders[i].set_initial_action_time(time);
			}

		}

		void initialize_move_status() {

			// TODO:
			// change move status and speed
			// of objects based on level

		}

		void draw_net() {

			// TODO:

		}

		void draw_defenders() {

			// TODO:

		}

		void draw_player() {

			// TODO:

		}

		void level_cleared () {
			
			// TODO:
			// check if the net has been goaled

		}

		void reset_level() {

			// TODO:

		}

		void next_level() {

			// TODO:

		}

		void game_over() {

			// TODO:

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
