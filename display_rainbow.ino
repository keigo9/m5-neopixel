#include <M5Core2.h>
#include <Adafruit_NeoPixel.h>

#define M5STACK_MPU6886 // 6軸IMU for M5Core2
#define PIN 32        // M5Core2 Grobe Pin
#define NUMPIXELS 30 // Enough for LEDs
#define DELAYVAL 500   // Time (in milliseconds) to pause between pixels
#define gyroX_max 500
#define gyroX_min -500
#define gyroY_max 500
#define gyroY_min -500
#define gyroZ_max 500
#define gyroZ_min -500
// RGB config
int led_color[3][3] = {{10, 0, 0}, {0, 10, 0}, {0, 0, 10}};

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

/////////////////////////////////////
void setup_serial(){

  Serial.begin(115200);
  while (!Serial);

}

void setup(){

  pixels.begin();

  // 初期化
  M5.begin();

  setup_serial();

  M5.IMU.Init();
  
  // 起動画面設定
  M5.Lcd.fillScreen(TFT_NAVY);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("START");

}

void loop() {
  float accX, accY, accZ;
  float gyroX, gyroY, gyroZ;
  float pitch, roll, yaw;

  // 画面初期化
  M5.Lcd.fillScreen(TFT_NAVY);

  pixels.clear();

  for (int i = 0; i < NUMPIXELS; i++)
  {
    int rgb = i % 3;
    pixels.setPixelColor(i, pixels.Color(led_color[rgb][0], led_color[rgb][1], led_color[rgb][2]));
  }
  pixels.show();
  
  delay(10);
  
  // 角度センサ（ジャイロ）の取得
  M5.IMU.getGyroData( &gyroX, &gyroY, &gyroZ );

  // 画面表示
//  M5.Lcd.setCursor(0, 0);
//  M5.Lcd.printf("gyro=(%5.1f, %5.1f, %5.1f)", gyroX, gyroY, gyroZ);
//  // シリアル通信出力
//  Serial.printf("gyro=(%5.1f, %5.1f, %5.1f)", gyroX, gyroY, gyroZ);
//  Serial.println();

  // 加速度センサの取得
//  M5.IMU.getAccelData( &accX, &accY, &accZ );

  // 画面表示
//  M5.Lcd.setCursor(0, 50);
//  M5.Lcd.printf("acc=(%5.1f, %5.1f, %5.1f)", accX, accY, accZ);
//  // シリアル通信出力
//  Serial.printf("acc=(%5.1f, %5.1f, %5.1f)", accX, accY, accZ);
//  Serial.println();

  // 姿勢角度センサの取得
//  M5.IMU.getAhrsData( &pitch, &roll, &yaw );

  // 画面表示
//  M5.Lcd.setCursor(0, 100);
//  M5.Lcd.printf("PRY=(%5.1f, %5.1f, %5.1f)", pitch, roll, yaw);
//  // シリアル通信出力
//  Serial.printf("PRY=(%5.1f, %5.1f, %5.1f)", pitch, roll, yaw);
//  Serial.println();

  if (gyroX > gyroX_max || gyroX < gyroX_min) {
    M5.Lcd.fillScreen(TFT_NAVY);
    for (int i = 0; i < NUMPIXELS; i++)
    {
      int rgb = i % 3;
      pixels.setPixelColor(i, pixels.Color(255,0,0));
    }
    pixels.show();

    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("gyroX=(%5.1f)", gyroX);
    
    // 500ms待機
    delay(DELAYVAL);
    
  } else if (gyroY > gyroY_max || gyroY < gyroY_min) {
    
    M5.Lcd.fillScreen(TFT_NAVY);
    for (int i = 0; i < NUMPIXELS; i++)
    {
      int rgb = i % 3;
      pixels.setPixelColor(i, pixels.Color(0,255,0));
    }
    pixels.show();

    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("gyroY=(%5.1f)", gyroY);
    
    // 500ms待機
    delay(DELAYVAL);
    
  } else if (gyroZ > gyroZ_max || gyroZ < gyroZ_min) {
    
    M5.Lcd.fillScreen(TFT_NAVY);
    for (int i = 0; i < NUMPIXELS; i++)
    {
      int rgb = i % 3;
      pixels.setPixelColor(i, pixels.Color(0,0,255));
    }
    pixels.show();

    M5.Lcd.setCursor(0, 0);
    M5.Lcd.printf("gyroZ=(%5.1f)", gyroZ);
    
    // 500ms待機
    delay(DELAYVAL);
    
  } 
  

//  M5.Lcd.setCursor(0, 0);
//  M5.Lcd.printf("gyro=(%5.1f, %5.1f, %5.1f)", gyroX, gyroY, gyroZ);
//  delay(1000);
    
  
//  for (int i = 0; i < NUMPIXELS; i++)
//  {
//    int rgb = i % 3;
//    pixels.setPixelColor(i, pixels.Color(led_color[rgb][0], led_color[rgb][1], led_color[rgb][2]));
//  }
//  pixels.show();
//  
//  // 500ms待機
//  delay(DELAYVAL);
  
 

}
