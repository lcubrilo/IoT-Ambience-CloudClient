#include <Gattlibpp.h>
#include <iostream>
#include <unistd.h> // for usleep
#include "Wolk.h"

typedef enum airCharacteristics {TEMP, HUMI, PRES} AirCharacteristic;
std::unique_ptr<wolkabout::Wolk> wolk;

	Gattlib::BLECentral* arduino = Gattlib::BLECentral::getInstance();	
	const std::string &BLEDeviceUUID = "F1:5D:35:C4:08:9F";
	std::string serviceUUIDs[] = {"0xCCC0", "0xDDD0", "0xEEE0"};
	std::string characteristicUUIDs[] = {"0xCCC1", "0xDDD1","0xEEE1"};


void notificationCallback(const Gattlib::BinaryBuffer &data, AirCharacteristic x){
	Gattlib::BinaryBuffer::const_iterator i = data.begin();
	
	//We receive a 4 digit hexadecimal number, always split into 2 packages.
	//First package represents last 2 digits and vice versa
	//Sometimes the first 2 digits are zeros. We don't care about them.
	int firstPacket = (int)*i++, secondPacket = (int)*i;
	int value = secondPacket*16*16 + firstPacket;
	
	switch(x){
		case TEMP: wolk -> addSensorReading("T", value); std::cout << "Temp: " << value << std::endl; break;
		case PRES: wolk -> addSensorReading("P", value); std::cout << "Pres: " << value << std::endl; break;
		case HUMI: wolk -> addSensorReading("H", value); std::cout << "Humi: " << value << std::endl << "--------" << std::endl; break;
	}
	wolk -> publish();
}

void tempCallback(const Gattlib::BinaryBuffer &data) {
	notificationCallback(data, TEMP);
}
void presCallback(const Gattlib::BinaryBuffer &data){
	notificationCallback(data, PRES);
}
void humiCallback(const Gattlib::BinaryBuffer &data){
	 notificationCallback(data, HUMI);
}


int main(int argc, char *argv[]) 
{
	//BLE setup	
	arduino -> enable([]//succ enable
	{
		std::cout << "Enabled BLE connection." << std::endl;
		arduino -> connectionParameters().security = BT_SEC_LOW;
		arduino -> connectionParameters().destinationType = Gattlib::Address::LE_PUBLIC;
		
		arduino -> connect(BLEDeviceUUID,[]//succ connect
		{
			std::cout << "Connected to a BLE device." << std::endl;
			arduino -> startNotification(BLEDeviceUUID, serviceUUIDs[TEMP], characteristicUUIDs[TEMP], tempCallback);
			arduino -> startNotification(BLEDeviceUUID, serviceUUIDs[PRES], characteristicUUIDs[PRES], presCallback);
			arduino -> startNotification(BLEDeviceUUID, serviceUUIDs[HUMI], characteristicUUIDs[HUMI], humiCallback);	
		}
		,[]//fail connect
		{
			std::cout << "Failed connect.";
		});
	}
	,[]//fail enable
	{
		std::cout << "Failed enable.";
	});
	
	
	//Wolk setup
	wolkabout::Device WolkDeviceCredentials("471d6850-8bed-4eca-9e6e-97fd9300b117", "5B2OD76EC4");
    wolk = wolkabout::Wolk::newBuilder(WolkDeviceCredentials).host("ssl://api-demo.wolkabout.com:8883").build();
    wolk->connect();
	std::cout << "Connected to Wolk." << std::endl;
	
	while (1) 
	{
		arduino -> processAsync();
		usleep(50000);
	}
}
