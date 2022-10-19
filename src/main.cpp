/*
TO DO:
- port CAN code from previous versions
- check schematic and define pins
- check APPS rules about safe state

*/

#include <Arduino.h>

// Pins defined here:
#define SOUND_HORN_OUT 0
#define IGNITION_INTERRUPT_PIN 0

// Constants defined here:
const float brake_pressure_limit = 0;
const float brake_pressure_ignition_limit = 0;
const float throttle_position_per_min_limit = 0;
const float battery_power_throttle_brake_limit = 0;

// States defined here:
volatile enum state_type {
	safety,
	drive,
	fault
};

// // Flags defined here:
// int state = 1;
// int running_flag = 0;
// int shut_down_flag = 0;

//Define variables:
volatile float throttle_position_per = 0;
volatile float battery_power = 0;
volatile float brake_pressure = 0;

volatile bool fail_condition;

state_type state = safety;

//Load functions:
void running_mode(void);
void standby_mode(void);
void shut_down_mode(void);
void start_car(void);

void setup() {
  // put your setup code here, to run once:

  // set all input/output pins
  pinMode(IGNITION_INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(IGNITION_INTERRUPT_PIN), start_car, FALLING);

  state = safety;
}

void loop() {
  // put your main code here, to run repeatedly:
  
}

void get_readings() {
  throttle_position_per = get_throttle_position_percentage(); 
  brake_pressure = get_brake_pressure();  
  battery_power = get_battery_power();
}

void readings_fail(){
  fail_condition = ((brake_pressure > brake_pressure_limit) || (throttle_position_per > throttle_position_per_min_limit) || (battery_power > battery_power_throttle_brake_limit));
}

void isr() {
  get_readings();
  readings_fail();
  if (fail_condition || state == fault) {
    state = fault;
    shut_down_mode();
  } else if (state == safety) {
    standby_mode();
  } else if (state == drive) {
    running_mode();
  } 
}

void running_mode() {
  // need to define this mode further
  send_power_to_MC(throttle_position_per);
}

void standby_mode() {
  // need to define this mode further
}

void shut_down_mode() {
  // need to define this mode further
}

float get_throttle_position_percentage() {
  // Measures voltage from APPS via ADC and converts to percentage of travel
  // Changes flags if the two percentages are too far appart
  return 0;
}

void send_power_to_MC(float throttle_position_percentage) {
  // Send message via CAN bus
  return;
}

float get_brake_pressure() {
  // Get brake pressure via ADC
  return 0;
}

float get_battery_power() {
  // Get battery power via ADC
  return 0;
}

float get_TS_V() {
  // Get brake pressure via ADC
  return 0;
}

float get_TS_I() {
  // Get battery power via ADC
  return 0;
}

void play_sound_horn(){
  // Set digital output for a time to HIGH
  return;  
}

void start_car() {
  // Start the car
  if(state==drive){
    state = safety;
  }
  else{
    brake_pressure = get_brake_pressure();
    if (brake_pressure >= brake_pressure_ignition_limit) {
      play_sound_horn();
      state = drive;
    }
  }
}