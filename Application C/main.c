#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "functions.h"
#include "cJSON.h"

int main()
{
    system("chcp 65001");
   // char yes[200];
   // char *no = "https://api-ratp.pierre-grimaud.fr/v4/stations/metros/a";
   // strcpy(yes,memset(no+(strlen(no)-1),"1",1));
   // printf("%s",yes);
    char choice[4];
    char postal[20];
    printf("\nBonjour à vous, ce logiciel vous permet de connaitre les horaires du metro parisien");
    if(write_file_from_link("lignes.json","https://api-ratp.pierre-grimaud.fr/v4/lines/metros"))
    {
        printf("le fichier n'a pas pu être chargé\n");
        return 1;
    }

    printf("Pour rappel, voici la liste des metros\n");
    parse_file_lignes("lignes.json",postal);
    printf("\nVeuillez rentrer le numéro du metro désiré.(ne marche qu'avec le metro 1 pour l'instant)\n");
    fgets(choice,sizeof(choice),stdin);
    int lineNumber = atoi(choice);
    printf("%s",choice);
    if(lineNumber >= 1 && lineNumber <= 14)
    {
        printf("Vous avez choisi le metro %s.\n",choice);
        parse_file_metro(choice);

    }
   /* else if(atoi(choice) == 2)
    {

    }*/
    else
    {
        printf("Ce metro n'existe pas");
    }

    printf("\nProgram finished");

    return 0;
}



