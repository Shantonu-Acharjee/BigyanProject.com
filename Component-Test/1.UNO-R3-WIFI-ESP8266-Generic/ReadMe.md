৮ নাম্বার পিন চালু থাকলে এটি বুট মোডে চালু হবে(wifi এর সাথে কানেক্ট করার জন্য)



Node MCU তে কোড আপলোড করার সময় ৭,৮ সুইচ অন করতে হবে
কোড আপলোড হয়ে গেলে ৭-৮ বন্ধ করে দিতে হবে।

আরডুইনোতে আপলোড করার সময় ৫-৬ চালু থাকবে(শিউর না)


Mode 1 – USB Serial connected to ATMega328 for both programming and serial communication
Mode 2 – USB Serial connected to ESP8266 for programming
Mode 3 – USB Serial connected to ESP8266 for serial communication
Mode 4 – ATMega328 and ESP8266 serial connected together, allowing for serial



Mode 1 - USB to AT - SW3 and SW4 on. All other off
Mode 2 - USB to ESP Prog - SW5, SW6, SW7 on. All other off
Mode 3 - USB to ESP - SW5, SW6 on. All other off
Mode 4 - AT to ESP - SW1, SW2 on. All other off


------------------------------------------------------------------------------------------

Mode 1: USB to ATMega328 (Programming & Communication){Select Just Arduino Uno}
Switches ON: SW3, SW4
Purpose: Program or communicate with the Arduino (ATMega328) via USB.


Mode 2: USB to ESP8266 (Programming the ESP8266){Have to select Generic ESP8266 Module}
Switches ON: SW5, SW6, SW7
Purpose: Program the ESP8266 via USB.


Mode 3: USB to ESP8266 (Serial Communication with ESP8266)
Switches ON: SW5, SW6
Purpose: Communicate with the ESP8266 via USB.



Mode 3: USB to ESP8266 (Serial Communication with ESP8266)
Switches ON: SW5, SW6
Purpose: Communicate with the ESP8266 via USB.