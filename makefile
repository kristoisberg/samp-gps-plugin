GPP = g++
GCC = gcc
OUTPUT = "./../samp2/plugins/gps.so"
CC_PARAMS =-std=c++14 -g -c -m32 -fPIC -O0 -w -DLINUX -I./lib/sdk/amx/

all:
	$(GPP) $(CC_PARAMS) ./lib/sdk/amxplugin.cpp
	$(GPP) $(CC_PARAMS) ./src/*.cpp
	$(GPP) -m32 -O0 -fshort-wchar -shared -g -o $(OUTPUT) *.o
	-rm -f *.o
