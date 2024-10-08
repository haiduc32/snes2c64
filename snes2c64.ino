// SNES defines
#define CLOCK_PIN 14
#define LATCH_PIN 15
#define DATA_PIN 16

#define CLOCK_PIN2 17
#define LATCH_PIN2 18
#define DATA_PIN2 19

#define SNES_BUTTON_B 0
#define SNES_BUTTON_Y 1
#define SNES_BUTTON_SELECT 2
#define SNES_BUTTON_START 3
#define SNES_BUTTON_UP 4
#define SNES_BUTTON_DOWN 5
#define SNES_BUTTON_LEFT 6
#define SNES_BUTTON_RIGHT 7
#define SNES_BUTTON_A 8
#define SNES_BUTTON_X 9
#define SNES_BUTTON_L 10
#define SNES_BUTTON_R 11
#define SNES_BUTTON_UNDEF_1 12
#define SNES_BUTTON_UNDEF_2 13
#define SNES_BUTTON_UNDEF_3 14
#define SNES_BUTTON_UNDEF_4 15

// C64 defines
#define C64_UP_PIN 12
#define C64_DOWN_PIN 11
#define C64_LEFT_PIN 10
#define C64_RIGHT_PIN 9
#define C64_FIRE_PIN 8 

#define C64_UP_PIN2 7
#define C64_DOWN_PIN2 6
#define C64_LEFT_PIN2 5
#define C64_RIGHT_PIN2 4
#define C64_FIRE_PIN2 3 

// handles buttons for controller 1
#define handleButton(snes_state, c64_pin) \
  if (button_states[snes_state] != button_states_previous[snes_state]) { \
    state_changed = true; \
    if (button_states[snes_state]) { \
      pullDown(c64_pin); \
      pressed = pressed || true; \
    } else { \
      neutral(c64_pin); \
    } \
  }

// handles buttons for controller 2
#define handleButton2(snes_state, c64_pin) \
  if (button_states2[snes_state] != button_states2_previous[snes_state]) { \
    state_changed = true; \
    if (button_states2[snes_state]) { \
      pullDown(c64_pin); \
      pressed = pressed || true; \
    } else { \
      neutral(c64_pin); \
    } \
  }

const uint8_t num_buttons = 16;

void neutral(uint8_t pin) {
  pinMode(pin, INPUT);
}

void pullDown(uint8_t pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

// setup 4 buffers that we'll use for our states
bool buff1[num_buttons];
bool buff2[num_buttons];
bool buff3[num_buttons];
bool buff4[num_buttons];

bool *button_states = buff1;
bool *button_states_previous = buff2;
bool *button_states2 = buff3;
bool *button_states2_previous = buff4;

void setup() {
  //Serial.begin(9600);
  
  // LED for various statuses
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  // SNES pins
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, INPUT);

  pinMode(CLOCK_PIN2, OUTPUT);
  pinMode(LATCH_PIN2, OUTPUT);
  pinMode(DATA_PIN2, INPUT);

  // C64 pins
  neutral(C64_UP_PIN);
  neutral(C64_DOWN_PIN);
  neutral(C64_LEFT_PIN);
  neutral(C64_RIGHT_PIN);
  neutral(C64_FIRE_PIN);

  neutral(C64_UP_PIN2);
  neutral(C64_DOWN_PIN2);
  neutral(C64_LEFT_PIN2);
  neutral(C64_RIGHT_PIN2);
  neutral(C64_FIRE_PIN2);

  digitalWrite(CLOCK_PIN, HIGH);
  digitalWrite(CLOCK_PIN2, HIGH);

  // null the state buffers
  memset(buff1, 0, sizeof(buff1));
  memset(buff2, 0, sizeof(buff2));
  memset(buff3, 0, sizeof(buff3));
  memset(buff4, 0, sizeof(buff4));
}

