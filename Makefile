CFLAGS=-g -std=c11 -pedantic -Wall -Wextra
LIBS=

toyLang: src/main.c
	mkdir -p bin
	$(CC) $(CFLAGS) -o bin/$@ $< $(LIBS)
	./bin/$@

clean:
	rm -rf bin/*
