GPP = g++
GCC = gcc
OUTPUT = "./build/GPS.so"
CC_PARAMS =-std=c++17 -c -m32 -fPIC -O3 -w -DLINUX -I./lib/sdk/amx/

all:
	$(GPP) $(CC_PARAMS) ./lib/sdk/amxplugin.cpp
	$(GPP) $(CC_PARAMS) ./lib/sdk/amxplugin2.cpp
	$(GPP) $(CC_PARAMS) ./src/*.cpp
	$(GPP) -m32 -O3 -fshort-wchar -shared -o $(OUTPUT) *.o
	-rm -f *.o
