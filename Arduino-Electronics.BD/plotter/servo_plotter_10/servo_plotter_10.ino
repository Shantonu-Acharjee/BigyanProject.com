/*
  servo_plottter_10.ino
  Code by lingib
  https://www.instructables.com/member/lingib/instructables/
  Last update 23 December 2020

  ------
  ABOUT
  ------
  This experimental plotter features:
   - servo motors rather than stepper motors
   - a simple G-CODE interpreter
   - a novel interrupt driven servo library in which all servos start together
     The servos are refreshed every 20 milliseconds which leaves 18.5mS for processing
   - two push buttons for calibrating the servos ... press both to exit

  Servos exhibit something called deadband which can be likened to hysterisis ... once
  servos reach their target position they need a large pulse-width change to get them moving.
  Without some form of correction the diagonals for CW and CCW squares are displaced

  After much experimenting I found that the servo pulsewidths at 10 degree clockwise intervals
  were on average 10uS less than the counter-clockwise pulse widths. To eliminate this deadband
  I add/subtract 10 uS to/from each servo pulse depending on the servo direction.

  This seems to work as the squares look squarer and the diagonals are now superimposed.

  ---------
  WARNINGS
  ---------
  (1) This experimental plotter is not accurate due to servo "deadband"
  (2) KEEP CLEAR OF THE SERVO ARMS AS THEY CAN MOVE RAPIDLY

  ----------
  COPYRIGHT
  ----------
  This code is free software: you can redistribute it and/or
  modify it under the terms of the GNU General Public License as published
  by the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This software is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License. If
  not, see <http://www.gnu.org/licenses/>.
*/

// ----- constants
#define PI 3.1415926535897932384626433832795
#define HALF_PI 1.5707963267948966192313216916398
#define TWO_PI 6.283185307179586476925286766559
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define RAD_TO_DEG 57.295779513082320876798154814105

// ----- Bit set/clear/check/toggle macros
#define SET(x,y) (x |=(1<<y))
#define CLR(x,y) (x &= (~(1<<y)))
#define CHK(x,y) (x & (1<<y))
#define TOG(x,y) (x^=(1<<y))

// ----- libraries
#include "myServo.h"
myServo servo1;                   // create servo1 object
myServo servo2;                   // create servo2 object
myServo servo3;                  // create penlift object

// ----- calibration
#define decrease 6                // button todecrease pulse-width
#define increase 7                // button to increase pulse-width
unsigned long cal_delay = 10;     // sets servo speed during calibration

// ----- servo connections
#define servo1_ctrl 2             // servo1 control pin
#define servo2_ctrl 3            // servo2 control pin
#define servo3_ctrl 4            // penlift control pin

// ----- enter servo calibration figures (uS)
/*
  The servo arms will collide if we try calibrating at 0 degrees
  Plus some servos can't rotate a full 180 degrees.
  Hobby servos are most accurate somewhere between these two limits
  We can accurately determine 90 degrees using a 45 degree set square.
  We can accurately determine 45 degrees using a 45 degree set square.
  The 90 degree and 0 degree pulsewidths are interpolated by adding|subtracting twice the difference.

  The following values are for my RDS-3225 servos ... yours will be slightly different
  Overwrite these pulse-widths using the values you get using the menu options
*/

unsigned long servo1_90 = 1166;   // pulse-width when arm1 is 90 degrees
unsigned long servo2_90 = 1137;   // pulse-width when arm2 is 90 degrees
unsigned long servo3_90 = 1250;   // pulse-width when servo horn is 90 degrees

unsigned long servo1_45 = 848;    // pulse-width when arm1 is 45 degrees
unsigned long servo2_45 = 828;    // pulse-width when arm2 is 45 degrees
unsigned long servo3_45 = 875;    // pulse-width when servo horn is 45 degrees

unsigned long maxWidth1 = servo1_90 + 2 * (servo1_90 - servo1_45);    // 1802
unsigned long minWidth1 = servo1_90 - 2 * (servo1_90 - servo1_45);    // 530
unsigned long maxWidth2 = servo2_90 + 2 * (servo2_90 - servo2_45);    // 1755
unsigned long minWidth2 = servo2_90 - 2 * (servo2_90 - servo2_45);    // 519
unsigned long maxWidth3 = servo3_90 + 2 * (servo3_90 - servo3_45);    // 2000
unsigned long minWidth3 = servo3_90 - 2 * (servo3_90 - servo3_45);    // 500

