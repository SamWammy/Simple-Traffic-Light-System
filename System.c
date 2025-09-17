// One-Way Street Traffic Light System with Pedestrian Crossing
// For Wokwi Arduino Simulator

// Pin definitions for traffic lights
const int RED_PIN = 13;
const int YELLOW_PIN = 12;
const int GREEN_PIN = 11;
const int BUTTON = 2;

const int PEDESTRIAN_RED = 4;
const int PEDESTRIAN_GREEN = 5;

// Timing constants (in milliseconds)
const int GREEN_TIME = 5000;    // 5 seconds green
const int YELLOW_TIME = 2000;   // 2 seconds yellow
const int RED_TIME = 5000;      // 5 seconds red
const int DELAY = 2000;         // 2 second delay 
const int indicator = 500;    // blink interval (500ms)

// State definitions
enum TrafficLightState {
	GREEN,
	YELLOW,
	RED,
	Delay,
	Crossing,
	/*Add another state(s) to handle pedestrian crossing
	*/
};

TrafficLightState currentState = RED;
unsigned long previousMillis = 0;
unsigned long blinkMillis = 0;

/*
  Define a boolean variable to track if a pedestrian has requested to cross
*/
bool pedestrianRequest = false;

void setup() {
	// Initialize pins as outputs
	pinMode(RED_PIN, OUTPUT);
	pinMode(YELLOW_PIN, OUTPUT);
	pinMode(GREEN_PIN, OUTPUT);

	pinMode(PEDESTRIAN_RED, OUTPUT);
	pinMode(PEDESTRIAN_GREEN, OUTPUT);
	pinMode(BUTTON, INPUT_PULLUP); // use pullup so button reads LOW when pressed

	// Initialize serial communication for monitoring
	Serial.begin(9600);
	Serial.println("Traffic Light System with Pedestrian Crossing Started");

	// Set initial state
	setLights(RED);

	// Initialize pedestrian lights - red on, green off
	digitalWrite(RED_PIN, HIGH);
	digitalWrite(PEDESTRIAN_RED, HIGH);
	/*
	  Make the red light on for the pedestrian
	*/
}

void loop() {
	unsigned long currentMillis = millis();

	/*Check for pedestrian button press
	  Check if the button is pressed (LOW) and if no previous request is pending
	  If pressed, set the pedestrian request variable to true and print a message
	  Add a small delay for debouncing. Use delay(200);
	  Your Code here
	*/
	
  if (digitalRead(BUTTON) == LOW && !pedestrianRequest) {
		pedestrianRequest = true;
		Serial.println("Pedestrian button pressed");
		delay(200);
	}

	switch (currentState) {

	case GREEN:
		if (currentMillis - previousMillis >= GREEN_TIME) {
			previousMillis = currentMillis;
			setLights(YELLOW);
			Serial.println("Switching to YELLOW");
		}
		break;

	case YELLOW:
		if (currentMillis - previousMillis >= YELLOW_TIME) {
			previousMillis = currentMillis;
			setLights(RED);
			Serial.println("Switching to RED");
		}
		break;

	case RED:
		/*
		  Check if a pedestrian has requested to cross
		  If so, transition to the pedestrian crossing state
		  Your Code here
		*/
		if (pedestrianRequest) {
			previousMillis = currentMillis;
			setLights(Delay);
			Serial.println("button was pressed, adding delay");
		} else if (currentMillis - previousMillis >= RED_TIME) {
			//No pedestrian request. The normal case
			previousMillis = currentMillis;
			setLights(GREEN);
			Serial.println("Switching to GREEN");
		}
		break;

	case Delay:
		if (currentMillis - previousMillis >= DELAY) {
			previousMillis = currentMillis;
			setLights(Crossing);
			Serial.println("delay is over");
		}
		break;

	case Crossing:
		if (currentMillis - previousMillis >= GREEN_TIME) {
			previousMillis = currentMillis;
			setLights(GREEN);
			pedestrianRequest = false; // reset request
			Serial.println("pedestrian crossing over, switching to green");
			break;
		}

		// blink for the last 2 seconds
		if (GREEN_TIME - (currentMillis - previousMillis) <= 2000) {
			if (currentMillis - blinkMillis >= indicator) {
				blinkMillis = currentMillis;
				int state = digitalRead(PEDESTRIAN_GREEN);
				digitalWrite(PEDESTRIAN_GREEN, !state); 
			}
		}
		break;
	}
}

void setLights(TrafficLightState newState) {
	// First turn all traffic lights off
	digitalWrite(RED_PIN, LOW);
	digitalWrite(YELLOW_PIN, LOW);
	digitalWrite(GREEN_PIN, LOW);
	digitalWrite(PEDESTRIAN_GREEN, LOW);
	digitalWrite(PEDESTRIAN_RED, HIGH);

	switch (newState) {
	case GREEN:
		digitalWrite(GREEN_PIN, HIGH);
		break;
	case YELLOW:
		digitalWrite(YELLOW_PIN, HIGH);
		break;
	case RED:
		digitalWrite(RED_PIN, HIGH);
		break;
	case Crossing:
		// add delay for crossing 
		digitalWrite(PEDESTRIAN_GREEN, HIGH);
		digitalWrite(PEDESTRIAN_RED, LOW);
		break;
	case Delay:
		digitalWrite(RED_PIN, HIGH);
		break;
	/*
	  add the case for pedestrian crossing
	*/
	}

	currentState = newState;
}

    
  }
  
  currentState = newState;
}
