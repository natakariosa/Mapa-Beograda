#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define INFINITY 9999

typedef struct _Cvor //cvor liste
{
    long id;
    char ime[100];
    double lat;
    double lon;
    double tezina;
    double rastojanje;
    double put;
    struct _Cvor *sledeci;
}Cvor;
            
typedef struct _Graf
{
    long dim;
    Cvor **putevi;
}Graf;

FILE *otvori( char *ime , char *kako );

Cvor *napravi_cvor( long id , double lat , double lon , double tezina , char *ime );

Graf *inicijalizuj_graf( Graf *G , long dim );

double haversine_formula( double lat1 , double lon1 , double lat2 , double lon2 );

void dodaj_ivicu( Cvor **A , Cvor *B , char *ime );

void dodaj_ime( Graf **G , long id_cvora , char *ime_puta );

Graf *napravi_graf();

void stampaj( Graf *G );
