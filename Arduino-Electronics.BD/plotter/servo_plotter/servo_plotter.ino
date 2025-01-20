/*
  servo_plotter.ino
  Code by lingib
  https://www.instructables.com/member/lingib/instructables/
  Last update 29 October 2020

  This software allows you to control multiple servos.
  The servos are refreshed every 20 milliseconds.
  Three servos leaves 12.5mS for processing  ... (20-3*2.5)=12.5mS

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

// ----- libraries
#include "myServo.h"
myServo servo1;               // create servo1 object
myServo servo2;               // create servo2 object
myServo pen_lift;             // create pen-lift object

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

// ----- servo1 (X-axis)
#define servo1_ctrl 2         // servo control pin
float servo1_arm = 200;       // length servo1 arm
int Xoffset = 100;             // servo1 Xoffset from (0,0) coordinate

// ----- servo2 (Y-axis)
#define servo2_ctrl 3        // servo control pin
float servo2_arm = 200;       // length servo2 arm
int Yoffset = 0;              // Servo1 Yoffset from (0,0) coordinate

// ----- plotter definitions
#define BAUD 115200
#define XOFF 0x13                     // pause transmission (19 decimal)
#define XON 0x11                      // resume transmission (17 decimal)
#define pen_ctrl 4           // pen-lift control

float ARC_MAX = 2;                          // maximum arc-length (controls smoothness)
bool Refresh_flag = false;    // plot() waits for interrupt at each step
unsigned long DELAY = 25;      // milliseconds (controls servo speed)

int THIS_X = Xoffset;         // x co-ordinate (rounded)
int THIS_Y = Yoffset;         // y co-ordinate (rounded)
int LAST_X = Xoffset;         // x co-ordinate (rounded)
int LAST_Y = Yoffset;         // y co-ordinate (rounded)

// ----- gcode definitions
#define STRING_SIZE 128               // string size
char BUFFER[STRING_SIZE + 1];
int INDEX = 0;                          // buffer index
String INPUT_STRING;
String SUB_STRING;
char INPUT_CHAR;
int START;                              // used for sub_string extraction
int FINISH;
float X;                                  // gcode float values held here
float Y;
float I;
float J;

// -----------------------
// SETUP
// -----------------------
void setup()
{
  // ----- serial link
  Serial.begin(BAUD);
  Serial.flush();           // clear TX buffer
  while (Serial.available()) Serial.read();   // clear RX buffer
  
  // ----- attach servos
  servo1.attach(servo1_ctrl);
  servo2.attach(servo2_ctrl);
  pen_lift.attach(pen_ctrl);

  // ----- set servo1 limits (MG 996R)
  float zero1 = 100.0;
  float scale1 = (float)(2000 / 180.0);
  servo1.calibrate(zero1, scale1);

  // ----- set servo2 limits  (MG 996R)
  float zero2 = 750.0;
  float scale2 = (float)(2000 / 180.0);
  servo2.calibrate(zero2, scale2);

  // ----- set servo1 limits    (SG-90)
  //float zero1 = 500.0;
  //float scale1 = (float)(2000/ 180.0);
  //servo1.calibrate(zero1, scale1);

  // ----- set servo2 limits    (SG-90)
  //float zero2 = 600.0;
  //float scale2 = (float)(1700 / 180.0);
  //servo2.calibrate(zero2, scale2);

  // ----- move to the home position
  set_servo_start_angles();

  // ----- plotter setup
  memset(BUFFER, '\0', sizeof(BUFFER));       // fill with string terminators
  INPUT_STRING.reserve(STRING_SIZE);
  INPUT_STRING = "";
  pen_up();

  // ----- 20 mS interrupt
  cli();                      // disable interrupts
  TCCR1A = 0;                 // clear control register A
  TCCR1B = 0;                 // clear control register B
  TCCR1B |= (1 << WGM12);     // turn on CTC mode
  TCCR1B |= (1 << CS12);      // set prescaler to 256
  TCNT1  = 0;                 // initialize counter value to 0
  OCR1A = 1249;               // 20mS (50Hz); 1249 = (16*10^6)/(256*50) - 1
  TIMSK1 |= (1 << OCIE1A);    // enable timer compare interrupt
  sei();                      // enable interrupts


  // ----- display commands
  menu();
}

//--------------------------------------------------------------------------
// MAIN LOOP
//--------------------------------------------------------------------------
void loop() {

  // ----- get the next instruction
  while (Serial.available()) {
    INPUT_CHAR = (char)Serial.read();         // read character
    Serial.write(INPUT_CHAR);                 // echo character to the screen
    BUFFER[INDEX++] = INPUT_CHAR;             // add char to buffer
    if (INPUT_CHAR == '\n') {                 // check for line feed
      Serial.flush();                         // clear TX buffer
      Serial.write(XOFF);                     // pause transmission
      INPUT_STRING = BUFFER;                  // convert to string
      process(INPUT_STRING);                  // interpret string and perform task
      memset(BUFFER, '\0', sizeof(BUFFER));   // fill buffer with string terminators
      INDEX = 0;                              // point to buffer start
      INPUT_STRING = "";                      // empty string
      Serial.flush();                         // clear TX buffer
      Serial.write(XON);                      // resume transmission
    }
  }
}

// -----------------------
//  ISR(TIMER1_COMPA_vect)
// -----------------------
ISR(TIMER1_COMPA_vect) {                      // refresh the servos every 20mS
  servo1.refresh();
  servo2.refresh();
  pen_lift.refresh();
  Refresh_flag = true;                        // signal plot() that refresh has been done
}

// -----------------------
//  set_servo_start_angles()
// -----------------------
void set_servo_start_angles() {

  // ---- get sides of triangle ABC
  float a = servo2_arm;
  float b = sqrt(sq((float)Xoffset) + sq((float)Yoffset));      // pythagoras
  float c = servo1_arm;

  //  // ----- calculate servo1_angle
  //  float alpha = atan2((float)Yoffset, (float)Xoffset);
  //  float a1 = acos((sq(b) + sq(c) - sq(a)) / (2 * b * c));   // cosine formula
  //  float start_angle1 = ((alpha + a1) * RAD_TO_DEG);

  // ----- calculate main angles (radians)
  float alpha = atan2((float)Yoffset, (float)Xoffset);
  float angleB = acos((sq(c) + sq(a) - sq(b)) / (2 * c * a));   // cosine formula

  // ----- convert to degrees
  float servo1_angle = (alpha + ((PI - angleB) / 2)) * RAD_TO_DEG;
  float servo2_angle = angleB * RAD_TO_DEG;

  // ----- update angles
  servo1.angle(servo1_angle);
  servo2.angle(servo2_angle);

  //Serial.print(servo1_angle); Serial.print("\t"); Serial.print(servo2_angle);
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
  Serial.println(F("    G00 X## Y## ........ goto XY (pen-up"));
  Serial.println(F("    G01 X## Y## ........ goto XY (pen-down)"));
  Serial.println(F("    T1 ................. pen up"));
  Serial.println(F("    T2 ................. pen down"));
  Serial.println(F("    T4 ................. all servos 0 degrees"));
  Serial.println(F("    T5 ................. all servos 90 degrees"));
  Serial.println(F("    T6 ................. all servos 180 degrees"));
  Serial.println(F("  --------------------------------------------------------------"));
}

//--------------------------------------------------------------------------
// PROCESS
//--------------------------------------------------------------------------
void process(String string) {

  // ----- convert string to upper case
  INPUT_STRING = string;
  INPUT_STRING.toUpperCase();

  // ----------------------------------
  // G00   linear move with pen_up
  // ----------------------------------
  if (INPUT_STRING.startsWith("G00")) {

    // ----- extract X
    START = INPUT_STRING.indexOf('X');
    if (!(START < 0)) {
      FINISH = START + 8;
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 1);
      X = SUB_STRING.toFloat();
    }

    // ----- extract Y
    START = INPUT_STRING.indexOf('Y');
    if (!(START < 0)) {
      FINISH = START + 8;
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 1);
      Y = SUB_STRING.toFloat();
    }

    pen_up();
    move_to(X, Y);
  }

  // ----------------------------------
  // G01   linear move with pen_down
  // ----------------------------------
  if (INPUT_STRING.startsWith("G01")) {

    // ----- extract X
    START = INPUT_STRING.indexOf('X');
    if (!(START < 0)) {
      FINISH = START + 8;
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 1);
      X = SUB_STRING.toFloat();
    }

    // ----- extract Y
    START = INPUT_STRING.indexOf('Y');
    if (!(START < 0)) {
      FINISH = START + 8;
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 1);
      Y = SUB_STRING.toFloat();
    }

    pen_down();
    move_to(X, Y);
  }

  // ----------------------------------
  // G02   clockwise arc with pen_down
  // ----------------------------------
  if (INPUT_STRING.startsWith("G02")) {

    // ----- extract X
    START = INPUT_STRING.indexOf('X');
    if (!(START < 0)) {
      FINISH = INPUT_STRING.indexOf('.', INPUT_STRING.indexOf('X'));
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 7);
      X = SUB_STRING.toFloat();
    }

    // ----- extract Y
    START = INPUT_STRING.indexOf('Y');
    if (!(START < 0)) {
      FINISH = INPUT_STRING.indexOf('.', INPUT_STRING.indexOf('Y'));
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 7);
      Y = SUB_STRING.toFloat();
    }

    // ----- extract I
    START = INPUT_STRING.indexOf('I');
    if (!(START < 0)) {
      FINISH = INPUT_STRING.indexOf('.', INPUT_STRING.indexOf('I'));
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 7);
      I = SUB_STRING.toFloat();
    }

    // ----- extract J
    START = INPUT_STRING.indexOf('J');
    if (!(START < 0)) {
      FINISH = INPUT_STRING.indexOf('.', INPUT_STRING.indexOf('J'));
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 7);
      J = SUB_STRING.toFloat();
    }

    pen_down();
    draw_arc_cw(X, Y, I, J);
  }

  // ------------------------------------------
  // G03   counter-clockwise arc with pen_down
  // ------------------------------------------
  if (INPUT_STRING.startsWith("G03")) {

    // ----- extract X
    START = INPUT_STRING.indexOf('X');
    if (!(START < 0)) {
      FINISH = INPUT_STRING.indexOf('.', INPUT_STRING.indexOf('X'));
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 7);
      X = SUB_STRING.toFloat();
    }

    // ----- extract Y
    START = INPUT_STRING.indexOf('Y');
    if (!(START < 0)) {
      FINISH = INPUT_STRING.indexOf('.', INPUT_STRING.indexOf('Y'));
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 7);
      Y = SUB_STRING.toFloat();
    }

    // ----- extract I
    START = INPUT_STRING.indexOf('I');
    if (!(START < 0)) {
      FINISH = INPUT_STRING.indexOf('.', INPUT_STRING.indexOf('I'));
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 7);
      I = SUB_STRING.toFloat();
    }

    // ----- extract J
    START = INPUT_STRING.indexOf('J');
    if (!(START < 0)) {
      FINISH = INPUT_STRING.indexOf('.', INPUT_STRING.indexOf('J'));
      SUB_STRING = INPUT_STRING.substring(START + 1, FINISH + 7);
      J = SUB_STRING.toFloat();
    }

    pen_down();
    draw_arc_ccw(X, Y, I, J);
  }

  // ----------------------------------
  // MENU
  // ----------------------------------
  if (INPUT_STRING.startsWith("MENU")) {
    menu();
  }

  // ----------------------------------
  // T1   pen up
  // ----------------------------------
  if (INPUT_STRING.startsWith("T1")) {
    pen_up();
  }

  // ----------------------------------
  // T2   pen down
  // ----------------------------------
  if (INPUT_STRING.startsWith("T2")) {
    pen_down();
  }

  // ----------------------------------
  // T3   draw square (100mm)
  // ----------------------------------
  if (INPUT_STRING.startsWith("T3")) {
    square();
  }

  // ----------------------------------
  // T4   all servos 0 degrees
  // ----------------------------------
  if (INPUT_STRING.startsWith("T4")) {
    servo1.angle(0.0);
    servo2.angle(1.0);
  }

  // ----------------------------------
  // T5   all servos 90 degrees
  // ----------------------------------
  if (INPUT_STRING.startsWith("T5")) {
    servo1.angle(90.0);
    servo2.angle(90.0);
  }

  // ----------------------------------
  // T6   all servos 180 degrees
  // ----------------------------------
  if (INPUT_STRING.startsWith("T6")) {
    servo1.angle(180.0);
    servo2.angle(180.0);
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
  THIS_X = round(x + Xoffset);
  THIS_Y = round(y + Yoffset);

  // ----- draw a line between these co-ordinates
  draw_line(LAST_X, LAST_Y, THIS_X, THIS_Y);

  // ----- remember last rounded co-ordinate
  LAST_X = THIS_X;
  LAST_Y = THIS_Y;
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
  float angleB = acos((sq(c) + sq(a) - sq(b)) / (2 * c * a));   // cosine formula

  // ----- convert to degrees
  float servo1_angle = (alpha + ((PI - angleB) / 2)) * RAD_TO_DEG;
  float servo2_angle = angleB * RAD_TO_DEG;

  //Serial.print(servo1_angle); Serial.print("\t"); Serial.println(servo2_angle);

  // ----- wait for interrupt (20mS intervals)
  while (Refresh_flag == false);
  Refresh_flag = false;             // flag was true to get here

  // ----- update servo angles between each refresh
  /* angles ready for next refresh */
  servo1.angle(servo1_angle);
  servo2.angle(servo2_angle);

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
    thisX = (LAST_X - Xoffset),  // true X co-ordinate
    thisY = (LAST_Y - Yoffset),  // true  Y co-ordinate
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
    if (arc > ARC_MAX) {
      segments = (int)(arc / ARC_MAX);
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
    thisX = (LAST_X - Xoffset),  // true X co-ordinate
    thisY = (LAST_Y - Yoffset),  // true Y co-ordinate
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
    if (arc > ARC_MAX) {
      segments = (int)(arc / ARC_MAX);
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

//---------------------------------------------------------------------------
// PEN_UP
//---------------------------------------------------------------------------
void pen_up() {
  pen_lift.angle(40);
}

//---------------------------------------------------------------------------
// PEN_DOWN
//---------------------------------------------------------------------------
void pen_down() {
  pen_lift.angle(0);
}

//----------------------------------------------------------------------------
// draw square
//----------------------------------------------------------------------------
void square() {

  // ----- draw square
  pen_down();
  move_to(0, 100);
  move_to(100, 100);
  move_to(100, 0);
  move_to(0, 0);
  pen_up();

  // ----- draw diagonals
  //move_to(100, 100);
  //pen_up();
  //move_to(0, 100);
  //pen_down();
  //move_to(100, 0);
  //pen_up();
  //move_to(0,0);
}
