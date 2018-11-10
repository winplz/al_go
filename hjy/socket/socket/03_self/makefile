CC = gcc
TARGET = server_exe

OBJECTS = server.o common.h

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) -lpthread


clean :
	rm *.o server_exe
