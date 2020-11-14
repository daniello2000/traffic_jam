/* Code by Daniel Engelhardt <dengelh@gmail.com>; if we meet some day and you found that useful, buy me a beer */

/*  Eine Implementierung des Nagel-Schreckenberg-Modells */

#include <stdlib.h>
#include <stdio.h>
#include "stauzeit.h"
#include "fahren.h"
#include "ausgabe.h"

#define HILFE_TEXT \
"Verkehr von Daniel Engelhardt (2004 / 2005) für die Facharbeit \n\
Selbstorganisation: Ist ein Verkehrsstau Chaos?\n\
\n\
Programmnutzung: \n\
$ verkehr <Parameter >\n\
\n\
Erklärung der Parameter : \n\
\n\
w <zahl>: Die Zahl gibt die Wahrscheinlichkeit an, daß ein Fahrer \n\
zufällig trödelt. <zahl> ist in diesem Falle eine Fließ-\n\
kommazahl 0 <= <zahl> <= 1. \n\
Voreinstellung für diese Option ist 0.5 \n\
\n\
v <zahl>: Die Zahl gibt die Maximalgeschwindigkeit (Felder / Zeitschritt)\n\
an. In diesem Falle ist <zahl> eine natürliche Zahl > 0. \n\
Voreinstellung ist 5. \n\
\n\
L <zahl>: Die Zahl gibt die Länge der kreisrunden Straße an (in\n\
Vielfachen der Länge eines Autos). Hier ist <zahl> eine \n\
natürliche Zahl > 0. \n\
Für diese Option gibt es keine Voreinstellung. \n\
\n\
a <zahl>: diese Zahl gibt die Anzahl der Autos auf der Straße von L\n\
an. <zahl> ist eine natürliche Zahl zwischen 0 und dem bei L\n\
angegebenem Wert. \n\
Für diese Option gibt es keine Voreinstellung \n\
\n\
k <string>: Bei dieser Option kann man direkt eine Straße (mit Autos)\n\
eingeben, wie sie von 'p 2' ausgegeben wird. Diese Option \n\
schließt Option 'a' und 'L' aus.\n\
\n\
d <zahl>: diese Option gibt an, wie viele Zyklen das Programm berechnen \n\
soll. <zahl> ist hier eine natürliche Zahl > 0. \n\
\n\
p <zahl>: diese Zahl legt den Ausgabemodus fest: \n\
Für <zahl> = 1 : \n\
Keine Ausgabe der einzelnen Schritte \n\
<zahl> = 2 : \n\
Gibt die Straße in ASCII aus \n\
<zahl> = 3 : \n\
Gibt die Straße im ppm−Format aus \n\
<zahl> = 4\n\
Gibt die Durchschnittsgeschwindigkeiten in jeder \n\
Runde aus\n\
Andere Zahlen sind nicht gültig.\n\
Voreinstellung i s t <zahl> = 2. \n\
\n\
f: Bei Wahl dieser Option werden am Ende auch noch die Daten des \n\
Fundamentaldiagramms, der Fluß und die Dichte ausgegeben,\n\
wobei die Dichte einfach a/L ist.\n\
\n\
t: Bei Wahl dieser Option werden am Ende auch noch die Stauzeiten\n\
ausgegeben. Jedes Fahrzeug das nicht mit 'v_max', also der Höchst-\n\
Geschwindigkeit steht, steht laut Definition hier im Stau.\n\
\n"

void hilfe(void);

int main (int argc, char ** argv) {
    int i;

    unsigned /* Typische Modellannahmen */
        v_max = 5,
        lae = 100,
        anz = 20,
        durchlaeuf = 0,
        druck = 2,
        fundamental = 0,
        stauzeitbestimmung = 0;
    const char * str = 0;
    double wa = 0.5;
    struct Konfiguration* strasse;
    for(i = 1; i < argc; ++i) {
        switch(argv[ i ] [ 0 ]) {
        case 'w':
            if (argv[i +1] != 0) wa = atof(argv[++i]);
            else hilfe();
            break;
        case'v':
            if (argv[i +1] != 0) v_max = atoi(argv[++i]);
            else hilfe();
            break;
        case'L':
            if (argv[i +1] != 0) lae = atoi(argv[++i]);
            else hilfe();
            break;
        case'a':
            if (argv[i +1] != 0) anz = atoi(argv[++i]);
            else hilfe();
            break;
        case'k':
            if (argv[i +1] != 0) str = argv[++i];
            else hilfe();
            break;
        case'd':
            if (argv[i +1] != 0) durchlaeuf = atoi(argv[++i]);
            else hilfe();
            break;
        case'p':
            if (argv[i +1] != 0) druck = atoi(argv[++i]);
            break;
        case'f':
                fundamental = 1;
            break;
        case't':
            stauzeitbestimmung = 1;
            break;
        default :
            hilfe ();
        }
    }
    if (str && *str && wa && v_max)
        strasse = initialisieren2(wa, v_max, str);
    else if(wa && lae && v_max && anz)
        strasse = initialisieren1(wa, v_max,lae, anz);
    else
        hilfe ();


    if (durchlaeuf == 0) durchlaeuf = -1;    /* dann läuft das Programm nicht. */


    if (stauzeitbestimmung)
        initialisiere_stauzeiten ();

    for(i = 0; i < (int) durchlaeuf; ++i) {
        if(stauzeitbestimmung)
            fahren(strasse, i);
        else
            fahren(strasse, -1);

        ausgabe (strasse, druck, durchlaeuf);
    }

    deinitialisiere(strasse);

    if( fundamental)
        printf("Fahrzeugdichte: %g, Verkehrsfluß: %g\n",(double)(anz) / lae,
                (double)(strasse ->fahrzeuge_pro_einheit) / durchlaeuf);

    if( stauzeitbestimmung) {
        drucke_stauzeiten ();
        deinitialisiere_stauzeiten ();
    }
    return 0;
}

void hilfe(void) {
    puts(HILFE_TEXT);
    exit(EXIT_FAILURE);
}