// ----- deadband correction
float last_angle1;
float last_angle2;
unsigned long deadband1 = 10L;    // motor1 deadband (uS)
unsigned long deadband2 = 10L;    // motor2 deadband (uS)

// ----- communications
#define baud 9600               // serial communication speed
#define XOFF 0x13                 // pause transmission (19 decimal)
#define XON 0x11                  // resume transmission (17 decimal)

// ----- plotter dimensions
float servo1_arm = 200;           // length servo1 arm
float servo2_arm = 200;           // length servo2 arm
float Xoffset = 50.0;             // servo1 Xoffset from (0,0) coordinate
float Yoffset = 0.0;              // servo1 Yoffset from (0,0) coordinate

int thisX = round(Xoffset);       // X co-ordinate (rounded)
int thisY = round(Yoffset);       // Y co-ordinate (rounded)
int lastX = round(Xoffset);       // previous X co-ordinate (rounded)
int lastY = round(Yoffset);       // previous Y co-ordinate (rounded)

// ----- gcode interpreter
String inputString = "";          // a String to hold incoming data
String subString;                 // used for subString extraction
int start;                        // used for subString extraction
int finish;                       // used for subString extraction

float X;                          // gcode float values held here
float Y;
float I;
float J;

float arcMax = 2;                 // maximum arc-length (controls smoothness)
bool refresh_flag = false;        // plot() waits for interrupt at each step
unsigned long DELAY = 25;         // milliseconds (controls servo speed)

unsigned long count = 1500UL;

// -----------------------
// SETUP
// -----------------------
void setup()
{
  // ----- 20 mS interrupt
  cli();                            // disable interrupts
  TCCR1A = 0;                       // clear control register A
  TCCR1B = 0;                       // clear control register B
  TCCR1B |= (1 << WGM12);           // turn on CTC mode
  TCCR1B |= (1 << CS12);            // set prescaler to 256
  TCNT1  = 0;                       // initialize counter value to 0
  OCR1A = 1249;                     // 20mS (50Hz); 1249 = (16*10^6)/(256*50) - 1
  TIMSK1 |= (1 << OCIE1A);          // enable timer compare interrupt
  sei();                            // enable interrupts

  // ----- attach servos
  servo1.attach(servo1_ctrl);
  servo2.attach(servo2_ctrl);
  servo3.attach(servo3_ctrl);

  // ----- configure the servos
  servo1.max_width(maxWidth1);
  servo1.min_width(minWidth1);
  servo2.max_width(maxWidth2);
  servo2.min_width(minWidth2);
  servo3.max_width(maxWidth3);
  servo3.min_width(minWidth3);

  // ----- attach buttons
  pinMode(increase, INPUT_PULLUP);
  pinMode(decrease, INPUT_PULLUP);

  // ------ home
  home();

  // ----- serial link
  Serial.begin(baud);
  Serial.flush();                             // clear TX buffer
  while (Serial.available()) Serial.read();   // clear RX buffer
  inputString.reserve(200);

  // ----- display commands
  menu();
}

//--------------------------------------------------------------------------
// MAIN LOOP
//--------------------------------------------------------------------------
void loop()
{
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar;
    if (inChar == '\n') {
      Serial.write(XOFF);                     // pause transmission
      Serial.flush();                         // clear TX buffer
      process(inputString);                   // interpret string and perform task
      inputString = "";                       // empty string
      Serial.flush();                         // clear TX buffer
      Serial.write(XON);                      // resume transmission
    }
  }
}

// -----------------------
//  ISR(TIMER1_COMPA_vect)
// -----------------------
ISR(TIMER1_COMPA_vect) {
  /*
    20mS servo refresh
    all servos start together
  */

  // ----- get servo pulse widths
  unsigned long w1 = servo1.width();
  unsigned long w2 = servo2.width();
  unsigned long w3 = servo3.width();

  // ----- sort widths into ascending order
  unsigned long minimum = min(w1, min(w2, w3));
  unsigned long maximum = max(w1, max(w2, w3));
  unsigned long middle = w1 + w2 + w3 - minimum - maximum;

  // ----- calculate delays
  /*  micros() doesn't work in an ISR */
  unsigned long delay1 = minimum;
  unsigned long delay2 = middle - minimum;
  unsigned long delay3 = maximum - middle;

  // ----- generate leading edges
  servo1.ctrl_high();
  servo2.ctrl_high();
  servo3.ctrl_high();

  delayMicroseconds(delay1);
  if (minimum == w1) servo1.ctrl_low();
  if (minimum == w2) servo2.ctrl_low();
  if (minimum == w3) servo3.ctrl_low();

  delayMicroseconds(delay2);
  if (middle == w1) servo1.ctrl_low();
  if (middle == w2) servo2.ctrl_low();
  if (middle == w3) servo3.ctrl_low();

  delayMicroseconds(delay3);
  if (maximum == w1) servo1.ctrl_low();
  if (maximum == w2) servo2.ctrl_low();
  if (maximum == w3) servo3.ctrl_low();

  refresh_flag = true;                           // tell plot() that refresh has been done
}

