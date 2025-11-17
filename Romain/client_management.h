
#ifndef CLIENT_MANAGEMENT_H
#define CLIENT_MANAGEMENT_H

#define TAILLE_MAX 100

typedef struct 
{
	char last_name[TAILLE_MAX];
	char first_name[TAILLE_MAX];
	char mail[TAILLE_MAX];
} client_user;

// Prototypes des fonctions de gestion des clients
void show_all_clients();
int search_client(const char *last_name, const char *first_name);
void add_client();
void edit_client();
void delete_client();
void client_management_menu();

#endif // CLIENT_MANAGEMENT_H
