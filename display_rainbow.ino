#include <M5Core2.h>
#include <Adafruit_NeoPixel.h>
#include <time.h>
#include <WiFi.h>
#ifdef __AVR__
#include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define M5STACK_MPU6886 // 6軸IMU for M5Core2
#define LED_PIN 32      // M5Core2 Grobe Pin
#define LED_COUNT 400   // Enough for LEDs
#define DELAYVAL 500    // Time (in milliseconds) to pause between pixels
#define gyroX_max 150
#define gyroX_min -150
#define gyroY_max 150
#define gyroY_min -150
#define gyroZ_max 150
#define gyroZ_min -150

//float accX = 0.0F;
//float accY = 0.0F;
//float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

//float pitch = 0.0F;
//float roll  = 0.0F;
//float yaw   = 0.0F;

char ssid[] = "圭悟のiPhone";
char pass[] = "keigo111";

// Time
char ntpServer[] = "ntp.jst.mfeed.ad.jp";
const long gmtOffset_sec = 9 * 3600;
const int daylightOffset_sec = 0;
struct tm timeinfo;
String dateStr;
String timeStr;

File file;

// 保存するファイル名
const char *fname = "/1020.csv";

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void getTimeFromNTP()
{
  // NTPサーバと時刻を同期
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (!getLocalTime(&timeinfo))
  {
    delay(1000);
  }
}

void getTime()
{
  // 時刻の取得と表示
  getLocalTime(&timeinfo);
  dateStr = (String)(timeinfo.tm_year + 1900) + "/" + (String)(timeinfo.tm_mon + 1) + "/" + (String)timeinfo.tm_mday;
  timeStr = (String)timeinfo.tm_hour + ":" + (String)timeinfo.tm_min + ":" + (String)timeinfo.tm_sec;

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(0, 50, 1);
  M5.Lcd.println(dateStr + "   ");
  M5.Lcd.println(timeStr + "   ");
}

void writeData(float paramStr, float paramStr2, float paramStr3)
{
  // SDカードへの書き込み処理（ファイル追加モード）
  // SD.beginはM5.begin内で処理されているので不要
  file = SD.open(fname, FILE_APPEND);
  file.println(dateStr + "," + timeStr + "," + paramStr + "," + paramStr2 + "," + paramStr3);
  file.close();
}

void setup()
{

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
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(4);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.printf("START");

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    M5.Lcd.print(".");
  }
  M5.Lcd.println("\nWiFi connected.");

  // timeSet
  getTimeFromNTP();
}

void loop()
{
  M5.update();
  getTime();
  M5.Lcd.setCursor(0, 100, 1);

  angularVelocityInterrupt();
}

void angularVelocityInterrupt()
{
  // 角度センサ（ジャイロ）の取得
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
  // 加速度センサの取得
  //  M5.IMU.getAccelData( &accX, &accY, &accZ );
  // 姿勢角度センサの取得
  //  M5.IMU.getAhrsData( &pitch, &roll, &yaw );

  if (gyroX > gyroX_max || gyroX < gyroX_min)
  {
    writeData(gyroX, gyroY, gyroZ);
    theaterChase(strip.Color(255, 0, 0), 500); //red
  }
  else if (gyroY > gyroY_max || gyroY < gyroY_min)
  {
    writeData(gyroX, gyroY, gyroZ);
    theaterChase(strip.Color(0, 255, 0), 500); //green
  }
  else if (gyroZ > gyroZ_max || gyroZ < gyroZ_min)
  {
    writeData(gyroX, gyroY, gyroZ);
    theaterChase(strip.Color(0, 0, 255), 500); //blue
  }
  simpleLight(strip.Color(100, 100, 100)); //white
  writeData(gyroX, gyroY, gyroZ);
  delay(20);
}

void simpleLight(uint32_t color)
{
  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, color);
  }
  strip.show();
  //    delay(wait);
}

void theaterChase(uint32_t color, int wait)
{
  for (int a = 0; a < 1; a++)
  { // Repeat 10 times...
    for (int b = 0; b < 3; b++)
    {                //  'b' counts from 0 to 2...
      strip.clear(); //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in steps of 3...
      for (int c = b; c < strip.numPixels(); c += 3)
      {
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show(); // Update strip with new contents
      delay(wait);  // Pause for a moment
    }
  }
}

void rainbow(int wait)
{
  // Hue of first pixel runs 5 complete loops through the color wheel.
  // Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
  for (long firstPixelHue = 0; firstPixelHue < 5 * 65536; firstPixelHue += 256)
  {
    for (int i = 0; i < strip.numPixels(); i++)
    { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the single-argument hue variant. The result
      // is passed through strip.gamma32() to provide 'truer' colors
      // before assigning to each pixel:
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait)
{
  int firstPixelHue = 0; // First pixel starts at red (hue 0)
  for (int a = 0; a < 30; a++)
  { // Repeat 30 times...
    for (int b = 0; b < 3; b++)
    {                //  'b' counts from 0 to 2...
      strip.clear(); //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for (int c = b; c < strip.numPixels(); c += 3)
      {
        // hue of pixel 'c' is offset by an amount to make one full
        // revolution of the color wheel (range 65536) along the length
        // of the strip (strip.numPixels() steps):
        int hue = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color);                       // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}