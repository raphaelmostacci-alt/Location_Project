#include <stdio.h>
#include <string.h>
#include "room_management.h"
#include "includes.h"
#define ROOMS_DATA_FILE "rooms.dat"


// Make a room unavailable (logical delete)
void delete_room(room_user *room)
{
    FILE *f = fopen(ROOMS_DATA_FILE, "r+b");
    if (!f)
    {
        printf("Error opening rooms.dat file for update.\n");
        return;
    }
    room_user r;
    while (fread(&r, sizeof(room_user), 1, f) == 1)
    {
        if (r.id == room->id)
        {
            r.available = 0;
            fseek(f, -sizeof(room_user), SEEK_CUR);
            fwrite(&r, sizeof(room_user), 1, f);
            printf("Room ID %d is now unavailable.\n", room->id);
            fclose(f);
            return;
        }
    }
    fclose(f);
    printf("Room not found for deletion.\n");
}

// Edit a room's information by ID (called from search)
void edit_room_by_id(int id)
{
    FILE *f = fopen(ROOMS_DATA_FILE, "r+b");
    if (!f)
    {
        printf("Error opening rooms.dat file.\n");
        return;
    }
    room_user r;
    int found = 0;
    while (fread(&r, sizeof(room_user), 1, f) == 1)
    {
        if (r.id == id)
        {
            found = 1;
            printf("Editing room: %d\nCurrent name: %s\nCurrent capacity: %d\nCurrent available: %s\n", r.id, r.name, r.capacity, r.available ? "Yes" : "No");
            printf("New name (leave empty to keep current): ");
            char new_name[ROOM_NAME_SIZE];
            fgets(new_name, ROOM_NAME_SIZE, stdin);
            size_t len = strlen(new_name);
            if (len > 0 && new_name[len - 1] == '\n') new_name[len - 1] = '\0';
            if (strlen(new_name) > 0) strncpy(r.name, new_name, ROOM_NAME_SIZE);
            printf("New capacity (enter -1 to keep current): ");
            int new_capacity;
            scanf("%d", &new_capacity);
            getchar();
            if (new_capacity != -1) r.capacity = new_capacity;
            printf("Available? (1 = Yes, 0 = No, -1 to keep current): ");
            int new_available;
            scanf("%d", &new_available);
            getchar();
            if (new_available == 0 || new_available == 1) r.available = new_available;
            fseek(f, -sizeof(room_user), SEEK_CUR);
            fwrite(&r, sizeof(room_user), 1, f);
            printf("Room updated successfully!\n");
            break;
        }
    }
    fclose(f);
    if (!found)
    {
        printf("Room with ID %d not found.\n", id);
    }
}

