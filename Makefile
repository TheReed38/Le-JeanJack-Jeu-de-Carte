CPP=gcc    #Commande du compilateur
CFLAGS=`sdl2-config --cflags --libs` -Wall -Wextra#Option d'optimisation du programme
LDFLAGS=`sdl2-config --cflags --libs` -L/usr/lib/x86_64-linux-gnu -lSDL2_image -lSDL2_mixer -lSDL2_ttf#Linker
LD_DEBUG=all make


all: main serv client

main: main.o deck.o fonctions.o lobby.o carte.o jeu.o options.o lespetitesbites.o sel.o fesses.o memphis.o japan.o appartJJ.o appartTR.o
	${CPP} $(CFLAGS) -o main main.o fonctions.o deck.o lobby.o carte.o jeu.o options.o lespetitesbites.o sel.o fesses.o memphis.o japan.o appartJJ.o appartTR.o ${LDFLAGS}

serv: serveur.o
	${CPP} $(CFLAGS) -o serv serveur.o ${LDFLAGS}

client: testclient.o
	${CPP} $(CFLAGS) -o client testclient.o ${LDFLAGS}

%.o: %.c
	$(CPP) -c $< $(CFLAGS)



clean:
	rm *.o

mrproper: clean
	rm main
