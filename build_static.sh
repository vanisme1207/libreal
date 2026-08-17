echo "building static library"
g++-16 -Ofast -c src/real.cpp -o build/objects/real.o
ar rcs build/staticlib/libreal.a build/objects/real.o
