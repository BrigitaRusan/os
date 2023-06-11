Zadatak
Nadograditi program iz druge vježbe (ali u lab3) tako da ima više radnih dretvi i da se njihov
broj može mijenjati, prema naredbama jedne on ostalih „upravljačkih“ dretvi. Primjerice, jedna
upravljačka dretva utječe na izračun (može promijeniti broj), dok druga definira broj radnih
dretvi.
Neka u programu postoje dvije globalne varijable, npr. broj_dretvi te postavljeni_broj_dretvi.
Prva označava koliko trenutno ima radnih dretvi, a druga koliko ih treba biti. Kada se u
upravljačkoj dretvi postavi novi broj dretvi onda ta upravljačka dretva može odmah stvoriti nove
radne dretve ako je novopostavljeni broj veći. Radne dretve na početku svake iteracije trebaju
provjeriti je li broj dretvi veći od postavljenog broja dretvi. Ako jest onda ta dretva završava s
radom (može i s pthread_exit(NULL), ali prije toga izaći iz monitora).
Koristiti monitore te sve operacije koje koriste zajedničku strukturu podataka (uključujući i
operacije nad datotekama) ostvariti unutar monitora.

ispis nakon pokretanja:

rusan@DESKTOP-LFP7I37:/mnt/c/Users/Rusan/Documents/Brigita/os/lab3$ make pokreni

Program s PID=1573 krenuo s radom
Nitko nece broj 1, povecavam ga
Dretva 2 radi s brojem 2
Upisan broj 4 u datoteku obrada.
Dretva 2 staje s radom

Dretva 3 radi s brojem 3
Upisan broj 9 u datoteku obrada.
Dretva 3 staje s radom

Dretva 4 radi s brojem 4
Upisan broj 16 u datoteku obrada.
Dretva 4 staje s radom

Dretva 5 radi s brojem 5
Upisan broj 25 u datoteku obrada.
Dretva 5 staje s radom

Dretva 6 radi s brojem 6
Upisan broj 36 u datoteku obrada.
Dretva 6 staje s radom

Nitko nece broj 7, povecavam ga
Dretva 2 radi s brojem 8
Upisan broj 64 u datoteku obrada.
Dretva 2 staje s radom

Dretva 3 radi s brojem 9
Upisan broj 81 u datoteku obrada.
Dretva 3 staje s radom

Dretva 5 radi s brojem 10
Upisan broj 100 u datoteku obrada.
Dretva 5 staje s radom

Nitko nece broj 11, povecavam ga
Dretva 6 radi s brojem 12
Upisan broj 144 u datoteku obrada.
Dretva 6 staje s radom

Nitko nece broj 13, povecavam ga
Dretva 2 radi s brojem 14
Upisan broj 196 u datoteku obrada.
Dretva 2 staje s radom

Dretva 5 radi s brojem 15
Upisan broj 225 u datoteku obrada.
Dretva 5 staje s radom

Dretva 4 radi s brojem 16
Upisan broj 256 u datoteku obrada.
Dretva 4 staje s radom