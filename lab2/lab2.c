#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include "signali.h"
#include "datoteka.h"

FILE *fp;

char *dat_status, *dat_obrada, *dat_mreza;
int broj = 0;
int nije_kraj = 1;

int A = 1; /* broj radnih dretvi */
int B = 1; /* broj neradnih dretvi */

/* funkcije koje rade dretve */
void *obrada(void *);
void *mreza(void *);

int main(int argc, char *argv[])
{
	if (argc < 4) {
		fprintf(stderr, "Koristenje: %s <status-datoteka> "
		 "<datoteka-s-podacima> <cjevovod>\n", argv[0]);
		exit(1);
	}
	dat_status = argv[1];
	dat_obrada = argv[2];
	dat_mreza = argv[3];

	postavi_signale();
    printf("Program s PID=%ld krenuo s radom\n", (long) getpid());




	/* dohvati prvi broj i zapisi 0 u status */
	/* todo */
    broj = procitaj_status();
    int kvadrat;    /* broj koji cemo pronaci je potpun kvadrat i bit ce potrebno korjenovati ga za nastavak */
    /* ako je broj == 0 onda citaj brojeve iz datoteke s podacima dok ne dodjes do kraja datoteke */
    /* broj = korijen zadnjeg procitanog broja */
    if (broj == 0) {
        kvadrat = pronadji_zadnji_broj();
        broj = (int)sqrt(kvadrat);
    }
    /* prije nastavka rada u status-datoteku upisi 0 umjesto prijasnjeg broja */
    zapisi_status(0);

	/* stvori dretve: radnu i mreznu */
	/* todo */

    pthread_t opisnik[A + B];
    int i, j, id[A + B];
    for (i = 0; i < A; i++) {   /* stvaranje radnih dretvi */
        id[i] = i;
        if (pthread_create(&opisnik[i], NULL, obrada, &id[i])) {
            fprintf(stderr, "Ne mogu stvoriti novu dretvu, id = %d!\n", i);
            exit(0);
        }
    }
    for (i = 0; i < B; i++) {   /* stvaranje neradnih dretvi */
        id[A + i] = i;
        if (pthread_create(&opisnik[A + i], NULL, mreza, &id[i])) {
            fprintf(stderr, "Ne mogu stvoriti novu dretvu, id = %d!\n", i);
            exit(0);
        }
    }
    
    int x;
	while(nije_kraj) {
		//procitaj broj iz konzole, npr. sa scanf
		//ako je > 0 onda ga postavi kao broj
		//inace prekidni s radom => postavi nije_kraj = 0
        sleep(1);
        scanf("%d", &x);
        if (x > 0) broj = x - 1;
        else nije_kraj = 0;
	}

	//cekaj na kraj dretve obrada, ali ne i one druge mrezne
    for (j = 0; j < A; j++)
        pthread_join(opisnik[j], NULL);

    printf("Program s PID=%ld zavrsio s radom\n", (long) getpid());

	return 0;
}

void *obrada(void *p)
{
	/* vise-manje glavni dio iz lab1 */
    /* ako je broj > 0, program nastavlja s proracunom */
    if (broj > 0) {
        int x;
        /* beskonacna petlja */
        while(nije_kraj) {
            broj = broj + 1;
            printf("Program: kvadriranje broja %d\n", broj);
            x = broj * broj;
            
            /* dodaj x u datoteku s podacima */
            dodaj_broj(x);
            sleep(5);   /* odgodi(5) */
        }
    }

	return NULL;
}

void *mreza(void *p)
{
	//u petlji cekaj da se nesto pojavi u cijevi
	//ako je procitano > 0 onda ga postavi u broj
    int x = 0;
    while(nije_kraj) {
        sleep(1);
        // je li se nesto pojavilo u cijevi?
        // ako jest, dohvati i spremi u x koristeci funkciju dohvati_iz_cijevi()
        x = dohvati_iz_cijevi();
        if (x > 0) broj = x - 1;
    }

	return NULL;

	return NULL;
}
