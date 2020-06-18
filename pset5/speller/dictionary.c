// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// Number of buckets in hash table
const unsigned int N = 100;

// Hash table
node *table[N];


// Number of words in the dict
unsigned int num_words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    char word_to_check[LENGTH + 1];
    strcpy(word_to_check, word);
    // Hash word to obtain hash value
    int hash_val = hash(word_to_check);
    node *tmp = table[hash_val];
    // Access linked list at that index in the hash table
    while (tmp != NULL)
    {
        // traverse linked list, loking for the word (strcasecmp)
        if (strcasecmp(tmp->word, word) == 0)
        {
            return true;
        }
        tmp = tmp->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Source: djib2 by Dan Bernstein (http://www.cse.yorku.ca/~oz/hash.html))
    unsigned long hash = 5381;
    int c = *word;
    c = tolower(c);
    while (*word != 0)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
        c = *word++;
        c = tolower(c);
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary)
{
    // Open dictionary file to read
    FILE *file = fopen(dictionary, "r");
    // check if open was successful
    if (file == NULL)
    {
        return false;
    }
    char word[LENGTH + 1];
    int hash_val;

    // Read strings from file one at a time
    while(fscanf(file, "%s", word) != EOF)
    {
        // Create new node for each word
        node *n = malloc(sizeof(node));
        // check if malloc was successful
        if (n == NULL)
        {
            return false;
        }
        // insert data to our node n
        strcpy(n->word, word);
        n->next = NULL;
        // Hash word to obtain a hash value
        hash_val = hash(word);
        size();
        // Insert node into hash table at index hash_val
        if (table[hash_val] == NULL)
        {
            table[hash_val] = n;
        }
        else // node already exists at that index
        {
            // direct n's next pointer to match table[hash_val]
            n->next = table[hash_val];
            // move table[hash_val] pointer to new head of linked list
            table[hash_val] = n;
        }
    }
    //Close the file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return num_words++;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        node *p = table[i];
        node *tmp = table[i];
        while (p != NULL)
        {
            p = p->next;
            free(tmp);
            tmp = p;
        }
    }

    return true;
}