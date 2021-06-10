CC = gcc
CFLAGS=-W -Wall
LDFLAGS = -lwiringPi -lrt -lm -li2c
OBJS = main.o ict_sys.o am2315.o 
TARGET = ICT_GRADER_SYSTEM

all : $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

default:
	@echo "Type \"make filename without .c\" to compile...."

clean:
	rm -rf $(TARGET) *.o *.csv *.gch D* L* T* W*

