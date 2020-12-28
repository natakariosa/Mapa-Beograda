#include "projekat.h"
#include "red1.h"
#include <math.h>

FILE *otvori( char *ime , char *kako )
{
    FILE *d = fopen( ime , kako );
    
    if( d == NULL ) exit( EXIT_FAILURE );
    
    return( d );
}

Cvor *napravi_cvor( long id , double lat , double lon , double tezina , char *ime, long brojac )
{
    Cvor *novi_el = ( Cvor * ) malloc( sizeof( Cvor ) );
    if( novi_el == NULL ) exit( EXIT_FAILURE );
    
    novi_el->id = id;
    novi_el->redniBroj = brojac;
    novi_el->lat = lat;
    novi_el->lon = lon;
    novi_el->tezina = tezina;
    novi_el->put = INFINITY;
    novi_el->rastojanje = INFINITY;
    novi_el->sledeci = NULL;
    novi_el->dim_jednosmerne = 0;
    novi_el->jednosmerne = ( long * ) malloc( sizeof( long ) );

    strcpy( novi_el->ime , ime );
    
    return( novi_el );
}

Graf *inicijalizuj_graf( Graf *G , long dim )
{
    int i;
    
    G = ( Graf * ) malloc( sizeof( Graf ) );
    if( G == NULL ) exit( EXIT_FAILURE );
    
    G->dim = dim;
    G->putevi = ( Cvor ** ) malloc( sizeof( Cvor * ) * G->dim );
    
    for( i = 0 ; i < G->dim ; i++ ) G->putevi[i] = ( Cvor * ) malloc( sizeof( Cvor ) );
    
    return( G );
}

double haversine_formula( double lat1 , double lon1 , double lat2 , double lon2 ) 
{ 
    double dLat = (lat2 - lat1) * M_PI/180.0; 
    double dLon = (lon2 - lon1) * M_PI/180.0; 
    
    lat1 = (lat1) * M_PI/180.0; //radijani
    lat2 = (lat2) * M_PI/180.0; 
  
    double a = sin(dLat/2)*sin(dLat/2) + sin(dLon/2)*sin(dLon/2) * cos(lat1)*cos(lat2); //formula
    double rad = 6371; //polurecnik Zemlje u km
    double c = 2*asin(sqrt(a)); 
    
    return( 1000.0*rad*c ); //posto bi inace vratio u kilometrima 
}     

void dodaj_ivicu( Cvor **A , Cvor *B , char *ime )
{
    Cvor *pom = NULL;
    double tezina = haversine_formula( (*A)->lat , (*A)->lon , B->lat , B->lon );

    if( *A == NULL ) *A = napravi_cvor( B->id , B->lat , B->lon , tezina , ime, B->redniBroj );
    else
    {
        pom = *A;

        while( pom->sledeci ) pom = pom->sledeci;

        pom->sledeci = napravi_cvor( B->id , B->lat , B->lon , tezina , ime, B->redniBroj ); 
    };
}

