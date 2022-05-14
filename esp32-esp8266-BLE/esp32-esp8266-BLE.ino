#define RXD2 22 //RX,
#define TXD2 23 //TX
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>

const int IDLE_TIME = 6000;    // 發送數據間隔時間：5秒
unsigned long lastTimeMillis = 0;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b" //in here we use default UUID
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8" //in here we use default UUID
//declare a BLE scan object

class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks
{
void onResult(BLEAdvertisedDevice advertisedDevice)
  {//Serial.printf("Advertised Device: %s \n", advertisedDevice.toString().c_str());
    }
};
int scanTime=5;  //set scantime, if time too less, you will lost some beacons that not discovered

void HttpSend(int RSSiA, int RSSiB, int RSSiC ,int RSSiD)
{
  
  //String addWifi="AT+CWJAP_DEF=\"sasaxxxxxx\",\"212xxxxxxx\"";
  String addWifi="AT+CWJAP_DEF=\"WiFi\",\"09285xxxxx\"";
  String connectToWifi="AT+CIPSTART=\"TCP\",\"coreyqaq.ddns.net\",1880";
  String HOST="coreyqaq.ddns.net";
  String httpMsg = "GET /bt_test?A=";
  httpMsg += String(RSSiA);
  httpMsg +="&B=";
  httpMsg += String(RSSiB);
  httpMsg +="&C=";
  httpMsg += String(RSSiC);
  httpMsg +="&D=";
  httpMsg += String(RSSiD);
  httpMsg +=" HTTP/1.1\r\n";
  httpMsg += "User-Agent: ESP8266\r\n";
  Serial2.println(addWifi);
  delay(2500);
  
  Serial2.println(connectToWifi);
  delay(1000);
  
  Serial2.println("AT+CIPSTART=\"TCP\",\"" + HOST + "\",1880");
  delay(500);  // 等待OK回應
  
  Serial2.println("AT+CIPMODE=1");  // 透傳
  delay(500);
  
  Serial2.println("AT+CIPSEND");    // 啟動發送
  delay(1000);
  
  Serial2.println(httpMsg);  // 發送HTTP訊息
  delay(1000);
}

void setup()
{
  Serial.begin(9600);//create service at baud rate 9600 
  Serial.begin(115200);
  Serial2.begin(115200,SERIAL_8N1, RXD2, TXD2);
  while (!Serial2)
  {
    Serial.println("not ready");
    ;
    }
  Serial.println("ESP8266 Ready!");
}

void loop()
{
  Serial.println("Beacon Scanning....");
  BLEDevice::init("");                                                       //clear beacon list
  BLEScan* pBLEScan = BLEDevice::getScan();                                  //create scan
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setActiveScan(true);                                             //start to scan
  BLEScanResults foundDevices = pBLEScan->start(scanTime);                  //wait for scanning
  Serial.print("Found Beacons: ");
  Serial.println(foundDevices.getCount());
  Serial.println("End of Scanning");
  int BeaconCount=foundDevices.getCount();                                  //list discover beacons data
  Serial.println("Show Beacons content:");
  int array[]{0,0,0,0};
  for (int i=0; i<BeaconCount; i++)
  {
       BLEAdvertisedDevice d=foundDevices.getDevice(i);
       String BeaconAddress=d.getAddress().toString().c_str();             //in here we use those mac address below to filter the address we want
       if(BeaconAddress=="f5:1c:f4:51:f4:71")
       {
          Serial.print("Address=");
          Serial.print(BeaconAddress);
          int BeaconRSSI=d.getRSSI();
          Serial.print(",RSSI=");
          Serial.println(BeaconRSSI); //Show RSSI
          array[0]=BeaconRSSI;
       }
       if(BeaconAddress=="c6:3b:a0:12:13:1b")
       {
          Serial.print("Address=");
          Serial.print(BeaconAddress);
          int BeaconRSSI=d.getRSSI();
          Serial.print(",RSSI=");
          Serial.println(BeaconRSSI); //Show RSSI
          array[1]=BeaconRSSI;
       }
       if(BeaconAddress=="20:91:48:21:71:9f")
       {
          Serial.print("Address=");
          Serial.print(BeaconAddress);
          int BeaconRSSI=d.getRSSI();
          Serial.print(",RSSI=");
          Serial.println(BeaconRSSI); //Show RSSI
          array[2]=BeaconRSSI;
       }
       if(BeaconAddress=="dc:e9:bc:a9:e1:22")
       {
          Serial.print("Address=");
          Serial.print(BeaconAddress);
          int BeaconRSSI=d.getRSSI();
          Serial.print(",RSSI=");
          Serial.println(BeaconRSSI); //Show RSSI
          array[3]=BeaconRSSI;
       }
  }
  
  if (millis() - lastTimeMillis > IDLE_TIME) 
  {
    lastTimeMillis = millis();
    HttpSend(array[0],array[1],array[2],array[3]);   // send RSSI
  }
  while (Serial2.available()) 
  {
    char c = Serial2.read();
    Serial.print(c);
  }
  
}
