#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <math.h>

FILE *fp;

/* funkcije za obradu signala */
void obradi_dogadjaj(int sig);
void obradi_sigterm(int sig);
void obradi_sigint(int sig);


/* funkcije za rad s datotekama */
int procitaj_status();
void zapisi_status(int broj);
void dodaj_broj(int broj);
int pronadji_zadnji_broj();


int nije_kraj = 1;

/* globalne varijable */
int broj = 0;
const char* status = "status.txt";
const char* obrada = "obrada.txt";

int main()
{
    struct sigaction act;

    /* 1. maskiranje signala SIGUSR1 */
    
    act.sa_handler = obradi_dogadjaj; /* funkcija kojom se signal obradjuje */
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, SIGTERM); /* blokirati i SIGTERM za vrijeme obrade */
    act.sa_flags = 0; /* naprednije mogucnosti preskocene */
    sigaction(SIGUSR1, &act, NULL); /* maskiranje signala preko sucelja OS-a */

    /* 2. maskiranje signala SIGTERM */
    
    act.sa_handler = obradi_sigterm;
    sigemptyset(&act.sa_mask);
    sigaction(SIGTERM, &act, NULL);

    /* 3. maskiranje signala SIGINT */
    
    act.sa_handler = obradi_sigint;
    sigaction(SIGINT, &act, NULL);

    /* pocetak */
    broj = procitaj_status();
    
    int x;
    if (broj == 0) {
		printf("program bio prekinut, potraga za brojem...\n");
		broj = pronadji_zadnji_broj();
		broj = (int) sqrt(broj);
	}
	zapisi_status(0); //radim
	//printf("krecem s radom, zadnji broj=%d\n", broj);
	printf("Program s PID=%ld krenuo s radom\n", (long) getpid());

	//simulacija obrade
	while (nije_kraj) {
		printf("Program: iteracija %d\n", broj++);
		x = broj * broj;
		dodaj_broj(x);
		sleep(1);
	}

	//kraj
	zapisi_status(broj);
	//printf("kraj rada, zadnji broj=%d\n", broj);
	printf("Program s PID=%ld zavrsio s radom\n", (long) getpid());

    return 0;
}

int procitaj_status()
{
	int broj;
	FILE *fp;

	fp = fopen(status, "r");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", status);
		exit(1);
	}
	if (fscanf(fp, "%d", &broj) != 1) {
		printf("Nije procitan broj iz %s!\n", status);
		exit(1);
	}
	fclose(fp);
	return broj;
}

void zapisi_status(int broj)
{
	FILE *fp;

	fp = fopen(status, "w");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", status);
		exit(1);
	}
	if (fprintf(fp, "%d\n", broj) < 1) {
		printf("Nije upisan broj u %s!\n", status);
		exit(1);
	}
	fclose(fp);
}

void dodaj_broj(int broj)
{
	FILE *fp;

	fp = fopen(obrada, "a");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", obrada);
		exit(1);
	}
	if (fprintf(fp, "%d\n", broj) < 1) {
		printf("Nije upisan broj u %s!\n", obrada);
		exit(1);
	}
	fclose(fp);
}

int pronadji_zadnji_broj()
{
	FILE *fp;
	int broj = -1;

	fp = fopen(obrada, "r");
	if (!fp) {
		printf("Ne mogu otvoriti %s\n", obrada);
		exit(1);
	}
	while(!feof(fp))
		if (fscanf(fp, "%d", &broj) != 1)
			break;
	fclose(fp);
	return broj;
}


void obradi_dogadjaj(int sig)
{
	int i;
	printf("Pocetak obrade signala %d\n", sig);
	for (i = 1; i <= 5; i++) {
		printf("Obrada signala %d: %d/5\n", sig, i);
		sleep(1);
	}
	printf("Kraj obrade signala %d\n", sig);
}
void obradi_sigterm(int sig)
{
    printf("Primio signal SIGTERM\n");

    fp = fopen(status, "w");
    if (fp == NULL) {
        printf("Greska!\n");
        exit(0);
    }
    fprintf(fp, "%d", broj);	/* zapisi broj u status.txt */
    fclose(fp);

    nije_kraj = 0;
}
void obradi_sigint(int sig)
{
    printf("Primio signal SIGINT, prekidam rad\n");
    exit(1);
}
