LIBS  = -lbsd
CFLAGS = -Wall -W -g
SRC=$(wildcard *.c)
TARGET=hash-table-example

$(TARGET): $(SRC)
	gcc -o $@ $^ $(CFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)
