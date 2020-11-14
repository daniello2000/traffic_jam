/* Code by Daniel Engelhardt <dengelh@gmail.com>; if we meet some day and you found that useful, buy me a beer */

#include "fahren.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>

struct Konfiguration* initialisieren1(double wa, int v, int lae, int anz) {
    /* Baut ein Spielfeld entsprechend der Daten */
    struct Konfiguration* s = malloc(sizeof * s);
    int i;

    if(!s) exit(EXIT_FAILURE);


    s->strasse = malloc(( lae +1)* sizeof(* s->strasse));
    s->tmp_strasse = malloc(( lae +1)* sizeof(* s->tmp_strasse));

    if( ! s->strasse || ! s->tmp_strasse)

        exit(EXIT_FAILURE);

    for(i = 0; i < lae; ++i) {
        struct Label leer = { INT_MAX, 0, INT_MAX, 0 };
        s->strasse[i].v = s->tmp_strasse[i].v = -1; /* Kein Auto */
        s->strasse[i].label = s->tmp_strasse[i].label = leer;
    }


    s->laenge = lae;
    s->wahrscheinlichkeit = wa;
    s->v_max = v;
    s->fahrzeuge_pro_einheit = 0;

    if( anz >= lae) {
        puts("Zu viele Autos - die Anzahl kann höchstens der Länge der Straße entsprechen.");
        anz = lae -1;

        /* Vollstau -- nur ein freies Feld */
    }

    /* Alle Autos mit v <= v_max zufällig auf der Straße verteilen: */
    srand(time(0));

    while(anz > 0) {
        int r = rand ()% lae;
        if(s->strasse[r].v == -1) {
            s->strasse[r].v = rand ()%( s->v_max + 1);
            --anz;
        }
    }

    return s;
}

void hilfe(void);

struct Konfiguration* initialisieren2(double wa, int v, const char * stra) {
    /* Baut ein Spielfeld wie in stra */
    int i, geschw;
    struct Konfiguration* s;

    s = initialisieren1(wa, v, strlen(stra), 0);

    for(i = 0; i < s->laenge; ++i) {
        if(stra[i] =='.') s->strasse[i].v = -1; /* Kein Auto */
        else if(isdigit(stra[i]) &&(geschw=stra[i]-'0') <= s->v_max) {
            s->strasse[i].v = geschw;
        } else {
            puts("Ungültige Konfiguration eingegeben");
            hilfe ();
        }
    }

    return s;
}

void deinitialisiere(struct Konfiguration* s) {
    free(s->tmp_strasse);
    free(s->strasse);
    free(s);
}

int dist(struct Konfiguration *s, int n) {
    /* Ermittelt die Distanz zum Vordermann. Daß ein Vordermann
    existiert wird sichergestellt, indem die Autos auf einer
    Kreisbahn stehen. So ist im Zweifelsfalle bei nur einem Auto
    jeder sein eigener Vordermann.*/
    int d = 1;
    n =(n + 1)% s->laenge;

    while(s->strasse[n].v == -1) {
        ++d;
        n =(n+1)%s->laenge;
    }

    return d;
}

int troedeln(struct Konfiguration * s) {
    return (rand() / (RAND_MAX + 1.0)) < s->wahrscheinlichkeit;
}

void fahren(struct Konfiguration* s, int staulebenszeitbestimmung) {
    int i;

    for(i = 0; i < s->laenge; ++i) {
        if(s->strasse[i].v != -1) { /* Auto */
            int d = dist(s, i);  /* Abstand zum nächsten Auto */
            struct Auto a = s->strasse[i];
            int v = a.v;

            /* Hier gilt:
                i+d ist die Position des nächsten Autos
                i+v ist die nächste Position dieses Autos
                => v < d
            */


            if(v < s->v_max) v++; /* Beschleunigen bis zum Limit */

            if(v >= d) v = d-1; /* Bremsen */

            if(staulebenszeitbestimmung > 10) {
                s->strasse[(i+v)%s->laenge].v = v;
                s->strasse[i].v = 0;
                stauzeiten(s, (i+v)%s->laenge, staulebenszeitbestimmung);
                a = s->strasse[( i+v)%s->laenge];

                s->strasse[(i+v)%s->laenge].v = -1; /* Zurücksetzen */
                s->strasse[i].v = v;
            }
            if(v > 0 && troedeln(s)) v--;
            /* Zufälliges Herumtrödeln */

            if((i+v)%s->laenge < i) s->fahrzeuge_pro_einheit ++;

            /* Den Fluß bestimmen */
            a.v = v;
            s->tmp_strasse[( i+v)%s->laenge] = a; /* Fahrzeug weiterrücken */
        }
    }

    for(i = 0; i < s->laenge; ++i) {
        s->strasse[i] = s->tmp_strasse[i];
        s->tmp_strasse[i].v = -1;
    }
}

