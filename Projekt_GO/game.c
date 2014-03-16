#include <stdio.h>
#include <stdlib.h>
#include "game.h"

/********************************************************
SPRAWDZCZA CZY nLiczba ZNAJDUJE SIE W TABLICY Tablica[],
KTORA MA IloscElementow ELEMENTOW. ZWRACA:
   1 - wystapila
   0 - nie wystapila
********************************************************/
int CzyLiczbaJestWTablicy(int Tablica[], int IloscElementow, int nLiczba)
{
    for(int i=0; i<IloscElementow; i++)
    {
        if(Tablica[i] == nLiczba) return 1;
    }
    return 0;
}

//#####################################################################################
int Przez_ile_obcych_otoczone(struct POLE ObecnePole)
{
    int z_ilu_stron=0;

    enum STONE zla_wartosc;
    if(ObecnePole.wartosc==CZARNY) zla_wartosc=BIALY;
    else zla_wartosc=CZARNY;

    if(ObecnePole.dolne != NULL && ObecnePole.dolne->wartosc == zla_wartosc) z_ilu_stron++;
    if(ObecnePole.gorne != NULL && ObecnePole.gorne->wartosc == zla_wartosc) z_ilu_stron++;
    if(ObecnePole.lewe != NULL && ObecnePole.lewe->wartosc ==zla_wartosc) z_ilu_stron++;
    if(ObecnePole.prawe != NULL && ObecnePole.prawe->wartosc ==zla_wartosc) z_ilu_stron++;

    return z_ilu_stron;
}
//#####################################################################################


int Przez_ile_NULL(struct POLE ObecnePole)
{
    int z_ilu_stron=0;

    if(ObecnePole.dolne == NULL) z_ilu_stron++;
    if(ObecnePole.gorne == NULL) z_ilu_stron++;
    if(ObecnePole.lewe == NULL) z_ilu_stron++;
    if(ObecnePole.prawe == NULL) z_ilu_stron++;

    return z_ilu_stron;

}

//#####################################################################################


int Przez_ile_swojich_otoczone(struct POLE ObecnePole)
{
    int z_ilu_stron=0;

    if(ObecnePole.dolne != NULL && ObecnePole.dolne->wartosc == ObecnePole.wartosc) z_ilu_stron++;
    if(ObecnePole.gorne != NULL && ObecnePole.gorne->wartosc == ObecnePole.wartosc) z_ilu_stron++;
    if(ObecnePole.lewe != NULL && ObecnePole.lewe->wartosc == ObecnePole.wartosc) z_ilu_stron++;
    if(ObecnePole.prawe != NULL && ObecnePole.prawe->wartosc == ObecnePole.wartosc) z_ilu_stron++;

    return z_ilu_stron;
}

//#####################################################################################



