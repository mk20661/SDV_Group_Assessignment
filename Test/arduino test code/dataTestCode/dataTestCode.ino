#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

// Wi-Fi 网络信息
const char* ssid = "YOUR_SSID";           // 替换为您的Wi-Fi SSID
const char* password = "YOUR_PASSWORD";   // 替换为您的Wi-Fi密码

// NTP 客户端设置
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", 0, 60000); // UTC 时间，更新频率60秒

// 获取学期开始日期：每年9月的第5周星期一
tmElements_t getTermStart(int year) {
    tmElements_t termStart;
    termStart.Year = year - 1970;  // 年份从1970开始计算
    termStart.Month = 9;          // 9月
    termStart.Day = 1;            // 初始化为9月1日
    termStart.Hour = 0;
    termStart.Minute = 0;
    termStart.Second = 0;

    // 将日期转换为时间戳
    time_t rawTime = makeTime(termStart);

    // 找到第5周的星期一
    int weekCount = 0;
    for (int i = 0; i < 30; i++) {  // 遍历整个9月
        if (weekday(rawTime) == 2) { // 星期一（2表示Monday）
            weekCount++;
            if (weekCount == 5) { // 第5个星期一
                break;
            }
        }
        rawTime += SECS_PER_DAY;  // 每次加一天
    }
    termStart.Day = day(rawTime); // 更新日期为第5周星期一
    return termStart;
}

// 计算当前是学期的第几周
int calculateWeek(tmElements_t termStart, tmElements_t currentDate) {
    // 将日期转换为时间戳
    time_t termStartTime = makeTime(termStart);
    time_t currentTime = makeTime(currentDate);

    if (currentTime < termStartTime) {
        return -1;  
    }

    // 计算天数差
    int daysDifference = (currentTime - termStartTime) / SECS_PER_DAY;

    // 计算第几周
    int weekNumber = daysDifference / 7 + 1;
    return weekNumber;
}

void setup() {
    Serial.begin(115200);
    
    // 连接 Wi-Fi
    Serial.print("Connecting Wi-Fi: ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println();
    Serial.println("Wi-Fi connected");
    Serial.print("设备 IP address: ");
    Serial.println(WiFi.localIP());

    // 启动 NTP 客户端
    timeClient.begin();
    while (!timeClient.update()) {
        timeClient.forceUpdate();
    }

    // 获取当前日期
    unsigned long epochTime = timeClient.getEpochTime(); // 获取当前时间（时间戳）
    tmElements_t currentDate;
    breakTime(epochTime, currentDate); // 将时间戳转换为 tmElements_t 格式

    // 获取当前年份
    int currentYear = currentDate.Year + 1970;

    // 获取学期开始日期
    tmElements_t termStart = getTermStart(currentYear);

    // 计算当前是第几周
    int weekNumber = calculateWeek(termStart, currentDate);

    // 输出结果
    Serial.print("Term from ");
    Serial.print(termStart.Year + 1970);
    Serial.print("-");
    Serial.print(termStart.Month);
    Serial.print("-");
    Serial.print(termStart.Day);
    Serial.println(" Start");

    Serial.print("Today is ");
    Serial.print(currentDate.Year + 1970);
    Serial.print("-");
    Serial.print(currentDate.Month);
    Serial.print("-");
    Serial.print(currentDate.Day);
    if (weekNumber > 0) {
        Serial.print(" ，it is ");
        Serial.print(weekNumber);
        Serial.println(" Week");
    } else {
        Serial.println(" ，The current date is earlier than the semester start date");
    }
}

void loop() {
    delay(60000); 
}
