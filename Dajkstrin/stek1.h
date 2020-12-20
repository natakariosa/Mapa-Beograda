#include <stdio.h>
#include <stdlib.h>

typedef unsigned char BYTE;


typedef struct _Stek
{
  int vrednost;
  struct _Stek *sledeci;
} Stek;

Stek *push( Stek *top, int broj )
{
   Stek *novi = NULL;

   novi = ( Stek * ) malloc( sizeof( Stek ) );

   novi->vrednost = broj;

   novi->sledeci = top;

   top = novi;

   return( top );
}

Stek *pop( Stek *top )
{
  Stek *pom = top;

  if( !( top->sledeci ) )
  {
    free( top );
    return( NULL );
  }

  else
  {
    top = pom->sledeci;

    free( pom );

    return( top );
  }


}

BYTE jel_prazan( Stek *vrh )
{
  return( vrh != NULL );
}
