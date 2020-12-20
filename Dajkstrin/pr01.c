#include <stdio.h>
#include <stdlib.h>
#include "pr01.h"

/*U datoteci prvi broj je ukupni broj cvorova
Prvi broj u svakom redu je cvor, drugi broj u redu je broj suseda
Zatim redom ide susedni cvor i njegova tezina*/
void main()
{
  Graf **G = NULL;

  int poc, kraj;
  Brojaci dim;

  printf("Unesite od kog cvora zelite da krenete, a zatim do kog zelite da stignete:\n" );
  scanf( "%d %d", &poc, &kraj );

  G = napravi_tezinski_graf( &dim ); //Za Dajkstrin algoritam

  Dijkstra( G, dim + 1, poc, kraj );

  stampaj_put( G, poc, kraj );

}
