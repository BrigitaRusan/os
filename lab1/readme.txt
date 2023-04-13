Zadatak: 

Osmisliti primjer koji će koristiti barem tri signala za neke radnje.
Prethodni primjer programa koristi SIGINT, SIGTERM i SIGUSR1, svaki sa svojom
(uobičajenom) namjenom.
Primjer zadatka za program – može se ostvariti ovaj ili slični, ali ne jednostavniji.
Neka program simulira neki dugotrajni posao (slično servisima) koji koristi dvije datoteke: u
jednu dodaje do sada izračunate vrijednosti (npr. kvadrati slijednih brojeva), a u drugu podatak
do kuda je stigao. Npr. u obrada.txt zapisuje 1 4 9 … a u status.txt informaciju o tome
gdje je stao ili kako nastaviti. Npr. ako je zadnji broj u obrada.txt 100 u status.txt moglo
bi se zapisati 10 tako da u idućem pokretanju može nastaviti raditi i dodavati brojeve.
Prije pokretanja te se datoteke mogu ručno napraviti i inicijalizirati, ali ih može i program, ako ne postoje. Početne vrijednosti mogu biti iste – broj 1 u obje datoteke.
Pri pokretanju programa on bi trebao otvoriti obje datoteke, iz status.txt, pročitati tamo
zapisanu vrijednost. Ako je ona veća od nule program nastavlja s proračunom s prvom idućom
vrijednošću i izračunate vrijednosti nadodaje u obrada.txt. Prije nastavka rada u
status.txt upisuje 0 umjesto prijašnjeg broja, što treba označavati da je obrada u tijeku, da
program radi.
Na signal (npr. SIGUSR1) program treba ispisati trenutni broj koji koristi u obradi. Na signal
SIGTERM otvoriti status.txt i tamo zapisati zadnji broj (umjesto nule koja je tamo) te završiti
s radom.
Na SIGINT samo prekinuti s radom čime će u status.txt ostati nula (čak se taj signal niti
ne mora maskirati – prekid je pretpostavljeno ponašanje!). To će uzrokovati da iduće
pokretanje detektira prekid – nula u status.txt, te će za nastavak rada, tj. određivanje
idućeg broja morati „analizirati“ datoteku obrada.txt i od tamo zaključiti kako nastaviti
(pročitati zadnji broj i izračunati korijen).
Operacije s datotekama, radi jednostavnosti, uvijek mogu biti open+fscanf/fprintf+close, tj. ne
držati datoteke otvorenima da se izbjegnu neki drugi problemi.
U obradu dodati odgodu (npr. sleep(5)) da se uspori izvođenje.

Primjer pseudokoda za navedeni primjer zadatka:

globalne varijable:
	broj
	imena datoteka
	
program
	maskiraj signale
	
	broj = pročitaj broj iz status.txt
	ako je broj == 0 onda
		čitaj brojeve iz obrada.txt dok ne dođeš do kraja datoteke
		broj = korijen od zadnjeg pročitanog broja
	zapiši 0 u status.txt na početak datoteke (prepiši ono što je bilo!)
	ponavljaj //beskonačna petlja
		broj = broj + 1
		x = obrada(broj)
		dodaj x u obrada.txt
		odgodi(5)
	
na sigusr1:
	ispiši broj
	
na sigterm:
	zapiši broj u status.txt
	završi program
	
na sigint:
	završi program
	
	
Koristiti Makefile za prevođenje i pokretanje uz ciljeve pokreni i obrisi. U prvom labosu može
sav kod biti u jednoj datoteci, ali može i u više njih (za to pogledati primjer Makefile-a iz drugog
labosa).
