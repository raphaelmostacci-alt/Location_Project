// Utility functions for the application
#include "includes.h"
#include "useful_fonction.h"

// Exits the application with a message
void exit_application() 
{
	printf("Exiting application. Goodbye!\n");
	exit(0);
}

// Returns to the previous menu
void return_menu()
{
	printf("Returning to previous menu...\n");
	return;
}
