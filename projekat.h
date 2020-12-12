#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define INFINITY 9999

typedef unsigned short int Brojaci;
typedef unsigned char BYTE;

typedef struct _graf
{
  int id;
  char ime[50];
  int br_ulice;
  double longituda;
  double latituda;
  double tezina;//rastojanje izmedju dva cvora
  double rastojanje;//ukupno rastojanje od pocetnog cvora do njega
  double put;//oznacava njegovog pretka, za najblizi put
  struct _graf *sledeci;
} Graf;

FILE *otvori( char *ime , char *kako );

Graf *napravi_element( int vrednost , double tezina , char *ime , int br_ulice );

void dodaj_ivicu( Graf **glava , double vrednost , double tezina , char *ime );

int prazna_linija( char *linija );

char **napravi_niz( char **niz , int dim );

Graf **napravi_graf( Graf **G , int *dim1 );

void stampaj( Graf **G, Brojaci dim );
