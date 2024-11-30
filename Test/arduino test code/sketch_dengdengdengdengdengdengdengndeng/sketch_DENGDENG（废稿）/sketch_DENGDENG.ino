#include <Adafruit_NeoPixel.h>

// 配置灯带
#define LED_PIN    1          // 数据引脚
#define LED_COUNT  37         // 灯带上的LED数目（根据实际灯带数量调整）

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();            // 初始化灯带
  strip.setBrightness(20);
  strip.show();             // 确保灯带关闭
  strip.fill(strip.Color(0, 0, 0)); // 全部熄灭
}

void loop() {
  countdown();              // 开始倒计时
  delay(10000);             // 倒计时结束后，等待10秒再重新启动（根据需求修改）
}

void countdown() {
  int activeLEDs = LED_COUNT; // 初始点亮所有LED
  int totalLEDs = LED_COUNT;  // 总LED数
  int timePerLED = 1; // 每个LED熄灭的时间（秒）

  // 初始设置为灰色
  for (int i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(32, 32, 32)); // 灰色
  }
  strip.show();

  // 每隔一段时间逐个熄灭
  for (int i = 0; i < totalLEDs; i++) {
    delay(timePerLED * 1000); // 等待每个LED的时间（毫秒）
    strip.setPixelColor(i, 0); // 熄灭当前LED
    activeLEDs--;             // 更新剩余点亮的LED数量

    // 当剩余点亮的LED数量达到三分之二时，变为粉色
    if (activeLEDs == (2 * totalLEDs) / 3) {
      setColorForActiveLEDs(255, 105, 180); // 粉色
    }

    // 当剩余点亮的LED数量达到三分之一时，变为蓝色
    if (activeLEDs == totalLEDs / 3) {
      setColorForActiveLEDs(0, 191, 255); // 蓝色
    }

    strip.show();
  }
}

// 将剩余点亮的LED更改为指定颜色
void setColorForActiveLEDs(int red, int green, int blue) {
  for (int i = 0; i < LED_COUNT; i++) {
    if (strip.getPixelColor(i) != 0) { // 仅改变当前未熄灭的LED颜色
      strip.setPixelColor(i, strip.Color(red, green, blue));
    }
  }
}
