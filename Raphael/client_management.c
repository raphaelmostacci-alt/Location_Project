// Add a new client to clients.dat
void add_client()
{
	client_user new_client;

	printf("==== Create New Account ====");
	printf("\nEnter last name: ");
	fgets(new_client.last_name, TAILLE_MAX, stdin);
	new_client.last_name[strcspn(new_client.last_name, "\n")] = 0;

	printf("Enter first name: ");
	fgets(new_client.first_name, TAILLE_MAX, stdin);
	new_client.first_name[strcspn(new_client.first_name, "\n")] = 0;

	printf("Enter email: ");
	fgets(new_client.mail, TAILLE_MAX, stdin);
	new_client.mail[strcspn(new_client.mail, "\n")] = 0;

	FILE *file = fopen("clients.dat", "a");
	if (!file)
	{
		printf("Error: cannot open clients.dat for writing.\n");
		return;
	}
	fprintf(file, "%s %s %s\n", new_client.last_name, new_client.first_name, new_client.mail);
	fclose(file);
	printf("Account successfully created!\n");
}

// Includes for standard libraries and client structure/prototypes
#include "client_management.h"
#include "includes.h"

// Search for a client by last name and first name in clients.dat
int search_client(const char *last_name, const char *first_name)
{
	FILE *file = fopen("clients.dat", "r");
	if (!file)
	{
		printf("Error: cannot open clients.dat\n");
		return 0;
	}

	client_user client;
	while (fscanf(file, "%99s %99s %99s", client.last_name, client.first_name, client.mail) == 3)
	{
		if (strcmp(client.last_name, last_name) == 0 && strcmp(client.first_name, first_name) == 0)
		{
			fclose(file);
			return 1;
		}
	}
	fclose(file);
	return 0;
}
