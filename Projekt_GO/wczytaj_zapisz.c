#include <stdio.h>
#include <stdlib.h>
#include "wczytaj_zapisz.h"
#include "game.h"

int ZaladujGre(char *sciezka_do_pliku)
{
    FILE *zapisana_gra;

    if((zapisana_gra=fopen(sciezka_do_pliku,"rb"))==NULL )
    {
        fprintf(stderr, "Nie mozna otworzyc pliku: %s\n",sciezka_do_pliku);
        return 1;
    }
    int temp[5];
    fread(temp, sizeof(int),5,zapisana_gra);
    if((temp[0]==9 || temp[0]==13 || temp[0] == 19)&&(temp[1]==1 || temp[1]==2))
    {
        ZbiteCzarne = temp[2];
        ZbiteBiale = temp[3];
        if(temp[4] == 1) Komi = _KOMI;

        RozmiarPlanszy = temp[0];
        AktualnyGracz = temp[1];

        int *buf_plansza = (int*)malloc(sizeof(int)*RozmiarPlanszy*RozmiarPlanszy);
        fread(buf_plansza, sizeof(int), RozmiarPlanszy*RozmiarPlanszy,zapisana_gra);

        for(int i=0; i< RozmiarPlanszy*RozmiarPlanszy;i++)
        {
            Plansza[i].wartosc = buf_plansza[i];
        }

        free(buf_plansza);

    }
    else
    {
        fprintf(stderr, "Zle dane w pliku");
        return 1;
    }

    fclose(zapisana_gra);

    return 0;
}

int ZapiszGre(char *sciezka_do_pliku)
{
    FILE *zapisana_gra;

    if((zapisana_gra=fopen(sciezka_do_pliku,"wb"))==NULL )
    {
        fprintf(stderr, "Nie mozna utworzyc pliku: %s\n",sciezka_do_pliku);
        return 1;
    }
    int CzyKomi;
    if(Komi == _KOMI) CzyKomi=1;
    else CzyKomi = 0;

    int temp[] = {RozmiarPlanszy, AktualnyGracz, ZbiteCzarne, ZbiteBiale, CzyKomi};
    fwrite(temp, sizeof(int),5,zapisana_gra);

    int *buf_plansza = (int*)malloc(sizeof(int)*RozmiarPlanszy*RozmiarPlanszy);

    for(int i=0; i< RozmiarPlanszy*RozmiarPlanszy;i++)
    {
        buf_plansza[i] = Plansza[i].wartosc;
    }

    fwrite(buf_plansza, sizeof(int), RozmiarPlanszy*RozmiarPlanszy,zapisana_gra);

    free(buf_plansza);

    fclose(zapisana_gra);

    return 0;
}
