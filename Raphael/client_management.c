#define MAX_SIZE 100
#include "includes.h"
#include "client_management.h"
#include "useful_fonction.h"
#include "reservation_room.h"

// Edit a client by last name and first name
void edit_client() {
    char last_name[MAX_SIZE];
    char first_name[MAX_SIZE];
    printf("Enter client last name to edit: ");
    fgets(last_name, MAX_SIZE, stdin);
    last_name[strcspn(last_name, "\n")] = '\0';
    printf("Enter client first name to edit: ");
    fgets(first_name, MAX_SIZE, stdin);
    first_name[strcspn(first_name, "\n")] = '\0';
    int found = 0;
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].last_name, last_name) == 0 && strcmp(clients[i].first_name, first_name) == 0 && clients[i].available) {
            found = 1;
            printf("Editing client: %s %s\nCurrent email: %s\n", clients[i].last_name, clients[i].first_name, clients[i].mail);
            printf("New last name (leave empty to keep current): ");
            char new_last[MAX_SIZE];
            fgets(new_last, MAX_SIZE, stdin);
            new_last[strcspn(new_last, "\n")] = '\0';
            if (strlen(new_last) > 0) strncpy(clients[i].last_name, new_last, MAX_SIZE);
            printf("New first name (leave empty to keep current): ");
            char new_first[MAX_SIZE];
            fgets(new_first, MAX_SIZE, stdin);
            new_first[strcspn(new_first, "\n")] = '\0';
            if (strlen(new_first) > 0) strncpy(clients[i].first_name, new_first, MAX_SIZE);
            printf("New email (leave empty to keep current): ");
            char new_mail[MAX_SIZE];
            fgets(new_mail, MAX_SIZE, stdin);
            new_mail[strcspn(new_mail, "\n")] = '\0';
            if (strlen(new_mail) > 0) strncpy(clients[i].mail, new_mail, MAX_SIZE);
            save_clients();
            printf("[SUCCESS] Client updated successfully!\n");
            return;
        }
    }
    if (!found) {
        printf("[ERROR] Client not found for editing.\n");
    }
}

// Delete (deactivate) a client by last name and first name
void delete_client() {
    char last_name[MAX_SIZE];
    char first_name[MAX_SIZE];
    printf("Enter client last name to delete: ");
    fgets(last_name, MAX_SIZE, stdin);
    last_name[strcspn(last_name, "\n")] = '\0';
    printf("Enter client first name to delete: ");
    fgets(first_name, MAX_SIZE, stdin);
    first_name[strcspn(first_name, "\n")] = '\0';
    int found = 0;
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].last_name, last_name) == 0 && strcmp(clients[i].first_name, first_name) == 0 && clients[i].available) {
            clients[i].available = 0;
            save_clients();
            printf("[SUCCESS] Client deactivated (logical delete).\n");
            found = 1;
            return;
        }
    }
    if (!found) {
        printf("[ERROR] Client not found for deletion.\n");
    }
}

// Main menu for client management
void client_management_menu() {
    int quit = 0;
    load_clients();
    while (!quit) {
        printf("\n==== Client Management Menu ====\n");
        printf("1. Show all clients\n");
        printf("2. Search for a client\n");
        printf("3. Add a client\n");
        printf("4. Return to previous menu\n");
        printf("5. Exit Application\n");
        printf("Choose an option: ");
        int choice = 0;
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                show_all_clients();
                break;
            case 2: {
                char last_name[MAX_SIZE];
                char first_name[MAX_SIZE];
                printf("Enter client last name: ");
                fgets(last_name, MAX_SIZE, stdin);
                last_name[strcspn(last_name, "\n")] = '\0';
                printf("Enter client first name: ");
                fgets(first_name, MAX_SIZE, stdin);
                first_name[strcspn(first_name, "\n")] = '\0';
                int found = -1;
                for (int i = 0; i < client_count; i++) {
                    if (strcmp(clients[i].last_name, last_name) == 0 && strcmp(clients[i].first_name, first_name) == 0) {
                        found = i;
                        printf("\n--- Client Information ---\n");
                        printf("Last name: %s\n", clients[i].last_name);
                        printf("First name: %s\n", clients[i].first_name);
                        printf("Email: %s\n", clients[i].mail);
                        if (!clients[i].available) {
                            printf("[INFO] This client is DEACTIVATED (unavailable).\n");
                        }
                        break;
                    }
                }
                if (found == -1) {
                    printf("[ERROR] Client not found in the database.\n");
                } else {
                    int sub_quit = 0;
                    while (!sub_quit) {
                        printf("\n--- What do you want to do with this client? ---\n");
                        printf("1. Edit client\n");
                        printf("2. Deactivate client\n");
                        printf("3. Show rooms reserved by this client\n");
                        printf("4. Return\n");
                        printf("Choose an option: ");
                        int sub_choice = 0;
                        scanf("%d", &sub_choice);
                        getchar();
                        switch (sub_choice) {
                            case 1:
                                printf("Editing client...\n");
                                printf("New last name (leave empty to keep current): ");
                                char new_last[MAX_SIZE];
                                fgets(new_last, MAX_SIZE, stdin);
                                new_last[strcspn(new_last, "\n")] = '\0';
                                if (strlen(new_last) > 0) strncpy(clients[found].last_name, new_last, MAX_SIZE);
                                printf("New first name (leave empty to keep current): ");
                                char new_first[MAX_SIZE];
                                fgets(new_first, MAX_SIZE, stdin);
                                new_first[strcspn(new_first, "\n")] = '\0';
                                if (strlen(new_first) > 0) strncpy(clients[found].first_name, new_first, MAX_SIZE);
                                printf("New email (leave empty to keep current): ");
                                char new_mail[MAX_SIZE];
                                fgets(new_mail, MAX_SIZE, stdin);
                                new_mail[strcspn(new_mail, "\n")] = '\0';
                                if (strlen(new_mail) > 0) strncpy(clients[found].mail, new_mail, MAX_SIZE);
                                save_clients();
                                printf("[SUCCESS] Client updated successfully!\n");
                                break;
                            case 2:
                                if (clients[found].available) {
                                    clients[found].available = 0;
                                    save_clients();
                                    invalidate_reservations_by_client(clients[found].mail);
                                    printf("[SUCCESS] Client deactivated (logical delete) and reservations released.\n");
                                } else {
                                    printf("[INFO] Client is already deactivated.\n");
                                }
                                break;
                            case 3: {
                                // Show rooms reserved by this client
                                extern void show_rooms_reserved_by_client(const char *client_mail);
                                show_rooms_reserved_by_client(clients[found].mail);
                                break;
                            }
                            case 4:
                                sub_quit = 1;
                                printf("Returning to previous menu.\n");
                                break;
                            default:
                                printf("Invalid option. Try again.\n");
                                break;
                        }
                    }
                }
                break;
            }
            case 3:
                add_client();
                break;
            case 4:
                quit = 1;
                printf("Returning to previous menu.\n");
                return_menu();
                break;
            case 5:
                quit = 1;
                printf("Exiting client management menu.\n");
                free_clients();
                exit_application();
                break;
            default:
                printf("Invalid option. Please try again.\n");
                break;
        }
    }
    free_clients();
}



