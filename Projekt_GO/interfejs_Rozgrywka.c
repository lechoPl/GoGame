#include <stdio.h>
#include <stdlib.h>
#include <gtk\gtk.h>
#include "interfejs.h"
#include "game.h"

int IloscPasow; // przechowuje ile razy po koleji zotal klikniety "Pas"

//posiadaj odrazu maksymalne wielkosci
// PALANSZA
struct POLE Plansza[F_SIZE*F_SIZE];
//POPRZEDNIE POLA
int Poprzednie_pola[F_SIZE*F_SIZE];

float rozmiar_przycisku; // DO OBSLUGI PLANSZY
GtkWidget *ObecnyGracz; //DO WYSWIETLANIA OBECNEGO GRACZA
GtkWidget *liczba_Black, *liczba_White; // DO WYSWIETLANIA LICZBY ZBITYCH KAMIENI
GtkWidget *frame;   //PRZECHOWUJE GUZIKI DO OBSLUGI PLANSZY
GtkWidget *rama_na_frame;   // DO WYSWIETLANIA AKTUALNEGO OBRAZU PLANSZY NA "frame"

/* Dodatkowe fukcje wykonywane w oknie rozgrywki */
void RysujPlansze(GtkWidget *przycisk, GtkWidget *rama);
void WykonajRuch(GtkWidget *przysisk, gpointer NrPola_gpoint);
void Pas(GtkWidget *przycisk);
void PodajSie(GtkWidget *przycisk, GtkWidget *main_window);
void KoniecGryPas();




/*************************************
**              ROZGRYWKA           **
*************************************/
static void OBECNY_GRACZ( GtkWidget *widget) // aktualizacja Aktualnego Gracza w oknie gry
{
    gchar *buff;

    if(AktualnyGracz == BLACK) buff = g_strdup_printf ("CZARNY");
    if(AktualnyGracz == WHITE) buff = g_strdup_printf ("BIALY");

    gtk_statusbar_push (GTK_STATUSBAR (ObecnyGracz), 1, buff);
    g_free (buff);
}
void ILOSC_BLACK(GtkWidget *przycisk) // aktualizacja ilosci zbitych czarnych w oknie gry
{
    gchar *buf;
    buf = g_strdup_printf ("%d",ZbiteCzarne);
    gtk_label_set_text(GTK_LABEL(liczba_Black), buf);

    free(buf);
}
void ILOSC_WHITE(GtkWidget *przycisk) // aktualizacja ilosci zbitych bialych w oknie gry
{
    gchar *buf;
    buf = g_strdup_printf ("%d",ZbiteBiale);
    gtk_label_set_text(GTK_LABEL(liczba_White), buf);

    free(buf);
}
void ZamknijOknoRozgrywki()
{
    if(StanGry != G_LOAD) Menu();
}
//------------------------------------------------------------------------------------

