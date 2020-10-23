if exist "build/" rd /q /s "build"
mkdir build
cd build
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="../toolchain-raspberry.cmake" ..