// Dynamic array for clients
client_user *clients = NULL;
int client_count = 0;

// Load clients from file
void load_clients() 
{
    FILE *f = fopen("clients.dat", "rb");
    if (!f) return;
    client_user temp;
    client_count = 0;
    clients = NULL;
    while (fread(&temp, sizeof(client_user), 1, f) == 1) {
        clients = realloc(clients, (client_count + 1) * sizeof(client_user));
        clients[client_count++] = temp;
    }
    fclose(f);
}



// Save clients to file
void save_clients() {
    FILE *f = fopen("clients.dat", "wb");
    if (!f) return;
    for (int i = 0; i < client_count; i++) {
        fwrite(&clients[i], sizeof(client_user), 1, f);
    }
    fclose(f);
}

// Free memory for clients
void free_clients() {
    free(clients);
    clients = NULL;
    client_count = 0;
}

// Show all clients (deactivated first)
void show_all_clients() {
    printf("\n--- All registered clients ---\n");
    int count = 0;
    // First, show deactivated clients
    for (int i = 0; i < client_count; i++) {
        if (!clients[i].available) {
            printf("%d. [DEACTIVATED] Last name: %s | First name: %s | Email: %s\n", ++count, clients[i].last_name, clients[i].first_name, clients[i].mail);
        }
    }
    // Then, show active clients
    for (int i = 0; i < client_count; i++) {
        if (clients[i].available) {
            printf("%d. Last name: %s | First name: %s | Email: %s\n", ++count, clients[i].last_name, clients[i].first_name, clients[i].mail);
        }
    }
    if (count == 0) {
        printf("[INFO] No clients found in the database.\n");
    }
}

// Search for a client by last name and first name
int search_client(const char *last_name, const char *first_name) {
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].last_name, last_name) == 0 && strcmp(clients[i].first_name, first_name) == 0 && clients[i].available) {
            return 1;
        }
    }
    return 0;
}


// Add a new client
void add_client() {
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
    if(strlen(new_client.last_name) == 0 || strlen(new_client.first_name) == 0 || strlen(new_client.mail) == 0) {
        printf("[ERROR] All fields must be filled.\n");
        return;
    }
    // Check if the client already exists
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].last_name, new_client.last_name) == 0 && strcmp(clients[i].first_name, new_client.first_name) == 0) {
            if (clients[i].available) {
                printf("[ERROR] A client with this name already exists and is active. Client not added.\n");
                return;
            } else {
                clients[i].available = 1;
                strncpy(clients[i].mail, new_client.mail, MAX_SIZE);
                save_clients();
                printf("[SUCCESS] Old deactivated client reactivated and updated!\n");
                return;
            }
        }
    }
    new_client.available = 1;
    clients = realloc(clients, (client_count + 1) * sizeof(client_user));
    clients[client_count++] = new_client;
    save_clients();
    printf("[SUCCESS] Client added successfully!\n");
}

// Show all rooms reserved by a specific client (with room name, date, time, only active reservations)
extern const char* get_room_name(int room_id);
void show_rooms_reserved_by_client(const char *client_mail) {
    load_reservations();
    printf("\n-----------------------------------\n");
    printf("--- Reservations for client: %s ---\n", client_mail);
    printf("-----------------------------------\n");
    int count = 0;
    printf("%-3s %-20s %-16s\n", "No.", "Room", "Date & Time");
    printf("%-45s\n", "---------------------------------------------");
    for (int i = 0; i < reservation_count; i++) {
        if (strcmp(reservations[i].client_mail, client_mail) == 0 && reservations[i].reservation_id != -1) {
            printf("%-3d %-20s %02d/%02d/%04d %02d:00\n",
                   count + 1,
                   get_room_name(reservations[i].room_id),
                   reservations[i].date.day,
                   reservations[i].date.month,
                   reservations[i].date.year,
                   reservations[i].date.hour);
            count++;
        }
    }
    if (count == 0) {
        printf("No reservations found for this client.\n");
    }
    free_reservations();
    return_menu();
}