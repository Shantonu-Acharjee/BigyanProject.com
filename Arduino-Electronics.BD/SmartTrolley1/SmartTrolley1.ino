#include "U8glib.h"  // Include U8glib library
// Create display and set pins:
U8GLIB_PCD8544 u8g(52, 51, 45, 47, 49);   // (CLK=13, DIN=11, CE=10, DC=9, RST=8)
int product_count = 0;
int cart_row = 0;
const char* product_names[][200] =         {
            { "", "", ""},
            { "ITEM 1", "1", "100"},
            { "ITEM 2", "2", "200" },
            { "ITEM 3", "3", "300"},
        };
int cart[3];
int total;
void setup(void)  // Start of setup
{
  Serial.begin(115200);
  u8g.setFont(u8g_font_6x10);
  u8g.setRot180();
  
}  // End of setup
void loop(void)  // Start of loop
{  
  u8g.firstPage(); // Beginning of the picture loop
  do  // Include all you want to show on the display:
  {
    u8g.setFont(u8g_font_5x7);
    u8g.drawStr(0, 7, "  SHOPPING CART");
    u8g.setFont(u8g_font_4x6);
    u8g.drawStr(0, 14, "ITEM");
    u8g.drawStr(40, 14, "QTY");
    u8g.drawStr(60, 14, "PRICE");
    u8g.drawLine(0, 15, 83, 15);
    u8g.drawLine(37, 8, 37, 40);
    u8g.drawLine(57, 8, 57, 40);
    u8g.drawLine(0, 40, 83, 40);
    u8g.drawStr(35, 48, "TOTAL=");
    u8g.setPrintPos(60, 48);  // (x,y)
    u8g.print(total);
    Serial.println("-------------------------");
    for(size_t i=0; i<=sizeof(cart) / sizeof(cart[0]); i++){
      if(cart[i] == '\0'){
      break; 
      }
    Serial.println(i);
      if(i==0){
        u8g.drawStr(0, 23, product_names[cart[i]][0]); 
        u8g.drawStr(40, 23, product_names[cart[i]][1]); 
        u8g.drawStr(60, 23, product_names[cart[i]][2]); 
      }
      if(i==1){
        u8g.drawStr(0, 31, product_names[cart[i]][0]); 
        u8g.drawStr(40, 31, product_names[cart[i]][1]); 
        u8g.drawStr(60, 31, product_names[cart[i]][2]); 
      }
      if(i==2){
        u8g.drawStr(0, 39, product_names[cart[i]][0]); 
        u8g.drawStr(40, 39, product_names[cart[i]][1]); 
        u8g.drawStr(60, 39, product_names[cart[i]][2]); 
      }
    }    
  } while(u8g.nextPage());  // End of the picture loop  
  
    Serial.println("=========================");
    Serial.println(product_count);
  if(product_count == 0){
    cart[0] = 1;  
    total = total + product_names[1][2];
  }
  if(product_count == 1){
    cart[1] = 2;   
    total = total + product_names[2][2];
  }
  if(product_count == 2){
    cart[2] = 3;   
    total = total + product_names[3][2];
  }

  product_count++;
  if(product_count > 4){
    product_count = 0;
    clearOLED();
    cart[0] = 0;
    cart[1] = 0;
    cart[2] = 0;
    total = 0;
  }
  delay(5000);
  
}  // End of loop
void clearOLED(){
  u8g.firstPage();
  do {
    u8g.setFont(u8g_font_5x7);
    u8g.drawStr(0, 7, "  SHOPPING CART");
    u8g.setFont(u8g_font_4x6);
    u8g.drawStr(0, 14, "ITEM");
    u8g.drawStr(40, 14, "QTY");
    u8g.drawStr(60, 14, "PRICE");
    u8g.drawLine(0, 15, 83, 15);
    u8g.drawLine(37, 8, 37, 40);
    u8g.drawLine(57, 8, 57, 40);
    u8g.drawLine(0, 40, 83, 40);
    u8g.drawStr(35, 48, "TOTAL=");
  } while( u8g.nextPage() );
}
