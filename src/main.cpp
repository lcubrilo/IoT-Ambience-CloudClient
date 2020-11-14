/*
 * read_write.cpp
 *
 *  Created on: Dec 18, 2017
 *
 *  Copyright (C) 2017 Pat Deegan, https://psychogenic.com
 *
 *  Usage:
 *    read_write <device_address> <read|write> <uuid> [<hex-value-to-write>]
 *
 *  This file is part of GattLib++.
 *
 *  GattLib++ is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *  GattLib++ is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GattLib++.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Gattlibpp.h>
#include <iostream>
#include <unistd.h> // for usleep

#define MSG_OUT(...) std::cout << __VA_ARGS__
#define MSG_OUTLN(...) std::cout << std::endl << __VA_ARGS__ << std::endl;

#define DBG_ENABLED

#ifdef DBG_ENABLED
#define DBG_OUT(...) std::cerr << __VA_ARGS__
#define DBG_OUTLN(...) std::cerr << std::endl << __VA_ARGS__ << std::endl
#else
#define DBG_OUT(...)
#define DBG_OUTLN(...)
#endif

// forward decl
void useEnabledBLECentral(const std::string &device,
                          const Gattlib::Service::UUIDList serviceUuids,
                          const Gattlib::Service::UUIDList characteristicUuids);

int main(int argc, char *argv[]) {

  const std::string device = "F8:A4:4B:F4:F7:06";
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

  while (1) {
    // our processAsync() call
    Gattlib::BLECentral::getInstance()->processAsync();

    // whatever else we want to do... here we'll
    // just sleep

    usleep(50000);
  }
}

void PrintInput(const Gattlib::BinaryBuffer &data) {
  // got them tasty bytes back
  MSG_OUT("Bytes received: ");
  bool allAscii = true;
  for (Gattlib::BinaryBuffer::const_iterator iter = data.begin();
       iter != data.end(); iter++) {
    MSG_OUT((int)*iter << " ");
    if ((*iter) < 0x09 || (*iter) > 0x7E) {
      allAscii = false;
    }
  }
  MSG_OUT(std::endl);

  // they were all 'printable' bytes, so lets do that too
  if (allAscii) {
    MSG_OUT("As ASCII: ");
    for (Gattlib::BinaryBuffer::const_iterator iter = data.begin();
         iter != data.end(); iter++) {
      MSG_OUT((char)*iter);
    }
    MSG_OUT(std::endl);
  }
}

void TemperatureCallback(const Gattlib::BinaryBuffer &data) {
  PrintInput(data);
  // TODO: lcubrilo
}

void HumidityCallback(const Gattlib::BinaryBuffer &data) {
  PrintInput(data);
  // TODO: lcubrilo
}

void PressureCallback(const Gattlib::BinaryBuffer &data) {
  PrintInput(data);
  // TODO: lcubrilo
}

void failedCallback() { MSG_OUTLN("Last operation failed!."); }

void useEnabledBLECentral(
    const std::string &device, const Gattlib::Service::UUIDList serviceUuids,
    const Gattlib::Service::UUIDList characteristicUuids) {

  Gattlib::BLECentral *central = Gattlib::BLECentral::getInstance();
  central->connectionParameters().security = BT_SEC_LOW;
  central->connectionParameters().destinationType = Gattlib::Address::LE_PUBLIC;

  MSG_OUTLN("Connecting to: " << device);
  central->connect(
      device,
      // connect success callback
      [=]() {
        MSG_OUTLN("Connection established");

        // Temperature
        if (central->startNotification(device, serviceUuids[0],
                                       characteristicUuids[0],
                                       TemperatureCallback, failedCallback))
          std::cout << "Registered to " << characteristicUuids[0]
                    << " callback!" << std::endl;
        else
          std::cout << "Failed to register to " << characteristicUuids[0]
                    << " callback!" << std::endl;

        // Humidity
        if (central->startNotification(device, serviceUuids[1],
                                       characteristicUuids[1], HumidityCallback,
                                       failedCallback))
          std::cout << "Registered to " << characteristicUuids[1]
                    << " callback!" << std::endl;
        else
          std::cout << "Failed to register to " << characteristicUuids[1]
                    << " callback!" << std::endl;

        // Pressure
        if (central->startNotification(device, serviceUuids[2],
                                       characteristicUuids[2], PressureCallback,
                                       failedCallback))
          std::cout << "Registered to " << characteristicUuids[2]
                    << " callback!" << std::endl;
        else
          std::cout << "Failed to register to " << characteristicUuids[2]
                    << " callback!" << std::endl;
      }, [](){
        std::cout << "Failed to connect to device." << std::endl << "Terminating..." << std::endl;
        std::terminate();
      });
}
