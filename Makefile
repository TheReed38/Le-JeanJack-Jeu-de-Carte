CPP=gcc    #Commande du compilateur
ifeq ($(OS),Windows_NT)
	LDFLAGS=-lwsock32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	CFLAGS=-Llib -Iinclude -Wall -Wextra
else
	LDFLAGS=-lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
	CFLAGS=-L/usr/lib/x86_64-linux-gnu -Iinclude -Wall -Wextra
endif
src := $(shell find . -wholename "./src/*.c" | grep -v "./src/serveur.c")
obj = $(src:.c=.o)


all: bin/LeJeanJackJeuDeCarte.exe bin/serveurJJJDC.exe

bin/LeJeanJackJeuDeCarte.exe: $(obj)
	${CPP} $(CFLAGS) -o $@ $^ ${LDFLAGS}

bin/serveurJJJDC.exe: src/serveur.o
	${CPP} $(CFLAGS) -o $@ $^ ${LDFLAGS}

%.o: %.c
	$(CPP) -c -o $@ $< $(CFLAGS) 

.PHONY: clean
clean :
	rm -f $(obj) bin/LeJeanJackJeuDeCarte.exe bin/serveurJJJDC.exe src/serveur.o