Graf *napravi_graf()
{
    Graf *G = NULL;
    Cvor *pomocni = NULL;
    FILE *dat1 = otvori( "pevac_putevi.txt", "r" ) , *dat2 = otvori( "pevac_cvorovi.txt", "r" );
    char c , ind1 = 0 , ind2 = 0 , ime[100] , pom[100] , *ime_puta = NULL;
    long i = 0 , j = 0 , k = 0 , m , ind = 0, brojac = 0, z = 0;
    long dim , id , prvi , drugi , id_cvora , id_puta , *niz_id_cvorova = NULL;
    double lat , lon;
    
    while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
    
    pom[j] = '\0';
    
    j = 0;
    
    dim = atof( pom );
 
    G = inicijalizuj_graf( G , dim );
    
    while( !feof( dat2 ) )
    {
        fscanf( dat2 , "%ld %lf %lf" , &id , &lat , &lon );
        
        G->putevi[i++] = napravi_cvor( id , lat , lon , 0 , "Nema", brojac++ );
    };
    
    i = 0;
    
    while( !feof( dat1 ) )
    {
        while( ( c = fgetc( dat1 ) ) != ' ' && c != EOF ) ime[i++] = c;
        
        if( c == EOF ) break;
        
        ime[i] = '\0'; 
        
        if( !strcmp( ime , "way" ) )
        {
            while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
            
            pom[j] = '\0';
            
            id_puta = atof( pom );
            
            if( k != 0 ) 
            {
                if( ime_puta )
                {
                    for( m = 0 ; m < k -1 ; m++ )
                    {
                        for( i = 0 ; i < G->dim , ( ind1 + ind2 ) != 2 ; i++ ) 
                        {
                            if( ind1 == 0 && G->putevi[i]->id == niz_id_cvorova[m] )
                            {
                                prvi = i;
                                ind1 = 1;
                            }
                            else if( ind2 == 0 && G->putevi[i]->id == niz_id_cvorova[m+1] )
                            {
                                drugi = i;
                                ind2 = 1;
                            };
                        };

                        dodaj_ivicu( &G->putevi[prvi] , G->putevi[drugi] , ime_puta );
                        
                        ind1 = 0;
                        ind2 = 0;
                    };
                }
                else 
                {
                    for( m = 0 ; m < k -1 ; m++ )
                    {
                        for( i = 0 ; i < G->dim , ( ind1 + ind2 ) != 2 ; i++ ) 
                        {
                            if( ind1 == 0 && G->putevi[i]->id == niz_id_cvorova[m] )
                            {
                                prvi = i;
                                ind1 = 1;
                            }
                            else if( ind2 == 0 && G->putevi[i]->id == niz_id_cvorova[m+1] )
                            {
                                drugi = i;
                                ind2 = 1;
                            };
                        };

                        dodaj_ivicu( &G->putevi[prvi] , G->putevi[drugi] , "Nema" );
                        
                        ind1 = 0;
                        ind2 = 0;
                    };
                };
            
                if( ind == 0 )
                {
                    if( ime_puta )
                    {
                        for( m = k - 1 ; m > 0 ; m-- )
                        {
                            for( i = 0 ; i < G->dim , ( ind1 + ind2 ) != 2 ; i++ ) 
                            {
                                if( ind1 == 0 && G->putevi[i]->id == niz_id_cvorova[m] )
                                {
                                    prvi = i;
                                    ind1 = 1;
                                }
                                else if( ind2 == 0 && G->putevi[i]->id == niz_id_cvorova[m-1] )
                                {
                                    drugi = i;
                                    ind2 = 1;
                                };
                            };

                            dodaj_ivicu( &G->putevi[prvi] , G->putevi[drugi] , ime_puta );
                        
                            ind1 = 0;
                            ind2 = 0;
                        };
                    }
                    else
                    {
                        for( m = k - 1 ; m > 0 ; m-- )
                        {
                            for( i = 0 ; i < G->dim , ( ind1 + ind2 ) != 2 ; i++ ) 
                            {
                                if( ind1 == 0 && G->putevi[i]->id == niz_id_cvorova[m] )
                                {
                                    prvi = i;
                                    ind1 = 1;
                                }
                                else if( ind2 == 0 && G->putevi[i]->id == niz_id_cvorova[m-1] )
                                {
                                    drugi = i;
                                    ind2 = 1;
                                };
                            };
                            
                            dodaj_ivicu( &G->putevi[prvi] , G->putevi[drugi] , "Nema" );
                        
                            ind1 = 0;
                            ind2 = 0;
                        };
                    };
                }
                else
                {
                    G->putevi[drugi]->jednosmerne[ G->putevi[drugi]->dim_jednosmerne] = G->putevi[prvi]->redniBroj;
                    G->putevi[drugi]->dim_jednosmerne++;
                };
                
                
                if( ime_puta ) ime_puta = NULL;

                k = 0;
            };
        }
        else if( !strcmp( ime , "id" ) )
        {
            while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
            
            pom[j] = '\0';
            
            id_cvora = atof( pom );
            
            niz_id_cvorova = ( long * ) realloc( niz_id_cvorova , sizeof( long ) * ( k + 1 ) );
            
            niz_id_cvorova[k++] = id_cvora;
        }
        else if( !strcmp( ime , "oneway" ) )
        {
            while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
            
            pom[j] = '\0';
            
            if( !strcmp( pom , "yes" ) ) ind = 1;
            else ind = 0;
        }
        else if( !strcmp( ime , "operator" ) || !strcmp( ime , "name" ) )
        {
            ime_puta = ( char * ) malloc( sizeof( char ) * 100 );
            
            while( ( c = fgetc( dat1 ) ) != '\n' && c != EOF ) pom[j++] = c;
            
            pom[j] = '\0';
            
            strcpy( ime_puta , pom );
        };
    
        i = 0;
        j = 0;
    };
    
    fclose( dat1 );
    fclose( dat2 );
    
    for( i = 0 ; i < G->dim ; i++ )  //ova for petlja je da bi se naziv ulice nalazio na oba cvora (A,B), ako se ukloni, naziv ulice ce se nalaziti samo na drugom (B) cvoru. Cvor = A->B
    {
        if( !strcmp( G->putevi[i]->ime , "Nema" ) )
        {
            pomocni = G->putevi[i];
            
            if( pomocni->sledeci )
            {
                if( strcmp( pomocni->sledeci->ime , "Nema" ) ) strcpy( G->putevi[i]->ime , pomocni->sledeci->ime );
            };
        };
    };
    
    return( G );
}

