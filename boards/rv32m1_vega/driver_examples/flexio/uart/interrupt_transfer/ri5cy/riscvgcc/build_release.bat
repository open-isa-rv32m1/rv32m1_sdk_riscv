cmake -DCMAKE_TOOLCHAIN_FILE="../../../../../../../../tools/cmake_toolchain_files/riscv32gcc.cmake" -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release  .
mingw32-make -j4
IF "%1" == "" ( pause ) 
