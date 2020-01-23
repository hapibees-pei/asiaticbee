#include <SPI.h>
#include <LoRa.h>

//Libraries for OLED Display
#include <Wire.h>

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//define the pins used by the LoRa transceiver module
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define BAND 866E6

//OLED pins
#define OLED_SDA 4
#define OLED_SCL 15
#define OLED_RST 16
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

//packet counter
int readingID = 0;

int counter = 0;
String LoRaMessage = "";


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

Adafruit_BME280 bme;


//Initialize OLED display
void startOLED(){
  bool status = bme.begin(0x76);
  if (!status) {
    Serial.println("could not find a valid bme280 sensor, check wiring!");
//    while (1);
  } else {

    Serial.println("We good!");
  }

    //reset OLED display via software
    pinMode(OLED_RST, OUTPUT);
    digitalWrite(OLED_RST, LOW);
    delay(20);
    digitalWrite(OLED_RST, HIGH);

    //initialize OLED
    Wire.begin(OLED_SDA, OLED_SCL);
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setTextSize(1);
    display.setCursor(0,0);
    display.print("LORA SENDER");
}

//Initialize LoRa module
void startLoRA(){
    //SPI LoRa pins
    SPI.begin(SCK, MISO, MOSI, SS);
    //setup LoRa transceiver module
    LoRa.setPins(SS, RST, DIO0);

    while (!LoRa.begin(BAND) && counter < 10) {
        Serial.print(".");
        counter++;
        delay(500);
    }
    if (counter == 10) {
        // Increment readingID on every new reading
        readingID++;
        Serial.println("Starting LoRa failed!");
    }
    Serial.println("LoRa Initialization OK!");
    display.setCursor(0,10);
    display.clearDisplay();
    display.print("LoRa Initializing OK!");
    display.display();
    delay(2000);
}

/*void startBME(){
    // default settings
    // (you can also pass in a Wire library object like &Wire2)
    bool status = bme.begin(0x76, &I2Cone);
    if (!status) {
        display.clearDisplay();
        display.setCursor(0,0);
        display.setTextSize(1);
        display.print("BME Failed");
        display.display();
        while (1);
    }

    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.print("BME Started");
    display.display();
}*/

void getReadings(){
    //pressure = dht.readPressure() / 100.0F;
}

void sendReadings() {

  String temperature = String(bme.readTemperature());
  String humidity = String(bme.readHumidity());
  String pressure = String(bme.readPressure());
    Serial.print(temperature);
    Serial.print(humidity);

    LoRaMessage = String(readingID) + "&" + (temperature) + "&" + (humidity) + "&" + (pressure);
    //Send LoRa packet to receiver
    LoRa.beginPacket();
    LoRa.print(LoRaMessage);
    LoRa.endPacket();

    display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(1);
    display.print("LoRa packet sent!");
    display.setCursor(0,20);
    display.print("Temperature:");
    display.setCursor(72,20);
    display.print(String(temperature));
    display.setCursor(0,30);
    display.print("Humidity:");
    display.setCursor(54,30);
    display.print(humidity);
    display.setCursor(0,40);
    display.print("Pressure:");
    display.setCursor(54,40);
    display.print(pressure);
    display.setCursor(0,50);
    display.print("Reading ID:");
    display.setCursor(66,50);
    display.print(readingID);
    display.display();
    Serial.print("Sending packet: ");
    Serial.println(readingID);
    readingID++;
}

void setup() {
    //initialize Serial Monitor
    Serial.begin(9600);
    startOLED();
    startLoRA();
    //startBME();
}
void loop() {
    getReadings();
    sendReadings();
    delay(2000);
}
