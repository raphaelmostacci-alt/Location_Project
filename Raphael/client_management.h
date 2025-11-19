#ifndef CLIENT_MANAGEMENT_H
#define CLIENT_MANAGEMENT_H

#include "includes.h"


typedef struct 
{
	char last_name[MAX_SIZE];
	char first_name[MAX_SIZE];
	char mail[MAX_SIZE];
	int available; // 1 = available, 0 = not available (logical deletion)
} client_user;


// Variables globales pour la gestion dynamique
extern client_user *clients;
extern int client_count;
extern char current_user_email[MAX_SIZE];

// Prototypes pour la gestion dynamique
void load_clients();
void save_clients();
void free_clients();
void show_all_clients();
int search_client(const char *last_name, const char *first_name);
void add_client();
void edit_client();
void delete_client();
void client_management_menu();

#endif // CLIENT_MANAGEMENT_H