// 1 - nie mozna wykonac ruchu
// 0 - mozna wykonac ruch
// -1 - blad
int SprawdzaniePoprawnosci(struct POLE plansza[], int R_Planszy, enum GRACZ wykonujacy, int nrPola)
{
    /*-----------------------------------*/
    /*-------POLE JEST JUZ ZAJETE -------*/
    /*-----------------------------------*/
    if(plansza[nrPola].wartosc!=PUSTE) return 1;


    /*-----------------------------------*/
    /*-------  POLE JEST WOLEN    -------*/
    /*-----------------------------------*/
    if(plansza[nrPola].wartosc==PUSTE)
    {

        plansza[nrPola].wartosc = wykonujacy; // tymczasowo przypisujemy wartosc

        /********************
        **** REGULA Ko ******
        ********************/
        //(dziala tylko gdy zbija sie tylko jeden kamien
        // i ruch poprzedni ruch byl dozwolonym ruchem samobujczym)
        //-------------------------------------
        if(Nr_zbitego_pola == nrPola && zbity_kolor == wykonujacy)
        {
            printf("        -Sprawdzanie poprawdnosci ruchu:\n\t regula Ko - nie dozwolny\n");

            plansza[nrPola].wartosc = PUSTE; // zeby zostal nie zmieniony wyglad planszy
            return 1;
        }

        //**** ruch samobujczy ****
        //(nie jest dopuszczalny gdy nie zbija kamieni przeciwnika)
        //-------------------------------------
        if(CzyUsuniecieKamieni(plansza, R_Planszy, nrPola, nrPola, -1, 0) == 1)
        {
            /**********************************************
            **** RUCH SAMOB.. DOPUSZCZALNY SPRAWDZANIE ****
            **********************************************/
            //------------------------------------------------------
            int powoduje_zbicie = 1111; // przechouje dane o zbitych polach - da sie odczytac po ktorej stronie zostaly zbite pola!!!

            int ilosc_zbitych = 0; // liczba zbitych pol

            enum STONE wartosc_przeciwnika;
            if(wykonujacy==WHITE) wartosc_przeciwnika = CZARNY;
            else wartosc_przeciwnika = BIALY;

            //pole sasiednie: prawe
            int Sprawdzone_Pola[19*19];

            if(plansza[nrPola+1].wartosc == wartosc_przeciwnika && CzyUsuniecieKamieni(plansza, R_Planszy, nrPola+1, nrPola+1, -1, 0) == 1)
            {
                for(int i =0; i<19*19; i++) Sprawdzone_Pola[i]=-1;

                powoduje_zbicie-=1000;
                ilosc_zbitych+=IleUsunieKamieni(plansza, nrPola+1, R_Planszy, Sprawdzone_Pola, 0);
            }
            //pole sasiednie: lewe
            if(plansza[nrPola-1].wartosc == wartosc_przeciwnika && CzyUsuniecieKamieni(plansza, R_Planszy, nrPola-1, nrPola-1, -1, 0) == 1)
            {
                for(int i =0; i<19*19; i++) Sprawdzone_Pola[i]=-1;

                powoduje_zbicie-=100;
                ilosc_zbitych+=IleUsunieKamieni(plansza, nrPola-1, R_Planszy, Sprawdzone_Pola, 0);
            }
            //pole sasiednie: dolne
            if(plansza[nrPola+R_Planszy].wartosc == wartosc_przeciwnika && CzyUsuniecieKamieni(plansza, R_Planszy, nrPola+R_Planszy, nrPola+R_Planszy, -1, 0) == 1)
            {
                for(int i =0; i<19*19; i++) Sprawdzone_Pola[i]=-1;

                powoduje_zbicie-=10;
                ilosc_zbitych+=IleUsunieKamieni(plansza, nrPola+R_Planszy, R_Planszy, Sprawdzone_Pola, 0);
            }
            //pole sasiednie: gorne
            if(plansza[nrPola-R_Planszy].wartosc == wartosc_przeciwnika && CzyUsuniecieKamieni(plansza, R_Planszy, nrPola-R_Planszy, nrPola-R_Planszy, -1, 0) == 1)
            {
                for(int i =0; i<19*19; i++) Sprawdzone_Pola[i]=-1;

                powoduje_zbicie-=1;
                ilosc_zbitych+=IleUsunieKamieni(plansza, nrPola-R_Planszy, R_Planszy, Sprawdzone_Pola, 0);
            }
            //-----------------------------------

            // --- Do regula Ko ----
            // uzupelnie czy zostalo zbite ostatnio jedno pole w ruchu samob..
            if(powoduje_zbicie == 1110 || powoduje_zbicie == 1101 || powoduje_zbicie == 1011 || powoduje_zbicie == 111)
            {
                //poprzednio zbite pole nr
                switch (powoduje_zbicie)
                {
                case  111:
                    Nr_zbitego_pola = nrPola+1;
                    break;
                case 1011:
                    Nr_zbitego_pola = nrPola-1;
                    break;
                case 1101:
                    Nr_zbitego_pola = nrPola+R_Planszy;
                    break;
                case 1110:
                    Nr_zbitego_pola = nrPola-R_Planszy;
                    break;

                default:
                    fprintf(stderr,"blad w SprawdzaniuPoprawnosci - 'powoduje_zbcie'\n");
                    exit(1);
                    break;
                }

                //kolor poprzednio zbitego pola
                if(wykonujacy == BLACK) zbity_kolor = BIALY;
                else zbity_kolor = CZARNY;

            }

            //------------ KONIEC(sprawdzania) DOPUSZCZALNEGO -----------


            //------------------------------------------------------
            /**********************************
            * RUCH SAMOB.. NIE DOPUSZCZALNY ***
            **********************************/
            if(ilosc_zbitych==0)
            {
                printf("        -Sprawdzanie poprawdnosci ruchu:\n\t samobojczy - nie dozwolny\n");

                plansza[nrPola].wartosc = PUSTE; // przywracanie standardowej wartosci polu
                return 1;
            }
            /******************************
            * RUCH SAMOB.. DOPUSZCZALNY ***
            ******************************/
            else
            {
                //Ruch samobujczy dozowlony
                if( ilosc_zbitych > 1)
                {
                    //zbito wiecej niz jeden kamien nie nadaje sie do reguly KO
                    //czyszczenie danych zbitego pola bo jest ich za duzo
                    Nr_zbitego_pola = -1;
                    zbity_kolor = PUSTE;
                }

                printf("        -Sprawdzanie poprawdnosci ruchu:\n\t samobojczy - dozowlony\n");
                return 0;
            }

        }
        //--- KONIEC SPRAWDZAANIA CZY RUCH JEST SAMOBUJCZ ---


        // ~~~~~~pole jest po prostu puste bez przeciw wskaza aby tam ustawic kamien

        //-- nie zostalo zbite pole potrzebne do reguly Ko ---
        //----------- (Ruch nie jest samobujczy) -------------
        Nr_zbitego_pola = -1;
        zbity_kolor = PUSTE;
        //----------------------------------------------------

        plansza[nrPola].wartosc = PUSTE; // przywracanie standardowej wartosci polu

        return 0;
    }

    return -1;
}

