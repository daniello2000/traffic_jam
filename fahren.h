/* Code by Daniel Engelhardt <dengelh@gmail.com>; if we meet some day and you found that useful, buy me a beer */

#ifndef FAHREN_H
#define FAHREN_H

struct Label {
    unsigned long t_start, t_end;
    unsigned long alt_t_start;
    unsigned long label_nr;
};

struct Auto {
    struct Label label;

    int v;

};
struct Konfiguration {
    struct Auto* strasse;


    int laenge;
    /* die Straßenkonfiguration; ist ein Feld -1, so steht dort kein
    Fahrzeug, alle anderen Nummern bedeuten die Geschwindigkeit */
    struct Auto * tmp_strasse;

    /* Eine Optimierungsmaßname */

    int v_max; /* Höchstgeschwindigkeit */

    double wahrscheinlichkeit;
    /* die Wahrscheinlichkeit, zufällig zu bremsen. (0<=w<=1)*/

    int fahrzeuge_pro_einheit; /* Variable zur Berechnung des Flußes */
};


struct Konfiguration* initialisieren1(double wahrscheinlichkeit, int v_max,
        int laenge, int anz_autos);
/* Baut eine Straße aus den gegebenen Daten */
struct Konfiguration* initialisieren2(double wahrscheinlichkeit, int v_max,
        const char * strasse);
/* Baut eine Straße nach Vorbild von 'strasse'*/



void deinitialisiere(struct Konfiguration* s);
/* löscht die Straße vorschriftsgemäß */

void fahren( struct Konfiguration* s, int stauzeitenbestimmung);
/* Kernfunktion -- hier spielt sich das Geschehen ab */

int dist(struct Konfiguration *s, int n);
/* Ermittelt den Abstand zum Vordermann */
#include "stauzeit.h"

#endif
