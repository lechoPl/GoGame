/*
ZALORZENIA:

       * sprawdzenie poprawnosci ruchu -jest
            - regula Ko - jest
            - ruch samobujczy - jest
        * ustawienie kamienia -jest
        * zbijanie kamieni  -jest

    -plansza    <- jest w plku interfejs_Rozgrywka.c
    -akutalny gracz - jest
    -liczenie punktow   jest
*/
#define _KOMI 6.5

/**************************
* WLASNE TYPY I STRUKTURY *
**************************/
enum STAN_GRY {G_NO_STARTED, G_STARTED, G_END, G_NEW, G_LOAD};

//Porownywanie typu GRACZ i STONE mozliwe dzieki ich wartosciom
enum GRACZ {BLACK =1, WHITE=2};
enum STONE {PUSTE=0, CZARNY=1, BIALY=2};

struct POLE
{
    enum STONE wartosc;

    struct POLE *gorne;
    struct POLE *dolne;
    struct POLE *prawe;
    struct POLE *lewe;
};
/***********************/


/************************
*** ZMIENNE GLOBALNE  ***
************************/
enum STAN_GRY StanGry;

int RozmiarPlanszy;
extern struct POLE Plansza[];
enum GRACZ AktualnyGracz;

/**DO PUNKTOW**/
//  -terytorium
int Pkt_WHITE, Pkt_BLACK; // <- przechowuja licze zdobytego terytorium przez graczy
int i_Pustych_pol;

//  -pozostale
int ZbiteCzarne, ZbiteBiale;
float Komi;
int Handicap;
/** end **/


/**DO ZAZNACZANIA TERYTORIUM**/
/*te dwie tablice beda przechowywaly nrPol stanowiace terytorium odpowiadnich graczy*/
int Terytorium_WHITE[19*19];
int Terytorium_BLACK[19*19];
int Terytorium_NEUTRALNE[19*19];
int _ile_pol_BLACK, _ile_pol_WHITE, _ile_pol_NEUTRALNE;

//dodatwkowa zmienan wymagana do kopiwania numerow pol stanowiacych terytorium gracza wykorzystana w LiczenieTerytorium();
int _ter_od_ktorego_indeksu;
/** end **/


//DO REGULY Ko wartosci zbitego pola w poprzednim ruchu;
int Nr_zbitego_pola;     // na poczatku gry trzba na =-1
enum STONE zbity_kolor; // na poczatku gry trzba na =PUSTE
//--------------------

//DO SPRAWDZANIA CzyUsuniecieKamieni i LiczenieTerytorium
int N;      //<- przechowuje ile elemetnow w Poprzednie_pola
extern int Poprzednie_pola[];
enum STONE GLOB_typ_pola; //<- do LiczenieTerytorium
/***********************/



/************************
*** PROTOTYPY FUNKCJI ***
************************/
void ZmienGracza();

int UstawienieKamienia(int Czy_Mozna, struct POLE Plansza[], enum GRACZ wykonujacy, int nrPola);
int UsuniecieKamieni(struct POLE plansza[], int nrPola, int R_Planszy);

//Do sprawdzania poprawdnosc ruchu
int SprawdzaniePoprawnosci(struct POLE plansza[], int R_Planszy, enum GRACZ wykonujacy, int nrPola); // sprawdza poprawdnosc ruchu
int CzyUsuniecieKamieni(struct POLE plansza[], int R_Planszy, int nrPola, int nrPOCZATKOWEGO_POLA, int nrPOPRZENIEGO_POLA, int Wywolanie);


//jesli wiadomo ze kamienie na polu=nrPola zostanie zbity!!
int IleUsunieKamieni(struct POLE plansza[], int nrPola, int R_Planszy, int TabSprPola[], int PozW_Tab); // zwraca liczbe kamini ktore zostana zbite


//Do liczenie terytorium:
void LiczenieTerytorium(int R_planszy, struct POLE plansza[]);
int CzyTylkoJedenTyp(struct POLE plansza[], int R_planszy, int nrPola);

int CzyLiczbaJestWTablicy(int Tablica[], int IloscElementow, int nLiczba);

