#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include <PulseSensorPlayground.h>
#include <SPI.h>
#include <Wire.h>

const String ssid = "Wizdem";
const String password = "wizdemroxx";
const String url = "http://192.168.43.209:8000/api/post_data/1";

const float text_size = 1.0;
float cur_lat, cur_lon;
int fall;

//const uint8_t scl = D6;
//const uint8_t sda = D7;
//const uint8_t MPU6050SlaveAddress = 0x68;
//const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
//const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
//const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
//const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
//const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
//const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
//const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
//const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
//const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
//const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
//const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;
//
//const uint16_t AccelScaleFactor = 16384;
//const uint16_t GyroScaleFactor = 131;
//int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

Adafruit_SSD1306 display(-1);

void setup() {
  Serial.begin(115200);
  
//  Wire.begin(sda, scl);
//  MPU6050_Init();
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(text_size);
  display.setTextColor(WHITE);
  display.clearDisplay();

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  
  Serial.println("");
  String wifiStatus = "Connected to: " + ssid;
  Serial.println(wifiStatus);
  Serial.print("User IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  cur_lat=19.0968;
  cur_lon=72.8517;
  
  fall = 0;
  
  httpClient();
//  delay(100);
}

void set_display(String cur_time, String cur_date, String locate, String bpm, char spike) {
  display.setCursor(0*text_size, 0*text_size);
  display.println("Time : " + cur_time);
  display.display();

  display.setCursor(0*text_size, 9*text_size);
  display.println("Date : " + cur_date);
  display.display();

  display.setCursor(0*text_size, 19*text_size);
  display.println("Location : " + locate);
  display.display();

  display.setCursor(0*text_size, 28*text_size);
  display.println("BPM : " + bpm);
  display.display();
  
  display.setCursor(55*text_size,28*text_size);
  display.write(3);
  display.display();

  display.setCursor(0*text_size, 37*text_size);

  if(spike == '1') {
    display.println("Safe : NO");
  } else {
    display.println("Safe : YES");
  }
  
  display.display();
  display.clearDisplay();
}

void httpClient() {
  int httpCode;
  String payload = "";
  String sensorData[4];
  
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(url);
    http.addHeader("Content-Type", "text/plain");

    String str_post = "";
    int httpCode = http.POST(str_post);

    if (httpCode == HTTP_CODE_OK || httpCode > 0) {
      payload = http.getString();
      
      Serial.println(payload);
      
      char spike = payload[0];
      String current_date = payload.substring(1, 9);
      String current_time = payload.substring(9, 17);
      String bpm = payload.substring(17, 19);
      String locate = payload.substring(19);
      set_display(current_time, current_date, locate, bpm, spike);
    } else {
      Serial.printf("HTTP error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }else {
    Serial.println("Error in WiFi connection");
  }
}

//void read_ag() {
//  double Ax, Ay, Az, T, Gx, Gy, Gz;
//  
//  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
//  
//  //divide each with their sensitivity scale factor
//  Ax = (double)AccelX/AccelScaleFactor;
//  Ay = (double)AccelY/AccelScaleFactor;
//  Az = (double)AccelZ/AccelScaleFactor;
//  T = (double)Temperature/340+36.53; //temperature formula
//  Gx = (double)GyroX/GyroScaleFactor;
//  Gy = (double)GyroY/GyroScaleFactor;
//  Gz = (double)GyroZ/GyroScaleFactor;
//
//  float at = sqrt(AccelX*AccelX+AccelY*AccelY+AccelZ*AccelZ);
//
//  float pre_gt = sqrt(GyroX*GyroX+GyroY*GyroY+GyroZ*GyroZ);  
//  float post_gt;
//    Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);
//  
//    //divide each with their sensitivity scale factor
//    Ax = (double)AccelX/AccelScaleFactor;
//    Ay = (double)AccelY/AccelScaleFactor;
//    Az = (double)AccelZ/AccelScaleFactor;
//    T = (double)Temperature/340+36.53; //temperature formula
//    Gx = (double)GyroX/GyroScaleFactor;
//    Gy = (double)GyroY/GyroScaleFactor;
//    Gz = (double)GyroZ/GyroScaleFactor;
//    
//    if(at > 25000)
//    {
//      delay(100);
//      
//      post_gt = sqrt(GyroX*GyroX+GyroY*GyroY+GyroZ*GyroZ);
//      
//       if (pre_gt>post_gt+600) 
//       {
////          digitalWrite(LED_BUILTIN,HIGH);
//          Serial.println("FALL");
////          Serial.println();
//// 
////          Serial.print(pre_gt);
////          Serial.print('\t');
////          Serial.print(post_gt);
////          Serial.print('\t');
////          Serial.print(at);
////          Serial.println();
//            fall=1;
//          delay(1000);
//       }
//      
//    }
//
//    else 
//    {
//        digitalWrite(LED_BUILTIN,LOW);
//    }
//  delay(100);  
//}
//
//void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data) {
//  Wire.beginTransmission(deviceAddress);
//  Wire.write(regAddress);
//  Wire.write(data);
//  Wire.endTransmission();
//}
//
//void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress) {
//  Wire.beginTransmission(deviceAddress);
//  Wire.write(regAddress);
//  Wire.endTransmission();
//  Wire.requestFrom(deviceAddress, (uint8_t)14);
//  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
//  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
//  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
//  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
//  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
//  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
//  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
//}
//
//void MPU6050_Init() {
//  delay(150);
//  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
//  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
//  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
//  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
//  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
//  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
//  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
//  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
//  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
//  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
//}
