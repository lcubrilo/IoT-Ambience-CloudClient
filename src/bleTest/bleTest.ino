#include <BLEPeripheral.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <bme680.h>
#include <bme680_defs.h>

BLEPeripheral blutut = BLEPeripheral(46, 2, 48);
BLEService ts = BLEService("CCC0");
BLEIntCharacteristic tc = BLEIntCharacteristic("CCC1", BLERead | BLENotify);
BLEDescriptor td = BLEDescriptor("2901", "Atmosphere: Temp, Humi, Pres");

BLEService hs = BLEService("DDD0");
BLEIntCharacteristic hc = BLEIntCharacteristic("DDD1", BLERead | BLENotify);
BLEDescriptor hd = BLEDescriptor("2901", "Humidity Percent");
//
BLEService ps = BLEService("EEE0");
BLEIntCharacteristic pc = BLEIntCharacteristic("EEE1", BLERead | BLENotify);
BLEDescriptor pd = BLEDescriptor("2901", "Pressure mbar");

Adafruit_BME680 senzor = Adafruit_BME680();
int tPreviousMillis = 0;
int hPreviousMillis = 0;
int pPreviousMillis = 0;
int i = 0;

void blututSetup() {

  blutut.setLocalName("ARDuino3");

  blutut.setAdvertisedServiceUuid(ts.uuid());
  blutut.addAttribute(ts);
  blutut.addAttribute(tc);
  blutut.addAttribute(td);

  blutut.setAdvertisedServiceUuid(hs.uuid());
  blutut.addAttribute(hs);
  blutut.addAttribute(hc);
  blutut.addAttribute(hd);

  blutut.setAdvertisedServiceUuid(ps.uuid());
  blutut.addAttribute(ps);
  blutut.addAttribute(pc);
  blutut.addAttribute(pd);

  blutut.setEventHandler(BLEConnected, connectToBLE);
  blutut.setEventHandler(BLEDisconnected, disconnectFromBLE);

  blutut.begin();
  Serial.println("setapovao");
}

bool BLEconnected = false;
void connectToBLE() {
  BLEconnected = true;
  Serial.println("\nPovezan.");
}
void disconnectFromBLE() {
  BLEconnected = false;
  Serial.println("Odvezan.");
}

void setup() {
  Serial.begin(9600);
  blututSetup();
  senzor.begin();
}

void loop() {
  blutut.poll();
  int currentMilis = millis(); //How much time has passed
  if (currentMilis - pPreviousMillis > 2000 && BLEconnected) //Not enough time since last time? skip.
  {
    hc.setValue(random(0,100));
    pPreviousMillis = currentMilis;
  } 
  if (currentMilis - hPreviousMillis > 2100 && BLEconnected)
  {
    pc.setValue(random(0,100));
    hPreviousMillis = currentMilis;
  } 
  if (currentMilis - tPreviousMillis > 2200 && BLEconnected)
  {
    tc.setValue(random(0,100));
    tPreviousMillis = currentMilis;
  }
}