//#####################################################################################

int UstawienieKamienia(int Czy_Mozna, struct POLE Plansza[], enum GRACZ wykonujacy, int nrPola)
{
    if(Czy_Mozna == 0)
    {
        Plansza[nrPola].wartosc = wykonujacy;
        return 0;
    }

    return 1;
}

//#####################################################################################


// 1 - usuwanie
// 0 - nie jest usuwane
int CzyUsuniecieKamieni(struct POLE plansza[], int R_Planszy, int nrPola, int nrPOCZATKOWEGO_POLA, int nrPOPRZENIEGO_POLA, int Wywolanie)
{
    if(CzyLiczbaJestWTablicy(Poprzednie_pola, N, nrPola) != 1)
    {
        Poprzednie_pola[N] = nrPola;
        N++;
    }


    //- POLE jest puste - czyli zostaje;
    if(plansza[nrPola].wartosc == PUSTE) return 0;

    //pierwszy kamien
    if(Wywolanie == 0)
    {
        //ostoczony ze wszystkich stron przez obce
        if(Przez_ile_obcych_otoczone(plansza[nrPola]) == 4 - Przez_ile_NULL(plansza[nrPola])) return 1;


        if(Przez_ile_obcych_otoczone(plansza[nrPola]) + Przez_ile_swojich_otoczone(plansza[nrPola]) != 4-Przez_ile_NULL(plansza[nrPola]))
        {
            //jest przynajmniej jedno puste pole
            return 0;
        }
        else
        {
            //brak pustych pul do okola i przynajmniej jedno tego samego koloru


            int suma = 0; // zwieksza sie o jeden jesli sasiednie pole bedzie usuniete

            //pole dol
            if(plansza[nrPola].dolne != NULL && plansza[nrPola].dolne->wartosc == plansza[nrPola].wartosc)
            {
                if(CzyUsuniecieKamieni(plansza, R_Planszy, nrPola + R_Planszy, nrPOCZATKOWEGO_POLA, nrPola, 1) == 0) return 0;
                else suma++;
            }

            //pole gor
            if(plansza[nrPola].gorne!= NULL && plansza[nrPola].gorne->wartosc == plansza[nrPola].wartosc)
            {
                if(CzyUsuniecieKamieni(plansza, R_Planszy, nrPola - R_Planszy, nrPOCZATKOWEGO_POLA, nrPola, 1)==0) return 0;
                else suma++;
            }

            //pole lewe
            if(plansza[nrPola].lewe!=NULL && plansza[nrPola].lewe->wartosc == plansza[nrPola].wartosc)
            {
                if(CzyUsuniecieKamieni(plansza, R_Planszy, nrPola - 1, nrPOCZATKOWEGO_POLA, nrPola, 1)==0) return 0;
                else suma++;
            }

            //pole prawe
            if(plansza[nrPola].prawe && plansza[nrPola].prawe->wartosc == plansza[nrPola].wartosc)
            {
                if(CzyUsuniecieKamieni(plansza, R_Planszy, nrPola + 1, nrPOCZATKOWEGO_POLA, nrPola, 1)==0) return 0;
                else suma++;
            }

            //sprawdzanie czy po wstawieniu spowoduje usuniecie pozostalych
            if(suma == Przez_ile_swojich_otoczone(plansza[nrPola])) return 1;
            else return 0;
        }

    }

    //kolene kamienie
    if(Wywolanie == 1)
    {
        //otoczony ze wszystkich pozostalych stron przez kamienie innego koloru
        if(Przez_ile_obcych_otoczone(plansza[nrPola]) == 3 - Przez_ile_NULL(plansza[nrPola])) return 1;

        //wystapienie pustego pola
        if(Przez_ile_obcych_otoczone(plansza[nrPola]) + Przez_ile_swojich_otoczone(plansza[nrPola]) != 4 - Przez_ile_NULL(plansza[nrPola]))
        {
            return 0;
        }
        else
        {
            int suma = 0;

            //---------------------------
            if(plansza[nrPola].dolne != NULL && plansza[nrPola].dolne->wartosc == plansza[nrPola].wartosc)
            {
                if(CzyLiczbaJestWTablicy(Poprzednie_pola, N, nrPola+R_Planszy) == 1) suma++;
                else
                {
                    if(CzyUsuniecieKamieni(plansza, R_Planszy, nrPola + R_Planszy, nrPOCZATKOWEGO_POLA, nrPola, 1) == 0) return 0;
                    else suma++;
                }
            }

            //pole gor
            if(plansza[nrPola].gorne!= NULL && plansza[nrPola].gorne->wartosc == plansza[nrPola].wartosc)
            {
                if(CzyLiczbaJestWTablicy(Poprzednie_pola, N, nrPola-R_Planszy) == 1) suma++;
                else
                {
                    if(CzyUsuniecieKamieni(plansza, R_Planszy, nrPola - R_Planszy, nrPOCZATKOWEGO_POLA, nrPola, 1)==0) return 0;
                    else suma++;
                }
            }

            //pole lewe
            if(plansza[nrPola].lewe!=NULL && plansza[nrPola].lewe->wartosc == plansza[nrPola].wartosc)
            {

                if(CzyLiczbaJestWTablicy(Poprzednie_pola, N, nrPola-1) == 1) suma++;
                else
                {
                    if(CzyUsuniecieKamieni(plansza, R_Planszy, nrPola - 1, nrPOCZATKOWEGO_POLA, nrPola, 1)==0) return 0;
                    else suma++;
                }
            }

            //pole prawe
            if(plansza[nrPola].prawe && plansza[nrPola].prawe->wartosc == plansza[nrPola].wartosc)
            {
                if(CzyLiczbaJestWTablicy(Poprzednie_pola, N, nrPola+1) == 1) suma++;
                else
                {
                    if(CzyUsuniecieKamieni(plansza, R_Planszy, nrPola + 1, nrPOCZATKOWEGO_POLA, nrPola, 1)==0) return 0;
                    else suma++;
                }
            }

            //jest otoczony ze wszystkich ston i trzba sprawdzic reszte tego samego koloru
            if(suma == Przez_ile_swojich_otoczone(plansza[nrPola]))
            {
                return 1; // 1 - usuwanie
            }
            else
            {
                return 0; // 0 - nie jest usuwane
            }
        }
    }


    return 0;
}