//---------------------------------------------------------------------------
// MENU
//---------------------------------------------------------------------------
/*
   The Arduino F() flash macro is used to conserve RAM.
*/
void menu() {
  Serial.println(F(""));
  Serial.println(F("  -------------------------------------------------------------"));
  Serial.println(F("                         MENU"));
  Serial.println(F("  -------------------------------------------------------------"));
  Serial.println(F("    MENU ............... menu"));
  Serial.println(F("    G00 X## Y## ........ goto XY (pen up"));
  Serial.println(F("    G01 X## Y## ........ goto XY (pen down)"));
  Serial.println(F("    T1 ................. home"));
  Serial.println(F("    T2 ................. pen up"));
  Serial.println(F("    T3 ................. pen down"));
  Serial.println(F("    T4 ................. calibrate: servo1 90 degrees"));
  Serial.println(F("    T5 ................. calibrate: servo2 90 degrees"));
  Serial.println(F("    T6 ................. calibrate: servo1 45 degrees"));
  Serial.println(F("    T7 ................. calibrate: servo2 45 degrees"));
  Serial.println(F("    T8 ................. draw 100mm square (with deadband)"));
  Serial.println(F("    T9 ................. draw 100mm square (compensated)"));
  Serial.println(F("  --------------------------------------------------------------"));
}

