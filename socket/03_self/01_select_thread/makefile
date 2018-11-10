CC = gcc
TARGET = server_exe

OBJECTS = server.o common.o common.h

$(TARGET) : $(OBJECTS)
	$(CC) -Wall -o $(TARGET) $(OBJECTS) -lpthread


clean :
	rm *.o server_exe