void updateButtons(bool button_states[num_buttons], bool button_states2[num_buttons]) {
  // use these variables to track if any button is pressed - for testing
  bool state_changed = false;
  bool pressed = false;

  // check that controller is connected
  // both up and down will be high if not connected
  if (!(button_states[SNES_BUTTON_UP] && button_states[SNES_BUTTON_DOWN])) {
      handleButton(SNES_BUTTON_UP, C64_UP_PIN);
      handleButton(SNES_BUTTON_DOWN, C64_DOWN_PIN);
      handleButton(SNES_BUTTON_LEFT, C64_LEFT_PIN);
      handleButton(SNES_BUTTON_RIGHT, C64_RIGHT_PIN);
    // all action buttons treated as fire
    if ((button_states[SNES_BUTTON_A] != button_states_previous[SNES_BUTTON_A]) || 
        (button_states[SNES_BUTTON_B] != button_states_previous[SNES_BUTTON_B]) || 
        (button_states[SNES_BUTTON_X] != button_states_previous[SNES_BUTTON_X]) || 
        (button_states[SNES_BUTTON_Y] != button_states_previous[SNES_BUTTON_Y])) {

      // Serial.print(button_states[SNES_BUTTON_A] ? "true" : "false");Serial.print(":");
      // Serial.println(button_states_previous[SNES_BUTTON_A] ? "true" : "false");

      state_changed = true;

      if (button_states[SNES_BUTTON_A] ||  
          button_states[SNES_BUTTON_B] || 
          button_states[SNES_BUTTON_X] || 
          button_states[SNES_BUTTON_Y]) {
        pullDown(C64_FIRE_PIN);
        pressed = pressed || true;
      } else {
        neutral(C64_FIRE_PIN);
      }
    }
  }
  
  if (!(button_states2[SNES_BUTTON_UP] && button_states2[SNES_BUTTON_DOWN])) {
      handleButton2(SNES_BUTTON_UP, C64_UP_PIN2);
      handleButton2(SNES_BUTTON_DOWN, C64_DOWN_PIN2);
      handleButton2(SNES_BUTTON_LEFT, C64_LEFT_PIN2);
      handleButton2(SNES_BUTTON_RIGHT, C64_RIGHT_PIN2);
    if ((button_states2[SNES_BUTTON_A] != button_states2_previous[SNES_BUTTON_A]) || 
        (button_states2[SNES_BUTTON_B] != button_states2_previous[SNES_BUTTON_B]) || 
        (button_states2[SNES_BUTTON_X] != button_states2_previous[SNES_BUTTON_X]) || 
        (button_states2[SNES_BUTTON_Y] != button_states2_previous[SNES_BUTTON_Y])) {

      state_changed = true;

      if (button_states2[SNES_BUTTON_A] || 
          button_states2[SNES_BUTTON_B] || 
          button_states2[SNES_BUTTON_X] || 
          button_states2[SNES_BUTTON_Y]) {
        pullDown(C64_FIRE_PIN2);
        pressed = pressed || true;
      } else {
        neutral(C64_FIRE_PIN2);
      }
    }
  }

  if (state_changed) {
    // use the led as an indicator to test buttons
    digitalWrite(LED_BUILTIN, pressed);
  }
}

void loop() {
  // switch buffers first
  bool *temp = button_states_previous;
  button_states_previous = button_states;
  button_states = temp;
  temp = button_states2_previous;
  button_states2_previous = button_states2;
  button_states2 = temp;

  // Collect button state info from controller.
  // Send data latch.
  digitalWrite(LATCH_PIN, HIGH);
  digitalWrite(LATCH_PIN2, HIGH);
  delayMicroseconds(12);
  digitalWrite(LATCH_PIN, LOW);
  digitalWrite(LATCH_PIN2, LOW);

  delayMicroseconds(6);

  for (uint8_t id = 0; id < num_buttons; id++) {
    // Sample the button state.
    int button_pressed = digitalRead(DATA_PIN) == LOW;
    int button_pressed2 = digitalRead(DATA_PIN2) == LOW;
    button_states[id] = button_pressed;
    button_states2[id] = button_pressed2;

    digitalWrite(CLOCK_PIN, LOW);
    digitalWrite(CLOCK_PIN2, LOW);
    delayMicroseconds(6);
    digitalWrite(CLOCK_PIN, HIGH);
    digitalWrite(CLOCK_PIN2, HIGH);
    delayMicroseconds(6);
  }

  // Update button states.
  updateButtons(button_states, button_states2);

  // this number is chosen to produce an exact 1000Hz sample rate
  // is this really important? not at all.. but it's awesome :) 
  // that means we don't introduce more then 1ms lag in our converter.
  delayMicroseconds(369);
}
