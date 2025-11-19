

#ifndef CLIENT_MANAGEMENT_H
#define CLIENT_MANAGEMENT_H

#include "includes.h"

typedef struct 
{
	char last_name[MAX_SIZE];
	char first_name[MAX_SIZE];
	char mail[MAX_SIZE];
} client_user;

// Prototypes for client management functions
void show_all_clients();
int search_client(const char *last_name, const char *first_name);
int get_client_info(const char *last_name, const char *first_name, client_user *client);
void add_client();
void edit_client();
void delete_client();
void client_management_menu();

#endif // CLIENT_MANAGEMENT_H
