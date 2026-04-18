#include "countnames.h"


static NameCountData *hashtab[HASHSIZE];  /* pointer table */

/* This is the hash function: form hash value for string s */
/* You can use a simple hash function: pid % HASHSIZE */
unsigned hash(char *name) {
    unsigned hashval = 0;
    for (int i = 0; name[i] != '\0'; i++)
        hashval += name[i];
    return hashval % HASHSIZE;
}

/* lookup: look for s in hashtab */
NameCountData *lookup(char *name) {
    NameCountData *np;
    for (np = hashtab[hash(name)]; np != NULL; np = np->next)
        if (strcmp(name, np->name) == 0)
            return np; /* found */
    return NULL; /* not found */
}

/*  insert: put (name, defn) in hashtab
    This insert returns a nlist node. Thus when you call insert in your main function
    you will save the returned nlist node in a variable (mynode).
    Then you can set the starttime and finishtime from your main function:
    mynode->starttime = starttime; mynode->finishtime = finishtime; */
NameCountData *insert(NameCountData *ncd) {
    NameCountData *np;
    if ((np = lookup(ncd->name)) == NULL) {
        np = (NameCountData *) malloc(sizeof(NameCountData));   // Create new entry.
        if (np == NULL) return NULL;        // NULL tester
        np->name = strdup(ncd->name);       // Copy name to np->name structure
        if (np->name == NULL) {             // Check if name exists. If not then return NULL.
            free(np);
            return NULL;
        }
        np->count = ncd->count;             // Set count to actual count

        unsigned hashval = hash(ncd->name); // Set hashval to hash(name)
        np->next = hashtab[hashval];        // Set next entry to current entry at hashtable
        hashtab[hashval] = np;              // Set current entry at hashtable to np.
    } else {
        np->count += ncd->count;            // Increment counter for name.
    }
    return np;                              // Return NameCountData structure
}
void table_print() {
    for (int i = 0; i < HASHSIZE; i++) {
        NameCountData *np = hashtab[i];
        while (np != NULL) {
            printf("%s: %d\n", np->name, np->count);
            np = np->next;
        }
    }
}
void table_destroy() {
    for (int i = 0; i < HASHSIZE; i++) {
        NameCountData *np = hashtab[i];
        while (np != NULL) {
            NameCountData *next = np->next;  // Save next pointer before freeing
            free(np->name);                  // Free strdup string
            free(np);                        // Free node itself
            np = next;
        }
        hashtab[i] = NULL;
    }
}