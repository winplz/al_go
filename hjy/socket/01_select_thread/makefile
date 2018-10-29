CC = gcc
TARGET = server_exe

OBJECTS = server.o

$(TARGET) : $(OBJECTS)
	$(CC) -Wall -o $(TARGET) $(OBJECTS) -lpthread


clean :
	rm *.o server_exe
