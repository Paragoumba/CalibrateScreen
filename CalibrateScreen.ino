#include <U8g2lib.h>
#define OLED_RESET U8X8_PIN_NONE  // Reset pin
#define OLED_SDA 5
#define OLED_SCL 6
#define LED_BUILTIN 8
#define BOOT_BUTTON 9

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, OLED_RESET, OLED_SCL, OLED_SDA);
int width = 72;
int height = 40;
int xOffset = 30;  // = (132-w)/2
int yOffset = 24;  // = (64-h)/2

int c = 0;
int x = xOffset;
int y = yOffset;
int min_x = -1;
int min_y = -1;
int max_x = -1;
int max_y = -1;

void display_oled_size(int min_x, int max_x, int min_y, int max_y) {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_4x6_tr);
  char buffer[20];
  snprintf(buffer, sizeof(buffer), "minx%i maxx%i", min_x, max_x);
  u8g2.drawStr(xOffset + 0, yOffset + 20, buffer);
  snprintf(buffer, sizeof(buffer), "miny%i maxy%i", min_y, max_y);
  u8g2.drawStr(xOffset + 0, yOffset + 30, buffer);
  u8g2.sendBuffer();
}

void setup(void) {
  Serial.begin(115200);
  delay(1000);
  u8g2.begin();
  u8g2.setContrast(255);     // set contrast to maximum
  u8g2.setBusClock(400000);  //400kHz I2C
  u8g2.setDrawColor(2);
  Serial.println(String("LED_BUILTIN ") + LED_BUILTIN);
  Serial.println(String() + u8g2.getDisplayWidth() + "x" + u8g2.getDisplayHeight());
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BOOT_BUTTON, INPUT);
}

void loop(void) {
  if (digitalRead(BOOT_BUTTON) == LOW){
    if (min_x <= -1 || min_y <= -1){
      min_x = x;
      min_y = y;
      Serial.println(String("Min is x") + min_x + " y" + min_y);
    } else if (max_x <= -1 || max_y <= -1){
      max_x = x;
      max_y = y;
      Serial.println(String("Max is x") + max_x + " y" + max_y);
    }
  }

  if (min_x <= -1 || max_x <= -1){
    ++x;

    if (x > width + xOffset){
      x = xOffset;
      y++;
    }

    u8g2.clearBuffer();
    u8g2.drawVLine(x, y, height);
    u8g2.drawHLine(x, y, width);
    u8g2.sendBuffer();
  } else if (min_y <= -1 || max_y <= -1){
    ++y;
    u8g2.clearBuffer();
    u8g2.drawVLine(x, y, height);
    u8g2.drawHLine(x, y, width);
    u8g2.sendBuffer();
  } else {
    display_oled_size(min_x, max_x - 1, min_y, max_y);
  }

  c++;
  Serial.println(String("Moving to x") + x + " y" + y);

  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, digitalRead(BOOT_BUTTON));
  delay(500);
}