#include "includes.h"
#include "room_management.h"
#include "useful_fonction.h"
#include "reservation_room.h"
#include "useful_mutual.h"

// Dynamic array for rooms
room_user *rooms = NULL;
int room_count = 0;

// Load rooms from file
void load_rooms() {
    rooms = (room_user*)mutual_load("rooms.dat", sizeof(room_user), &room_count);
}

// Save rooms to file
void save_rooms() {
    mutual_save("rooms.dat", rooms, sizeof(room_user), room_count);
}

// Add a room
void add_room_dynamic(room_user new_room) {
    rooms = realloc(rooms, (room_count + 1) * sizeof(room_user));
    rooms[room_count++] = new_room;
    save_rooms();
}

// Free room memory
void free_rooms() {
    mutual_free(rooms);
    rooms = NULL;
    room_count = 0;
}

// Make a room unavailable (logical delete)
void delete_room(room_user *room) {
    int found = 0;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].id == room->id) {
            rooms[i].available = 0;
            invalidate_reservations_by_room(room->id);
            save_rooms();
            print_success("Room is now unavailable.");
            found = 1;
            break;
        }
    }
    if (!found) {
        print_error("Room not found for logical deletion.");
    }
}

// Edit a room's information by ID (called from search)
void edit_room_by_id(int id) {
    int found = 0;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].id == id) {
            found = 1;
            printf("Editing room: %d\nCurrent name: %s\nCurrent capacity: %d\nCurrent available: %s\n", rooms[i].id, rooms[i].name, rooms[i].capacity, rooms[i].available ? "Yes" : "No");
            char new_name[ROOM_NAME_SIZE];
            read_string("New name (leave empty to keep current): ", new_name, ROOM_NAME_SIZE);
            if (strlen(new_name) > 0) strncpy(rooms[i].name, new_name, ROOM_NAME_SIZE);
            int new_capacity = read_int("New capacity (enter -1 to keep current): ");
            if (new_capacity != -1) rooms[i].capacity = new_capacity;
            int new_available = read_int("Available? (1 = Yes, 0 = No, -1 to keep current): ");
            if (new_available == 0 || new_available == 1) rooms[i].available = new_available;
            save_rooms();
            print_success("Room updated successfully!");
            break;
        }
    }
    if (!found) {
        print_error("Room not found for editing.");
    }
}

// Search for a room by ID and display its info
void search_room() {
    int id = read_int("Enter the room ID to search: ");
    int found = 0;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].id == id) {
            found = 1;
            printf("\n--- Room Information ---\n");
            printf("ID: %d\nName: %s\nCapacity: %d\nAvailable: %s\n", rooms[i].id, rooms[i].name, rooms[i].capacity, rooms[i].available ? "Yes" : "No");
            int sub_quit = 0;
            while (!sub_quit) {
                printf("\n--- What do you want to do? ---\n");
                printf("1. Edit room\n");
                printf("2. Make room unavailable\n");
                printf("8. Return to previous menu\n");
                printf("9. Exit Application\n");
                int sub_choice = read_int("Choose an option: ");
                switch (sub_choice) {
                    case 1:
                        edit_room(&rooms[i]);
                        sub_quit = 1;
                        break;
                    case 2:
                        delete_room(&rooms[i]);
                        sub_quit = 1;
                        break;
                    case 8:
                        sub_quit = 1;
                        print_success("Returning to previous menu.");
                        break;
                    case 9:
                        free_rooms();
                        exit_application();
                        return;
                    default:
                        printf("Invalid option. Try again.\n");
                        break;
                }
            }
            break;
        }
    }
    if (!found) {
        print_error("Room not found in database.");
    }
}

// Edit an existing room's information
void edit_room(room_user *room) {
    printf("\n--- Edit Room ---\n");
    char old_name[ROOM_NAME_SIZE];
    strncpy(old_name, room->name, ROOM_NAME_SIZE);
    printf("Current name: %s\n", room->name);
    char buffer[ROOM_NAME_SIZE];
    read_string("Enter new name (leave empty to keep current): ", buffer, ROOM_NAME_SIZE);
    if (strlen(buffer) > 0) {
        strncpy(room->name, buffer, ROOM_NAME_SIZE);
    }
    printf("Current capacity: %d\n", room->capacity);
    int new_capacity = read_int("Enter new capacity (-1 to keep current): ");
    if (new_capacity != -1) {
        room->capacity = new_capacity;
    }
    printf("Current available: %s\n", room->available ? "Yes" : "No");
    int new_available = read_int("Enter new available (1 = Yes, 0 = No, -1 to keep current): ");
    if (new_available == 0 || new_available == 1) {
        room->available = new_available;
    }
    save_rooms();
    print_success("Room information updated successfully!");
}

// Show all rooms in the database
void show_all_rooms() {
    printf("\n--- All Registered Rooms ---\n");
    int count = 0;
    for (int i = 0; i < room_count; i++) {
        printf("%d. ID: %d | Name: %s | Capacity: %d | Available: %s\n", ++count, rooms[i].id, rooms[i].name, rooms[i].capacity, rooms[i].available ? "Yes" : "No");
    }
    if (count == 0) {
        print_error("No rooms found in the database.");
    }
}

// Add a new room
void add_room() {
    room_user new_room;
    printf("\n--- Add a new room ---\n");
    new_room.id = read_int("Room ID: ");
    read_string("Room name: ", new_room.name, ROOM_NAME_SIZE);
    new_room.capacity = read_int("Room capacity: ");
    new_room.available = read_int("Available? (1 = Yes, 0 = No): ");
    if (new_room.id < 0 || is_empty(new_room.name) || new_room.capacity < 0 || (new_room.available != 0 && new_room.available != 1)) {
        print_error("All fields must be filled correctly.");
        return;
    }
    // Check for unique ID
    int id_exists = 0;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].id == new_room.id) {
            id_exists = 1;
            break;
        }
    }
    if (id_exists) {
        print_error("A room with this ID already exists. Room not added.");
        return;
    }
    add_room_dynamic(new_room);
    print_success("Room added successfully!");
}

void room_management_menu()
{
    int quit = 0;
    load_rooms();
    while (!quit) {
        printf("\n==== Room Management Menu ====\n");
        printf("1. Show all rooms\n");
        printf("2. Search room\n");
        printf("3. Add room\n");
        printf("8. Return to previous menu\n");
        printf("9. Exit Application\n");
        int choice = read_int("Choose an option: ");
        switch (choice) {
            case 1:
                show_all_rooms();
                break;
            case 2:
                search_room();
                break;
            case 3:
                add_room();
                break;
            case 8:
                quit = 1;
                print_success("Returning to previous menu.");
                break;
            case 9:
                quit = 1;
                print_success("Exiting room management menu.");
                free_rooms();
                exit_application();
                break;
            default:
                print_error("Invalid option. Try again.");
                break;
        }
    }
    free_rooms();
}