//--------------------------------------------------------------------------
// PROCESS
//--------------------------------------------------------------------------
void process(String string) {

  // ----- convert string to upper case
  inputString = string;
  inputString.toUpperCase();
  Serial.println(inputString);            // display string command

  // ----------------------------------
  // G00   linear move with pen_up
  // ----------------------------------
  if (inputString.startsWith("G00")) {
    Serial.println("hello");

    // ----- extract X
    start = inputString.indexOf('X');
    if (!(start < 0)) {
      finish = start + 8;
      subString = inputString.substring(start + 1, finish + 1);
      X = subString.toFloat();
    }

    // ----- extract Y
    start = inputString.indexOf('Y');
    if (!(start < 0)) {
      finish = start + 8;
      subString = inputString.substring(start + 1, finish + 1);
      Y = subString.toFloat();
    }

    pen_up();
    move_to(X, Y);
  }

  // ----------------------------------
  // G01   linear move with pen_down
  // ----------------------------------
  if (inputString.startsWith("G01")) {

    // ----- extract X
    start = inputString.indexOf('X');
    if (!(start < 0)) {
      finish = start + 8;
      subString = inputString.substring(start + 1, finish + 1);
      X = subString.toFloat();
    }

    // ----- extract Y
    start = inputString.indexOf('Y');
    if (!(start < 0)) {
      finish = start + 8;
      subString = inputString.substring(start + 1, finish + 1);
      Y = subString.toFloat();
    }

    pen_down();
    move_to(X, Y);
  }

  // ----------------------------------
  // G02   clockwise arc with pen_down
  // ----------------------------------
  if (inputString.startsWith("G02")) {

    // ----- extract X
    start = inputString.indexOf('X');
    if (!(start < 0)) {
      finish = inputString.indexOf('.', inputString.indexOf('X'));
      subString = inputString.substring(start + 1, finish + 7);
      X = subString.toFloat();
    }

    // ----- extract Y
    start = inputString.indexOf('Y');
    if (!(start < 0)) {
      finish = inputString.indexOf('.', inputString.indexOf('Y'));
      subString = inputString.substring(start + 1, finish + 7);
      Y = subString.toFloat();
    }

    // ----- extract I
    start = inputString.indexOf('I');
    if (!(start < 0)) {
      finish = inputString.indexOf('.', inputString.indexOf('I'));
      subString = inputString.substring(start + 1, finish + 7);
      I = subString.toFloat();
    }

    // ----- extract J
    start = inputString.indexOf('J');
    if (!(start < 0)) {
      finish = inputString.indexOf('.', inputString.indexOf('J'));
      subString = inputString.substring(start + 1, finish + 7);
      J = subString.toFloat();
    }

    pen_down();
    draw_arc_cw(X, Y, I, J);
  }

  // ------------------------------------------
  // G03   counter-clockwise arc with pen_down
  // ------------------------------------------
  if (inputString.startsWith("G03")) {

    // ----- extract X
    start = inputString.indexOf('X');
    if (!(start < 0)) {
      finish = inputString.indexOf('.', inputString.indexOf('X'));
      subString = inputString.substring(start + 1, finish + 7);
      X = subString.toFloat();
    }

    // ----- extract Y
    start = inputString.indexOf('Y');
    if (!(start < 0)) {
      finish = inputString.indexOf('.', inputString.indexOf('Y'));
      subString = inputString.substring(start + 1, finish + 7);
      Y = subString.toFloat();
    }

    // ----- extract I
    start = inputString.indexOf('I');
    if (!(start < 0)) {
      finish = inputString.indexOf('.', inputString.indexOf('I'));
      subString = inputString.substring(start + 1, finish + 7);
      I = subString.toFloat();
    }

    // ----- extract J
    start = inputString.indexOf('J');
    if (!(start < 0)) {
      finish = inputString.indexOf('.', inputString.indexOf('J'));
      subString = inputString.substring(start + 1, finish + 7);
      J = subString.toFloat();
    }

    pen_down();
    draw_arc_ccw(X, Y, I, J);
  }

  // ----------------------------------
  // MENU
  // ----------------------------------
  if (inputString.startsWith("MENU")) {
    menu();
  }

  // ----------------------------------
  // T1 home
  // ----------------------------------
  if (inputString.startsWith("T1")) {
    home();
  }

  // ----------------------------------
  // T2   pen up
  // ----------------------------------
  if (inputString.startsWith("T2")) {
    pen_up();
  }

  // ----------------------------------
  // T3   pen down
  // ----------------------------------
  if (inputString.startsWith("T3")) {
    pen_down();
  }

  // ----------------------------------
  // T4   calibrate servo1 90
  // ----------------------------------
  if (inputString.startsWith("T4")) {
    cal_servo1_90();
  }

  // ----------------------------------
  // T5   calibrate servo2 90
  // ----------------------------------
  if (inputString.startsWith("T5")) {
    cal_servo2_90();
  }

  // ----------------------------------
  // T6   calibrate servo 1 45
  // ----------------------------------
  if (inputString.startsWith("T6")) {
    cal_servo1_45();
  }

  // ----------------------------------
  // T7   calibrate servo2 45
  // ----------------------------------
  if (inputString.startsWith("T7")) {
    cal_servo2_45();
  }

  // ----------------------------------
  // T8   square (CW)
  // ----------------------------------
  if (inputString.startsWith("T8")) {
    square1();
  }

  // ----------------------------------
  // T9   draw square (CCW)
  // ----------------------------------
  if (inputString.startsWith("T9")) {
    square2();
  }
}

// -----------------------
//  home()
// -----------------------
void home() {
  /* set the servo start angles */

  // ---- get sides of triangle ABC
  float a = servo2_arm;
  float b = sqrt(sq(Xoffset) + sq(Yoffset));                      // pythagoras
  float c = servo1_arm;

  // ----- calculate main angles (radians)
  if (Xoffset < 1) Xoffset = 1;                                   // avoids a potential divide by zero error
  float alpha = atan2(Yoffset, Xoffset);
  float angleBAC = acos((sq(b) + sq(c) - sq(a)) / (2 * b * c));   // cosine formula
  float angleB = acos((sq(c) + sq(a) - sq(b)) / (2 * c * a));     // cosine formula

  // ----- calculate servo angles (degrees)
  //float servo1_angle = (alpha + ((PI - angleB) / 2)) * RAD_TO_DEG;
  float servo1_angle = (angleBAC + alpha) * RAD_TO_DEG;
  float servo2_angle = angleB * RAD_TO_DEG;

  // ----- update servo start angles
  servo1.angle(servo1_angle, 0);
  servo2.angle(servo2_angle, 0);
  servo3.angle(0, 0);                                               // pen up

  // ----- record last_angles for plot() function
  last_angle1 = servo1_angle;
  last_angle2 = servo2_angle;
}

