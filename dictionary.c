#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>

#include "dictionary.h"

//size of the hash table
#define N 143107

int (*default_suggestion_funcs[])(char *, char **, int) = {&suggestion_by_insertion, NULL};

// Represents a node in a hash table
typedef struct node
{
    char *word; 
    struct node *next;
}
node;



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
    static char *buffer;

    buffer = malloc(buf_size);

    FILE *file = fopen(dictionary, "r");
    if(file == NULL)
        return false;

    node *already_there;
    node *new_word;
    unsigned int hsh;
    unsigned char len;

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


int suggestion_by_insertion(char *word, char **suggestions, int limit)
{
    printf("\nSomebody called me on the word %s\n", word);
    
    int added = 0;
    int word_len = strlen(word) + 1;
    char word_copy[word_len+1];

    for(int i = 0; i < word_len; i++)
    {
        for(char c = 'a'; c <= 'z'; c++)
        {
            for(int j = 0, k = 0; j < word_len + 1; j++)
            {
                if ( j == i)
                {
                    word_copy[k++] = c;
                }    
                word_copy[k++] = word[j];
            }

            word_copy[word_len] = '\0';
            
            if (check(word_copy))
            {
                printf("\n Added %s as suggestion for %s\n", word_copy, word);
                strcpy(*suggestions++, word_copy);
                added++;
            }
            if(added == limit)
                return added;
        }
    }

    return added;
    

}

int getSuggestionsCustom(char *misspelled_word, char **suggestions,
        int (*suggest)(char *, char **, int), int limit)
{
    int size = 0;
    
    while(size < limit && size >= 0)
    {
        size += suggest(misspelled_word, suggestions++, 1);

        if(!size) --size;
    }
    
    if (size < 0)
        return 0;

    return size;
}


int getSuggestions(char *misspelled_word, char **suggestions, int limit)
{

    int num_of_suggestions;

    for(int i = 0; i < limit; i++)
    {
        num_of_suggestions += getSuggestionsCustom(misspelled_word, suggestions, default_suggestion_funcs[0], 1);
    }

    return num_of_suggestions;

}