void Gra(GtkWidget *przycisk, GtkWidget *poprzenie_okno)
{
    printf("|rozgrywka\n");

    //temp po to aby nie pokazywac menu przy niszczeniu poprzedniego okna
    enum STAN_GRY temp = StanGry;
    StanGry = G_STARTED;

    if(poprzenie_okno!=NULL) gtk_widget_destroy(poprzenie_okno);
    StanGry=temp;

    /***************************
    *** PRZYGOTOWANIE DO GRY ***
    ***************************/
    if(StanGry == G_NEW)
    {
        IloscPasow = 0;
        Pkt_BLACK=Pkt_WHITE=0;
    }

    //------------CZYSCZENIE PLANSZY i uzupelnianie krawedzi planszy;
    for(int i = 0; i<RozmiarPlanszy; i++)
    {
        for(int j=0; j<RozmiarPlanszy; j++)
        {

            //--------- Uzupelnie kamieni
            if(StanGry == G_NEW) Plansza[j+i*RozmiarPlanszy].wartosc=PUSTE;
            //-------------KONIEC uzupelniani kamieni

            if(j+i*RozmiarPlanszy + RozmiarPlanszy < RozmiarPlanszy*RozmiarPlanszy) Plansza[j+i*RozmiarPlanszy].dolne = &Plansza[j+i*RozmiarPlanszy + RozmiarPlanszy];
            else  Plansza[j+i*RozmiarPlanszy].dolne = NULL;


            if(j+i*RozmiarPlanszy - RozmiarPlanszy >= 0 ) Plansza[j+i*RozmiarPlanszy].gorne = &Plansza[j+i*RozmiarPlanszy -RozmiarPlanszy];
            else Plansza[j+i*RozmiarPlanszy].gorne=NULL;

            if(j > 0) Plansza[j+i*RozmiarPlanszy].lewe = &Plansza[j+i*RozmiarPlanszy-1];
            else Plansza[j+i*RozmiarPlanszy].lewe=NULL;

            if(j+1 < RozmiarPlanszy) Plansza[j+i*RozmiarPlanszy].prawe = &Plansza[j+i*RozmiarPlanszy+1];
            else Plansza[j+i*RozmiarPlanszy].prawe=NULL;
        }
    }

    if(StanGry == G_NEW)
    {
        Pkt_WHITE=Pkt_BLACK=0;
        //-----------------
        //-- OBSLUGA HANDICAP
        //----------------
        int PktSzczegolne[9];
        switch (RozmiarPlanszy)
        {
        case 9:
            PktSzczegolne[0] = 20;
            PktSzczegolne[1] = 24;
            PktSzczegolne[2] = 56;
            PktSzczegolne[3] = 60;
            break;
        case 13:
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<3; j++)
                {
                    PktSzczegolne[j+3*i] = 3 + j*3 + 3*13 + i*3*13;
                }
            }
            break;
        case 19:
            for(int i=0; i<3; i++)
            {
                for(int j=0; j<3; j++)
                {
                    PktSzczegolne[j+3*i] = 3 + j*6 + 3*19 + i*6*19;
                }
            }
            break;

        default:
            break;
        }


        if(Handicap >1)
        {
            switch (Handicap)
            {
            case 2:
                if(RozmiarPlanszy==9)
                    Plansza[PktSzczegolne[1]].wartosc=Plansza[PktSzczegolne[2]].wartosc = CZARNY;
                else
                    Plansza[PktSzczegolne[2]].wartosc=Plansza[PktSzczegolne[6]].wartosc = CZARNY;

                break;
            case 3:
                if(RozmiarPlanszy==9)
                    Plansza[PktSzczegolne[0]].wartosc=Plansza[PktSzczegolne[1]].wartosc=Plansza[PktSzczegolne[2]].wartosc = CZARNY;
                else
                    Plansza[PktSzczegolne[2]].wartosc=Plansza[PktSzczegolne[6]].wartosc=Plansza[PktSzczegolne[4]].wartosc = CZARNY;

                break;
            case 4:
                if(RozmiarPlanszy==9)
                    Plansza[PktSzczegolne[0]].wartosc = Plansza[PktSzczegolne[1]].wartosc =
                                                            Plansza[PktSzczegolne[2]].wartosc = Plansza[PktSzczegolne[3]].wartosc = CZARNY;
                else
                    Plansza[PktSzczegolne[2]].wartosc = Plansza[PktSzczegolne[6]].wartosc =
                                                            Plansza[PktSzczegolne[0]].wartosc = Plansza[PktSzczegolne[8]].wartosc = CZARNY;

                break;
            case 5:
                Plansza[PktSzczegolne[2]].wartosc = Plansza[PktSzczegolne[6]].wartosc =
                                                        Plansza[PktSzczegolne[0]].wartosc = Plansza[PktSzczegolne[8]].wartosc =
                                                                Plansza[PktSzczegolne[4]].wartosc = CZARNY;
                break;
            case 6:
                Plansza[PktSzczegolne[2]].wartosc = Plansza[PktSzczegolne[6]].wartosc =
                                                        Plansza[PktSzczegolne[0]].wartosc = Plansza[PktSzczegolne[8]].wartosc =
                                                                Plansza[PktSzczegolne[3]].wartosc = Plansza[PktSzczegolne[5]].wartosc = CZARNY;
                break;
            case 7:
                Plansza[PktSzczegolne[2]].wartosc = Plansza[PktSzczegolne[6]].wartosc =
                                                        Plansza[PktSzczegolne[0]].wartosc = Plansza[PktSzczegolne[8]].wartosc =
                                                                Plansza[PktSzczegolne[3]].wartosc = Plansza[PktSzczegolne[5]].wartosc =
                                                                        Plansza[PktSzczegolne[4]].wartosc = CZARNY;
                break;
            case 8:
                Plansza[PktSzczegolne[2]].wartosc = Plansza[PktSzczegolne[6]].wartosc =
                                                        Plansza[PktSzczegolne[0]].wartosc = Plansza[PktSzczegolne[8]].wartosc =
                                                                Plansza[PktSzczegolne[3]].wartosc = Plansza[PktSzczegolne[5]].wartosc =
                                                                        Plansza[PktSzczegolne[1]].wartosc = Plansza[PktSzczegolne[7]].wartosc = CZARNY;
                break;
            case 9:
                Plansza[PktSzczegolne[2]].wartosc = Plansza[PktSzczegolne[6]].wartosc =
                                                        Plansza[PktSzczegolne[0]].wartosc = Plansza[PktSzczegolne[8]].wartosc =
                                                                Plansza[PktSzczegolne[3]].wartosc = Plansza[PktSzczegolne[5]].wartosc =
                                                                        Plansza[PktSzczegolne[1]].wartosc = Plansza[PktSzczegolne[7]].wartosc =
                                                                                Plansza[PktSzczegolne[4]].wartosc = CZARNY;
                break;

            default:
                break;
            }
        }
    }
    //-----------------
    /*********************************
    *** KONIEC PRZYGOTOWANN DO GRY ***
    *********************************/

    StanGry = G_STARTED;

    GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GO");
    gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    GtkWidget *v_podzial;
    v_podzial = gtk_vbox_new(FALSE,10);
    gtk_container_add(GTK_CONTAINER(window), v_podzial);

    /************************/
    /****** GORNY PASEK *****/
    /************************/
    GtkWidget *hbox_gora;
    hbox_gora = gtk_hbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(v_podzial), hbox_gora);

    GtkWidget *zapisz;
    zapisz = gtk_button_new_from_stock(GTK_STOCK_SAVE);
    gtk_box_pack_start(GTK_BOX(hbox_gora), zapisz, FALSE, FALSE, 0);

    g_signal_connect(G_OBJECT(zapisz), "clicked",G_CALLBACK( Zapisz ), window );

    GtkWidget *wczytaj;
    wczytaj = gtk_button_new_with_label("Wczytaj");
    gtk_box_pack_start(GTK_BOX(hbox_gora), wczytaj, FALSE, FALSE, 0);


    g_signal_connect(G_OBJECT(wczytaj), "clicked",G_CALLBACK( Wczytaj ), window );

    GtkWidget *zamknij;
    zamknij = gtk_button_new_from_stock(GTK_STOCK_CLOSE);
    gtk_box_pack_start(GTK_BOX(hbox_gora), zamknij, FALSE, FALSE, 20);

    g_signal_connect(G_OBJECT(zamknij), "clicked", G_CALLBACK(Anuluj), window);




    //--------------- tutaja trzymana jest plansza i prawa strona dol
    GtkWidget *h_podzial;
    h_podzial = gtk_hbox_new(FALSE, 10);
    gtk_container_add(GTK_CONTAINER(v_podzial), h_podzial);


    /************************/
    /********* PLANSZA ******/
    /************************/
    GtkWidget *Field[F_SIZE*F_SIZE];
    //GtkWidget *frame;

    frame = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(h_podzial), frame);



    //************ OBSLUGA PLANSZY ***************
    //float rozmiar_przycisku; // DO OBSLUGI PLANSZY
    switch (RozmiarPlanszy)
    {
    case 9:
        rozmiar_przycisku = 50;
        break;

    case 13:
        rozmiar_przycisku = 34.6;
        break;

    case 19:
        rozmiar_przycisku = 23.6;
        break;

    default:
        fprintf(stderr,"blad! - rozmiaru przyciskow\n");
        exit(1);
        break;
    }

    for(int i=0; i<RozmiarPlanszy; i++)
    {
        for(int j=0; j<RozmiarPlanszy; j++)
        {
            Field[j+i*RozmiarPlanszy] = gtk_button_new_with_label("");

            //zmianna wielkosc przyciskow w zaleznosci od wielkosci planszy
            gtk_widget_set_size_request(Field[j+i*RozmiarPlanszy], rozmiar_przycisku, rozmiar_przycisku);
            gtk_fixed_put(GTK_FIXED(frame), Field[j+i*RozmiarPlanszy], rozmiar_przycisku*i, rozmiar_przycisku*j);
        }
    }
    //************ OBSLUGA PLANSZY KONIEC ***************


    //************ RYSOWANIE PLANSZY ***************
    RysujPlansze(NULL, frame);
    //************ RYSOWANIE PLANSZY KONIEC ***************

    //obsluga przyciskow
    for(int i=0; i<RozmiarPlanszy; i++)
    {
        for(int j=0; j<RozmiarPlanszy; j++)
        {
            g_signal_connect(G_OBJECT(Field[j+i*RozmiarPlanszy]), "pressed",G_CALLBACK(WykonajRuch),(gpointer) i+j*RozmiarPlanszy);
            //g_signal_connect(G_OBJECT(Field[j+i*RozmiarPlanszy]), "pressed",G_CALLBACK(RysujPlansze),frame); <- za miast tego funkcjia RysujPlansze jest w WykonajRuch

            g_signal_connect(G_OBJECT(Field[j+i*RozmiarPlanszy]), "pressed",G_CALLBACK(OBECNY_GRACZ),NULL);
            g_signal_connect(G_OBJECT(Field[j+i*RozmiarPlanszy]), "pressed",G_CALLBACK(ILOSC_BLACK),NULL);
            g_signal_connect(G_OBJECT(Field[j+i*RozmiarPlanszy]), "pressed",G_CALLBACK(ILOSC_WHITE),NULL);
        }
    }


    /************************/
    /******* PRAWY DOL ******/
    /************************/
    GtkWidget *PrawaStrona_Rama;
    PrawaStrona_Rama = gtk_fixed_new();
    gtk_container_add(GTK_CONTAINER(h_podzial), PrawaStrona_Rama);


    GtkWidget *box_akt_g;
    box_akt_g = gtk_vbox_new(FALSE, 5);
    gtk_fixed_put(GTK_FIXED(PrawaStrona_Rama), box_akt_g, 10, 10);

    GtkWidget *ramka_AktualnyGracz;
    ramka_AktualnyGracz = gtk_frame_new("Aktualny Gracz");
    gtk_box_pack_start(GTK_BOX(box_akt_g), ramka_AktualnyGracz, FALSE, FALSE, 0);

    ObecnyGracz = gtk_statusbar_new ();//gtk_label_new("");
    gtk_container_set_border_width (GTK_CONTAINER(ObecnyGracz), 5);
    gtk_statusbar_set_has_resize_grip(GTK_STATUSBAR(ObecnyGracz), FALSE);

    gtk_container_add(GTK_CONTAINER(ramka_AktualnyGracz), ObecnyGracz);

    // ------------ PRZYCISKI
    GtkWidget *PAS, *PODAJ_SIE, *vbox;
    vbox = gtk_vbox_new(TRUE, 5);

    PAS = gtk_button_new_with_label(" PAS ");
    PODAJ_SIE = gtk_button_new_with_label(" PODDAJ SIE ");

    gtk_fixed_put(GTK_FIXED(PrawaStrona_Rama), vbox, 15, 100);
    gtk_container_add(GTK_CONTAINER(vbox), PAS);
    gtk_container_add(GTK_CONTAINER(vbox), PODAJ_SIE);

    g_signal_connect(G_OBJECT(PAS), "pressed",G_CALLBACK( Pas ), NULL );
    g_signal_connect(G_OBJECT(PAS), "pressed",G_CALLBACK(OBECNY_GRACZ), NULL);

    g_signal_connect(G_OBJECT(PODAJ_SIE), "pressed",G_CALLBACK( PodajSie ),window);


    //------------- ZBITE KAMIENIE
    GtkWidget *ramka_Zbite;
    ramka_Zbite = gtk_frame_new("ZBITE KAMIENIE");
    gtk_fixed_put(GTK_FIXED(PrawaStrona_Rama), ramka_Zbite, 10, 200);

    GtkWidget *hbox_zbite;
    hbox_zbite = gtk_hbox_new(FALSE, 10);
    gtk_container_set_border_width (GTK_CONTAINER(hbox_zbite), 10);
    gtk_container_add(GTK_CONTAINER(ramka_Zbite), hbox_zbite);


    GtkWidget *vbox_White, *vbox_Black, *separator;
    vbox_White = gtk_vbox_new(FALSE, 10);
    vbox_Black = gtk_vbox_new(FALSE, 10);

    separator = gtk_vseparator_new();


    gtk_container_add(GTK_CONTAINER(hbox_zbite), vbox_White);
    gtk_container_add(GTK_CONTAINER(hbox_zbite), separator);
    gtk_container_add(GTK_CONTAINER(hbox_zbite), vbox_Black);

    GtkWidget *img_Black, *img_White;
    img_Black = gtk_image_new_from_file("image\\StoneBlack_19x19.png");

    img_White = gtk_image_new_from_file("image\\StoneWhite_19x19.png");

    gtk_container_add(GTK_CONTAINER(vbox_White), img_White);
    gtk_container_add(GTK_CONTAINER(vbox_Black), img_Black);

    //GtkWidget *liczba_Black, *liczba_White; wykorzystane
    liczba_Black = gtk_label_new("");
    liczba_White = gtk_label_new("");

    gtk_container_add(GTK_CONTAINER(vbox_White), liczba_White);
    gtk_container_add(GTK_CONTAINER(vbox_Black), liczba_Black);
    //-----------------------------------------------------------

    g_signal_connect(G_OBJECT(window), "show",G_CALLBACK(ILOSC_BLACK),NULL);
    g_signal_connect(G_OBJECT(window), "show",G_CALLBACK(ILOSC_WHITE),NULL);
    g_signal_connect(G_OBJECT(window), "show",G_CALLBACK(OBECNY_GRACZ),NULL);
    g_signal_connect (window, "destroy", G_CALLBACK (ZamknijOknoRozgrywki), NULL);

    gtk_widget_show_all(window);
}



