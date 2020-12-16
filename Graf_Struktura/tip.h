typedef unsigned short Brojac;
typedef unsigned char BYTE;

typedef struct _cvor
{
    int vrednost;
    struct _cvor *sledeci;
} Cvor;

typedef struct _graf
{
    int broj_cvorova;
    Cvor **lista_povezanosti;
    BYTE *poseceni;
} Graf;
