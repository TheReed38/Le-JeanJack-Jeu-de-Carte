CPP=gcc    #Commande du compilateur

LDFLAGS=-lws2_32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf
CFLAGS=-Llib -Iinclude -Wall -Wextra

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