//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------
void WykonajRuch(GtkWidget *przysisk, gpointer NrPola_gpoint)
{
    IloscPasow = 0; // zostal wykonany kolejny ruch

    int NrPola = (int) NrPola_gpoint;

    // <---- Do Sprawdzanie poprawnoci( zerowanie tablicy) ----->
    N=0;
    for(int i =0; i < RozmiarPlanszy*RozmiarPlanszy; i++) Poprzednie_pola[i] = -1;
    //               <----- ----->

    if(StanGry == G_STARTED && SprawdzaniePoprawnosci(Plansza, RozmiarPlanszy, AktualnyGracz, NrPola)== 0)
    {
        printf("    +wstawiono");
        if(AktualnyGracz==BLACK) printf(" czarny. ");
        if(AktualnyGracz==WHITE) printf(" bialy. ");
        printf("NrPola %d\n", NrPola+1);

        UstawienieKamienia(0, Plansza,AktualnyGracz, NrPola);

        // -------- zamiana AktualnegoGracza
        if(AktualnyGracz==BLACK) AktualnyGracz=WHITE;
        else AktualnyGracz = BLACK;

        //*******************************************************
        //---- Sprawdzanie czy zostaly zbite jakies kamienie ----
        //*******************************************************
        for(int i =0 ; i < RozmiarPlanszy*RozmiarPlanszy; i++)
        {
            //------------ PRZYGOTOWYWANIE ZMIENNYCH POMOCNICZYCH --------------
            N=0;
            for(int i =0; i < RozmiarPlanszy*RozmiarPlanszy; i++) Poprzednie_pola[i] = -1;
            //----------------------------------------------------------------------


            //*** wystepuje "== AktualnyGracz" bo nastapila zmiana gracza;
            if(Plansza[i].wartosc == AktualnyGracz && CzyUsuniecieKamieni(Plansza, RozmiarPlanszy, i, i, -1, 0)  == 1 )
            {
                int ile = UsuniecieKamieni(Plansza, i, RozmiarPlanszy);

                if(AktualnyGracz == WHITE) ZbiteBiale += ile;
                if(AktualnyGracz == BLACK) ZbiteCzarne += ile;

                printf("    -zbito: %d  \n", ile);
            }
        }

        RysujPlansze(NULL, frame);
        printf("\n");
    }
}