//#####################################################################################

// zwraca liczbe kamieni do usuniecia
int IleUsunieKamieni(struct POLE plansza[], int nrPola, int R_Planszy, int TabSprPola[], int PozW_Tab)
{
    if(CzyLiczbaJestWTablicy(TabSprPola, PozW_Tab, nrPola) == 0)
    {
        TabSprPola[N] = nrPola;
        PozW_Tab++;
    }

    int ile=1;

    enum STONE usuwana_wartosc = plansza[nrPola].wartosc;

    if(plansza[nrPola].dolne!=NULL && CzyLiczbaJestWTablicy(TabSprPola, PozW_Tab, nrPola + R_Planszy) == 1 &&
       plansza[nrPola].dolne->wartosc == usuwana_wartosc) ile+=IleUsunieKamieni(plansza, nrPola + R_Planszy, R_Planszy, TabSprPola, PozW_Tab);

    if(plansza[nrPola].gorne!=NULL && CzyLiczbaJestWTablicy(TabSprPola, PozW_Tab, nrPola - R_Planszy) == 1 &&
       plansza[nrPola].gorne->wartosc == usuwana_wartosc) ile+=IleUsunieKamieni(plansza, nrPola - R_Planszy, R_Planszy, TabSprPola, PozW_Tab);

    if(plansza[nrPola].lewe!=NULL && CzyLiczbaJestWTablicy(TabSprPola, PozW_Tab, nrPola - 1) == 1 &&
       plansza[nrPola].lewe->wartosc == usuwana_wartosc) ile+=IleUsunieKamieni(plansza, nrPola - 1, R_Planszy, TabSprPola, PozW_Tab);

    if(plansza[nrPola].prawe!=NULL && CzyLiczbaJestWTablicy(TabSprPola, PozW_Tab, nrPola + 1) == 1 &&
       plansza[nrPola].prawe->wartosc == usuwana_wartosc) ile+=IleUsunieKamieni(plansza, nrPola + 1, R_Planszy, TabSprPola, PozW_Tab);

    return ile;
}
//#####################################################################################

