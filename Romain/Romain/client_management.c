
#include "includes.h"
#include "client_management.h"
#define CLIENTS_DATA_FILE "clients.dat"

// Search for a client by last name and first name
int search_client(const char *last_name, const char *first_name) {
	FILE *f = fopen(CLIENTS_DATA_FILE, "rb");
	if (!f) return 0;
	client_user c;
	while (fread(&c, sizeof(client_user), 1, f) == 1) {
		if (strcmp(c.last_name, last_name) == 0 && strcmp(c.first_name, first_name) == 0) {
			fclose(f);
			return 1;
		}
	}
	fclose(f);
	return 0;
}

// Get client information by last name and first name
int get_client_info(const char *last_name, const char *first_name, client_user *client) {
	FILE *f = fopen(CLIENTS_DATA_FILE, "rb");
	if (!f) return 0;
	while (fread(client, sizeof(client_user), 1, f) == 1) {
		if (strcmp(client->last_name, last_name) == 0 && strcmp(client->first_name, first_name) == 0) {
			fclose(f);
			return 1;
		}
	}
	fclose(f);
	return 0;
}



// Add a new client to the database
void add_client() {
	client_user new_client;
	printf("\n--- Add a client ---\n");
	printf("Last name: ");
	fgets(new_client.last_name, MAX_SIZE, stdin);
	new_client.last_name[strcspn(new_client.last_name, "\n")] = '\0';

	printf("First name: ");
	fgets(new_client.first_name, MAX_SIZE, stdin);
	new_client.first_name[strcspn(new_client.first_name, "\n")] = '\0';

	printf("Email: ");
	fgets(new_client.mail, MAX_SIZE, stdin);
	new_client.mail[strcspn(new_client.mail, "\n")] = '\0';

	// Basic check (improve if needed)
	if(strlen(new_client.last_name) == 0 || strlen(new_client.first_name) == 0 || strlen(new_client.mail) == 0) {
		printf("Error: all fields must be filled.\n");
		return;
	}

	// Add to the data file
	FILE *f = fopen(CLIENTS_DATA_FILE, "ab");
	if (!f) {
		printf("Error opening client file.\n");
		return;
	}
	fwrite(&new_client, sizeof(client_user), 1, f);
	fclose(f);
	printf("Client added successfully!\n");
}
