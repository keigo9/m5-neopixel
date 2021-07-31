#include <M5Core2.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define M5STACK_MPU6886 // 6軸IMU for M5Core2
#define LED_PIN 32      // M5Core2 Grobe Pin
#define LED_COUNT 400 // Enough for LEDs
#define DELAYVAL 500   // Time (in milliseconds) to pause between pixels
#define gyroX_max 500
#define gyroX_min -500
#define gyroY_max 500
#define gyroY_min -500
#define gyroZ_max 500
#define gyroZ_min -500

//float accX = 0.0F;
//float accY = 0.0F;
//float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

//float pitch = 0.0F;
//float roll  = 0.0F;
//float yaw   = 0.0F;

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup(){

  // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(20); // (max = 255)

  // 初期化
  M5.begin();

  M5.IMU.Init();
  
  // 起動画面設定
  M5.Lcd.fillScreen(TFT_NAVY);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("START");

}

void loop() {
  // 画面初期化
  M5.Lcd.fillScreen(TFT_NAVY);
  
  angularVelocityInterrupt();
}

void angularVelocityInterrupt() {
  // 角度センサ（ジャイロ）の取得
  M5.IMU.getGyroData( &gyroX, &gyroY, &gyroZ );

  // 加速度センサの取得
//  M5.IMU.getAccelData( &accX, &accY, &accZ );

  // 姿勢角度センサの取得
//  M5.IMU.getAhrsData( &pitch, &roll, &yaw );

  if (gyroX > gyroX_max || gyroX < gyroX_min) {
    
    simpleLight(strip.Color(255,0,0)); //red    
    // 500ms待機
    delay(DELAYVAL);
    
  } else if (gyroY > gyroY_max || gyroY < gyroY_min) {
    
    simpleLight(strip.Color(0,255,0)); //blue
    // 500ms待機
    delay(DELAYVAL);
    
  } else if (gyroZ > gyroZ_max || gyroZ < gyroZ_min) {
    
    simpleLight(strip.Color(0,0,255)); //green
    // 500ms待機
    delay(DELAYVAL);
    
  } 
  simpleLight(strip.Color(100,100,100)); //white
  delay(20);
}

void simpleLight(uint32_t color) {
    for (int i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, color);
    }
    strip.show();
}
