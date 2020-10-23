// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h>

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// Prototypes
bool check(const char *word);
unsigned int hash(const char *word);
bool load(const char *dictionary);
unsigned int size(void);
bool unload(void);

/*
 puts a list of generated suggestions upto num_suggestions into 
 suggestions_array
 it returns the number of suggestions that are actually generated (may be less than
 num_suggestions because less suggestions were found)
 if less than num_suggestions are found, the rest of the array upto the num_suggestion's
 place are replaced by 0
*/

int getSuggestions(char *misspelled_word, char **suggestions, int limit);

int getSuggestionsCustom(char *misspelled_word, char **suggestions,
        int (*suggest)(char *, char **, int), int limit);

int suggestion_by_insertion(char *word, char **suggestions, int limit);

#endif // DICTIONARY_H
