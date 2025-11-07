#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 20
#define ok 0

typedef struct client
{
    char last_name[MAX];
    char first_name[MAX];
    long phone_number;
} client;