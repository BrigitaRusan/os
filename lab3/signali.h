#pragma once /*zaglavlje se ne uključuje više puta u istu datoteku */

extern int broj, nije_kraj;
extern char* dat_status; 

/* funkcije za obradu signala */
void postavi_signale();
void obradi_dogadjaj(int sig);
void obradi_sigterm(int sig);
void obradi_sigint(int sig);
