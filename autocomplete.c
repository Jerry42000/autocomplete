#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <math.h>
#include "autocomplete.h"



int compare_terms(const void *a, const void *b) {
    const term *t1 = (const term *) a;
    const term *t2 = (const term *) b;
    return strcmp(t1->term, t2->term);
}

int compare_weight(const void *a, const void *b) {
    const term *t1 = (const term *) a;
    const term *t2 = (const term *) b;
    if (t1->weight < t2->weight) {
        return 1;
    } else if (t1->weight > t2->weight) {
        return -1;
    } else {
        return 0;
    }
}

char* get_substring(char *source, char* destination, int start, int end){
    for (int i = 0; i < end-start+1 && source[start+i]!='\0'; i++){
        destination[i] = source[start+i];
    }
    destination[end-start+1] = '\0';
    return destination;
}
void read_in_terms(struct term **terms, int *pnterms, char *filename)
{
    char line[200];
    FILE *fp = fopen(filename, "r");
    fgets(line, sizeof(line), fp);
    *pnterms = atoi(line);
    *terms = (struct term*)malloc(sizeof(struct term) * (*pnterms));
    if (*terms == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    for(int i = 0; i < *pnterms; i++)
    {
        fgets(line, sizeof(line), fp);
        int j = 0;
        strcpy((*terms + i)->term, line);
        while ((isspace((*terms + i)->term[j])) != 0)
        {
            j++;
        }
        int WS = j;
        while ((isspace((*terms + i)->term[j])) == 0)
        {
            j++;
        }
        int WE = j - 1;
        int TS = j + 1;
        int TE = strlen((*terms + i)->term) -1-1;
        char weight[200];
        strncpy(weight,(*terms + i)->term+WS, WE);
        (*terms + i)->weight = (double)atoll(weight);
        char term[200];
        strncpy(term, (*terms + i)->term+TS, TE);
        strcpy((*terms + i)->term, term);
        int len = strlen((*terms + i)->term);
        if ((*terms + i)->term[len - 1] == '\n')
        {   
            (*terms + i)->term[len - 1] = '\0';
        }

    }
    qsort(*terms, *pnterms, sizeof(struct term), compare_terms);
}


int lowest_match(term *terms, int nterms, char *substr) {
    int low = 0, high = nterms - 1;
    int result = -1; 
    while (low <= high) {
        int mid = (low + high) / 2;
        if (strncmp(terms[mid].term, substr, strlen(substr)) == 0) {
            result = mid;  
            high = mid - 1; 
        } 
        else if (strcmp(terms[mid].term, substr) < 0) {
            low = mid + 1;
        } 
        else {
            high = mid - 1;
        }
    }
    return result;  
}

int highest_match(term *terms, int nterms, char *substr) {
    int low = 0, high = nterms - 1;
    int result = -1; 
    while (low <= high) {
        int mid = (low + high) / 2;
        if (strncmp(terms[mid].term, substr, strlen(substr)) == 0) {
            result = mid;  
            low = mid + 1; 
        } 
        else if (strcmp(terms[mid].term, substr) < 0) {
            low = mid + 1;
        } 
        else {
            high = mid - 1;
        }
    }
    return result; 
}
void autocomplete(term **answer, int *n_answer, term *terms, int nterms, char *substr) {

    int count = 0;
    int low = lowest_match(terms, nterms, substr);
    int high = highest_match(terms, nterms, substr);
    if (low == -1 || high == -1 || high < low) {
        *n_answer = 0;
        *answer = NULL;
        return;
    }
    *n_answer = high-low+1;
    *answer = (struct term *)malloc((*n_answer )* sizeof(struct term));
    if (*answer == NULL) {
        printf("Memory allocation failed!HAHAHHHHAHA\n");
        exit(1);
    }
    for (int i = low; i < low + *n_answer; i++)
    {
        strcpy((*answer + count)->term, (terms + i)->term);
        (*answer + count)->weight = (terms + i)->weight;
        count++;
    }
    qsort(*answer, *n_answer, sizeof(term), compare_weight);
}

