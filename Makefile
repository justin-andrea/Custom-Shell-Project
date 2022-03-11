CC	= gcc
CFLAGS  = -Wall -g
LDFLAGS = -lreadline
OBJFILES = main.o shell.o schedule.o
TARGET	= main

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	rm -f $(OBJFILES) $(TARGET) *~

