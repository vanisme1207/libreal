echo "building dynamic library"
g++-16 -Ofast -shared -fPIC src/real.cpp -o ./build/dynamiclib/libreal.so
