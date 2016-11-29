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

// Abstract class for objects that are moveable
class Moveable {
	public:
		explicit Moveable(bool move):can_move(move), 
									last_action_time(0){};
		void set_can_move(bool move) {
			this->can_move = move;
		}

		void set_speed(int speed) {
			this->elapsed = 1000 / speed;
		}

		bool ready_to_act() {
			return millis() - last_action_time > this->elapsed;
		}

		virtual void _move() {
			this->last_action_time = millis();
		}
	private:
		bool can_move;
		int last_action_time;
		int elapsed; // moves / sec
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
		int get_x() {
			return this->x;
		}
		void set_y(int y) {
			this->y = y;
		}
		int get_y() {
			return this->y;
		}
		virtual void draw() = 0;
		virtual void erase() = 0;
	private:
		int x;
		int y;
};



// Arduino's native setup function
// runs only once
void setup() {

}

// Arduino's native loop function
// runs millions of times per second
void loop() {

}
