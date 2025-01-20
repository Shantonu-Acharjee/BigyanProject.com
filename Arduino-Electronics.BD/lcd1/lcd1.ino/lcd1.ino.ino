#include <Keypad.h>
#include <LiquidCrystal.h>
#include <EtherCard.h>
byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x01, 0x01 };
const char website[] PROGMEM = "192.168.0.102";
byte Ethernet::buffer[700];
Stash stash;
char* username;
char* password;
static byte session;
const byte rows = 4;
const byte cols = 4;
char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[rows] = {A4, A5, A6, A7};
byte colPins[cols] = {A3, A2, A1, A0};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, rows, cols);
LiquidCrystal lcd(A8, A9, A10, A11, A12, A13);
int currentPosition = 0;
int flag_user = 0;
int flag_pass = 0;
int flag_check = 0;
const int currentPassword = 0;
const int lock = A15;
void setup() {
  Serial.begin(57600);
  if (ether.begin(sizeof Ethernet::buffer, mymac, 53) == 0)
    Serial.println(F("Failed to access Ethernet controller"));
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");
  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);
  ether.parseIp(ether.hisip, "192.168.0.102");
  ether.printIp("SRV: ", ether.hisip);
  lcd.begin(20, 4);
  displayCodeEntryScreen();

  pinMode(lock, OUTPUT);
}

