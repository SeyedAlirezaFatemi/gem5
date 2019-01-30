# By default, the code is compiled for a "big endian" machine.
# To compile on a "little endian" machine set the LITTLE_ENDIAN flag.
# To make smaller object code, but run a little slower, don't use UNROLL_LOOPS.
# To use NIST's modified SHA of 7/11/94, define USE_MODIFIED_SHA

ARCH = arm
TARGET = ./bin/sha

ifeq ($(ARCH),x86)
    CXX = g++
    ISA_FILE =
else
    CXX =arm-linux-gnueabi-g++
    ISA_FILE = ./m5op_arm.S
endif

# CC = gcc
# ISA_FILE = ./m5op_arm.S
# CFLAGS = -static -O0 -Wall

# sha:  sha_driver.o sha.o
# 	$(CC) -o $@ sha_driver.o sha.o
# 	strip $@

sha:  sha_driver.o sha.o
	$(CXX) -static -O0 -o $(TARGET) sha_driver.c sha.c -lm $(ISA_FILE)
	# strip $@

clean:
	rm -rf $(TARGET)
