
CC = gcc

CFLAGS = -Wall -Werror
LDFLAGS = -lssl

OUTPUT = flysums
OBJ = flysums.o

.PHONY: all
all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(CC) -o $@ $< $(LDFLAGS)

.c.o: $<
	$(CC) $(CFLAGS) -o $@ -c $<

.PHONY: clean
clean:
	rm -f $(OUTPUT) $(OBJ)

.PHONY: install uninstall

install: $(OUTPUT)
	install -m755 -o0 -g0 $(OUTPUT) $(DESTDIR)/usr/bin/$(OUTPUT)

uninstall:
	rm -f $(DESTDIR)/usr/bin/$(OUTPUT)

