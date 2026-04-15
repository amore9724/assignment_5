/**
 * The reason for using a hashtable is to store the commands in hash slots by their pids.
 * This way you can lookup a command by a pid and retrieve all the info you need:
 *  command, index (this index means the line number in the text file you read),
 *  start time, and anything else you might need.
 *
 * A hashtable (as you might remember from CS146)
 * has slots and each slot contains a linked list of nodes 
 * (these are the entries that contain all the command info).
 * Thus the hashtable (see hashtab array variable below) is 
 * implemented as an array of nlists. Each array position is a 
 * slot and a linked list of nlist nodes starts at each array slot. 
 * Each array position is a hahstable slot.
 *
 * You find the hashtable slot for a pid by using a hash function, 
 * which will map the pid to a hashtable slot (array position).
 *
 * You can copy this entire code directly in your .c code. No need to have
 * many files. 
 *
 *
 * This nlist is a node, which stores one command's info in the hashtable.
 * TODO: You will need to adopt this code a little.
 *
 * The char *name and char *defn you can remove.
 * The nlist *next field is a pointer to the next node in the linked list.  
 * There is one hashtable slot in each array position, 
 * consequently there is one linked list of nlists under a hashtable slot. 
 */
#include "countnames.h"


struct nlist {
    /* table entry: */
    struct nlist *next; /* next entry in chain */
    int index; // this is the line index in the input text file
    int pid; // the process id. you can use the pid result of wait to lookup in the hashtable
    char *command; // command. This is good to store for when you decide to restart a command
    int starttime;
    int finishtime;
};

#define HASHSIZE 101
static struct nlist *hashtab[HASHSIZE]; /* pointer table */

/* This is the hash function: form hash value for string s */
/* You can use a simple hash function: pid % HASHSIZE */
unsigned hash(int pid) {
    return pid % HASHSIZE;
}

/* lookup: look for s in hashtab */
/* This is traversing the linked list under a slot of the hash table. The array position to look in is returned by the hash function */
struct nlist *lookup(int pid) {
    struct nlist *np;
    for (np = hashtab[hash(pid)]; np != NULL; np = np->next)
        if (pid == np->pid)
            return np; /* found */
    return NULL; /* not found */
}

/* insert: put (name, defn) in hashtab */
/* This insert returns a nlist node. Thus when you call insert in your main function  */
/* you will save the returned nlist node in a variable (mynode). */
/* Then you can set the starttime and finishtime from your main function: */
/* mynode->starttime = starttime; mynode->finishtime = finishtime; */
struct nlist *insert(char *command, int pid, int index) {
    struct nlist *np;
    unsigned hashval;

    if ((np = lookup(pid)) == NULL) {
        /* Case 1: pid not found — allocate a new node */
        np = (struct nlist *) malloc(sizeof(*np));
        if (np == NULL)
            return NULL; /* out of memory */

        /* Copy the command string safely */
        np->command = strdup(command);
        if (np->command == NULL) {
            free(np);
            return NULL;
        }

        np->pid = pid;
        np->index = index;
        np->starttime = 0;
        np->finishtime = 0;

        /* Insert at the front of the chain */
        hashval = hash(pid);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else {
        /* Case 2: pid already exists — update command and index */
        free(np->command);
        np->command = strdup(command);
        np->index = index;
    }

    return np;
}