int UsuniecieKamieni(struct POLE plansza[], int nrPola, int R_Planszy)
{
    int ile=1;

    enum STONE usuwana_wartosc = plansza[nrPola].wartosc;

    plansza[nrPola].wartosc = PUSTE;

    if(plansza[nrPola].dolne!=NULL && plansza[nrPola].dolne->wartosc == usuwana_wartosc) ile+=UsuniecieKamieni(plansza, nrPola + R_Planszy, R_Planszy);
    if(plansza[nrPola].gorne!=NULL && plansza[nrPola].gorne->wartosc == usuwana_wartosc) ile+=UsuniecieKamieni(plansza, nrPola - R_Planszy, R_Planszy);
    if(plansza[nrPola].lewe!=NULL && plansza[nrPola].lewe->wartosc == usuwana_wartosc) ile+=UsuniecieKamieni(plansza, nrPola - 1, R_Planszy);
    if(plansza[nrPola].prawe!=NULL && plansza[nrPola].prawe->wartosc == usuwana_wartosc) ile+=UsuniecieKamieni(plansza, nrPola + 1, R_Planszy);

    return ile;
}

void ZmienGracza()
{
    if(AktualnyGracz == BLACK) AktualnyGracz = WHITE;
    else AktualnyGracz = BLACK;
}