void pokupi_argumente( int argc, char **argv, long *pocetak, long *kraj )
{
  if( argc != 3 )
  {
      printf( "Greska pri unosenju argumenata!\n" );
      exit( EXIT_FAILURE );
  };

  *pocetak = atof( *( argv + 1 ) );

  *kraj    = atof( *( argv + 2 ) );
}

void nadji_cvorove( Graf *G, long pocetak, long kraj, long *A, long *B )
{
  long i;
  unsigned char ind1 = 0, ind2 = 0;

  *A = -1;

  *B = -1;

  for( i = 0 ; i < G->dim, ( ind1 + ind2 ) != 2 ; i++ )
  {
    if( !ind1 && G->putevi[i]->id == pocetak )
    {
      *A = i;
      
      ind1 = 1;
    }

    else if( !ind2 && G->putevi[i]->id == kraj )
    {
      *B = i;

      ind2 = 1;
    }
  };

  if( *A == -1 || *B == -1 )
  {
    printf( "Uneti cvorovi nisu nadjeni!\n" );

    exit( EXIT_FAILURE );
  }

}

void stampaj_putanju( Graf *G, long start, long cilj )
{
  long i, k = 0 , trenutni = cilj , *pom = ( long * ) calloc( 200 , sizeof( long ) );
  long pomocni, brojac = 0, memorija = 100;
  long *niz = ( long * ) malloc( sizeof( long ) * 200 );
  char **niz_pom = NULL;

  niz[brojac++] = cilj;

  while( trenutni != start )
  {
    if( brojac == memorija )
    {
      niz = ( long * ) realloc( niz, sizeof( long ) * ( brojac + 200 ) );

      memorija += 100;
    };

    niz[brojac++] = G->putevi[trenutni]->put;
    
    if( !strcmp( G->putevi[trenutni]->ime , "Nema" ) ) pom[k] = G->putevi[trenutni]->id;
    
    k++;

    trenutni = G->putevi[trenutni]->put;
  };
  
  i = 0;
  
  while( i < k-i-1 ) //niz je obrnut da bi se poklapali indeksi u for petlji za stampanje
  {
    pomocni = pom[i];   
    pom[i] = pom[k-i-1];
    pom[k-i-1] = pomocni;
        
    i++;
  };
  
  k = 0;
  
  printf( "Putanja kojom treba ici od %s do %s je:\n", G->putevi[start]->ime, G->putevi[cilj]->ime );
  
  niz_pom = ( char ** ) malloc( sizeof( char * ) * brojac );
  if( niz_pom == NULL ) exit( EXIT_FAILURE );

  for( i = brojac - 1 ; i >= 0 ; i-- ) 
  {
      niz_pom[k] = ( char * ) malloc( sizeof( char ) * 100 );
      
      strcpy( niz_pom[k++] , G->putevi[*( niz + i )]->ime );
  };

  for( i = 0 ; i < brojac - 2 ; i++ )
  {
      if( !i || strcmp( niz_pom[i] , niz_pom[i+1] ) ) 
      {
          if( i ) 
          {
              if( !strcmp( niz_pom[i] , "Nema" ) ) printf( "%ld(%s) ->%s -> ", pom[i-1] , niz_pom[i] , niz_pom[i+1] );
              else if( !strcmp( niz_pom[i+1] , "Nema" ) ) printf( "%s -> %ld(%s) -> ", niz_pom[i] , pom[i] , niz_pom[i+1] );
              else printf( "%s -> %s -> ", niz_pom[i] , niz_pom[i+1] );
          }
          else 
          {
              if( strcmp( niz_pom[i] , "Nema" ) ) printf( "%s -> ", niz_pom[i] );
              else printf( "%ld(%s) -> ", pom[i-1] , niz_pom[i] );
          };
      };
  };

  printf( "%s(Kрај)\n" , niz_pom[brojac-1]);

  printf( "\nA rastojanje je: %.0lfm\n", G->putevi[cilj]->rastojanje );
  
  for( i = 0 ; i < brojac ; i++ ) free( niz_pom[i] );
  
  free( pom );
  
  free( niz );
  
  free( niz_pom );
}

