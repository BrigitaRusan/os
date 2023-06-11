#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include "signali.h"
#include "datoteka.h"

#define BR_DRETVI 5
#define BROJI_DO 25  //max broj dretvi

pthread_mutex_t m;  // monitor
pthread_cond_t red; // red uvjeta

int broj = 0 ; //1, 2, ...
int uzeo_broj = 0;
time_t zadnja_obrada = 0;

int nije_kraj = 1;

int postavljeni_broj_dretvi = 5; //10
int broj_dretvi = 0;
int dost_DRETVE[BROJI_DO];

FILE *fp;

char *dat_status, *dat_obrada, *dat_mreza;

//funkcije lab 3
void *dretva (void *p);
void *kada_nitko_nece(void *p);

// funkcije koje rade dretve lab2
void *obrada(void *);
void *mreza(void *);

int main ( int argc, char *argv[] )
{
    if (argc < 4) 
    {
        fprintf(stderr, "Koristenje: %s <status-datoteka> "
            "<datoteka-s-podacima> <cjevovod>\n", argv[0]);
        exit(1);
	}
	dat_status = argv[1];
	dat_obrada = argv[2];
	dat_mreza = argv[3];

    fp = fopen(dat_status, "r");
	if (!fp)
	{
		fp = fopen(dat_status, "w");
		fprintf(fp, "1 ");
		fclose(fp);
	}
	else fclose(fp);

	fp = fopen(dat_obrada, "r");
	if (!fp)
	{
		fp = fopen(dat_obrada, "w");
		fprintf(fp, "1 ");
		fclose(fp);
	}
	else fclose(fp);

	fp = fopen(dat_mreza, "r");
	if (!fp)
	{
		fp = fopen(dat_mreza, "w");
		fclose(fp);
	}
	else fclose(fp);
	
    postavi_signale();

    if (BR_DRETVI > BROJI_DO)
    {
        printf("Postavljeni broj dretvi veci je od maksimalnog broja dretvi"
                "Potrebno je nanovo inicijalizirati globalnu varijablu.\n");
        exit(3);
    } 

	printf("Program s PID=%ld krenuo s radom\n", (long)getpid());

    broj = procitaj_status();
    int kvadrat; 
    if (broj == 0) 
    {
        kvadrat = pronadji_zadnji_broj();
        broj = (int)sqrt(kvadrat);
    }
    zapisi_status(0);

    pthread_t t[BR_DRETVI + 1];
    int i, id[BR_DRETVI];

    for (i = 0; i < BROJI_DO; i++)
        dost_DRETVE[i] = 0;  // inicijalizacija polja dostupnost

    pthread_mutex_init (&m, NULL);
    pthread_cond_init (&red, NULL);

    //broj = 2;
    zadnja_obrada = time(NULL);

    for (i = 0; i < BR_DRETVI; i++) 
    {
        id[i] = i + 2;

        pthread_create (&t[i], NULL, dretva, &id[i]);
        broj_dretvi++; 
        dost_DRETVE[i] = 1; 

    }
    pthread_create (&t[i], NULL, kada_nitko_nece, NULL);

    int novi, j;
    while (nije_kraj)
    {
        sleep(1);
        scanf("%d", &novi);
        if (novi > 0)
        {
            if (novi <= BROJI_DO)
            {
                postavljeni_broj_dretvi = novi;
                if (postavljeni_broj_dretvi > broj_dretvi)
                {
                    int n = postavljeni_broj_dretvi - broj_dretvi;
                    for ( i = 0; i < n; i++)
                    { 
                        for (j = 0; j < BR_DRETVI; j++)
                        { 
                            if (!dost_DRETVE[j])
                            {
                                id[j] = j + 1;  // pocetna 1
                                break;
                            } 
                        }
                        if (pthread_create(&t[j], NULL, dretva, &id[j]))
                        {
                            fprintf(stderr, "Ne mogu stvoriti novu dretvu, id = %d!\n", j + 1);
                            exit(1);
                        }
                        broj_dretvi++;
                        dost_DRETVE[j] = 1;
                    }
                }
            }
            else
            {
                printf("Ponoviti unos! Novi broj radnih dretvi mora biti <= %d.\n", BR_DRETVI);
            }
        }       
        else nije_kraj = 0;
	}


    for (i = 0; i < BROJI_DO + 1; i++)
    {
        if (dost_DRETVE[i] == 1)
            pthread_join (t[i], NULL);
    }
    pthread_mutex_destroy (&m);
    pthread_cond_destroy (&red);

	printf("Program s PID=%ld zavrsio s radom\n", (long)getpid());

    return 0;
}


void *dretva (void *p)
{
	int id = *((int *) p);

	pthread_mutex_lock (&m);
	while (broj <= BROJI_DO)
    {
		while (broj <= BROJI_DO && (uzeo_broj != 0 || broj % id != 0))
			pthread_cond_wait (&red, &m);
		if (broj > BROJI_DO)
			break;
		uzeo_broj = 1;
		pthread_mutex_unlock (&m);

        if (broj_dretvi > postavljeni_broj_dretvi && id != 0)
		{
			dost_DRETVE[id] = 0;
			uzeo_broj = 0;
			broj_dretvi--;
			pthread_exit(NULL);
		}

		printf ("Dretva %d radi s brojem %d\n", id, broj);
		sleep(1);

        pthread_mutex_lock (&m);

        int y;
        y = broj * broj;
        dodaj_broj(y); 
        pthread_mutex_unlock (&m);

		printf ("Dretva %d staje s radom\n\n", id);

		pthread_mutex_lock (&m);
		uzeo_broj = 0;
		broj++;
		zadnja_obrada = time(NULL);
		pthread_mutex_unlock (&m);
		pthread_cond_broadcast (&red);
		sleep(BR_DRETVI - (id - 1));
		pthread_mutex_lock (&m);
	}
	pthread_mutex_unlock (&m);
	pthread_cond_broadcast (&red);

	return NULL;
}

void *kada_nitko_nece(void *p)
{
	pthread_mutex_lock (&m);
	while (broj <= BROJI_DO) {
		if (uzeo_broj != 0)
        {
			//netko je uzeo broj
			pthread_cond_wait (&red, &m);
		}
		else if (zadnja_obrada + BR_DRETVI > time(NULL))
        {
			//daj im vremena da uzmu broj
			pthread_mutex_unlock (&m);
			sleep(zadnja_obrada + BR_DRETVI - time(NULL));
			pthread_mutex_lock (&m);
		}
		else
        {
			printf("Nitko nece broj %d, povecavam ga\n", broj);
			broj++;
			zadnja_obrada = time(NULL);
			pthread_cond_broadcast (&red);
		}
	}
	pthread_mutex_unlock (&m);
    return NULL;
}

void *mreza(void *p)
{
    int broj, br_cij = 0;
    while (nije_kraj)
	{
		br_cij = dohvati_iz_cijevi();

		if (br_cij > broj && br_cij > 0)
		{
			printf("Broj %d se pojavio u cijevi.\n", br_cij);
			broj = br_cij - 1;
			br_cij = 0;
		}
		sleep(3);
	}
	return NULL;
}