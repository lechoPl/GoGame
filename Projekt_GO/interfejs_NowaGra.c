#include <stdio.h>
#include <stdlib.h>
#include <gtk\gtk.h>
#include "interfejs.h"
#include "game.h"

/*************************************
**              NOWA GRA            **
*************************************/
// --- Do Handicap(aby mozna bylo przekazac wartosc spin_button)
GtkWidget *Handicap_spin;


// DO USTANALNIA ROZMIARU PLANSZY
void Rozmiar1()
{
    if(Handicap > 4) Handicap = 4;
    RozmiarPlanszy = 9;
}
void Rozmiar2()
{
    RozmiarPlanszy = 13;
}
void Rozmiar3()
{
    RozmiarPlanszy = 19;
}



//DO USTALANIA INNYCH OPCJI
void BRAK()
{
    Komi = 0;
    Handicap = 0;
    AktualnyGracz = BLACK;
}
void KOMI()
{
    Komi = 6.5;
    Handicap = 0;
    AktualnyGracz = BLACK;
}

void HANDICAP_WARTOSC(GtkWidget *przycisk)
{
    gchar *buf;

    buf = g_strdup_printf ("%d", gtk_spin_button_get_value_as_int (GTK_SPIN_BUTTON(Handicap_spin)));

    Handicap = atoi(buf);
    g_free (buf);
}
void HANDICAP_ZASTOSUJ(GtkWidget *przycisk, GtkWidget *okno)
{
    if(okno!=NULL) gtk_widget_destroy(okno);
}

void HANDICAP(GtkWidget *przycisk, GtkWidget *okno_main) // do zrobienia
{
    Komi = 0;

    GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GO - Handicap");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);


    GtkWidget *rama;
    rama = gtk_vbox_new(TRUE, 5);
    gtk_container_add(GTK_CONTAINER(window), rama);


    if(RozmiarPlanszy==9) Handicap_spin = gtk_spin_button_new_with_range(2, 4, 1);
    else Handicap_spin = gtk_spin_button_new_with_range(2, 9, 1);
    gtk_container_add(GTK_CONTAINER(rama), Handicap_spin);

    GtkWidget *ok;
    ok = gtk_button_new_from_stock(GTK_STOCK_APPLY);
    gtk_container_add(GTK_CONTAINER(rama), ok);


    g_signal_connect_after (ok, "clicked", G_CALLBACK (HANDICAP_ZASTOSUJ),window);

    g_signal_connect (window, "destroy", G_CALLBACK (HANDICAP_WARTOSC), NULL);

    AktualnyGracz = WHITE;

    gtk_widget_show_all (window);
}