//-------------------------------------------------------------------------------------
//#####################################################################################
//-------------------------------------------------------------------------------------

/********************************************************************
LICZY TERYTORIUM GRACZY
//Terytorium w go to wszystkie puste przeciêcia otoczone wy³¹cznie przez kamienie danego koloru.
//Przeciêcia pomiêdzy kamieniami ró¿nych kolorów s¹ niczyje.

    wykorzysuje dwie globalne zmienne:
    - Poprzenie_pola[] <- przechowuje sprawdzone juz pola
    - N <- ilosc elementow w tablicy
    - GLOB_typ_pola <- przchowuje wartoscji z jakim polem graniczy
********************************************************************/
void LiczenieTerytorium(int R_planszy, struct POLE plansza[])
{
    /**Poprzednie_pola[] zostało wyczszczone przed wywoalniem funkcji**/

    i_Pustych_pol=0;
    int zwraca_wartosc;

    //DO ZAZNACZANIA TERYTORIUM
    _ile_pol_BLACK = _ile_pol_WHITE = _ile_pol_NEUTRALNE=0;

    for(int i=0; i< R_planszy * R_planszy; i++)
    {
        if(plansza[i].wartosc==PUSTE && CzyLiczbaJestWTablicy(Poprzednie_pola, N, i) == 0 )
        {
            //DO ZAZNACZANIA TERYTORIUM
            _ter_od_ktorego_indeksu=N;

            GLOB_typ_pola = PUSTE;


            zwraca_wartosc = CzyTylkoJedenTyp(plansza, R_planszy, i);

            if(zwraca_wartosc < 0)
            {
                i_Pustych_pol+= (zwraca_wartosc * -1);

                //DO ZAZNACZANIA TERYTORIUM
                for(int i=_ter_od_ktorego_indeksu;i<N;i++)
                {
                    Terytorium_NEUTRALNE[_ile_pol_NEUTRALNE]=Poprzednie_pola[i];
                    _ile_pol_NEUTRALNE+=1;
                }
            }
            else
            {
                i_Pustych_pol+=zwraca_wartosc;
                if(GLOB_typ_pola==CZARNY)
                {
                    Pkt_BLACK+=zwraca_wartosc;

                    //DO ZAZNACZANIA TERYTORIUM
                    for(int i=_ter_od_ktorego_indeksu;i<N;i++)
                    {
                        Terytorium_BLACK[_ile_pol_BLACK]=Poprzednie_pola[i];
                        _ile_pol_BLACK++;
                    }
                }
                if(GLOB_typ_pola==BIALY)
                {
                    Pkt_WHITE+=zwraca_wartosc;

                    //DO ZAZNACZANIA TERYTORIUM
                    for(int i=_ter_od_ktorego_indeksu;i<N;i++)
                    {
                        Terytorium_WHITE[_ile_pol_WHITE]=Poprzednie_pola[i];
                        _ile_pol_WHITE++;
                    }
                }
            }
        }
    }

    printf("\tTERYTORIUM:\n");
    printf("\t-czarny: %d\n",Pkt_BLACK);
    printf("\t-bialy: %d\n",Pkt_WHITE);
    printf("\t-cale: %d\n",i_Pustych_pol);
}


