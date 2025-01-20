#include <SoftwareSerial.h> // we include SoftwareSerial 
#include <TinyGPS.h> // we include TinyGPS

TinyGPS gps; // We declare the object gps 
SoftwareSerial serialgps(4,3); // We declare pin 4 Tx and 3 Rx 

// We declare the variables to obtain data 
int  year ;
byte  month , day , hour , minute , second , hundredths;
unsigned  long chars;
unsigned  short sentences, failed_checksum;

void  setup ()
{
  Serial . begin (115200); // We start the 
  serialgps.begin (9600); // Start the gps serial port 
  // Print: 
  Serial . println ( "" );
  Serial . println ( "GPS GY-GPS6MV2 Leantec" );
  Serial . println ( "--- Looking for signal ---" );
  Serial . println ( "" );
}

void  loop ()
{
  while (serialgps. available ())
  {
    int c = serialgps. read (); 
 
    if (gps. encode (c))  
    {
      float latitude, longitude;
      gps.f_get_position (& latitude, & longitude);
      Serial . print ( "Latitude / Longitude:" ); 
      Serial . print (latitude, 5); 
      Serial . print ( "," ); 
      Serial . println (longitude, 5); 


  gps.crack_datetime (& year , & month , & day , & hour , & minute , & second , & hundredths);
      Serial . print ( "Date:"); Serial . print ( day , DEC ); Serial . print ( "/" ); 
      Serial . print ( month , DEC ); Serial . print ( "/" ); Serial . print ( year );
      Serial . print ( "Time:" ); Serial . print ( hour , DEC ); Serial .print ( ":" ); 
      Serial . print ( minute , DEC ); Serial . print ( ":" ); Serial . print ( second , DEC ); 
      Serial . print ( "." ); Serial . println (hundredths, DEC );
      Serial . print ( "Altitude (meters):" ); 
      Serial . println (gps.f_altitude ()); 
      Serial . print ( "Course (degrees):" ); Serial . println (gps. f_course ()); 
      Serial . print ( "Speed ​​(kmph):" ); 
      Serial . println (gps. f_speed_kmph ());
      Serial . print ( "Satellites:" ); Serial . println (gps. satellites ());
      Serial . println ();
      gps.stats (& chars, & sentences, & failed_checksum);  
    }
  }
}