// Search for a room by ID and display its info
void search_room()
{
    int id;
    printf("Enter the room ID to search: ");
    scanf("%d", &id);
    getchar();

    FILE *f = fopen(ROOMS_DATA_FILE, "rb");
    if (!f)
    {
        printf("Error opening rooms.dat file.\n");
        return;
    }
    room_user r;
    int found = 0;
    while (fread(&r, sizeof(room_user), 1, f) == 1)
    {
        if (r.id == id)
        {
            found = 1;
            printf("\n--- Room Information ---\n");
            printf("ID: %d\nName: %s\nCapacity: %d\nAvailable: %s\n", r.id, r.name, r.capacity, r.available ? "Yes" : "No");
            int sub_quit = 0;
            while (!sub_quit)
            {
                printf("\n--- What do you want to do? ---\n");
                printf("1. Edit room\n");
                printf("2. Make room unavailable\n");
                printf("3. Return\n");
                printf("Choose an option: ");
                int sub_choice = 0;
                scanf("%d", &sub_choice);
                getchar();
                switch (sub_choice)
                {
                    case 1:
                        edit_room(&r);
                        sub_quit = 1;
                        break;
                    case 2:
                        delete_room(&r);
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
    fclose(f);
    if (!found)
    {
        printf("Room not found.\n");
    }
}

// Edit an existing room's information
void edit_room(room_user *room)
{
    printf("\n--- Edit Room ---\n");
    char old_name[ROOM_NAME_SIZE];
    strncpy(old_name, room->name, ROOM_NAME_SIZE);
    int old_capacity = room->capacity;
    int old_available = room->available;

    printf("Current name: %s\nEnter new name (leave empty to keep current): ", room->name);
    char buffer[ROOM_NAME_SIZE];
    fgets(buffer, ROOM_NAME_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    if (strlen(buffer) > 0)
    {
        strncpy(room->name, buffer, ROOM_NAME_SIZE);
    }
    printf("Current capacity: %d\nEnter new capacity (-1 to keep current): ", room->capacity);
    int new_capacity;
    scanf("%d", &new_capacity);
    getchar();
    if (new_capacity != -1)
    {
        room->capacity = new_capacity;
    }
    printf("Current available: %s\nEnter new available (1 = Yes, 0 = No, -1 to keep current): ", room->available ? "Yes" : "No");
    int new_available;
    scanf("%d", &new_available);
    getchar();
    if (new_available == 0 || new_available == 1)
    {
        room->available = new_available;
    }
    // Update room in file
    FILE *f = fopen(ROOMS_DATA_FILE, "r+b");
    if (!f)
    {
        printf("Error opening rooms.dat file for update.\n");
        return;
    }
    room_user r;
    while (fread(&r, sizeof(room_user), 1, f) == 1)
    {
        if (r.id == room->id)
        {
            fseek(f, -sizeof(room_user), SEEK_CUR);
            fwrite(room, sizeof(room_user), 1, f);
            printf("Room information updated successfully!\n");
            fclose(f);
            return;
        }
    }
    fclose(f);
    printf("Error: room not found for update.\n");
}

// Show all rooms in the database
void show_all_rooms()
{
    FILE *f = fopen(ROOMS_DATA_FILE, "rb");
    if (!f)
    {
        printf("Error opening rooms file.\n");
        return;
    }
    room_user r;
    int count = 0;
    printf("\n--- All Registered Rooms ---\n");
    while (fread(&r, sizeof(room_user), 1, f) == 1)
    {
        printf("%d. ID: %d | Name: %s | Capacity: %d | Available: %s\n", ++count, r.id, r.name, r.capacity, r.available ? "Yes" : "No");
    }
    fclose(f);
    if (count == 0)
    {
        printf("No rooms found in the database.\n");
    }
}

// Ajoute une nouvelle salle dans rooms.dat
void add_room()
{
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
    FILE *f = fopen(ROOMS_DATA_FILE, "rb");
    if (f)
    {
        room_user r;
        while (fread(&r, sizeof(room_user), 1, f) == 1)
        {
            if (r.id == new_room.id)
            {
                id_exists = 1;
                break;
            }
        }
        fclose(f);
    }
    if (id_exists)
    {
        printf("Error: a room with this ID already exists. Room not added.\n");
        return;
    }

    // Add the room
    f = fopen(ROOMS_DATA_FILE, "ab");
    if (!f)
    {
        printf("Error opening rooms.dat file.\n");
        return;
    }
    fwrite(&new_room, sizeof(room_user), 1, f);
    fclose(f);
    printf("Room added successfully!\n");
}

void room_management_menu()
{
    int quit = 0;
    while (!quit)
    {
        printf("\n==== Room Management Menu ====\n");
        printf("1. Show all rooms\n");
        printf("2. Search room\n");
        printf("3. Add room\n");
        printf("4. Exit Application\n");
        printf("Choose an option: ");
        int choice = 0;
        scanf("%d", &choice);
        getchar(); // consume newline
        switch (choice)
        {
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
                printf("Exiting room management menu.\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
                break;
        }
    }
}
