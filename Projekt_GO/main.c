
#include <stdio.h>
#include <gtk/gtk.h>
#include "interfejs.h"
#include "game.h"

void Menu();
void Wyjscie(GtkWidget *przycisk, GtkWidget *okno);

//-----MAIN-----------
int main(int argc, char *argv[])
{
    AktualnyGracz=BLACK;

    gtk_init (&argc, &argv);

    Menu();

    gtk_main();
    return 0;
}

//-----RESZTA---------
void Menu()
{
    StanGry = G_NO_STARTED;

    GtkWidget *Window_Menu;
    Window_Menu = gtk_window_new(GTK_WINDOW_TOPLEVEL);

    gtk_window_set_resizable(GTK_WINDOW(Window_Menu), FALSE);
    gtk_container_set_border_width(GTK_CONTAINER(Window_Menu), 10);

    gtk_window_set_position(GTK_WINDOW(Window_Menu), GTK_WIN_POS_CENTER);



    GtkWidget *box_main;
    box_main = gtk_vbox_new(FALSE, 30);
    gtk_container_add(GTK_CONTAINER(Window_Menu), box_main);


    //---------------BOX_1--------------------
    GtkWidget *box;
    box = gtk_vbox_new(FALSE, 5);
    gtk_container_add(GTK_CONTAINER(box_main), box);

    GtkWidget *button_new_game;
    button_new_game = gtk_button_new_with_label("Nowa gra");
    gtk_container_add(GTK_CONTAINER(box), button_new_game);

    GtkWidget *button_load;
    button_load = gtk_button_new_with_label("Wczytaj");
    gtk_container_add(GTK_CONTAINER(box), button_load);

    //---------------BOX_2--------------------
    GtkWidget *box2;
    box2 = gtk_vbox_new(FALSE, 30);
    gtk_container_add(GTK_CONTAINER(box_main), box2);

    GtkWidget *button_info;
    button_info = gtk_button_new_with_label("O projekcje");
    gtk_container_add(GTK_CONTAINER(box2), button_info);

    GtkWidget *button_exit;
    button_exit = gtk_button_new_from_stock(GTK_STOCK_QUIT);
    gtk_container_add(GTK_CONTAINER(box2), button_exit);

    //---------------OBSLUGA PRZYCISKOW--------
    g_signal_connect(button_new_game, "clicked", G_CALLBACK (NowaGraPoczatek), Window_Menu);
    g_signal_connect(button_load, "clicked", G_CALLBACK (Wczytaj), Window_Menu);

    g_signal_connect (button_info, "clicked", G_CALLBACK (Info), NULL);

    g_signal_connect (button_exit, "clicked", G_CALLBACK (gtk_main_quit), NULL);
    g_signal_connect (Window_Menu, "destroy", G_CALLBACK (Wyjscie), Window_Menu);

    //------------------------------------------
    gtk_widget_show_all(Window_Menu);
}

void Wyjscie(GtkWidget *przycisk, GtkWidget *okno)
{
    if(StanGry == G_NO_STARTED) gtk_main_quit();
    else  gtk_widget_destroy(okno);
}
