/* Code by Daniel Engelhardt <dengelh@gmail.com>; if we meet some day and you found that useful, buy me a beer */


#include "stauzeit.h"
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>

static struct Label * staus;
static unsigned long label_nr = 0, label_nr_max;

void initialisiere_stauzeiten (void) {
    label_nr_max = 200;
    staus = malloc (label_nr_max * sizeof (* staus));


    if (! staus) {
        printf ("Kein Arbeitsspeicher mehr frei");
        exit(EXIT_FAILURE);
    }
}

void deinitialisiere_stauzeiten (void) {
    free (staus);
}

void drucke_stauzeiten (void) {
    unsigned i, t;
    for (i = 1; i <= label_nr; ++i) {
        t = staus[i].t_end - staus[i].t_start;
        if (t != 0)
            fprintf (stderr, "%d\n", t);
    }
}

static void fuege_stau_ein (struct Label l) {
    if (l.label_nr >= label_nr_max) {
        void *p;

        if (label_nr_max * 2 < label_nr_max) {
            puts ("Speicher voll");
            free (staus);
            exit(EXIT_FAILURE);
        }

        label_nr_max *= 2;
        p = realloc (staus, label_nr_max * sizeof(* staus));
        /* Speicherbereich verdoppeln */

        if (! p) {
            puts ("Speicher voll");
            free (staus);
            exit(EXIT_FAILURE);

        }
        staus = p;
    }

    staus[l.label_nr] = l;
}

void stauzeiten (struct Konfiguration* s, int index, unsigned long t) {
    struct Auto dieses, naechstes;
    int i, j = 0;

    i = index;
    while (s->strasse[i = (i +1)%s->laenge].v != -1) {
        naechstes = s->strasse[i];

        if (j++ > s->v_max) break;
        /* Dann kann ein Auto nicht mehr für ein Abbremsmanöver verantwortlich sein */
    }

    dieses = s->strasse[index];
    dieses.label.alt_t_start = dieses.label.t_start;

    if (s->strasse[index].v == s->v_max) {
        /* Schnelle Autos stehen nicht im Stau -> kein label */
        s->strasse[index].label.t_start = INT_MAX;
        s->strasse[index].label.alt_t_start = INT_MAX;
        s->strasse[index].label.t_end = 0;

    } else {
        /* Hier die langsamen Autos */
        if (naechstes.label.t_start < dieses.label.alt_t_start &&
                naechstes.label.t_start < t) {
            /* Das nächste Auto vor mir sorgt für Stau
            -> übernehme dessen label */
            dieses.label.t_start = naechstes.label.t_start;
            dieses.label.label_nr = naechstes.label.label_nr;
        } else if (dieses.label.alt_t_start < naechstes.label.t_start &&
                   dieses.label.alt_t_start < t) {
            /* Das Auto hat noch nicht voll beschleunigt, es steht
            quasi noch im anderen Stau -> nichts tun */

        } else {
            /* Neuer Stau */
            dieses.label.t_start = t;
            dieses.label.label_nr = ++label_nr;
        }

        dieses.label.t_end = t; /* Das vorläufige Ende ist jetzt */
        fuege_stau_ein (dieses.label);
        s->strasse[index] = dieses;
    }
}
