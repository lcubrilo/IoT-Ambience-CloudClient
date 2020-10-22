cd build
del *.*
cmake -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="../toolchain-raspberry.cmake" ..