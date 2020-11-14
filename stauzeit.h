/* Code by Daniel Engelhardt <dengelh@gmail.com>; if we meet some day and you found that useful, buy me a beer */

#ifndef STAU_H
#define STAU_H

#include "fahren.h"

void initialisiere_stauzeiten(void);
void deinitialisiere_stauzeiten(void);

void drucke_stauzeiten(void);

void stauzeiten(struct Konfiguration* s, int index, unsigned long t);

#endif

