

#include "includes.h"

// #include "client_management.h" // Commenté pour simplifier la validation du menu

void account_authentification();


int main()
{
	account_authentification();
	// ... menu logic depending on user type ...
	return 0;
}




// Authentication menu function
void account_authentification()
{
	int authenticated = 0;
	int quit = 0;
	while (!authenticated && !quit)
	{
		char last_name[50]; // Taille réduite pour test
		char first_name[50];

		printf("==== Meeting Room Login ====\n");
		printf("Enter your last name: ");
		fgets(last_name, 50, stdin);
		last_name[strcspn(last_name, "\n")] = 0;

		printf("Enter your first name: ");
		fgets(first_name, 50, stdin);
		first_name[strcspn(first_name, "\n")] = 0;

		// Simulation de la recherche utilisateur (remplacer par search_client plus tard)
		int user_exists = 0;
		if ((strcmp(last_name, "BLACHERE") == 0 && strcmp(first_name, "Romain") == 0) ||
			(strcmp(last_name, "MOSTACCI") == 0 && strcmp(first_name, "Raphael") == 0))
		{
			user_exists = 1;
		}

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
				// add_client(); // Commenté pour test
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