//---------------------------------------------------------------------------
// PEN_UP
//---------------------------------------------------------------------------
/*
   Caution ... The servo horn for the oenlift is displaced 90 degrees
*/
void pen_up() {
  servo3.angle(0, 0);
}

//---------------------------------------------------------------------------
// PEN_DOWN
//---------------------------------------------------------------------------
/*
   Caution ... The servo horn for the oenlift is displaced 90 degrees
*/
void pen_down() {
  servo3.angle(90, 0);
}

//---------------------------------------------------------------------------
// calibrate servo1 90
//---------------------------------------------------------------------------
void cal_servo1_90() {
  Serial.println("Press both buttons to exit");
  while (true) {
    Serial.print("Servo1 90: ");
    if (digitalRead(increase) == LOW) servo1_90++;
    if (digitalRead(decrease) == LOW) servo1_90--;
    servo1.calibrate(servo1_90);
    Serial.println(servo1_90);
    delay(cal_delay);
    if (digitalRead(increase) == LOW && digitalRead(decrease) == LOW) {
      Serial.println("Copy this value to the header");
      menu();
      break;
    }
  }
}

//---------------------------------------------------------------------------
// calibrate servo2 90
//---------------------------------------------------------------------------
void cal_servo2_90() {
  Serial.println("Press both buttons to exit");
  while (true) {
    Serial.print("Servo2 90: ");
    if (digitalRead(increase) == LOW) servo2_90++;
    if (digitalRead(decrease) == LOW) servo2_90--;
    servo2.calibrate(servo2_90);
    Serial.println(servo2_90);
    delay(cal_delay);
    if (digitalRead(increase) == LOW && digitalRead(decrease) == LOW) {
      Serial.println("Copy this value to the header");
      menu();
      break;
    }
  }
}

//---------------------------------------------------------------------------
// calibrate servo1 45
//---------------------------------------------------------------------------
void cal_servo1_45() {
  Serial.println("Press both buttons to exit");
  while (true) {
    Serial.print("Servo1 45: ");
    if (digitalRead(increase) == LOW) servo1_45++;
    if (digitalRead(decrease) == LOW) servo1_45--;
    servo1.calibrate(servo1_45);
    Serial.println(servo1_45);
    delay(cal_delay);
    if (digitalRead(increase) == LOW && digitalRead(decrease) == LOW) {
      Serial.println("Copy this value to the header");
      menu();
      break;
    }
  }
}

//---------------------------------------------------------------------------
// calibrate servo1 45
//---------------------------------------------------------------------------
void cal_servo2_45() {
  Serial.println("Press both buttons to exit");
  while (true) {
    Serial.print("Servo2 45: ");
    if (digitalRead(increase) == LOW) servo2_45++;
    if (digitalRead(decrease) == LOW) servo2_45--;
    servo2.calibrate(servo2_45);
    Serial.println(servo2_45);
    delay(cal_delay);
    if (digitalRead(increase) == LOW && digitalRead(decrease) == LOW) {
      Serial.println("Copy this value to the header");
      menu();
      break;
    }
  }
}

// -------------------------------
// MOVE_TO
// -------------------------------
/*
  This plotter is essentially a polar (angle-distance) plotter
  which means that the line between any two points is curved.

  This routine uses Bresenham's line drawing algorithm to subdivide
  the straight line between LAST(X,Y) and THIS(X,Y)

  Each of these coordinateis then parsed tgo the plot() function.
  This results in a straight line comprising tiny curved segments.
*/
void move_to(float x, float y) {

  // ----- apply offset
  thisX = round(x + Xoffset);
  thisY = round(y + Yoffset);

  // ----- draw a line between these co-ordinates
  draw_line(lastX, lastY, thisX, thisY);

  // ----- remember last rounded co-ordinate
  lastX = thisX;
  lastY = thisY;
}