//---------------------------------------------------------------------------------------------------------------------------
void RysujPlansze(GtkWidget *przycisk, GtkWidget *rama)
{
    //GtkWidget *rama_na_frame;
        if(rama_na_frame!=NULL) gtk_widget_destroy(rama_na_frame);

        rama_na_frame=gtk_fixed_new();
        gtk_fixed_put(GTK_FIXED(rama), rama_na_frame, 0, 0);


    //----------- WCZYTANIE NA NOWO TLA -----------------
    GtkWidget *Goban_img;
    Goban_img = gtk_image_new();
    switch (RozmiarPlanszy)
    {
    case 9:
        gtk_image_set_from_file((GtkImage*)Goban_img, "image\\Goban_9x9.png");
        break;

    case 13:
        gtk_image_set_from_file((GtkImage*)Goban_img, "image\\Goban_13x13.png");
        break;

    case 19:
        gtk_image_set_from_file((GtkImage*)Goban_img, "image\\Goban_19x19.png");
        break;

    default:
        fprintf(stderr,"blad! - wczytywanie planszy\n");
        exit(1);
        break;
    }
    gtk_fixed_put(GTK_FIXED(rama_na_frame), Goban_img, 0, 0);


    //-------------- WSTAWIANIE POSZCZEGOLNYCH KAMIENIE ----------------
    for(int i=0; i <RozmiarPlanszy*RozmiarPlanszy; i++)
    {
        GtkWidget *Kamien;
        Kamien = gtk_image_new();

        if(StanGry != G_END && Plansza[i].wartosc == 0) continue;

        if(Plansza[i].wartosc==1)
        {
            switch (RozmiarPlanszy)
            {
            case 9:
                gtk_image_set_from_file ((GtkImage*)Kamien, "image\\StoneBlack_9x9.png");
                break;
            case 13:
                gtk_image_set_from_file ((GtkImage*)Kamien, "image\\StoneBlack_13x13.png");
                break;
            case 19:
                gtk_image_set_from_file ((GtkImage*)Kamien, "image\\StoneBlack_19x19.png");
                break;

            default:
                fprintf(stderr, "blad - obraz kamienia.\n");
                exit(2);
                break;
            }

        }
        if(Plansza[i].wartosc==2)
        {
            switch ((RozmiarPlanszy))
            {
            case 9:
                gtk_image_set_from_file ((GtkImage*)Kamien, "image\\StoneWhite_9x9.png");
                break;
            case 13:
                gtk_image_set_from_file ((GtkImage*)Kamien, "image\\StoneWhite_13x13.png");
                break;
            case 19:
                gtk_image_set_from_file ((GtkImage*)Kamien, "image\\StoneWhite_19x19.png");
                break;

            default:
                fprintf(stderr, "blad - obraz kamienia.\n");
                exit(2);
                break;
            }

        }


        //DO ZAZNACZANIA TERYTORIUM
        if(StanGry == G_END)
        {
            switch (RozmiarPlanszy)
            {
            case 9:
                if(CzyLiczbaJestWTablicy(Terytorium_WHITE, _ile_pol_WHITE, i)==1)gtk_image_set_from_file ((GtkImage*)Kamien, "image\\Zajete_White_9x9.png");
                else
                {
                    if(CzyLiczbaJestWTablicy(Terytorium_BLACK, _ile_pol_BLACK, i)==1) gtk_image_set_from_file ((GtkImage*)Kamien, "image\\Zajete_Black_9x9.png");
                    else if(CzyLiczbaJestWTablicy(Terytorium_NEUTRALNE, _ile_pol_NEUTRALNE, i)==1) gtk_image_set_from_file ((GtkImage*)Kamien, "image\\Zajete_NEUTRALNE_9x9.png");
                }
                break;
            case 13:
                if(CzyLiczbaJestWTablicy(Terytorium_WHITE, _ile_pol_WHITE, i)==1)gtk_image_set_from_file ((GtkImage*)Kamien, "image\\Zajete_White_13x13.png");
                else
                {
                    if(CzyLiczbaJestWTablicy(Terytorium_BLACK, _ile_pol_BLACK, i)==1) gtk_image_set_from_file ((GtkImage*)Kamien, "image\\Zajete_Black_13x13.png");
                    else if(CzyLiczbaJestWTablicy(Terytorium_NEUTRALNE, _ile_pol_NEUTRALNE, i)==1) gtk_image_set_from_file ((GtkImage*)Kamien, "image\\Zajete_NEUTRALNE_13x13.png");
                }
                break;
            case 19:
                if(CzyLiczbaJestWTablicy(Terytorium_WHITE, _ile_pol_WHITE, i)==1)gtk_image_set_from_file ((GtkImage*)Kamien, "image\\Zajete_White_19x19.png");
                else
                {
                    if(CzyLiczbaJestWTablicy(Terytorium_BLACK, _ile_pol_BLACK, i)==1) gtk_image_set_from_file ((GtkImage*)Kamien, "image\\Zajete_Black_19x19.png");
                    else if(CzyLiczbaJestWTablicy(Terytorium_NEUTRALNE, _ile_pol_NEUTRALNE, i)==1) gtk_image_set_from_file ((GtkImage*)Kamien, "image\\Zajete_NEUTRALNE_19x19.png");
                }
                break;

            default:
                fprintf(stderr, "blad - obraz kamienia.\n");
                exit(2);
                break;
            }

        }


        gtk_fixed_put(GTK_FIXED(rama_na_frame), Kamien, i%RozmiarPlanszy*rozmiar_przycisku, i/RozmiarPlanszy*rozmiar_przycisku);
    }

    gtk_widget_show_all(rama);
}
//---------------------------------------------------------------------------------------------------------------------------------