void nadji_cilj_kola( Graf *G, int argc, char **argv )
{  
  Cvor *pom, *pom1;

  long poseceni[G->dim];
  long id1, id2, start, cilj, poc, kraj, ind, indeks;
  long i;

  Red *niz = NULL;
  Red *rep = NULL;

  for( i = 0 ; i < G->dim ; i++ ) poseceni[i] = 0;

  pokupi_argumente( argc, argv, &id1, &id2 );

  nadji_cvorove( G, id1, id2, &start, &cilj );

  poc = start;

  kraj = cilj;

  G->putevi[poc]->rastojanje = 0;

  push( &niz, &rep, poc );

  while( jel_red_prazan( niz ) ) //sve dok ne proverimo sve puteve
  {
    poc = pop( &niz );

    poseceni[poc] = 1;

    pom = G->putevi[poc];

    pom = pom->sledeci;

    while( pom ) //BFS Obilazak
    {
        if( !poseceni[pom->redniBroj] )
        {
            push( &niz, &rep, pom->redniBroj );
            poseceni[pom->redniBroj] = 1;
            ind++;
        };

        if( G->putevi[poc]->rastojanje + pom->tezina < G->putevi[pom->redniBroj]->rastojanje ) //ako je put preko tog cvora manji od starog
        {
            G->putevi[pom->redniBroj]->rastojanje = G->putevi[poc]->rastojanje + pom->tezina;

            G->putevi[pom->redniBroj]->put = poc; //Oznacavamo kako posle da znamo put od poc do cilja
        };

        pom = pom->sledeci;
        
    };
  };

  stampaj_putanju( G, start, cilj );

  free( pom );
}

void nadji_cilj_pesak( Graf *A, int argc, char **argv )
{  
  Cvor *pom, *pom1;

  Graf *G = A;
  long poseceni[G->dim];
  long id1, id2, start, cilj, poc, kraj, ind, indeks;
  long i;

  Red *niz = NULL;
  Red *rep = NULL;

  for( i = 0 ; i < G->dim ; i++ ) poseceni[i] = 0;

  pokupi_argumente( argc, argv, &id1, &id2 );

  nadji_cvorove( G, id1, id2, &start, &cilj );

  poc = start;

  kraj = cilj;

  G->putevi[poc]->rastojanje = 0;

  push( &niz, &rep, poc );

  while( jel_red_prazan( niz ) ) //sve dok ne proverimo sve puteve
  {
    poc = pop( &niz );

    poseceni[poc] = 1;

    pom = G->putevi[poc];

    for( i = 0; i < pom -> dim_jednosmerne; i++ )
    {
        if( !poseceni[pom->jednosmerne[i]] )
        {
            push( &niz, &rep, pom -> jednosmerne[i] );
            poseceni[pom->jednosmerne[i]]=1;
        }

        if( G -> putevi[poc] -> rastojanje + G -> putevi[pom->jednosmerne[i]] -> tezina < G -> putevi[pom->jednosmerne[i]] -> rastojanje )
        {
            G -> putevi[pom->jednosmerne[i]] -> rastojanje = G -> putevi[poc] -> rastojanje + G -> putevi[pom->jednosmerne[i]] -> tezina;
            G -> putevi[pom->jednosmerne[i]] -> put = poc;
        }
    }

    pom = pom->sledeci;

    while( pom ) //BFS Obilazak
    {
        if( !poseceni[pom->redniBroj] )
        {
            push( &niz, &rep, pom->redniBroj );
            poseceni[pom->redniBroj] = 1;
        };

        if( G->putevi[poc]->rastojanje + pom->tezina < G->putevi[pom->redniBroj]->rastojanje ) //ako je put preko tog cvora manji od starog
        {
            G->putevi[pom->redniBroj]->rastojanje = G->putevi[poc]->rastojanje + pom->tezina;

            G->putevi[pom->redniBroj]->put = poc; //Oznacavamo kako posle da znamo put od poc do cilja
        };

        pom = pom->sledeci;
        
    };
  };

  stampaj_putanju( G, start, cilj );

  free( pom );
}

void stampaj( Graf *G )
{
    int i;
    
    for( i = 0 ; i < G->dim ; i++ )
    {
        while( G->putevi[i] )  //ovo if( !strcmp( G->putevi[i]->ime , "Милутина Гарашанина" ) ) je samo primer, da se vidi da radi
        {
            if( !strcmp( G->putevi[i]->ime , "Милутина Гарашанина" ) ) printf( "%ld %lf %lf %lf %s\n", G->putevi[i]->id , G->putevi[i]->lat , G->putevi[i]->lon , G->putevi[i]->tezina , G->putevi[i]->ime );
            
            G->putevi[i] = G->putevi[i]->sledeci;
        };
    };
}

void obrisi_graf( Graf *G )
{
    long i;
    Cvor *pom;
    
    for( i = 0 ; i < G->dim ; i++ )
    {
        if( G->putevi[i]->dim_jednosmerne ) free( G->putevi[i]->jednosmerne );
        
        while( G->putevi[i] )
        {
            pom = G->putevi[i];
            G->putevi[i] = G->putevi[i]->sledeci;
            free( pom );
        };
    };
    
    free( G );
}
