

#include "includes.h"
#include "client_management.h"

// Authentication menu function
void account_authentification()
{
	int authenticated = 0;
	int quit = 0;
	while (!authenticated && !quit)
	{
		char last_name[TAILLE_MAX];
		char first_name[TAILLE_MAX];

		printf("==== Meeting Room Login ====\n");
		printf("Enter your last name: ");
		fgets(last_name, TAILLE_MAX, stdin);
		last_name[strcspn(last_name, "\n")] = 0;

		printf("Enter your first name: ");
		fgets(first_name, TAILLE_MAX, stdin);
		first_name[strcspn(first_name, "\n")] = 0;

		// Check if the user exists in clients.dat
		int user_exists = search_client(last_name, first_name);

		if (user_exists)
		{
			printf("Welcome, %s %s!\n", first_name, last_name);
			authenticated = 1;
			// TODO: call admin or user menu depending on the name
		}
		else
		{
			printf("Account not found.\n");
			printf("1. Create a new account\n2. Try again\n3. Quit\nChoose an option: ");
			int choice = 0;
			scanf("%d", &choice);
			getchar(); // consume newline
			if (choice == 1)
			{
				add_client();
				printf("Account created. Please login again.\n");
			}
			else if (choice == 3)
			{
				quit = 1;
				printf("Exiting application.\n");
			}
		}
	}
}

int main()
{
	account_authentification();
	// ... menu logic depending on user type ...
	return 0;
}