// ------------------------------------------------------------------------
// DRAW LINE
// ------------------------------------------------------------------------
/*
  This routine sub-divides any line between two coordinates into a number of
  intermediate points. These intermediate coordinates are then sent to a plot()
  routine which effectively linearizes the output.

  The algorithm automatically maps all "octants" to "octant 0" and
  automatically swaps the XY coordinates if dY is greater than dX. A swap
  flag determines which motor moves for any combination X,Y inputs. The swap
  algorithm is further optimised by realising that dY is always positive
  in quadrants 0,1 and that dX is always positive in "quadrants" 0,3.

  Each intermediate XY co-ordinate is plotted which results in a straight line
*/
void draw_line(int x1, int y1, int x2, int y2) {

  // ----- locals
  int
  x = x1,                             //current X-axis position
  y = y1,                             //current Y-axis position
  dy,                                 //line slope
  dx,
  slope,
  longest,                            //axis lengths
  shortest,
  maximum,
  error,                              //bresenham thresholds
  threshold;

  // ----- find longest and shortest axis
  dy = y2 - y1;                         //vertical distance
  dx = x2 - x1;                         //horizontal distance
  longest = max(abs(dy), abs(dx));      //longest axis
  shortest = min(abs(dy), abs(dx));     //shortest axis

  // ----- scale Bresenham values by 2*longest
  error = -longest;                     //add offset to so we can test at zero
  threshold = 0;                        //test now done at zero
  maximum = (longest << 1);             //multiply by two
  slope = (shortest << 1);              //multiply by two ... slope equals (shortest*2/longest*2)

  // ----- initialise the swap flag
  /*
     The XY axes are automatically swapped by using "longest" in
     the "for loop". XYswap is used to decode the motors.
  */
  bool XYswap = true;                    //used for motor decoding
  if (abs(dx) >= abs(dy)) XYswap = false;

  // ----- pretend we are always in octant 0
  /*
     The current X-axis and Y-axis positions will now be incremented (decremented) each time
     through the loop. These intermediate steps are parsed to the plot(x,y) function which calculates
     the number of steps required to reach each of these intermediate co-ordinates. This effectively
     linearises the plotter and eliminates unwanted curves.
  */
  for (int i = 0; i < longest; i++) {

    // ----- move left/right along X axis
    if (XYswap) {   //swap
      if (dy < 0) {
        y--;
      } else {
        y++;
      }
    } else {    //no swap
      if (dx < 0) {
        x--;
      } else {
        x++;
      }
    }

    // ----- move up/down Y axis
    error += slope;
    if (error > threshold) {
      error -= maximum;

      // ----- move up/down along Y axis
      if (XYswap) {  //swap
        if (dx < 0) {
          x--;
        } else {
          x++;
        }
      } else {  //no swap
        if (dy < 0) {
          y--;
        } else {
          y++;
        }
      }
    }

    // ----- plot the next rounded XY co-ordinate
    plot(x, y);
  }
}

// -------------------------------
// PLOT
// -------------------------------
/*
  This routine is called by the draw_line() function

  It only works if both servo arms are the same length

  The hypotenuse of the triangle formed by XY is calculated using Pythagoras

  The servo2_angle forms an isosecles triangle with the hypotenuse which means:
   - the Servo2_angle can be calculated using the cosine formula
   - the remaining interior angles are both (PI-Servo2_angle)/2

  The servo_angle1 is therefore (atan2(Y,X)+(PI-Servo2_angle)/2)
*/

void plot(int x_, int y_) {           // these values include Xoffset and Yoffset

  // ----- error check input
  float x = (float) x_;
  float y = (float) y_;
  if (x < 1) x = 1;             // avoids a potential divide by zero error

  // ---- get sides of triangle ABC
  float a = servo2_arm;
  float b = sqrt(sq(x) + sq(y));      // pythagoras
  float c = servo1_arm;

  // ----- calculate main angles (radians)
  float alpha = atan2(y, x);
  float angleA = acos((sq(b) + sq(c) - sq(a)) / (2 * b * c));   // cosine formula
  float angleB = acos((sq(c) + sq(a) - sq(b)) / (2 * c * a));   // cosine formula

  // ----- convert to degrees
  float servo1_angle = (angleA + alpha) * RAD_TO_DEG;
  float servo2_angle = angleB * RAD_TO_DEG;

  // ----- wait for interrupt (20mS intervals)
  while (refresh_flag == false);
  refresh_flag = false;             // flag was true to get here

  // ----- calculate deadband correction
  /* angles ready for next refresh */
  unsigned long correction1;
  unsigned long correction2;
  (servo1_angle < last_angle1) ? correction1 = -deadband1 : correction1 = deadband1;
  (servo2_angle < last_angle2) ? correction2 = -deadband2 : correction2 = deadband2;

  // ----- update servo angles between each refresh
  /* angles now ready for next refresh */
  servo1.angle(servo1_angle, correction1);
  servo2.angle(servo2_angle, correction2);

  // ----- update last angles
  last_angle1 = servo1_angle;
  last_angle2 = servo2_angle;

  delay(DELAY);                     // adjust servo speed
}

