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

int A = 1;
int B = 1; // broj neradnih 

void *obrada(void *);
void *mreza(void *);

int main(int argc, char *argv[])
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
    printf("Program s PID=%ld krenuo s radom\n", (long) getpid());

    broj = procitaj_status();
    int kvadrat;   
    if (broj == 0)
    {
        kvadrat = pronadji_zadnji_broj();
        broj = (int)sqrt(kvadrat);
    }
    zapisi_status(0);

    pthread_t opisnik[A + B];
    int i, j, id[A + B];
    for (i = 0; i < A; i++)
    {
        id[i] = i;
        if (pthread_create(&opisnik[i], NULL, obrada, &id[i])) {
            fprintf(stderr, "Ne mogu stvoriti novu dretvu, id = %d!\n", i);
            exit(0);
        }
    }
    for (i = 0; i < B; i++)
    {
        id[A + i] = i;
        if (pthread_create(&opisnik[A + i], NULL, mreza, &id[i])) {
            fprintf(stderr, "Ne mogu stvoriti novu dretvu, id = %d!\n", i);
            exit(0);
        }
    }
    
    int x;
	while(nije_kraj)
    {
        sleep(1);
        scanf("%d", &x);
        if (x > 0) broj = x - 1;
        else nije_kraj = 0;
	}

    for (j = 0; j < A; j++)
        pthread_join(opisnik[j], NULL);

    printf("Program s PID=%ld zavrsio s radom\n", (long) getpid());

	return 0;
}

void *obrada(void *p)
{
    if (broj > 0)
    {
        int x;
        while(nije_kraj)
        {
            broj = broj + 1;
            printf("Program: kvadriranje broja %d\n", broj);
            x = broj * broj;

            dodaj_broj(x);
            sleep(5);
        }
    }
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
