#include <stdio.h>
#include <stdlib.h>
#include <gtk\gtk.h>
#include "interfejs.h"
#include "game.h"
#include "wczytaj_zapisz.h"

/************************************************************
Zawiera kod odpowiedzialny za tworzenie okna zapisz i wczytaj
************************************************************/

#define _ile_slotow 5 // mozna zwiekszyc wtedy bedzie wiecej miejsc do zapiswywania

void Anuluj(GtkWidget *przycisk, GtkWidget *okno)
{
    if(okno!= NULL) gtk_widget_destroy(okno);

    if(StanGry == G_NEW) Menu();
}

void PoprzedniStanGry(GtkWidget *przycisk, gpointer poprzedni)
{
    StanGry = (enum STAN_GRY) poprzedni;
}

void ZamknijWczesniejszOkno(GtkWidget *przycisk, GtkWidget *okno)
{
    if(okno!=NULL) gtk_widget_destroy(okno);
}

void ObslugaSlotu(GtkWidget *przycisk, char *buf)
{
    if(StanGry == G_LOAD)
    {
        if(ZaladujGre(buf)==0) Gra(NULL,NULL);
        else Menu();
    }
    if(StanGry == G_STARTED)
    {
        if(ZapiszGre(buf)==1) fprintf(stderr, "Blad zapisu gry");
    }
}

void Wczytaj(GtkWidget *przycisk, GtkWidget *okno)
{
    enum STAN_GRY temp = StanGry;
    StanGry = G_LOAD;

    printf ("|wczytaj\n");

    GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GO - Wczytaj");

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *box=gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *sloty[_ile_slotow];

    FILE *fp;
    char *buf;
    for(int i=0; i<_ile_slotow; i++)
    {
        buf = g_strdup_printf ("save\\save%d.go",i+1);

        if((fp=fopen(buf, "rb"))==NULL)
        {
            sloty[i] = gtk_label_new("- PUSTY -");
        }
        else
        {
            buf = g_strdup_printf ("GRA %d",i+1);
            sloty[i] = gtk_button_new_with_label(buf);
            buf = g_strdup_printf ("save\\save%d.go",i+1);

            g_signal_connect(G_OBJECT(sloty[i]), "clicked",G_CALLBACK(ZamknijWczesniejszOkno), okno);
            g_signal_connect(G_OBJECT(sloty[i]), "clicked",G_CALLBACK(ObslugaSlotu),buf);
            g_signal_connect(G_OBJECT(sloty[i]), "clicked",G_CALLBACK(Anuluj), window);

        }
        gtk_box_pack_start (GTK_BOX (box), sloty[i], TRUE, TRUE, 0);

    }

    buf = g_strdup_printf ("EMPTY");
    free(buf);
    fclose(fp);

    GtkWidget *linia = gtk_hseparator_new();
    gtk_box_pack_start (GTK_BOX (box), linia, TRUE, TRUE, 5);

    GtkWidget *zamknij = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
    gtk_box_pack_start (GTK_BOX (box), zamknij, TRUE, TRUE, 0);

    g_signal_connect(G_OBJECT(zamknij), "clicked",G_CALLBACK(PoprzedniStanGry),(gpointer)temp);
    g_signal_connect(G_OBJECT(zamknij), "clicked",G_CALLBACK(Anuluj),window);


    gtk_widget_show_all(window);
}


void Zapisz()
{
    if(StanGry != G_END)
    {
        printf ("|zapisz\n");

        GtkWidget *window;
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
        gtk_window_set_title(GTK_WINDOW(window), "GO - Zapisz");

        gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
        gtk_container_set_border_width(GTK_CONTAINER(window), 10);

        gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

        GtkWidget *box=gtk_vbox_new(FALSE, 5);
        gtk_container_add(GTK_CONTAINER(window), box);

        GtkWidget *sloty[_ile_slotow];

        FILE *fp;
        char *buf;
        for(int i=0; i<_ile_slotow; i++)
        {
            buf = g_strdup_printf ("save\\save%d.go",i+1);



            if((fp=fopen(buf, "rb"))==NULL)
            {
                sloty[i] = gtk_button_new_with_label("- PUSTY -");
            }
            else
            {
                buf = g_strdup_printf ("GRA %d",i+1);
                sloty[i] = gtk_button_new_with_label(buf);
                buf = g_strdup_printf ("save\\save%d.go",i+1);
            }

            g_signal_connect(G_OBJECT(sloty[i]), "clicked",G_CALLBACK(ObslugaSlotu),buf);
            g_signal_connect(G_OBJECT(sloty[i]), "clicked",G_CALLBACK(Anuluj),window);
            gtk_box_pack_start (GTK_BOX (box), sloty[i], TRUE, TRUE, 0);
        }
        buf = g_strdup_printf ("END");
        free(buf);
        fclose(fp);

        GtkWidget *linia = gtk_hseparator_new();
        gtk_box_pack_start (GTK_BOX (box), linia, TRUE, TRUE, 5);

        GtkWidget *zamknij = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
        gtk_box_pack_start (GTK_BOX (box), zamknij, TRUE, TRUE, 0);

        g_signal_connect(G_OBJECT(zamknij), "clicked",G_CALLBACK(Anuluj),window);


        gtk_widget_show_all(window);
    }
}

void Info()
{
    GtkWidget *window;
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "GO - O projekcje");

    gtk_window_set_resizable(GTK_WINDOW(window), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);

    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    GtkWidget *box=gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(window), box);

    GtkWidget *box2=gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), box2, TRUE, TRUE, 0);

    GtkWidget *napis = gtk_label_new("Nazwa projektu:");
    gtk_box_pack_start(GTK_BOX(box2), napis, TRUE, TRUE, 0);

    napis =gtk_label_new("  GO");
    gtk_box_pack_start(GTK_BOX(box2), napis, TRUE, TRUE, 5);


    GtkWidget *box3=gtk_hbox_new(FALSE, 5);
    gtk_box_pack_start(GTK_BOX(box), box3, TRUE, TRUE, 0);

    napis =gtk_label_new("Autor: ");
    gtk_box_pack_start(GTK_BOX(box3), napis, TRUE, TRUE, 0);

    napis =gtk_label_new("  Michal Lech");
    gtk_box_pack_start(GTK_BOX(box3), napis, TRUE, TRUE, 5);

    GtkWidget *linia = gtk_hseparator_new();
    gtk_box_pack_start(GTK_BOX(box), linia, TRUE, TRUE, 5);

    GtkWidget *ok = gtk_button_new_from_stock(GTK_STOCK_OK);
    gtk_box_pack_start(GTK_BOX(box), ok, TRUE, TRUE, 5);


    g_signal_connect(G_OBJECT(ok), "clicked",G_CALLBACK(Anuluj),window);

    gtk_widget_show_all(window);
}
