#define F_SIZE 19

extern void Menu();

void Anuluj(GtkWidget *przycisk, GtkWidget *okno); // ->interfejs.c

//Ustawianie opcji nowej gry
void NowaGraPoczatek(GtkWidget *menu, gpointer Menu_main); // -> interfejs_NowaGra.c // SKONCZONY!!

//Wczytanie zapisanej gry
void Wczytaj(GtkWidget *przycisk, GtkWidget *okno); // ->interfejs.c
void Zapisz(); // ->interfejs.c

//Interfejs z obsluga nowej gry lub wczytanej gry
void Gra(GtkWidget *przycisk, GtkWidget *poprzenie_okno); // -> interfejs_Rozgrywka.c

//informacje o projekcie
void Info(); // ->interfejs.c
