#include <Adafruit_NeoPixel.h>

#define LED_PIN    1          // 数据引脚
#define LED_COUNT  37         // 灯带上的LED数目

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();           // 初始化灯带
  strip.setBrightness(30); // 设置亮度
  strip.show();            // 确保灯带关闭
  strip.fill(strip.Color(0, 0, 0)); // 全部熄灭
}

void loop() {
  countdown(); // 启动倒计时
  delay(10000); // 倒计时结束后，等待10秒再重新启动
}

void countdown() {
  // 灯带分为三段：灰、红、蓝
  int segmentSize = LED_COUNT / 3; // 每段的LED数量

  // 点亮所有LED，按段分配颜色
  for (int i = 0; i < LED_COUNT; i++) {
    if (i < segmentSize) {
      strip.setPixelColor(i, strip.Color(32, 32, 32)); // 灰色
    } else if (i < 2 * segmentSize) {
      strip.setPixelColor(i, strip.Color(155, 5, 80)); // 红色
    } else {
      strip.setPixelColor(i, strip.Color(0, 91, 155)); // 蓝色
    }
  }

  // 将第6个和第15个灯设置为黄色
  strip.setPixelColor(5, strip.Color(55, 55, 0)); // 第6个灯（索引从0开始）
  strip.setPixelColor(14, strip.Color(55, 55, 0)); // 第15个灯

  strip.show(); // 显示所有点亮的灯

  // 每隔一段时间逐个熄灭LED
  int timePerLED = 1; // 每个LED熄灭的时间，单位为秒
  for (int i = 0; i < LED_COUNT; i++) {
    delay(timePerLED * 1000); // 等待1秒
    strip.setPixelColor(i, 0); // 熄灭当前LED
    strip.show(); // 刷新灯带状态
  }
}
