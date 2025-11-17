// ...existing code...
#include "includes.h"
#include "client_management.h"
#define CLIENTS_DATA_FILE "clients.dat"


// Show all clients in the database
void show_all_clients()
{
	FILE *f = fopen(CLIENTS_DATA_FILE, "rb");
	if (!f)
	{
		printf("Error opening client file.\n");
		return;
	}
	client_user c;
	int count = 0;
	printf("\n--- All Registered Clients ---\n");
	while (fread(&c, sizeof(client_user), 1, f) == 1)
	{
		printf("%d. Last name: %s | First name: %s | Email: %s\n", ++count, c.last_name, c.first_name, c.mail);
	}
	fclose(f);
	if (count == 0)
	{
		printf("No clients found in the database.\n");
	}
}


// Search for a client by last name and first name
int search_client(const char *last_name, const char *first_name) 
{
	FILE *f = fopen(CLIENTS_DATA_FILE, "rb");
	if (!f) return 0;
	client_user c;
	while (fread(&c, sizeof(client_user), 1, f) == 1) 
    {
		if (strcmp(c.last_name, last_name) == 0 && strcmp(c.first_name, first_name) == 0) 
        {
			fclose(f);
			return 1;
		}
	}
	fclose(f);
	return 0;
}


// Add a new client to the database
void add_client() 
{
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

	// Basic check
	if(strlen(new_client.last_name) == 0 || strlen(new_client.first_name) == 0 || strlen(new_client.mail) == 0)
	{
		printf("Error: all fields must be filled.\n");
		return;
	}

	// Check if client already exists
	FILE *f_check = fopen(CLIENTS_DATA_FILE, "rb");
	if (f_check)
	{
		client_user c;
		while (fread(&c, sizeof(client_user), 1, f_check) == 1)
		{
			if (strcmp(c.last_name, new_client.last_name) == 0 && strcmp(c.first_name, new_client.first_name) == 0)
			{
				printf("Error: client already exists.\n");
				fclose(f_check);
				return;
			}
		}
		fclose(f_check);
	}

	// Add to the data file
	FILE *f_add = fopen(CLIENTS_DATA_FILE, "ab");
	if (!f_add)
	{
		printf("Error opening client file.\n");
		return;
	}
	fwrite(&new_client, sizeof(client_user), 1, f_add);
	fclose(f_add);
	printf("Client added successfully!\n");
}


// Edit an existing client's personal information
void edit_client(client_user *client)
{
	printf("\n--- Edit Client ---\n");
	// Save old values
	char old_last_name[MAX_SIZE];
	char old_first_name[MAX_SIZE];
	strncpy(old_last_name, client->last_name, MAX_SIZE);
	strncpy(old_first_name, client->first_name, MAX_SIZE);

	printf("Current last name: %s\n", client->last_name);
	printf("Enter new last name (leave empty to keep current): ");
	char buffer[MAX_SIZE];
	fgets(buffer, MAX_SIZE, stdin);
	buffer[strcspn(buffer, "\n")] = '\0';
	if (strlen(buffer) > 0)
	{
		strncpy(client->last_name, buffer, MAX_SIZE);
	}

	printf("Current first name: %s\n", client->first_name);
	printf("Enter new first name (leave empty to keep current): ");
	fgets(buffer, MAX_SIZE, stdin);
	buffer[strcspn(buffer, "\n")] = '\0';
	if (strlen(buffer) > 0)
	{
		strncpy(client->first_name, buffer, MAX_SIZE);
	}

	printf("Current email: %s\n", client->mail);
	printf("Enter new email (leave empty to keep current): ");
	fgets(buffer, MAX_SIZE, stdin);
	buffer[strcspn(buffer, "\n")] = '\0';
	if (strlen(buffer) > 0)
	{
		strncpy(client->mail, buffer, MAX_SIZE);
	}

	// Update client in file using old values
	FILE *f = fopen(CLIENTS_DATA_FILE, "r+b");
	if (!f)
	{
		printf("Error opening client file for update.\n");
		return;
	}
	client_user c;
	while (fread(&c, sizeof(client_user), 1, f) == 1)
	{
		if (strcmp(c.last_name, old_last_name) == 0 && strcmp(c.first_name, old_first_name) == 0)
		{
			fseek(f, -sizeof(client_user), SEEK_CUR);
			fwrite(client, sizeof(client_user), 1, f);
			printf("Client information updated successfully!\n");
			fclose(f);
			return;
		}
	}
	fclose(f);
	printf("Error: client not found for update.\n");
}


