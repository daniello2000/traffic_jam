/* Code by Daniel Engelhardt <dengelh@gmail.com>; if we meet some day and you found that useful, buy me a beer */

#include "ausgabe.h"
#include "fahren.h"
#include <stdio.h>
#include <stdlib.h>

/* Farbsetting: Klassisch: Auto schwarz (0), Leer weiß (255) */

#define FARBE_AUTO " 0 0 0 "

#define FARBE_LEER " 255 255 255 "

static void ausgabe_normal(struct Konfiguration* s) {
    int i;

    for(i = 0; i < s->laenge; ++i) {
        if(s->strasse[i].v == -1) putchar ('.');
        else putchar(s->strasse[i].v+'0');
    }

    putchar ('\n');
}

static void ausgabe_ppm(struct Konfiguration* s) {
    int i;

    for(i = 0; i < s->laenge; ++i) {
        if(s->strasse[i].v == -1)
            printf (FARBE_LEER);
        else
            printf (FARBE_AUTO);
    }
    puts ("");
}

static void ausgabe_geschw(struct Konfiguration* s) {
    int v = 0, k = 0, i;
    static int zaehl = 0;

    for(i = 0; i < s->laenge; ++i)
        if(s->strasse[i].v != -1) {
            v += s->strasse[i].v;
            ++k;
        }

    printf("%d %g \n", ++zaehl, (double)(v) / k);
}

void ausgabe(struct Konfiguration* s, int x, int durchlae) {
    static int erster_aufruf = 1;

    if(erster_aufruf && x == 3) {
        /* Das ppm-Dateiformat initialisieren */
        puts("P3\n");
        printf("%d %d\n", s->laenge, durchlae);
        puts(" 255 ");
    }

    if(x == 2) ausgabe_normal(s);
    if(x == 3) ausgabe_ppm(s);
    if(x == 4) ausgabe_geschw(s);

    erster_aufruf = 0;
}
