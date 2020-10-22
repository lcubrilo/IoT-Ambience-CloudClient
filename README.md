Cloud client

Steps for setup:
1. Install CMAKE (https://cmake.org/download/)
2. Install RaspberryPi toolchain (https://sysprogs.com/getfile/478/raspberry-gcc6.3.0-r5.exe)
3. Add the /bin folder from raspberry pi toolchain location to environment variables
4. Install Qt Creator

QtCreator CMake Configuration for kit:
CMAKE_GENERATOR:STRING=Unix Makefiles
CMAKE_TOOLCHAIN_FILE:STRING=toolchain-raspberry.cmake