// Delete a client from the database
void delete_client(client_user *client)
{
	char old_last_name[MAX_SIZE];
	char old_first_name[MAX_SIZE];
	strncpy(old_last_name, client->last_name, MAX_SIZE);
	strncpy(old_first_name, client->first_name, MAX_SIZE);

	FILE *f = fopen(CLIENTS_DATA_FILE, "rb");
	if (!f)
	{
		printf("Error opening client file for deletion.\n");
		return;
	}
	FILE *temp = fopen("clients_temp.dat", "wb");
	if (!temp)
	{
		printf("Error creating temporary file.\n");
		fclose(f);
		return;
	}
	client_user c;
	int deleted = 0;
	while (fread(&c, sizeof(client_user), 1, f) == 1)
	{
		if (strcmp(c.last_name, old_last_name) == 0 && strcmp(c.first_name, old_first_name) == 0)
		{
			deleted = 1;
			continue; // skip writing this client
		}
		fwrite(&c, sizeof(client_user), 1, temp);
	}
	fclose(f);
	fclose(temp);
	if (deleted)
	{
		remove(CLIENTS_DATA_FILE);
		rename("clients_temp.dat", CLIENTS_DATA_FILE);
		printf("Client deleted successfully!\n");
	}
	else
	{
		remove("clients_temp.dat");
		printf("Client not found for deletion.\n");
	}
}


// Client management menu
void client_management_menu()
{
	int quit = 0;
	while (!quit)
	{
		printf("\n==== Client Management Menu ====\n");
        printf("1. Show Clients\n");
        printf("2. Search Client\n");
        printf("3. Add Client\n");
        printf("4. Exit\n");
		printf("Choose an option: ");
		int choice = 0;

		scanf("%d", &choice);
		getchar(); // consume newline
		switch (choice)
		{
			case 1:
			{
				show_all_clients();
				break;
			}
			case 2:
			{
				printf("Search client selected.\n");
				char last_name[MAX_SIZE];
				char first_name[MAX_SIZE];
				printf("Enter client's last name: ");
				fgets(last_name, MAX_SIZE, stdin);
				last_name[strcspn(last_name, "\n")] = '\0';
				printf("Enter client's first name: ");
				fgets(first_name, MAX_SIZE, stdin);
				first_name[strcspn(first_name, "\n")] = '\0';
				FILE *f = fopen(CLIENTS_DATA_FILE, "rb");
				if (!f)
				{
					printf("Error opening client file.\n");
					break;
				}
				client_user c;
				int found = 0;
				while (fread(&c, sizeof(client_user), 1, f) == 1)
				{
					if (strcmp(c.last_name, last_name) == 0 && strcmp(c.first_name, first_name) == 0)
					{
						found = 1;
						printf("\n--- Client Information ---\n");
						printf("Last name: %s\n", c.last_name);
						printf("First name: %s\n", c.first_name);
						printf("Email: %s\n", c.mail);
						// TODO: Display reserved rooms for this client

						int sub_quit = 0;
						while (!sub_quit)
						{
							printf("\n--- What do you want to do? ---\n");
							printf("1. Edit client\n");
							printf("2. Delete client\n");
							printf("3. Edit reservation\n");
							printf("4. Return\n");
							printf("Choose an option: ");
							int sub_choice = 0;
							scanf("%d", &sub_choice);
							getchar();
							switch (sub_choice)
							{
								case 1:
									edit_client(&c);
									sub_quit = 1;
									break;
								case 2:
									delete_client(&c);
									sub_quit = 1;
									break;
								case 3:
									// TODO: edit_reservation(&c);
									printf("Edit reservation selected.\n");
									break;
								case 4:
									sub_quit = 1;
									printf("Returning to previous menu.\n");
									break;
								default:
									printf("Invalid option. Try again.\n");
									break;
							}
						}
						break;
					}
				}
				fclose(f);
				if (!found)
				{
					printf("Client not found.\n");
				}
				break;
			}
			case 3:
			{
				add_client();
				break;
			}
			case 4:
			{
				quit = 1;
				printf("Exiting client management menu.\n");
				break;
			}
			default:
			{
				printf("Invalid option. Try again.\n");
			}
		}
	}
}