//----------------------------------------------------------------------------
// DRAW ARC CLOCKWISE (G02)
//----------------------------------------------------------------------------
void draw_arc_cw(float x, float y, float i, float j) {

  // ----- inkscape sometimes produces some crazy values for i,j
  if ((i < -100) || (i > 100) || (j < -100) || (j > 100)) {
    move_to(x, y);
  } else {

    // ----- variables
    float
    thisX = (lastX - Xoffset),  // true X co-ordinate
    thisY = (lastY - Yoffset),  // true  Y co-ordinate
    nextX = x,                      // next X co-ordinate
    nextY = y,                      // next Y co-ordinate
    newX,                           // interpolated X co-ordinate
    newY,                           // interpolated Y co-ordinate
    I = i,                          // horizontal distance thisX from circle center
    J = j,                          // vertical distance thisY from circle center
    circleX = thisX + I,            // circle X co-ordinate
    circleY = thisY + J,            // circle Y co-ordinate
    delta_x,                        // horizontal distance between thisX and nextX
    delta_y,                        // vertical distance between thisY and nextY
    chord,                          // line_length between lastXY and nextXY
    radius,                         // circle radius
    alpha,                          // interior angle of arc
    beta,                           // fraction of alpha
    arc,                            // subtended by alpha
    current_angle,                  // measured CCW from 3 o'clock
    next_angle;                     // measured CCW from 3 o'clock

    // ----- calculate arc
    delta_x = thisX - nextX;
    delta_y = thisY - nextY;
    chord = sqrt(delta_x * delta_x + delta_y * delta_y);
    radius = sqrt(I * I + J * J);
    alpha = 2 * asin(chord / (2 * radius));   //see construction lines
    arc = alpha * radius;                     //radians

    // ----- sub-divide alpha
    int segments = 1;
    if (arc > arcMax) {
      segments = (int)(arc / arcMax);
      beta = alpha / segments;
    } else {
      beta = alpha;
    }

    // ----- calculate current angle
    /*
      atan2() angles between 0 and PI are CCW +ve from 3 o'clock.
      atan2() angles between 2*PI and PI are CW -ve relative to 3 o'clock
    */
    current_angle = atan2(-J, -I);
    if (current_angle <= 0) current_angle += 2 * PI;        //angles now 360..0 degrees CW

    // ----- plot intermediate CW co-ordinates
    next_angle = current_angle;                             //initialise angle
    for (int segment = 1; segment < segments; segment++) {
      next_angle -= beta;                                   //move CW around circle
      if (next_angle < 0) next_angle += 2 * PI;             //check if angle crosses zero
      newX = circleX + radius * cos(next_angle);            //standard circle formula
      newY = circleY + radius * sin(next_angle);
      move_to(newX, newY);
    }

    // ----- draw final line
    move_to(nextX, nextY);
  }
}