void Pas(GtkWidget *przycisk)
{
    if(StanGry == G_STARTED)
    {
        if(IloscPasow == 1)
        {
            N=0;
            for(int i =0; i < RozmiarPlanszy*RozmiarPlanszy; i++) Poprzednie_pola[i] = -1;

            StanGry = G_END;
            printf("|Koniec gry\n");

            //DO ZAZNACZANIA TERYTORIUM
            for(int i = 0;i < 19*19; i++)
            {
                Terytorium_WHITE[i] = Terytorium_BLACK[i] = Terytorium_NEUTRALNE[i] = -1;
            }

            LiczenieTerytorium(RozmiarPlanszy, Plansza);


            RysujPlansze(NULL,frame);

            KoniecGryPas();
        }
        else
        {
            IloscPasow++;
            ZmienGracza();
        }
    }
}
void KoniecGryPas()
{
    if(StanGry == G_END)
    {
        GtkWidget *window;
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "GO - Koniec Gry: Wyniki");

        gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
        gtk_container_set_border_width(GTK_CONTAINER(window), 10);

        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

        GtkWidget *box_main;
        box_main = gtk_vbox_new(FALSE,10);
        gtk_container_add(GTK_CONTAINER(window), box_main);


        GtkWidget *ramka = gtk_frame_new("Punkty");
        gtk_box_pack_start (GTK_BOX (box_main), ramka, TRUE, TRUE, 0);


        GtkWidget *vbox_punkty = gtk_vbox_new(FALSE, 5);
        gtk_container_add(GTK_CONTAINER(ramka), vbox_punkty);

        GtkWidget *box_wyniki;
        box_wyniki = gtk_hbox_new(FALSE, 10);
        gtk_container_set_border_width(GTK_CONTAINER(box_wyniki), 10);
        gtk_box_pack_start (GTK_BOX (vbox_punkty), box_wyniki, TRUE, TRUE, 0);



        GtkWidget *box_nazwa, *box_black, *box_white;
        box_nazwa=gtk_vbox_new(FALSE, 5);
        box_black=gtk_vbox_new(FALSE, 5);
        box_white=gtk_vbox_new(FALSE, 5);

        gtk_box_pack_start (GTK_BOX (box_wyniki), box_nazwa, TRUE, TRUE, 0);
        gtk_box_pack_start (GTK_BOX (box_wyniki), box_black, TRUE, TRUE, 0);
        gtk_box_pack_start (GTK_BOX (box_wyniki), box_white, TRUE, TRUE, 0);



        GtkWidget *napis = gtk_label_new("");
        gtk_box_pack_start (GTK_BOX (box_nazwa), napis, TRUE, TRUE, 0);
        napis=gtk_label_new("Komi");
        gtk_box_pack_start (GTK_BOX (box_nazwa), napis, TRUE, TRUE, 0);
        napis=gtk_label_new("Zbite kamienie");
        gtk_box_pack_start (GTK_BOX (box_nazwa), napis, TRUE, TRUE, 0);
        napis=gtk_label_new("*Zdobyte terytorium");
        gtk_box_pack_start (GTK_BOX (box_nazwa), napis, TRUE, TRUE, 0);
        napis=gtk_label_new("SUMA");
        gtk_box_pack_start (GTK_BOX (box_nazwa), napis, TRUE, TRUE, 5);
        //------------------------------------------------------------
        gchar *buf;

        napis=gtk_label_new("CZARNY");
        gtk_box_pack_start (GTK_BOX (box_black), napis, TRUE, TRUE, 0);

        napis=gtk_label_new("-");
        gtk_box_pack_start (GTK_BOX (box_black), napis, TRUE, TRUE, 0);

        napis=gtk_label_new("");
        buf = g_strdup_printf ("%d",ZbiteBiale);
        gtk_label_set_text(GTK_LABEL(napis), buf);
        gtk_box_pack_start (GTK_BOX (box_black), napis, TRUE, TRUE, 0);

        napis=gtk_label_new("");
        buf = g_strdup_printf ("%d",Pkt_BLACK);
        gtk_label_set_text(GTK_LABEL(napis), buf);
        gtk_box_pack_start (GTK_BOX (box_black), napis, TRUE, TRUE, 0);

        napis=gtk_label_new("");
        buf = g_strdup_printf ("%.1f",(double)Pkt_BLACK + ZbiteBiale);
        gtk_label_set_text(GTK_LABEL(napis), buf);
        gtk_box_pack_start (GTK_BOX (box_black), napis, TRUE, TRUE, 5);

        //------------------------------------------------------------
        napis = gtk_label_new("BIALY");
        gtk_box_pack_start (GTK_BOX (box_white), napis, TRUE, TRUE, 0);

        napis=gtk_label_new("");
        buf = g_strdup_printf ("%.1f",Komi);
        gtk_label_set_text(GTK_LABEL(napis), buf);
        gtk_box_pack_start (GTK_BOX (box_white), napis, TRUE, TRUE, 0);

        napis=gtk_label_new("");
        buf = g_strdup_printf ("%d",ZbiteCzarne);
        gtk_label_set_text(GTK_LABEL(napis), buf);
        gtk_box_pack_start (GTK_BOX (box_white), napis, TRUE, TRUE, 0);

        napis=gtk_label_new("");
        buf = g_strdup_printf ("%d",Pkt_WHITE);
        gtk_label_set_text(GTK_LABEL(napis), buf);
        gtk_box_pack_start (GTK_BOX (box_white), napis, TRUE, TRUE, 0);

        napis=gtk_label_new("");
        buf = g_strdup_printf ("%.1f",(double)ZbiteCzarne+Pkt_WHITE+Komi);
        gtk_label_set_text(GTK_LABEL(napis), buf);
        gtk_box_pack_start (GTK_BOX (box_white), napis, TRUE, TRUE, 5);


        GtkWidget *linia = gtk_hseparator_new();
        gtk_box_pack_start (GTK_BOX (vbox_punkty), linia, TRUE, TRUE, 0);

        GtkWidget *box_pozostale = gtk_hbox_new(FALSE, 5);
        gtk_container_set_border_width(GTK_CONTAINER(box_pozostale), 10);
        gtk_box_pack_start (GTK_BOX (vbox_punkty), box_pozostale, TRUE, TRUE, 0);

        napis=gtk_label_new("*Niczyje terytorium: ");
        gtk_box_pack_start (GTK_BOX (box_pozostale), napis, TRUE, TRUE, 5);

        napis=gtk_label_new("");
        buf = g_strdup_printf ("%d", i_Pustych_pol - (Pkt_BLACK+Pkt_WHITE) );
        gtk_label_set_text(GTK_LABEL(napis), buf);
        gtk_box_pack_start (GTK_BOX (box_pozostale), napis, TRUE, TRUE, 5);


        //zwolnie wsk;
        free(buf);
        //--------------------------------------------------------------

        GtkWidget *informacja = gtk_label_new("*W razie watpliwosci nalezy sprawdzic samodzielnie\n ilosc zdobytego terytorium.");
        gtk_box_pack_start (GTK_BOX (box_main), informacja, TRUE, TRUE, 10);

        GtkWidget *button_ok = gtk_button_new_from_stock(GTK_STOCK_OK);
        gtk_box_pack_start (GTK_BOX (box_main), button_ok, TRUE, TRUE, 0);

        g_signal_connect(G_OBJECT(button_ok), "clicked",G_CALLBACK(Anuluj),window);

        g_signal_connect(G_OBJECT(window), "destroy",G_CALLBACK(Anuluj),window);

        gtk_widget_show_all(window);

    }
}
//---------------------------------------------------------------------------------------------------------------------------------

