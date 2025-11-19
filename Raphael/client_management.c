#define MAX_SIZE 100
#include "includes.h"
#include "client_management.h"
#include "useful_fonction.h"
#include "reservation_room.h"
#include "useful_mutual.h"

// Edit a client by last name and first name
void edit_client() {
    char last_name[MAX_SIZE];
    char first_name[MAX_SIZE];
    read_string("Enter client last name to edit: ", last_name, MAX_SIZE);
    read_string("Enter client first name to edit: ", first_name, MAX_SIZE);
    int found = 0;
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].last_name, last_name) == 0 && strcmp(clients[i].first_name, first_name) == 0 && clients[i].available) {
            found = 1;
            printf("Editing client: %s %s\nCurrent email: %s\n", clients[i].last_name, clients[i].first_name, clients[i].mail);
            char new_last[MAX_SIZE];
            read_string("New last name (leave empty to keep current): ", new_last, MAX_SIZE);
            if (strlen(new_last) > 0) strncpy(clients[i].last_name, new_last, MAX_SIZE);
            char new_first[MAX_SIZE];
            read_string("New first name (leave empty to keep current): ", new_first, MAX_SIZE);
            if (strlen(new_first) > 0) strncpy(clients[i].first_name, new_first, MAX_SIZE);
            char new_mail[MAX_SIZE];
            read_string("New email (leave empty to keep current): ", new_mail, MAX_SIZE);
            if (strlen(new_mail) > 0) strncpy(clients[i].mail, new_mail, MAX_SIZE);
            save_clients();
            print_success("Client updated successfully!");
            return;
        }
    }
    if (!found) {
        print_error("Client not found for editing.");
    }
}

// Delete (deactivate) a client by last name and first name
void delete_client() {
    char last_name[MAX_SIZE];
    char first_name[MAX_SIZE];
    read_string("Enter client last name to delete: ", last_name, MAX_SIZE);
    read_string("Enter client first name to delete: ", first_name, MAX_SIZE);
    int found = 0;
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].last_name, last_name) == 0 && strcmp(clients[i].first_name, first_name) == 0 && clients[i].available) {
            clients[i].available = 0;
            save_clients();
            print_success("Client deactivated (logical delete).");
            found = 1;
            return;
        }
    }
    if (!found) {
        print_error("Client not found for deletion.");
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
        printf("8. Return to previous menu\n");
        printf("9. Exit Application\n");
        int choice = read_int("Choose an option: ");
        switch (choice) {
            case 1:
                show_all_clients();
                break;
            case 2: {
                char last_name[MAX_SIZE];
                char first_name[MAX_SIZE];
                read_string("Enter client last name: ", last_name, MAX_SIZE);
                read_string("Enter client first name: ", first_name, MAX_SIZE);
                int found = -1;
                for (int i = 0; i < client_count; i++) {
                    if (strcmp(clients[i].last_name, last_name) == 0 && strcmp(clients[i].first_name, first_name) == 0) {
                        found = i;
                        printf("\n--- Client Information ---\n");
                        printf("Last name: %s\n", clients[i].last_name);
                        printf("First name: %s\n", clients[i].first_name);
                        printf("Email: %s\n", clients[i].mail);
                        if (!clients[i].available) {
                            print_error("This client is DEACTIVATED (unavailable).");
                        }
                        break;
                    }
                }
                if (found == -1) {
                    print_error("Client not found in the database.");
                } else {
                    int sub_quit = 0;
                    while (!sub_quit) {
                        printf("\n--- What do you want to do with this client? ---\n");
                        printf("1. Edit client\n");
                        printf("2. Deactivate client\n");
                        printf("3. Show rooms reserved by this client\n");
                        printf("8. Return to previous menu\n");
                        printf("9. Exit Application\n");
                        int sub_choice = read_int("Choose an option: ");
                        switch (sub_choice) {
                            case 1:
                                printf("Editing client...\n");
                                char new_last[MAX_SIZE];
                                read_string("New last name (leave empty to keep current): ", new_last, MAX_SIZE);
                                if (strlen(new_last) > 0) strncpy(clients[found].last_name, new_last, MAX_SIZE);
                                char new_first[MAX_SIZE];
                                read_string("New first name (leave empty to keep current): ", new_first, MAX_SIZE);
                                if (strlen(new_first) > 0) strncpy(clients[found].first_name, new_first, MAX_SIZE);
                                char new_mail[MAX_SIZE];
                                read_string("New email (leave empty to keep current): ", new_mail, MAX_SIZE);
                                if (strlen(new_mail) > 0) strncpy(clients[found].mail, new_mail, MAX_SIZE);
                                save_clients();
                                print_success("Client updated successfully!");
                                break;
                            case 2:
                                if (clients[found].available) {
                                    clients[found].available = 0;
                                    save_clients();
                                    invalidate_reservations_by_client(clients[found].mail);
                                    print_success("Client deactivated (logical delete) and reservations released.");
                                } else {
                                    print_error("Client is already deactivated.");
                                }
                                break;
                            case 3: {
                                // Show rooms reserved by this client
                                extern void show_rooms_reserved_by_client(const char *client_mail);
                                show_rooms_reserved_by_client(clients[found].mail);
                                break;
                            }
                            case 8:
                                sub_quit = 1;
                                printf("Returning to previous menu.\n");
                                break;
                            case 9:
                                free_clients();
                                exit_application();
                                return;
                            default:
                                print_error("Invalid option. Try again.");
                                break;
                        }
                    }
                }
                break;
            }
            case 3:
                add_client();
                break;
            case 8:
                quit = 1;
                printf("Returning to previous menu.\n");
                return_menu();
                break;
            case 9:
                quit = 1;
                printf("Exiting client management menu.\n");
                free_clients();
                exit_application();
                break;
            default:
                print_error("Invalid option. Please try again.");
                break;
        }
    }
    free_clients();
}

// Dynamic array for clients
client_user *clients = NULL;
int client_count = 0;

// Load clients from file
void load_clients() {
    clients = (client_user*)mutual_load("clients.dat", sizeof(client_user), &client_count);
}



// Save clients to file
void save_clients() {
    mutual_save("clients.dat", clients, sizeof(client_user), client_count);
}

// Free memory for clients
void free_clients() {
    mutual_free(clients);
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
        print_error("No clients found in the database.");
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
    read_string("Last name: ", new_client.last_name, MAX_SIZE);
    read_string("First name: ", new_client.first_name, MAX_SIZE);
    read_string("Email: ", new_client.mail, MAX_SIZE);
    if(is_empty(new_client.last_name) || is_empty(new_client.first_name) || is_empty(new_client.mail)) {
        print_error("All fields must be filled.");
        return;
    }
    // Check if the client already exists
    for (int i = 0; i < client_count; i++) {
        if (strcmp(clients[i].last_name, new_client.last_name) == 0 && strcmp(clients[i].first_name, new_client.first_name) == 0) {
            if (clients[i].available) {
                print_error("A client with this name already exists and is active. Client not added.");
                return;
            } else {
                clients[i].available = 1;
                strncpy(clients[i].mail, new_client.mail, MAX_SIZE);
                save_clients();
                print_success("Old deactivated client reactivated and updated!");
                return;
            }
        }
    }
    new_client.available = 1;
    clients = realloc(clients, (client_count + 1) * sizeof(client_user));
    clients[client_count++] = new_client;
    save_clients();
    print_success("Client added successfully!");
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
        print_error("No reservations found for this client.");
    }
    free_reservations();
    return_menu();
}