//----------------------------------------------------------------------------
// DRAW ARC COUNTER-CLOCKWISE (G03)
//----------------------------------------------------------------------------
/*
   We know the start and finish co-ordinates which allows us to calculate the
   chord length. We can also calculate the radius using the biarc I,J values.
   If we bisect the chord the center angle becomes 2*asin(chord/(2*radius)).
   The arc length may now be calculated using the formula arc_length = radius*angle.
*/
void draw_arc_ccw(float x, float y, float i, float j) {

  // ----- inkscape sometimes produces some crazy values for i,j
  if ((i < -100) || (i > 100) || (j < -100) || (j > 100)) {
    move_to(x, y);
  } else {

    // ----- variables
    float
    thisX = (lastX - Xoffset),  // true X co-ordinate
    thisY = (lastY - Yoffset),  // true Y co-ordinate
    nextX = x,                      // next X co-ordinate
    nextY = y,                      // next Y co-ordinate
    newX,                           // interpolated X co-ordinate
    newY,                           // interpolated Y co-ordinate
    I = i,                          // horizontal distance thisX from circle center
    J = j,                          // vertical distance thisY from circle center
    circleX = thisX + I,            // circle X co-ordinate
    circleY = thisY + J,            // circle Y co-ordinate
    delta_x,                        // horizontal distance between thisX and nextX
    delta_y,                        // vertical distance between thisY and nextY
    chord,                          // line_length between lastXY and nextXY
    radius,                         // circle radius
    alpha,                          // interior angle of arc
    beta,                           // fraction of alpha
    arc,                            // subtended by alpha
    current_angle,                  // measured CCW from 3 o'clock
    next_angle;                     // measured CCW from 3 o'clock

    // ----- calculate arc
    delta_x = thisX - nextX;
    delta_y = thisY - nextY;
    chord = sqrt(delta_x * delta_x + delta_y * delta_y);
    radius = sqrt(I * I + J * J);
    alpha = 2 * asin(chord / (2 * radius));   // see construction lines
    arc = alpha * radius;                     // radians

    // ----- sub-divide alpha
    int segments = 1;
    if (arc > arcMax) {
      segments = (int)(arc / arcMax);
      beta = alpha / segments;
    } else {
      beta = alpha;
    }

    // ----- calculate current angle
    /*
        tan2() angles between 0 and PI are CCW +ve from 3 o'clock.
        atan2() angles between 2*PI and PI are CW -ve relative to 3 o'clock
    */
    current_angle = atan2(-J, -I);
    if (current_angle <= 0) current_angle += 2 * PI;        // angles now 360..0 degrees CW

    // ----- plot intermediate CCW co-ordinates
    next_angle = current_angle;                             // initialise angle
    for (int segment = 1; segment < segments; segment++) {
      next_angle += beta;                                   // move CCW around circle
      if (next_angle > 2 * PI) next_angle -= 2 * PI;        // check if angle crosses zero
      newX = circleX + radius * cos(next_angle);            // standard circle formula
      newY = circleY + radius * sin(next_angle);
      move_to(newX, newY);
    }

    // ----- draw final line
    move_to(nextX, nextY);
  }
}

// ----------------------
//  square1() (deadband)
// ----------------------
void square1() {

  // ----- remove compenstation
  deadband1 = 0;
  deadband2 = 0;

  // ----- clockwise square
  process(F("G00 X50 Y50"));
  process(F("G01 X50 Y150"));
  process(F("G01 X150 Y150"));
  process(F("G01 X150 Y50"));
  process(F("G01 X50 Y50"));
  //process(F("G00 X50 Y150"));   // diagonal
  //process(F("G01 X150 Y50"));
  //process(F("G00 X150 Y150"));  // diagonal
  //process(F("G01 X50 Y50"));
  process(F("G00 X0 Y0"));      // home
  /*
    // ----- pause
    delay(2000);

    // ----- counter clockwise square
    process(F("G00 X150 Y150"));
    process(F("G01 X50 Y150"));
    process(F("G01 X50 Y50"));
    process(F("G01 X150 Y50"));
    process(F("G01 X150 Y150"));
    process(F("G00 X150 Y50"));   // diagonal
    process(F("G01 X50 Y150"));
    process(F("G00 X50 Y50"));    // diagonal
    process(F("G01 X150 Y150"));
    process(F("G00 X0 Y0"));      // home
  */
  // ----- restore compenstation
  deadband1 = 10;
  deadband2 = 10;
}

// ----------------------
//  square2() (no deadband)
// ----------------------
void square2() {

  // ----- clockwise square
  process(F("G00 X50 Y50"));
  process(F("G01 X50 Y150"));
  process(F("G01 X150 Y150"));
  process(F("G01 X150 Y50"));
  process(F("G01 X50 Y50"));
  process(F("G00 X50 Y150"));   // diagonal
  process(F("G01 X150 Y50"));
  process(F("G00 X150 Y150"));  // diagonal
  process(F("G01 X50 Y50"));
  process(F("G00 X0 Y0"));      // home
  /*
    // ----- pause
    delay(2000);

    // ----- counter clockwise square
    process(F("G00 X150 Y150"));
    process(F("G01 X50 Y150"));
    process(F("G01 X50 Y50"));
    process(F("G01 X150 Y50"));
    process(F("G01 X150 Y150"));
    process(F("G00 X150 Y50"));   // diagonal
    process(F("G01 X50 Y150"));
    process(F("G00 X50 Y50"));    // diagonal
    process(F("G01 X150 Y150"));
    process(F("G00 X0 Y0"));      // home
  */
}
