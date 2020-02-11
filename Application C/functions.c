#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <stdint.h>
#include "cJSON.h"
#include "functions.h"
#include "common.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
    size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

int write_file_from_link(char *fileName,char *site)
{

    CURL *curl_handle;
    FILE *pagefile;


    curl_global_init(CURL_GLOBAL_ALL);

    /* init the curl session */
    curl_handle = curl_easy_init();

    /* set URL to get here */
    curl_easy_setopt(curl_handle, CURLOPT_URL, site);

    /* Switch on full protocol/debug output while testing */
  //  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

    /* disable progress meter, set to 0L to enable and disable debug output */
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);

    /* send all data to this function  */
    curl_easy_setopt(curl_handle, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

    /* open the file */
    pagefile = fopen(fileName, "wb");
    if(pagefile)
    {

        /* write the page body to this file handle */
        curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

        /* get it! */
        curl_easy_perform(curl_handle);

        /* close the header file */
        fclose(pagefile);
    }

    /* cleanup curl stuff */
    curl_easy_cleanup(curl_handle);

    curl_global_cleanup();

    return 0;
}



cJSON *parse_file_lignes(const char *filename, char *postal)
{
    cJSON *parsed = NULL;
    char *content = read_file(filename);//on stocke dans ce char le contenu du fichier
    parsed = cJSON_Parse(content);//on va maintenant parcourir ce contenu
    if(parsed == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }//on indique ou se trouve l'erreur
    }//si le contenu n'a pas été reconnu en tant que fichier JSON
    else
    {
        cJSON *resultItem = cJSON_GetObjectItem(parsed, "result");
        cJSON *metrosItem = cJSON_GetObjectItem(resultItem, "metros");
        int i, j = 0;;
        for (i = 0 ; i < cJSON_GetArraySize(metrosItem) ; i++)
        {
            cJSON *item = cJSON_GetArrayItem(metrosItem, i);
            printf("\n%s : ",cJSON_GetObjectItem(item, "name")->valuestring);
            printf("\n%s\n\n",cJSON_GetObjectItem(item, "directions")->valuestring);
        }//on va parcourir tous les objets du tableau d'objet de notre api
    }
    if (content != NULL)
    {
        free(content);
    }
    return parsed;
}


cJSON *parse_file_metro(char *metro)
{
    //printf("ligne : %s",metro);
    char filename[10] = "plan.json";
    char link[200];
    strcpy(link,"https://api-ratp.pierre-grimaud.fr/v4/stations/metros/");
    strcpy(link+strlen(link),metro);

    link[strlen(link)-1] = '\0';//car sinon c'est \n et ça bug
    printf("%s",link);
    if(write_file_from_link(filename,link))
    {
        printf("\nLe plan de la ligne n'a pas pu être chargé\n");
        return 1;
    }
    cJSON *parsed = NULL;
    char *content = read_file(filename);//on stocke dans ce char le contenu du fichier
    parsed = cJSON_Parse(content);//on va maintenant parcourir ce contenu
    if(parsed == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }//on indique ou se trouve l'erreur
    }//si le contenu n'a pas été reconnu en tant que fichier JSON
    else
    {
        cJSON *resultItem = cJSON_GetObjectItem(parsed, "result");
        cJSON *metrosItem = cJSON_GetObjectItem(resultItem, "stations");
        char **stations;
        stations = malloc(cJSON_GetArraySize(metrosItem)*200);
        char **stationsSlug;
        stationsSlug = malloc(cJSON_GetArraySize(metrosItem)*200);
        int i, j = 0;;
        for (i = 0 ; i < cJSON_GetArraySize(metrosItem) ; i++)
        {
            cJSON *item = cJSON_GetArrayItem(metrosItem, i);
            char *inter = cJSON_GetObjectItem(item, "name")->valuestring;
            stations[i] = inter;
            char *inter2 = cJSON_GetObjectItem(item, "slug")->valuestring;
            stationsSlug[i] = inter2;
            printf("\n%s : (%d)",cJSON_GetObjectItem(item, "name")->valuestring,i);
        }//on va parcourir tous les objets du tableau d'objet de notre api
        printf("\nVeuillez rentrer le numéro de l'arret choisi\n");
        char number[3];
        fgets(number,sizeof(number),stdin);
        if(atoi(number) < cJSON_GetArraySize(metrosItem) && atoi(number) >= 0)
        {
            char link2[200];
            strcpy(link2,"https://api-ratp.pierre-grimaud.fr/v4/schedules/metros/");
            strcpy(link2+strlen(link2),metro);
            link2[strlen(link2)-1] = '\0';//car sinon c'est \n et ça bug
            strcpy(link2+strlen(link2),"/");
            strcpy(link2+strlen(link2),stationsSlug[atoi(number)]);
            strcpy(link2+strlen(link2),"/R");
            char fileName2[14] =  "horaires.json";
            if(write_file_from_link(fileName2,link2))
            {
                printf("\nLes horaires n'ont pas pu être chargés\n");
                return -1;
            }
                printf("\nVoici les horaires pour la station : %s\n",stations[atoi(number)]);
                cJSON *parsed2 = NULL;
                char *content2 = read_file(fileName2);//on stocke dans ce char le contenu du fichier
                parsed2 = cJSON_Parse(content2);//on va maintenant parcourir ce contenu
                if(parsed2 == NULL)
                {
                    const char *error_ptr2 = cJSON_GetErrorPtr();
                    if (error_ptr2 != NULL)
                    {
                        printf("Le fichier est vide !");
                        //fprintf(stderr, "Error before: %s\n", error_ptr2);
                    }//on indique ou se trouve l'erreur
                }//si le contenu n'a pas été reconnu en tant que fichier JSON
                else
                {
                    cJSON *resultItem2 = cJSON_GetObjectItem(parsed2, "result");
                    cJSON *schedulesItem = cJSON_GetObjectItem(resultItem2, "schedules");
                    int i, j = 0;;
                    for (i = 0 ; i < cJSON_GetArraySize(schedulesItem) ; i++)
                    {
                        cJSON *item = cJSON_GetArrayItem(schedulesItem, i);
                        printf("\n%s",cJSON_GetObjectItem(item, "message")->valuestring);
                        printf("\n%s",cJSON_GetObjectItem(item, "destination")->valuestring);
                    }//on va parcourir tous les objets du tableau d'objet de notre api
                }
        }
        else
        {
            printf("\nLes horaires n'ont pas pu être chargés");
        }
        free(stations);
    }

    if (content != NULL)
    {
        free(content);
    }
    return parsed;
}