/*****************************************************************
ZWRACA ILOSC POL:
 - jseli jest mniejsza od 0 to graniczy z kamieniami roznego typu
 - jesli wieksza od 0 to graniczy tylko z kamieniami jednego typu
******************************************************************/
int CzyTylkoJedenTyp(struct POLE plansza[], int R_planszy, int nrPola)
{
    if(CzyLiczbaJestWTablicy(Poprzednie_pola, N, nrPola) == 0)
    {
        Poprzednie_pola[N] = nrPola;
        N+= 1;
    }

    int LiczbaPol = 1;
    int JedenTyp = 1;
    int W_zwracana;

    //PRAWE
    if(plansza[nrPola].prawe != NULL && CzyLiczbaJestWTablicy(Poprzednie_pola, N, nrPola+1) == 0)
    {
        if(plansza[nrPola].prawe->wartosc == PUSTE)
        {
            W_zwracana = CzyTylkoJedenTyp(plansza, R_planszy, nrPola+1);
            if(W_zwracana < 0)
            {
                JedenTyp=-1;
                LiczbaPol+=(W_zwracana * -1);
            }
            else LiczbaPol+=W_zwracana;
        }
        else
        {
            if(GLOB_typ_pola==PUSTE) GLOB_typ_pola = plansza[nrPola].prawe->wartosc;

            if(GLOB_typ_pola!=PUSTE && GLOB_typ_pola != plansza[nrPola].prawe->wartosc) JedenTyp=-1;
        }
    }

    //DOL
    if(plansza[nrPola].dolne != NULL && CzyLiczbaJestWTablicy(Poprzednie_pola, N, nrPola+R_planszy) == 0)
    {
        if(plansza[nrPola].dolne->wartosc == PUSTE)
        {
            W_zwracana = CzyTylkoJedenTyp(plansza, R_planszy, nrPola+R_planszy);
            if(W_zwracana < 0)
            {
                JedenTyp=-1;
                LiczbaPol+=(W_zwracana * -1);
            }
            else LiczbaPol+=W_zwracana;
        }
        else
        {
            if(GLOB_typ_pola==PUSTE) GLOB_typ_pola = plansza[nrPola].dolne->wartosc;



            if(GLOB_typ_pola!=PUSTE && GLOB_typ_pola != plansza[nrPola].dolne->wartosc) JedenTyp=-1;
        }
    }

    //LEWE
    if(plansza[nrPola].lewe != NULL && CzyLiczbaJestWTablicy(Poprzednie_pola, N, nrPola-1) == 0)
    {
        if(plansza[nrPola].lewe->wartosc == PUSTE)
        {
            W_zwracana = CzyTylkoJedenTyp(plansza, R_planszy, nrPola-1);
            if(W_zwracana < 0)
            {
                JedenTyp=-1;
                LiczbaPol+=(W_zwracana * -1);
            }
            else LiczbaPol+=W_zwracana;
        }
        else
        {
            if(GLOB_typ_pola==PUSTE) GLOB_typ_pola = plansza[nrPola].lewe->wartosc;

            if(GLOB_typ_pola!=PUSTE && GLOB_typ_pola != plansza[nrPola].lewe->wartosc) JedenTyp=-1;
        }
    }

    //GORA
    if(plansza[nrPola].gorne != NULL && CzyLiczbaJestWTablicy(Poprzednie_pola, N, nrPola-R_planszy) == 0)
    {
        if(plansza[nrPola].gorne->wartosc == PUSTE)
        {
            W_zwracana = CzyTylkoJedenTyp(plansza, R_planszy, nrPola-R_planszy);
            if(W_zwracana < 0)
            {
                JedenTyp=-1;
                LiczbaPol+=(W_zwracana * -1);
            }
            else LiczbaPol+=W_zwracana;
        }
        else
        {
            if(GLOB_typ_pola==PUSTE)
            {
                GLOB_typ_pola = plansza[nrPola].gorne->wartosc;
            }


            if(GLOB_typ_pola!=PUSTE && GLOB_typ_pola != plansza[nrPola].gorne->wartosc) JedenTyp=-1;
        }
    }


    return LiczbaPol*JedenTyp;
}
