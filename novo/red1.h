#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;

typedef struct _red
{
  int vrednost;
  struct _red *sledeci;
  struct _red *prethodni;
} Red;

Red *novi_cvor( int broj )
{
  Red *novi = ( Red * ) malloc( sizeof( Red ) );
  if( !novi )
  {
    printf("Greska pri alokaciji memorije!\n" );
    exit( EXIT_FAILURE );
  };

  novi->vrednost = broj;
  novi->sledeci = NULL;
  novi->prethodni = NULL;

  return( novi );
}

void push( Red **niz, Red **rep, int broj )
{
  Red *pom = NULL;

  if( *niz == NULL )
  {
    *niz = novi_cvor( broj );
    *rep = *niz;
  }

  else
  {
    (*rep)->sledeci = novi_cvor( broj );

    pom = *rep;

    *rep = (*rep)->sledeci;

    (*rep)->prethodni = pom;
  }
}

int pop( Red **niz )
{
  int broj;
  Red *pom = *niz;

  broj = pom->vrednost;

  if( !((*niz)->sledeci) )
  {
    free( *niz );
    *niz = NULL;
  }
  else
  {
    *niz = (*niz)->sledeci;
    pom->sledeci = NULL;
    free( pom );
  }

  return( broj );
}

BYTE jel_red_prazan( Red *red )
{
  return( red != NULL );
}
