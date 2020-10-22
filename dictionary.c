#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "dictionary.h"

//size of the hash table
#define N 143107

// Represents a node in a hash table
typedef struct node
{
    char *word; 
    struct node *next;
}
node;


char *buffer;

int free_counter = 0;

//current size of the dictionary
unsigned int dict_size = 0;

// Hash table
node *table[N];

node *places[N];
int place_counter = 0;

// Returns true if word is in dictionary
bool check(const char *word)
{
    static int word_len;

    word_len = strlen(word);
    char word_lower[word_len+1];

    for(int i = 0; word_lower[i]= word[i]; i++)
        //make it lower case
	    if(word_lower[i]>=65 && word_lower[i]<=92)
            word_lower[i] += 32;

    unsigned int hsh = hash(word_lower);
    node *tmp = table[hsh];

    if(tmp == NULL)
        return false;

    for(; tmp; tmp = tmp->next)
        if(!strcasecmp( tmp->word , word ))
            return true;

    return false;
}

//Hash function taken from http://www.cse.yorku.ca/~oz/hash.html
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (unsigned int) (hash % N);
}

// Loads dictionary into memory, returning true
bool load(const char *dictionary)
{
    
    static char buf_size = LENGTH;

    buffer = malloc(buf_size);

    FILE *file = fopen(dictionary, "r");
    if(file == NULL)
        return false;

    node *already_there;
    node *new_word;
    unsigned int hsh;
    char len;

    while ( fgets ( buffer, buf_size, file ) != NULL )
    {
        //replace last \n with \0
        len = strlen(buffer);
        buffer[--len] = '\0';

        new_word = malloc(sizeof(struct node));
        new_word->word = buffer;
        new_word->next = NULL;
        places[place_counter++] = new_word;

        hsh = hash(buffer);
        already_there = table[hsh];

        if(already_there == NULL) //this is the first word to hash to this value
        {
            table[hsh] = new_word;
            buffer = malloc(buf_size);
            dict_size++;
            continue;
        }

        while(already_there->next != NULL)
            already_there = already_there->next;

        already_there->next = new_word;
        dict_size++;
        buffer = malloc(buf_size);
    }

    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, otherwise 0 
unsigned int size(void)
{
    return dict_size;
}

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