void PodajSie(GtkWidget *przycisk, GtkWidget *main_window)
{
    if(StanGry == G_STARTED)
    {

        StanGry = G_END;
        printf("|Koniec gry\n");

        GtkWidget *powiadomienie;
        powiadomienie = gtk_window_new(GTK_WINDOW_TOPLEVEL);

        gtk_window_set_resizable(GTK_WINDOW(powiadomienie), FALSE);
        gtk_container_set_border_width(GTK_CONTAINER(powiadomienie), 10);

        gtk_window_set_position(GTK_WINDOW(powiadomienie), GTK_WIN_POS_CENTER);

        GtkWidget *box;
        box = gtk_vbox_new(FALSE, 5);
        gtk_container_add(GTK_CONTAINER(powiadomienie), box);

        GtkWidget *tresc;
        tresc = gtk_label_new("");

        if(AktualnyGracz == BLACK) gtk_label_set(GTK_LABEL(tresc), "Wygrywa BIALY!");
        else gtk_label_set(GTK_LABEL(tresc), "Wygrywa CZARNY!");

        gtk_box_pack_start (GTK_BOX (box), tresc, TRUE, TRUE, 0);

        GtkWidget *box2 = gtk_hbox_new(FALSE, 10);
        gtk_box_pack_start (GTK_BOX (box), box2, TRUE, TRUE, 0);

        GtkWidget *button_ok = gtk_button_new_from_stock(GTK_STOCK_OK);
        gtk_box_pack_start (GTK_BOX (box2), button_ok, TRUE, TRUE, 0);

        GtkWidget *button_quit = gtk_button_new_from_stock(GTK_STOCK_QUIT);
        gtk_box_pack_start (GTK_BOX (box2), button_quit, TRUE, TRUE, 0);

        g_signal_connect(G_OBJECT(button_ok), "clicked",G_CALLBACK(Anuluj),powiadomienie);
        g_signal_connect(G_OBJECT(button_quit), "clicked",G_CALLBACK(gtk_main_quit),NULL);

        g_signal_connect(G_OBJECT(powiadomienie), "destroy",G_CALLBACK(Anuluj),powiadomienie);


        gtk_widget_show_all(powiadomienie);
    }
}
//---------------------------------------------------------------------------------------------------------------------------------

