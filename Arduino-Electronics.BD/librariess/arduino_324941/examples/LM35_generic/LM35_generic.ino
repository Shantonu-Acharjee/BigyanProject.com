/**
 * @file LM35_generic.ino
 * @author José Guerra Carmenate <joseguerracarmenate@gmail.com>
 * @brief Use of LM35 driver with Energesis_Sensor interface.
 * @version 0.1
 * @date 2022-05-31
 * 
 * @details 
 * +-------+-------------+-----------+-------------+
 * |       |   LM35(GND) | LM35(VIN) | LM35(VOUT)  |
 * +-------+-------------+-----------+-------------+
 * |Arduino|      GND    |   5V      |  A0         | 
 * +-------+-------------+-----------+-------------+
 * 
 * @copyright Copyright (c) 2022
 */
#include <Arduino.h>
#include <Energesis_LM35.h>

// for access Energesis namespace
using namespace Energesis;

//Energesis_LM35 lm35; 
Energesis_LM35 lm35( LM35_C ); // specify LM35 serie

Energesis_Sensor* sensor;

sensor_details_t det;
 
const char *type2str( sensor_type_t t ){
  switch (t) {
    case SENSOR_TYPE_TEMPERATURE:       return "Temperature";
    case SENSOR_TYPE_CURRENT:           return "Current";
    case SENSOR_TYPE_RELATIVE_HUMIDITY: return "Relative Humidity";
    case SENSOR_TYPE_VOLTAGE:           return "Voltage";
    default:                            return "Invalid";

  }
}

const char *type2unit( sensor_type_t t ){
  switch (t){
    case SENSOR_TYPE_TEMPERATURE:       return "ºC";
    case SENSOR_TYPE_CURRENT:           return "mA";
    case SENSOR_TYPE_RELATIVE_HUMIDITY: return "%";
    case SENSOR_TYPE_VOLTAGE:           return "V";
    default:                            return "Invalid";
  }
}

void printDetails (sensor_details_t &d){
  Serial.println("---------------------------------");
  Serial.print( "Name: " );
  Serial.println( String(d.name) + " -> " + type2str(d.type) );

  Serial.print( "Serial Number: " );
  Serial.println( (unsigned long)(d.sensor_id) );
  
  Serial.print( "Max value: " );
  Serial.print( d.max_value);
  Serial.println( type2unit( d.type ) );

  Serial.print( "Min value: " );
  Serial.print( d.min_value);
  Serial.println( type2unit( d.type ) );

  Serial.print( "Current consum[mA]: " );
  Serial.println( d.power );

  Serial.print( "Resolution: " );
  Serial.print( d.resolution );
  Serial.println( type2unit( d.type ) );

  Serial.println("-----------------------------------------");

}

void setup() {

  // init serial port
  Serial.begin(9600);
  delay(500);

  // init LM35 temperature sensor
  lm35.begin( A0, 10, 5000 );

  // get generic interface for sensor
  sensor = lm35.getTemperatureSensor();
  
  // print sensor details on serial monitor
  sensor->printSensorDetails(&Serial);

}

void loop() {
  sensor_sample_t sample;

  sensor->getSample(&sample);
  Serial.print( type2str( sample.type ) );
  Serial.print(": ");
  Serial.print( sample.valueFloat );
  Serial.println(String(" ") + type2unit(sample.type));

  delay(2000);
}