/*****************************
****    NOWA GRA MAIN     ****
*****************************/
void NowaGraPoczatek(GtkWidget *menu, gpointer Menu_main)
{
    if(StanGry != G_NO_STARTED)
    {
        fprintf(stderr,"blad nowej gry.\n");
        exit(1);
    }

    StanGry = G_NEW;

    RozmiarPlanszy = 9;
    ZbiteBiale = ZbiteCzarne = 0;
    Komi = Handicap = 0;
    AktualnyGracz = BLACK;

    gtk_widget_destroy(Menu_main); // ukrycie menu

    printf ("|nowa gra\n");
    /*
    - Nowa Gra
        * Rozmiar planszy
            a) 9x9
            b) 13x13
            c) 19x19
        * Komi
            - standardowe: 6.5
        * Handicap
    */

    GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GO - Nowa Gra");
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    GtkWidget *frame;
    frame = gtk_vbox_new(FALSE, 10);
    gtk_container_add(GTK_CONTAINER(window), frame);

    //--------- ROZMIAR PLANSZY --------------------
    //GtkWidget *RozmiarPlanszy;

    GtkWidget *ramka_rozmiar;
    ramka_rozmiar = gtk_frame_new("Rozmiar planszy");
    gtk_container_add(GTK_CONTAINER(frame), ramka_rozmiar);
    //gtk_fixed_put(GTK_FIXED(frame), ramka_rozmiar, 0, 0);

    GtkWidget *box;
    box=gtk_vbox_new(TRUE, 0);
    gtk_container_border_width(GTK_CONTAINER(box), 5);
    gtk_container_add(GTK_CONTAINER(ramka_rozmiar),box);

    GtkWidget *radio1, *radio2, *radio3;
    radio1 = gtk_radio_button_new_with_label(NULL, "9x9");
    radio2 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "13x13");
    radio3 = gtk_radio_button_new_with_label_from_widget (GTK_RADIO_BUTTON (radio1), "19x19");

    gtk_container_add(GTK_CONTAINER(box), radio1);
    gtk_container_add(GTK_CONTAINER(box), radio2);
    gtk_container_add(GTK_CONTAINER(box), radio3);


    g_signal_connect(G_OBJECT(radio1), "pressed",G_CALLBACK(Rozmiar1),NULL);
    g_signal_connect(G_OBJECT(radio2), "pressed",G_CALLBACK(Rozmiar2),NULL);
    g_signal_connect(G_OBJECT(radio3), "pressed",G_CALLBACK(Rozmiar3),NULL);


    //---------------   INNE  -------------------
    GtkWidget *inne;
    inne = gtk_frame_new("Inne");
    gtk_container_add(GTK_CONTAINER(frame), inne);

    GtkWidget *vbox_RoznicaSily;
    vbox_RoznicaSily = gtk_vbox_new(FALSE, 0);
    gtk_container_border_width(GTK_CONTAINER(vbox_RoznicaSily), 5);
    gtk_container_add(GTK_CONTAINER(inne), vbox_RoznicaSily);

    //---------------   BRAK   -------------------
    GtkWidget *Brak;
    Brak = gtk_radio_button_new_with_label(NULL, "Brak");
    gtk_container_add(GTK_CONTAINER(vbox_RoznicaSily), Brak);

    //---------------   KOMI   -------------------
    GtkWidget *Komi;
    Komi = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(Brak),"Komi");
    gtk_container_add(GTK_CONTAINER(vbox_RoznicaSily), Komi);

    //--------------- HANDICAP ----------------
    GtkWidget *Handicap;
    Handicap = gtk_radio_button_new_with_label_from_widget(GTK_RADIO_BUTTON(Brak), "Handicap");
    gtk_container_add(GTK_CONTAINER(vbox_RoznicaSily), Handicap);


    g_signal_connect(G_OBJECT(Brak), "pressed",G_CALLBACK(BRAK),NULL);
    g_signal_connect(G_OBJECT(Komi), "pressed",G_CALLBACK(KOMI),NULL);
    g_signal_connect(G_OBJECT(Handicap), "pressed",G_CALLBACK(HANDICAP), NULL);


    //-------------- OK i ANULUJ -------------------
    GtkWidget *OK, *ANULUJ;
    OK = gtk_button_new_from_stock(GTK_STOCK_OK);
    ANULUJ = gtk_button_new_from_stock(GTK_STOCK_CANCEL);

    GtkWidget *linia;
    linia = gtk_hseparator_new ();
    gtk_container_add(GTK_CONTAINER(frame), linia);

    GtkWidget *przyciski_box;
    przyciski_box = gtk_hbox_new(FALSE, 10);
    gtk_container_add(GTK_CONTAINER(frame), przyciski_box);

    gtk_box_pack_start(GTK_BOX(przyciski_box), OK, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(przyciski_box), ANULUJ, TRUE, TRUE, 0);

    g_signal_connect (OK, "clicked", G_CALLBACK (Gra), window);
    g_signal_connect (ANULUJ, "clicked", G_CALLBACK (Anuluj), window);

    //-------------------------------------------
    g_signal_connect (window, "destroy", G_CALLBACK (Anuluj), NULL);


    gtk_widget_show_all(window);

}
/***********************************************************/
/***********************************************************/
