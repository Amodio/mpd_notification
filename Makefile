CC=gcc
CFLAGS=-Wall -pedantic $(shell pkg-config --cflags libmpdclient libnotify)
LDFLAGS=-Wall -pedantic $(shell pkg-config --libs libmpdclient libnotify)
TARGET=mpd_notification
SRC=$(TARGET).c
OBJ=$(SRC:.c=.o)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

all: $(TARGET)

clean:
	rm -f $(OBJ)

distclean: clean
	rm -f $(TARGET)

install: $(TARGET)
	cp -f $(TARGET) ~/.config/openbox/
	cp -f $(TARGET).desktop ~/.config/autostart/

uninstall:
	rm -f ~/.config/openbox/$(TARGET) ~/.config/autostart/$(TARGET).desktop
