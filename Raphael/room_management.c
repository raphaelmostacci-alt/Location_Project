#include "includes.h"
#include "room_management.h"
#include "useful_fonction.h"
#include "reservation_room.h"

// Dynamic array for rooms
room_user *rooms = NULL;
int room_count = 0;

// Load rooms from file
void load_rooms() {
    FILE *f = fopen("rooms.dat", "rb");
    if (!f) return;
    room_user temp;
    room_count = 0;
    rooms = NULL;
    while (fread(&temp, sizeof(room_user), 1, f) == 1) {
        rooms = realloc(rooms, (room_count + 1) * sizeof(room_user));
        rooms[room_count++] = temp;
    }
    fclose(f);
}

// Save rooms to file
void save_rooms() {
    FILE *f = fopen("rooms.dat", "wb");
    if (!f) return;
    for (int i = 0; i < room_count; i++) {
        fwrite(&rooms[i], sizeof(room_user), 1, f);
    }
    fclose(f);
}

// Add a room
void add_room_dynamic(room_user new_room) {
    rooms = realloc(rooms, (room_count + 1) * sizeof(room_user));
    rooms[room_count++] = new_room;
    save_rooms();
}

// Free room memory
void free_rooms() {
    free(rooms);
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
            printf("[SUCCESS] Room ID %d is now unavailable.\n", room->id);
            found = 1;
            break;
        }
    }
    if (!found) {
        printf("[ERROR] Room not found for logical deletion.\n");
    }
}

// Edit a room's information by ID (called from search)
void edit_room_by_id(int id) {
    int found = 0;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].id == id) {
            found = 1;
            printf("Editing room: %d\nCurrent name: %s\nCurrent capacity: %d\nCurrent available: %s\n", rooms[i].id, rooms[i].name, rooms[i].capacity, rooms[i].available ? "Yes" : "No");
            printf("New name (leave empty to keep current): ");
            char new_name[ROOM_NAME_SIZE];
            fgets(new_name, ROOM_NAME_SIZE, stdin);
            size_t len = strlen(new_name);
            if (len > 0 && new_name[len - 1] == '\n') new_name[len - 1] = '\0';
            if (strlen(new_name) > 0) strncpy(rooms[i].name, new_name, ROOM_NAME_SIZE);
            printf("New capacity (enter -1 to keep current): ");
            int new_capacity;
            scanf("%d", &new_capacity);
            getchar();
            if (new_capacity != -1) rooms[i].capacity = new_capacity;
            printf("Available? (1 = Yes, 0 = No, -1 to keep current): ");
            int new_available;
            scanf("%d", &new_available);
            getchar();
            if (new_available == 0 || new_available == 1) rooms[i].available = new_available;
            save_rooms();
            printf("[SUCCESS] Room updated successfully!\n");
            break;
        }
    }
    if (!found) {
        printf("[ERROR] Room with ID %d not found for editing.\n", id);
    }
}

// Search for a room by ID and display its info
void search_room() {
    int id;
    printf("Enter the room ID to search: ");
    scanf("%d", &id);
    getchar();
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
                printf("3. Return\n");
                printf("Choose an option: ");
                int sub_choice = 0;
                scanf("%d", &sub_choice);
                getchar();
                switch (sub_choice) {
                    case 1:
                        edit_room(&rooms[i]);
                        sub_quit = 1;
                        break;
                    case 2:
                        delete_room(&rooms[i]);
                        sub_quit = 1;
                        break;
                    case 3:
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
    if (!found) {
        printf("[ERROR] Room not found in database.\n");
    }
}

// Edit an existing room's information
void edit_room(room_user *room) {
    printf("\n--- Edit Room ---\n");
    char old_name[ROOM_NAME_SIZE];
    strncpy(old_name, room->name, ROOM_NAME_SIZE);
    int old_capacity = room->capacity;
    int old_available = room->available;

    printf("Current name: %s\nEnter new name (leave empty to keep current): ", room->name);
    char buffer[ROOM_NAME_SIZE];
    fgets(buffer, ROOM_NAME_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0) {
        strncpy(room->name, buffer, ROOM_NAME_SIZE);
    }
    printf("Current capacity: %d\nEnter new capacity (-1 to keep current): ", room->capacity);
    int new_capacity;
    scanf("%d", &new_capacity);
    getchar();
    if (new_capacity != -1) {
        room->capacity = new_capacity;
    }
    printf("Current available: %s\nEnter new available (1 = Yes, 0 = No, -1 to keep current): ", room->available ? "Yes" : "No");
    int new_available;
    scanf("%d", &new_available);
    getchar();
    if (new_available == 0 || new_available == 1) {
        room->available = new_available;
    }
    save_rooms();
    printf("Room information updated successfully!\n");
}

// Show all rooms in the database
void show_all_rooms() {
    printf("\n--- All Registered Rooms ---\n");
    int count = 0;
    for (int i = 0; i < room_count; i++) {
        printf("%d. ID: %d | Name: %s | Capacity: %d | Available: %s\n", ++count, rooms[i].id, rooms[i].name, rooms[i].capacity, rooms[i].available ? "Yes" : "No");
    }
    if (count == 0) {
        printf("[INFO] No rooms found in the database.\n");
    }
}

// Add a new room
void add_room() {
    room_user new_room;
    printf("\n--- Add a new room ---\n");
    printf("Room ID: ");
    scanf("%d", &new_room.id);
    getchar();
    printf("Room name: ");
    fgets(new_room.name, ROOM_NAME_SIZE, stdin);
    size_t len = strlen(new_room.name);
    if (len > 0 && new_room.name[len - 1] == '\n') new_room.name[len - 1] = '\0';
    printf("Room capacity: ");
    scanf("%d", &new_room.capacity);
    getchar();
    printf("Available? (1 = Yes, 0 = No): ");
    scanf("%d", &new_room.available);
    getchar();
    // Check for unique ID
    int id_exists = 0;
    for (int i = 0; i < room_count; i++) {
        if (rooms[i].id == new_room.id) {
            id_exists = 1;
            break;
        }
    }
    if (id_exists) {
        printf("[ERROR] A room with this ID already exists. Room not added.\n");
        return;
    }
    add_room_dynamic(new_room);
    printf("[SUCCESS] Room added successfully!\n");
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
        printf("4. Return to previous menu\n");
        printf("5. Exit Application\n");
        printf("Choose an option: ");
        int choice = 0;
        scanf("%d", &choice);
        getchar();
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
            case 4:
                quit = 1;
                printf("Returning to previous menu.\n");
                break;
            case 5:
                quit = 1;
                printf("Exiting room management menu.\n");
                free_rooms();
                exit_application();
                break;
            default:
                printf("Invalid option. Try again.\n");
                break;
        }
    }
    free_rooms();
}
