all: stau

stau: fahren.o stauzeit.o ausgabe.o main.c
	gcc -W -Wall -o stau main.c fahren.o stauzeit.o ausgabe.o

fahren.o: fahren.c
	gcc -W -Wall -c fahren.c

stauzeit.o: stauzeit.c
	gcc -W -Wall -c stauzeit.c

ausgabe.o: ausgabe.c
	gcc -W -Wall -c ausgabe.c

clean:
	rm stau fahren.o stauzeit.o ausgabe.o
