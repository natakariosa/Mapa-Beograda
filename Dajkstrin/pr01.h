#include <stdio.h>
#include <stdlib.h>
#include "stek1.h"
#include "red1.h"

typedef unsigned short int Brojaci;
typedef unsigned char BYTE;

#define INFINITY 9999

typedef struct _graf
{
  int broj;

  int tezina;//rastojanje izmedju dva cvora
  int rastojanje;//ukupno rastojanje od pocetnog cvora do njega
  int put;//oznacava njegovog pretka, za najblizi put

  struct _graf *sledeci;
} Graf;


//----------------------------------------------------


Graf *napravi_element( int vrednost, int tezina )
{
  Graf *novi = ( Graf * ) malloc( sizeof( Graf ) );
  novi->broj = vrednost;
  novi->tezina = tezina;
  novi->sledeci = NULL;

  return( novi );
}

void dodaj_ivicu( Graf **glava, int vrednost, int tezina )
{
  Graf *pom = NULL;

  if( *glava == NULL )
  {
    *glava = napravi_element( vrednost, tezina );
    ( *( glava ) )->rastojanje = tezina;
    ( *( glava ) )->tezina = 0;
  }

  else
  {
    pom = *glava;
    while( pom->sledeci ) pom = pom->sledeci;
    pom->sledeci = napravi_element( vrednost, tezina );
  }

}

Graf **napravi_tezinski_graf( Brojaci *dim1 )
{
  Graf **G = NULL;

  int cvor, sused, tezina;

  Brojaci dim2, i, j;
  Brojaci bru1e = 0;

  FILE *dat = fopen( "Dijkstra.txt", "r" );
  if( !dat )
  {
    printf( "Greska pri otvaranju datoteke!\n" );
    exit( EXIT_FAILURE );
  };

  fscanf( dat, "%hu", dim1 );

  G = ( Graf ** ) malloc( ( *dim1 + 1 ) * sizeof( Graf * ) ); //Notacija od 1, ne od 0..
  if( G == NULL )
  {
    printf( "Greska pri alokaciji memorije!\n" );
    exit( EXIT_FAILURE );
  };

  for( i = 0 ; i < *dim1 ; i++ )
  {
    fscanf( dat, "%d %hu", &cvor, &dim2 );
    dodaj_ivicu( &( *( G + cvor ) ), cvor, INFINITY );

    for( j = 0 ; j < dim2 ; j++ )
    {
      bru1e += fscanf( dat, "%d %d", &sused, &tezina );
      dodaj_ivicu( &( *( G + cvor ) ), sused, tezina );
    };

    if( bru1e != 2 * dim2 ) continue;

    bru1e = 0;
  };

  fclose( dat );

  return( G );
}

void Dijkstra( Graf **G, Brojaci dim, int poc, int kraj )
{
  Graf *pom = *( G + poc );

  int poseceni[dim];
  Brojaci i;

  Red *niz = NULL;
  Red *rep = NULL;

  for( i = 1 ; i < dim ; i++ ) poseceni[i] = 0;

  ( *( G + poc ) )->rastojanje = 0;

  if( !pom )
  {
    printf( "Ne postoji dati cvor u grafu!\n" );
    exit( EXIT_FAILURE );
  };

  stavi( &niz, &rep, poc );

  while( jel_red_prazan( niz ) ) //sve dok ne proverimo sve puteve
  {
    poc = skini( &niz );

    poseceni[ poc ] = 1;

    pom = *( G + poc );

    pom = pom->sledeci;

    while( pom ) //BFS Obilazak
    {
      if( !poseceni[pom->broj] )
      {
        stavi( &niz, &rep, pom->broj ); //Samo ako prethodno nije bio u redu

        poseceni[pom->broj] = 1;
      };

      if( (*( G + poc ))->rastojanje + pom->tezina < (*( G + pom->broj ))->rastojanje ) //ako je put preko tog cvora manji od starog
      {
        (*( G + pom->broj ))->rastojanje = (*( G + poc ))->rastojanje + pom->tezina;

        (*( G + pom->broj ))->put = poc; //Oznacavamo kako posle da znamo put od poc do cilja
      };

      pom = pom->sledeci;
    };
  };

  free( pom );
}

void stampaj_put( Graf **G, int poc, int kraj )
{
  Brojaci i = kraj;
  char cvorovi[14] = { ' ', 'A', 'B', 'D', 'H', 'S', 'F', 'G', 'E', 'C', 'L', 'I', 'J', 'K' };

  printf( "Najkrace rastojanje od cvora %c do cvora %c je %d\n", cvorovi[poc], cvorovi[kraj], ( *( G + kraj ) )->rastojanje );
  printf( "Putanja kojom treba da idemo je:\n" );
  printf("[%c] <- ", cvorovi[( *( G + i ) )->broj] );

  while( i != poc )
  {
    printf( "[%c] <- ", cvorovi[( *( G + i ) )->put] );
    i = ( *( G + i ) )->put;
  };

  printf( "\n" );
}

void oslobodi_memoriju( Graf **G, Brojaci dim )
{
  Brojaci i;
  Graf *pom;

  for( i = 0 ; i < dim + 1 ; i++ )
  {
    pom = *( G + i );

    while( pom )
    {
      *( G + i ) = pom;
      pom = pom->sledeci;
      free( *( G + i ) );
    }
  }

  free( G );
}
