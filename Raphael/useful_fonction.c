#include "includes.h"
#include "useful_fonction.h"
#include "room_management.h"
#include "client_management.h"
#include "reservation_room.h"

// Exits the application with a message
void exit_application() {
	printf("Exiting application. Goodbye!\n");
	exit(0);
}

// Returns to the previous menu
void return_menu(){
	printf("Returning to previous menu...\n");
	return;
}

void read_string(const char *prompt, char *buffer, int size) {
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

int read_int(const char *prompt) {
    char buffer[32];
    printf("%s", prompt);
    fgets(buffer, sizeof(buffer), stdin);
    return atoi(buffer);
}

void print_error(const char *msg) {
    printf("[ERROR] %s\n", msg);
}

void print_success(const char *msg) {
    printf("[SUCCESS] %s\n", msg);
}

int is_empty(const char *str) {
    return str == NULL || str[0] == '\0';
}