#include <WiFiNINA.h>
#include <Arduino_JSON.h>

// WiFi 网络信息
const char* ssid = "QAKIGHT";     // 替换为你的WiFi SSID
const char* password = "qhb12345678"; // 替换为你的WiFi密码

// API URL 信息
const char* api_host = "www.gov.uk";
const char* api_path = "/bank-holidays.json";

WiFiClient client;


void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // 等待串口初始化
  }

  // 连接 WiFi
  connectToWiFi();

  // 请求并解析 JSON 数据
  fetchAndParseHolidays();
}

void loop() {
  // 空循环
}

void connectToWiFi() {
  Serial.print("正在连接到 WiFi: ");
  Serial.println(ssid);

  while (WiFi.begin(ssid, password) != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\n已成功连接到 WiFi");
  Serial.print("IP 地址: ");
  Serial.println(WiFi.localIP());
}

void fetchAndParseHolidays() {
  Serial.println("正在连接到 API...");

  if (client.connect(api_host, 80)) {
    // 发起 HTTP GET 请求
    client.print("GET ");
    client.print(api_path);
    client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(api_host);
    client.println("Connection: close");
    client.println(); // 空行表示请求结束

    // 等待响应
    while (!client.available()) {
      delay(100);
    }

    // 读取 HTTP 响应
    String response = "";
    while (client.available()) {
      response += client.readString();
    }

    client.stop();

    // 提取 JSON 内容
    int jsonStart = response.indexOf("\r\n\r\n");
    if (jsonStart == -1) {
      Serial.println("未找到 JSON 数据");
      return;
    }

    String json = response.substring(jsonStart + 4);

    // 打印完整 JSON 数据
    Serial.println("收到的 JSON 数据:");
    Serial.println(json);

    // 解析 JSON 数据
    parseJSON(json);
  } else {
    Serial.println("无法连接到 API 服务器");
  }
}

void parseJSON(String json) {
  JSONVar data = JSON.parse(json);

  if (JSON.typeof(data) == "undefined") {
    Serial.println("JSON 解析失败");
    return;
  }

  // 提取 "england-and-wales" 假期
  JSONVar events = data["england-and-wales"]["events"];
  if (events == nullptr) {
    Serial.println("未找到假期事件数据");
    return;
  }

  // 查找并打印假期信息
  for (int i = 0; i < events.length(); i++) {
    String title = (const char*)events[i]["title"];
    String date = (const char*)events[i]["date"];
    String notes = (const char*)events[i]["notes"];
    bool bunting = (bool)events[i]["bunting"];

    Serial.println("假期信息:");
    Serial.print("名称: ");
    Serial.println(title);
    Serial.print("日期: ");
    Serial.println(date);
    Serial.print("备注: ");
    Serial.println(notes);
    Serial.print("是否悬挂旗帜: ");
    Serial.println(bunting ? "是" : "否");
    Serial.println("---------------------------");
  }
}