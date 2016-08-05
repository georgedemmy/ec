PREFIX = /usr/local
BINDIR = $(PREFIX)/bin

CC = g++
CFLAGS = -O2 -Wall

TARGET = ec

$(TARGET): ec.o resource.o
	$(CC) -o $@ -mwindows $^

%.o:%.cpp
	$(CC) -c $(CFLAGS) $(CPPFLAGS) $^ -o $@

resource.o: resource.rc emacs.ico
	windres -i $< -o $@

install:
	install -m 755 $(TARGET) $(BINDIR)

clean:
	rm -f $(TARGET) *.o

distclean:
	make clean
	rm -f *~
