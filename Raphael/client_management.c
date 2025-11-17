// ...existing code...
#include "includes.h"
#include "client_management.h"
#include "useful_fonction.h"
#define CLIENTS_DATA_FILE "clients.dat"


// Show all clients in the database
void show_all_clients()
{
	FILE *f = fopen(CLIENTS_DATA_FILE, "rb");
	if (!f)
	{
		printf("[ERROR] Unable to open clients.dat for listing.\n");
		return;
	}
	client_user c;
	client_user disabled_clients[1000];
	int disabled_count = 0;
	client_user enabled_clients[1000];
	int enabled_count = 0;
	printf("\n--- All Registered Clients ---\n");
	while (fread(&c, sizeof(client_user), 1, f) == 1)
	{
		if (!c.available)
		{
			disabled_clients[disabled_count++] = c;
		}
		else
		{
			enabled_clients[enabled_count++] = c;
		}
	}
	fclose(f);
	int count = 0;
	for (int i = 0; i < disabled_count; i++)
	{
		printf("%d. [DISABLED] Last name: %s | First name: %s | Email: %s\n", ++count, disabled_clients[i].last_name, disabled_clients[i].first_name, disabled_clients[i].mail);
	}
	for (int i = 0; i < enabled_count; i++)
	{
		printf("%d. Last name: %s | First name: %s | Email: %s\n", ++count, enabled_clients[i].last_name, enabled_clients[i].first_name, enabled_clients[i].mail);
	}
	if (count == 0)
	{
		printf("[INFO] No clients found in the database.\n");
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
		if (strcmp(c.last_name, last_name) == 0 && strcmp(c.first_name, first_name) == 0 && c.available) 
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
	printf("\n--- Add a new client ---\n");
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
		printf("[ERROR] All fields must be filled.\n");
		return;
	}

	// Check for existing client (by name)
	int found_disabled = 0;
	FILE *f_check = fopen(CLIENTS_DATA_FILE, "r+b");
	if (f_check)
	{
		client_user c;
		long pos = 0;
		while (fread(&c, sizeof(client_user), 1, f_check) == 1)
		{
			if (strcmp(c.last_name, new_client.last_name) == 0 && strcmp(c.first_name, new_client.first_name) == 0)
			{
				if (c.available) 
				{
					printf("[ERROR] A client with this name already exists and is enabled. Client not added.\n");
					fclose(f_check);
					return;
				} 
				else 
				{
					// Reactivate the disabled client
					found_disabled = 1;
					c.available = 1;
					strncpy(c.mail, new_client.mail, MAX_SIZE);
					fseek(f_check, pos, SEEK_SET);
					fwrite(&c, sizeof(client_user), 1, f_check);
					printf("[SUCCESS] Existing disabled client re-enabled and updated!\n");
					fclose(f_check);
					return;
				}
			}
			pos += sizeof(client_user);
		}
		fclose(f_check);
	}

	// Add to the data file
	new_client.available = 1; // Mark as available
	FILE *f_add = fopen(CLIENTS_DATA_FILE, "ab");
	if (!f_add)
	{
		printf("[ERROR] Unable to open clients.dat for adding new client.\n");
		return;
	}
	fwrite(&new_client, sizeof(client_user), 1, f_add);
	fclose(f_add);
	printf("[SUCCESS] Client added successfully!\n");
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
		printf("[ERROR] Unable to open clients.dat for client update.\n");
		return;
	}
	client_user c;
	while (fread(&c, sizeof(client_user), 1, f) == 1)
	{
		if (strcmp(c.last_name, old_last_name) == 0 && strcmp(c.first_name, old_first_name) == 0 && c.available)
		{
			fseek(f, -sizeof(client_user), SEEK_CUR);
			fwrite(client, sizeof(client_user), 1, f);
			printf("[SUCCESS] Client information updated successfully!\n");
			fclose(f);
			return;
		}
	}
	fclose(f);
	printf("[ERROR] Client not found for update.\n");
}


// Delete a client from the database
void delete_client(client_user *client)
{
	char old_last_name[MAX_SIZE];
	char old_first_name[MAX_SIZE];
	strncpy(old_last_name, client->last_name, MAX_SIZE);
	strncpy(old_first_name, client->first_name, MAX_SIZE);

	FILE *f = fopen(CLIENTS_DATA_FILE, "r+b");
	if (!f)
	{
		printf("[ERROR] Unable to open clients.dat for client logical deletion.\n");
		return;
	}
	client_user c;
	int deleted = 0;
	while (fread(&c, sizeof(client_user), 1, f) == 1)
	{
		if (strcmp(c.last_name, old_last_name) == 0 && strcmp(c.first_name, old_first_name) == 0 && c.available)
		{
			c.available = 0; // Mark as unavailable
			fseek(f, -sizeof(client_user), SEEK_CUR);
			fwrite(&c, sizeof(client_user), 1, f);
			deleted = 1;
			printf("[SUCCESS] Client logically deleted (unavailable)!\n");
			// TODO: When integrating with reservation_room, invalidate reservations for this client here.
			break;
		}
	}
	fclose(f);
	if (!deleted)
	{
		printf("[ERROR] Client not found for logical deletion.\n");
	}
}


// Client management menu
void client_management_menu()
{
	int quit = 0;
	while (!quit)
	{
		printf("\n==== Client Management Menu ====\n");
		printf("1. Show all clients\n");
		printf("2. Search client\n");
		printf("3. Add client\n");
		printf("4. Return to previous menu\n");
		printf("5. Exit Application\n");
		printf("Choose an option: ");
		int choice = 0;

		scanf("%d", &choice);
		getchar(); // consume newline
		switch (choice)
		{
			case 1:
				show_all_clients();
				break;
			case 2:
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
					printf("[ERROR] Unable to open clients.dat for searching.\n");
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
						if (!c.available) {
							printf("[INFO] This client is DISABLED (unavailable).\n");
						}
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
					printf("[ERROR] Client not found in database.\n");
				}
				break;
			case 3:
				add_client();
				break;
			case 4:
				quit = 1;
				printf("Returning to previous menu.\n");
				break;
			case 5:
				quit = 1;
				printf("Exiting client management menu.\n");
				exit_application();
				break;
			default:
				printf("Invalid option. Try again.\n");
				break;
		}
	}
}