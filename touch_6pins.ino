#include "TouchyTouch.h"

const struct {
  uint8_t pin;         
  const char* func;    
  float minVal;        
  float maxVal;       
  uint8_t decimal;     
} TOUCH_CONFIG[] = {
  {6, "color1",      1.00, 200.00, 0},   
  {7, "color2",     1.00, 200.00, 0},   
  {8, "color3",  1.00, 200.00, 0},   
  {3, "color4",       1.00, 200.00, 0},   
  {4, "color5",    1.00, 200.00, 0},  
  {5, "color6",    1.00, 200.00, 0}    
};
// 自动计算引脚数量（新增/删除引脚无需修改此值）
const uint8_t CONFIG_COUNT = sizeof(TOUCH_CONFIG) / sizeof(TOUCH_CONFIG[0]);

// 2. 初始化触摸对象和状态（每个引脚独立）
TouchyTouch touchSensors[CONFIG_COUNT];
bool lastTouchStates[CONFIG_COUNT] = {false};  

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600); 
  while (!Serial) delay(10);  
  
  randomSeed(analogRead(A0));  
  
  Serial.println("=== Arduino 6引脚触摸控制（TD联动）===");
  for (int i = 0; i < CONFIG_COUNT; i++) {
    touchSensors[i].begin(TOUCH_CONFIG[i].pin);
    delay(100); 
    Serial.print("初始化：引脚");
    Serial.print(TOUCH_CONFIG[i].pin);
    Serial.print(" → 功能：");
    Serial.print(TOUCH_CONFIG[i].func);
    Serial.print("（范围：");
    Serial.print(TOUCH_CONFIG[i].minVal);
    Serial.print("~");
    Serial.print(TOUCH_CONFIG[i].maxVal);
    Serial.println("，整数）");
  }
  Serial.println("--- 触摸引脚即可输出1~200随机数 ---");
}

void loop() {
  for (int i = 0; i < CONFIG_COUNT; i++) {
    touchSensors[i].update(); 
    bool currentTouch = touchSensors[i].touched();  
    uint8_t currentPin = TOUCH_CONFIG[i].pin;      
    const char* currentFunc = TOUCH_CONFIG[i].func; 
    
    static unsigned long lastOutputTime[CONFIG_COUNT] = {0};
    if (currentTouch) {
      digitalWrite(LED_BUILTIN, HIGH);  
      
      if (millis() - lastOutputTime[i] > 200) {
        float paramVal = generateParam(TOUCH_CONFIG[i].minVal, TOUCH_CONFIG[i].maxVal, TOUCH_CONFIG[i].decimal);
        
        // 核心：结构化输出（格式：功能名,引脚号,参数值），TD直接解析
        Serial.print(currentFunc);
        Serial.print(",");
        Serial.print(currentPin);
        Serial.print(",");
        Serial.println(paramVal, TOUCH_CONFIG[i].decimal);
        
        lastOutputTime[i] = millis();  
      }
    }
    
    lastTouchStates[i] = currentTouch;
  }
  
  if (allPinsReleased()) {
    digitalWrite(LED_BUILTIN, LOW);
  }
  
  delay(50);  // 全局防抖延迟，减少误触发
}

float generateParam(float min, float max, uint8_t decimal) {
  int scale = pow(10, decimal);  // 缩放系数（decimal=0→1，decimal=2→100）
  int minInt = min * scale;      // 最小值转整数（1.00→1，1.00→100<小数>）
  int maxInt = max * scale;      // 最大值转整数（200.00→200，200.00→20000<小数>）
  
  int randomInt = random(minInt, maxInt + 1);  // 生成1~200的整数（含200）
  return (float)randomInt / scale;             // 转回目标格式（整数/小数）
}

// 辅助函数2：判断所有引脚是否都释放
bool allPinsReleased() {
  for (int i = 0; i < CONFIG_COUNT; i++) {
    if (touchSensors[i].touched()) {
      return false;
    }
  }
  return true;
}