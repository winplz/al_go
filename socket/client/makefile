CC = gcc
TARGET = client_exe
OBJECTS = client.o

$(TARGET) : $(OBJECTS)
	$(CC) -o $(TARGET) $(OBJECTS) -lpthread

clean :
	rm *.o client_exe
