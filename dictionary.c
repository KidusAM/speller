// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char *word; // = malloc(LENGTH + 1);
    struct node *next;
}
node;


char *buffer;
// char *buf = buffer;
int buf_size = 47;

int free_counter = 0;

// Number of buckets in hash table
const unsigned int N = 143107;
unsigned int dict_size = 0;

int word_len;
// Hash table
node *table[N] = {NULL};

node *places[N] = {NULL};
int place_counter = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    word_len = strlen(word);
    char *word_lower = malloc(word_len+1);
    for(int i=0;i<=word_len;i++)
    {
	    if(word[i]>=65 && word[i]<=92) word_lower[i]=word[i]+32;
	    else word_lower[i] = word[i];
    }
    int hsh = hash(word_lower);
    node *tmp = table[hsh];

    if(tmp == NULL){
        free(word_lower);
        return false;
    }

    for(; tmp->next != NULL; tmp = tmp->next)
    {
        if(!strcasecmp( tmp->word , word ))
        {
            free(word_lower);
            return true;
        }
    }
    if(!strcasecmp( tmp->word , word ))
    {
        free(word_lower);
        return true;
    }


    free(word_lower);
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    //taken from http://www.cse.yorku.ca/~oz/hash.html
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (unsigned int) (hash % N);
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{

    buffer = malloc(buf_size);
    FILE *file = fopen(dictionary, "r");
    if(file == NULL) return false;
    node *temp2;
    char *wrd;
    int hsh;
    size_t len;

    while ( fgets ( buffer, buf_size, file ) != NULL )
    {
        len = strlen(buffer);
        buffer[--len] = '\0';

        node *temp = malloc(sizeof(struct node));
        temp->word = buffer;
        temp->next = NULL;
        places[place_counter++] = temp;

        hsh = hash(buffer);
        temp2 = table[hsh];


        if(temp2 == NULL)
        {
            table[hsh] = temp;
            buffer = malloc(buf_size);
            dict_size++;
            continue;
        }

        while(temp2->next != NULL) temp2 = temp2->next;

        temp2->next = temp;
        dict_size++;
        buffer = malloc(buf_size);
    }
    free(buffer);
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return dict_size;
}


// void free_node(node *a)
// {
//     if (a->next == NULL)
//     {
//         free(a->word);
//         free(a);
//         return;
//     }

//     free_node(a->next);
//     free(a->word);
//     free(a);
// }
// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for(int i = 0; i < place_counter; i++)
    {
        free(places[i]->word);
        free(places[i]);
    }
    return true;
}
