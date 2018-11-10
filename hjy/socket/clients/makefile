CC = gcc
TARGET = client_exe

OBJECTS = client.o client.h

$(TARGET) : $(OBJECTS)
	$(CC) -Wall -o $(TARGET) $(OBJECTS) -lpthread


clean :
	rm *.o client_exe
