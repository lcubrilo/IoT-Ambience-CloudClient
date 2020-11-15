#include <Gattlibpp.h>
#include <iostream>
#include <unistd.h> // for usleep
#include "Wolk.h"

std::unique_ptr<wolkabout::Wolk> wolk;

void useEnabledBLECentral(const std::string &device, const Gattlib::Service::UUIDList serviceUuids, const Gattlib::Service::UUIDList characteristicUuids);

int main(int argc, char *argv[]) {
	//BLE setup
	const std::string device = "F1:5D:35:C4:08:9F";
	const Gattlib::Service::UUIDList serviceUuids{"0xCCC0", "0xDDD0", "0xEEE0"};
	const Gattlib::Service::UUIDList characteristicUuids{"0xCCC1", "0xDDD1",
                                                       "0xEEE1"};
	bool foundDevice = false;

	try {
	Gattlib::BLECentral::getInstance()->enable(
		[&]() {
		  std::cout << "Device enabled. Scanning all devices." << std::endl;

		  Gattlib::BLECentral::getInstance()->scan(
			  serviceUuids, 15,
			  // scan completed
			  [&device, &foundDevice, &serviceUuids, &characteristicUuids]() {
				std::cout << "Scan completed." << std::endl;
				if (foundDevice)
				  useEnabledBLECentral(device, serviceUuids,
									   characteristicUuids);
				else {
				  std::cout << "Cannot find desired device. Shutting down."
							<< std::endl;
				  std::terminate();
				}
			  },
			  // discovery process
			  [&device, &foundDevice](const Gattlib::Discovery::Device &dev) {
				std::cout << "Discovered device: " << dev.id << std::endl;
				if (dev.id == device) {
				  std::cout << "Found desired device." << std::endl;
				  foundDevice = true;
				}
			  },
			  []() { std::cout << "Failed." << std::endl; });
		},
		[]() { std::cout << "FAILED TO ENABLE." << std::endl; });
	} catch (std::exception &e) {
	std::cout << "Exception occured: " << e.what() << std::endl;
	}

	
	
	//Wolk setup
	wolkabout::Device device1("471d6850-8bed-4eca-9e6e-97fd9300b117", "5B2OD76EC4");

    wolk = wolkabout::Wolk::newBuilder(device1).
			host("ssl://api-demo.wolkabout.com:8883").build();

    wolk->connect();
	std::cout << "Connected to Wolk." << std::endl;
	
	while (1) 
	{
		Gattlib::BLECentral::getInstance()->processAsync();
		usleep(50000);
	}
	
	wolk -> disconnect();
}

void TemperatureCallback(const Gattlib::BinaryBuffer &data) {
	Gattlib::BinaryBuffer::const_iterator i = data.begin();
	int temperature = (int)*(i++);
	wolk -> addSensorReading("T", temperature);
	wolk -> publish();
	std::cout << "Temperature " << temperature << std::endl << "--------" << std::endl;
    
}

void HumidityCallback(const Gattlib::BinaryBuffer &data) {
	Gattlib::BinaryBuffer::const_iterator i = data.begin();
	int humidity = (int)*(i++);
	wolk -> addSensorReading("H", humidity);
	wolk -> publish();
	std::cout << "Humidity " << humidity << std::endl << "--------" << std::endl;
}

void PressureCallback(const Gattlib::BinaryBuffer &data) {
	Gattlib::BinaryBuffer::const_iterator i = data.begin();
	int prviParCifara = (int)*i, drugiParCifara = (int)*++i;
	int pressure = prviParCifara + drugiParCifara*256;
	wolk -> addSensorReading("P", pressure);
	wolk -> publish();
	std::cout << "Pressure " << pressure << std::endl << "--------" << std::endl;
}

void useEnabledBLECentral(const std::string &device, const Gattlib::Service::UUIDList serviceUuids, const Gattlib::Service::UUIDList characteristicUuids) {
	Gattlib::BLECentral *central = Gattlib::BLECentral::getInstance();
	central->connectionParameters().security = BT_SEC_LOW;
	central->connectionParameters().destinationType = Gattlib::Address::LE_PUBLIC;
	
	std::cout << "Connecting to: " << device;
	central->connect(
	  device,
	  // connect success callback
	  [=]() {
		std::cout << "Connection established";

		// Temperature
		if (central->startNotification(device, "0xCCC0", "0xCCC1",
									   TemperatureCallback))
		  std::cout << "Registered to 0xCCC0 callback!" << std::endl;
		else
		  std::cout << "Failed to register to 0xCCC0 callback!" << std::endl;

		// Humidity
		if (central->startNotification(device, "0xDDD0", "0xDDD1",
									   HumidityCallback))
		  std::cout << "Registered to 0xDDD0 callback!" << std::endl;
		else
		  std::cout << "Failed to register to 0xDDD0 callback!" << std::endl;

		// Pressure
		if (central->startNotification(device, "0xEEE0", "0xEEE1",
									   PressureCallback))
		  std::cout << "Registered to 0xEEE0 callback!" << std::endl;
		else
		  std::cout << "Failed to register to 0xEEE0 callback!" << std::endl;
	  });
	}

