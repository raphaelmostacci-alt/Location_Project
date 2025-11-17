
#ifndef CLIENT_MANAGEMENT_H
#define CLIENT_MANAGEMENT_H

#include "includes.h"

#define MAX_SIZE 100

typedef struct 
{
	char last_name[MAX_SIZE];
	char first_name[MAX_SIZE];
	char mail[MAX_SIZE];
	int available; // 1 = available, 0 = not available (logical deletion)
} client_user;

// Prototypes for client management functions
void show_all_clients();
int search_client(const char *last_name, const char *first_name);
void add_client();
void edit_client(client_user *client);
void delete_client();
void client_management_menu();

#endif // CLIENT_MANAGEMENT_H