void loop() {
  int l;
  int i;
  char key = keypad.getKey();
  if (int(key) != 0) {
    if (key == 'A')
    {
      if (flag_user == 0 && flag_pass == 0 && flag_check == 0)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("********************");
        lcd.setCursor(0, 1);
        lcd.print("**  YOUR USERID:  **");
        lcd.setCursor(0, 2);
        lcd.print("**                **");
        lcd.setCursor(0, 3);
        //lcd.print("********************");
        lcd.print(username);
        currentPosition = 0;
        flag_user = 1;
        flag_pass = 0;
        flag_check = 0;
      }
      else if (flag_user == 1 && flag_pass == 0 && flag_check == 0)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("********************");
        lcd.setCursor(0, 1);
        lcd.print("** YOUR PASSWORD: **");
        lcd.setCursor(0, 2);
        lcd.print("**                **");
        lcd.setCursor(0, 3);
        //lcd.print("********************");
        
        lcd.print(password);
        currentPosition = 0;
        flag_user = 1;
        flag_pass = 1;
        flag_check = 0;
      }
      else if (flag_user == 1 && flag_pass == 1 && flag_check == 0)
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("********************");
        lcd.setCursor(0, 1);
        lcd.print("**  PLEASE WAIT.  **");
        lcd.setCursor(0, 2);
        lcd.print("**AUTHENTICATING..**");
        lcd.setCursor(0, 3);
        lcd.print(username);
        sendToSweety(username,password);
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("********************");
        lcd.setCursor(0, 1);
        lcd.print("**      SORRY     **");
        lcd.setCursor(0, 2);
        lcd.print("**I CAN'T READ YOU**");
        lcd.setCursor(0, 3);
        lcd.print("********************");
        resetall();
      }
    }
    else if (key == '0' || key == '1' || key == '2' || key == '3' || key == '4' || key == '5' || key == '6' || key == '7' || key == '8' || key == '9')
    {
      if (flag_user == 1 && flag_pass == 0 && flag_check == 0)
      {
        append_username(key);
        lcd.print(username);
      }
      else if (flag_user == 1 && flag_pass == 1 && flag_check == 0)
      {
        append_password(key);
        lcd.print(password);
      }
    }
    else if (key == 'B')
    {
      if (flag_user == 0 && flag_pass == 0 && flag_check == 0)
      {
       //memset(username, 0, sizeof(username));
      }
      else if (flag_user == 1 && flag_pass == 0 && flag_check == 0)
      {
       // memset(password, 0, sizeof(password));
      }
      lcd.setCursor(0, 2);
      lcd.print("**                **");
      currentPosition = 0;
    }
    else if (key == 'C')
    {
      resetall();
    }
  }
  ether.packetLoop(ether.packetReceive());

  const char* reply = ether.tcpReply(session);
  if (reply != 0) {
    Serial.println(reply);
    if(strstr(reply, "Correct") != NULL) {
      unlockDoor();
    }
    else
    {
      invalidCode();      
    }
  }
}
void sendToSweety (char* userid,char* password) {
  byte sd = stash.create();
  stash.save();
  int stash_size = stash.size();
  Stash::prepare(PSTR("GET http://$F/udp.php?userid=$D&password=$D HTTP/1.1" "\r\n"
                      "Host: $F" "\r\n"
                      "Content-Length: $D" "\r\n"
                      "\r\n"
                      "$H"),
                 website, userid, password, website,  stash_size, sd);
  session = ether.tcpSend();
}
void append_username(int key)
{
  int l;
  int i;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("********************");
  lcd.setCursor(0, 1);
  lcd.print("**  YOUR USERID:  **");
  lcd.setCursor(0, 2);
  lcd.print("**");
  int count1 = 18 - currentPosition;
  int count2 = count1 / 2;
  for (i = 2; i <= count2; i++)
  {
    lcd.print(" ");
  }
  for (l = 0; l <= currentPosition; ++l)
  {
    lcd.print("#");
  }
  if (currentPosition % 2)
  {
    for (i = 2; i <= count2; i++)
    {
      lcd.print(" ");
    }
  }
  else
  {
    for (i = 3; i <= count2; i++)
    {
      lcd.print(" ");
    }
  }
  lcd.print("**");
  lcd.setCursor(0, 3);
  //lcd.print("********************");
  lcd.print(username);
  username += key;
  ++currentPosition;
}
void append_password(int key)
{
  int l;
  int i;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("********************");
  lcd.setCursor(0, 1);
  lcd.print("** YOUR PASSWORD: **");
  lcd.setCursor(0, 2);
  lcd.print("**");
  int count1 = 18 - currentPosition;
  int count2 = count1 / 2;
  for (i = 2; i <= count2; i++)
  {
    lcd.print(" ");
  }
  for (l = 0; l <= currentPosition; ++l)
  {
    lcd.print("#");
  }
  if (currentPosition % 2)
  {
    for (i = 2; i <= count2; i++)
    {
      lcd.print(" ");
    }
  }
  else
  {
    for (i = 3; i <= count2; i++)
    {
      lcd.print(" ");
    }
  }
  lcd.print("**");
  lcd.setCursor(0, 3);
  lcd.print("********************");
  password += key;
  ++currentPosition;
}
void invalidCode()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("********************");
  lcd.setCursor(0, 1);
  lcd.print("** ACCESS DENIED! **");
  lcd.setCursor(0, 2);
  lcd.print("**  INVALID CODE  **");
  lcd.setCursor(0, 3);
  lcd.print("********************");

  delay(5000);
  resetall();
}

void unlockDoor()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("********************");
  lcd.setCursor(0, 1);
  lcd.print("** ACCESS GRANTED **");
  lcd.setCursor(0, 2);
  lcd.print("**   WELCOME!!    **");
  lcd.setCursor(0, 3);
  lcd.print("********************");
  digitalWrite(lock,HIGH);
  delay(5000);
  digitalWrite(lock,LOW);
  resetall();
}

void resetall()
{
  currentPosition = 0;
  flag_user = 0;
  flag_pass = 0;
  flag_check = 0;
  //memset(password, 0, sizeof(password));
  //memset(username, 0, sizeof(username));
  displayCodeEntryScreen();
}
void displayCodeEntryScreen()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("********************");
  lcd.setCursor(0, 1);
  lcd.print("  WELCOME TO LOCK");
  lcd.setCursor(0, 2);
  lcd.print(" PLEASE PRESS ENTER");
  lcd.setCursor(0, 3);
  lcd.print("********************");
}
