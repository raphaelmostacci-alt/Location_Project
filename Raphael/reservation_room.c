#include "includes.h"
#include "reservation_room.h"
#include "useful_fonction.h"
#include "room_management.h"
#include "useful_mutual.h"

// Dynamic array for reservations
ReservationRoom *reservations = NULL;
int reservation_count = 0;

// Logical deletion of all reservations for a client
void invalidate_reservations_by_client(const char *client_mail) {
    int changed = 0;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].reservation_id != -1 && strcmp(reservations[i].client_mail, client_mail) == 0) {
            reservations[i].reservation_id = -1;
            changed = 1;
        }
    }
    if (changed) save_reservations();
}

// Logical deletion of all reservations for a room
void invalidate_reservations_by_room(int room_id) {
    int changed = 0;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].reservation_id != -1 && reservations[i].room_id == room_id) {
            reservations[i].reservation_id = -1;
            changed = 1;
        }
    }
    if (changed) save_reservations();
}

// Load reservations from file
void load_reservations() {
    reservations = (ReservationRoom*)mutual_load("reservations.dat", sizeof(ReservationRoom), &reservation_count);
}

// Save reservations to file
void save_reservations() {
    mutual_save("reservations.dat", reservations, sizeof(ReservationRoom), reservation_count);
}

// Add a reservation
void add_reservation(ReservationRoom new_res) {
    reservations = realloc(reservations, (reservation_count + 1) * sizeof(ReservationRoom));
    reservations[reservation_count++] = new_res;
    save_reservations();
}

// Free reservation memory
void free_reservations() {
    mutual_free(reservations);
    reservations = NULL;
    reservation_count = 0;
}

// Global variable for current user's email
char current_user_email[MAX_SIZE] = "";

// Helper to get room name from its ID
const char* get_room_name(int room_id) {
    static char name[MAX_SIZE] = "Unknown";
    FILE *f = fopen("rooms.dat", "rb");
    if (f) {
        room_user r;
        while (fread(&r, sizeof(room_user), 1, f) == 1) {
            if (r.id == room_id) {
                strncpy(name, r.name, MAX_SIZE);
                break;
            }
        }
        fclose(f);
    }
    return name;
}

// Helper: check if a slot is available for a room on a given day and time
int is_slot_available(int room_id, int day, int month, int year, int hour) {
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].room_id == room_id &&
            reservations[i].date.day == day &&
            reservations[i].date.month == month &&
            reservations[i].date.year == year &&
            reservations[i].date.hour == hour) {
            return 0; // slot already taken
        }
    }
    return 1;
}

// Helper: show available rooms for a given slot
void show_available_rooms_for_slot(int day, int month, int year, int hour) {
    load_rooms();
    int found = 0;
    printf("\n--- Available Rooms for %02d/%02d/%d, %dh-%dh ---\n", day, month, year, hour, hour+2);
    for (int i = 0; i < room_count; i++) {
        if (!rooms[i].available) continue;
        if (is_slot_available(rooms[i].id, day, month, year, hour)) {
            printf("ID: %d | Name: %s | Capacity: %d\n", rooms[i].id, rooms[i].name, rooms[i].capacity);
            found = 1;
        }
    }
    if (!found) {
        print_error("No available rooms for this slot.");
    }
}

// Main reservation menu (harmonized)
void reservation_room_menu() {
    int quit = 0;
    load_reservations();
    while (!quit) {
        printf("\n==== Room Reservation Menu ====\n");
        printf("1. Show my reservations\n");
        printf("2. Book a reservation\n");
        printf("8. Return to previous menu\n");
        printf("9. Exit Application\n");
        int choice = read_int("Choose an option: ");
        switch (choice) {
            case 1: show_room_locked_by_client(); break;
            case 2: book_reservation(); break;
            case 8: free_reservations(); return_menu(); quit = 1; break;
            case 9: free_reservations(); exit_application(); break;
            default: print_error("Invalid option. Try again."); break;
        }
    }
}

// Show reservations for the current client
void show_room_locked_by_client() {
    printf("\n-----------------------------------\n");
    printf("--- My Reservations ---\n");
    printf("-----------------------------------\n");
    int count = 0;
    int *indices = malloc(reservation_count * sizeof(int));
    printf("%-3s %-20s %-16s\n", "No.", "Room", "Date & Time");
    printf("%-45s\n", "---------------------------------------------");
    for (int i = 0; i < reservation_count; i++) {
        if (strcmp(reservations[i].client_mail, current_user_email) == 0 && reservations[i].reservation_id != -1) {
            printf("%-3d %-20s %02d/%02d/%04d %02d:00\n",
                   count + 1, get_room_name(reservations[i].room_id), reservations[i].date.day, reservations[i].date.month, reservations[i].date.year, reservations[i].date.hour);
            indices[count++] = i;
        }
    }
    if (count == 0) {
        print_error("No reservations found for this user.");
        free(indices);
        return_menu();
        return;
    }
    int choice;
    printf("\nSelect a reservation (1-%d):\n", count);
    printf("8. Return to previous menu\n");
    printf("9. Exit application\n");
    choice = read_int("Choose an option: ");
    if (choice == 8) {
        free(indices);
        return_menu();
        return;
    }
    if (choice == 9) {
        free(indices);
        exit_application();
        return;
    }
    if (choice < 1 || choice > count) {
        print_error("Invalid selection.");
        free(indices);
        return_menu();
        return;
    }
    int idx = indices[choice - 1];
    int sub_quit = 0;
    while (!sub_quit) {
        printf("\n--- What do you want to do? ---\n");
        printf("1. Edit reservation\n");
        printf("2. Delete reservation\n");
        printf("8. Return to previous menu\n");
        printf("9. Exit Application\n");
        int sub_choice = read_int("Choose an option: ");
        switch (sub_choice) {
            case 1:
                edit_reservation_id(reservations[idx].reservation_id);
                sub_quit = 1;
                break;
            case 2:
                delete_reservation_id(reservations[idx].reservation_id);
                sub_quit = 1;
                break;
            case 8:
                sub_quit = 1;
                printf("Returning to previous menu.\n");
                break;
            case 9:
                free(indices);
                exit_application();
                return;
            default:
                print_error("Invalid option. Try again.");
                break;
        }
    }
    free(indices);
    return_menu();
}

// Edit reservation by ID
void edit_reservation_id(int reservation_id) {
    int found = 0;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].reservation_id == reservation_id && reservations[i].reservation_id != -1) {
            found = 1;
            printf("Current date: %02d/%02d/%04d %02d:%02d\n", reservations[i].date.day, reservations[i].date.month, reservations[i].date.year, reservations[i].date.hour, reservations[i].date.minute);
            printf("Enter new date (DD MM YYYY): ");
            int new_day = read_int("");
            int new_month = read_int("");
            int new_year = read_int("");
            printf("Enter new time (HH MM): ");
            int new_hour = read_int("");
            int new_minute = read_int("");
            if (new_hour < 8 || new_hour > 11) {
                print_error("Hour must be between 8am and 11am.");
                break;
            }
            time_t now = time(NULL);
            struct tm input_date = {0};
            input_date.tm_mday = new_day;
            input_date.tm_mon = new_month - 1;
            input_date.tm_year = new_year - 1900;
            input_date.tm_hour = new_hour;
            input_date.tm_min = new_minute;
            input_date.tm_sec = 0;
            time_t input_time = mktime(&input_date);
            double diff_days = difftime(input_time, now) / (60 * 60 * 24);
            if (diff_days < 0) {
                print_error("Cannot book in the past.");
                break;
            }
            if (diff_days > 6.99) {
                print_error("Cannot book more than 7 days from today.");
                break;
            }
            int slot_free = 1;
            for (int j = 0; j < reservation_count; j++) {
                if (reservations[j].room_id == reservations[i].room_id && reservations[j].date.day == new_day && reservations[j].date.month == new_month && reservations[j].date.year == new_year && reservations[j].date.hour == new_hour && reservations[j].reservation_id != -1 && reservations[j].reservation_id != reservation_id) {
                    slot_free = 0;
                    break;
                }
            }
            if (!slot_free) {
                print_error("This slot is already booked for this room and date.");
                break;
            }
            reservations[i].date.day = new_day;
            reservations[i].date.month = new_month;
            reservations[i].date.year = new_year;
            reservations[i].date.hour = new_hour;
            reservations[i].date.minute = new_minute;
            save_reservations();
            print_success("Reservation updated!");
            break;
        }
    }
    if (!found) {
        print_error("Reservation not found or already deleted.");
    }
}

// Delete reservation by ID
void delete_reservation_id(int reservation_id) {
    int found = 0;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].reservation_id == reservation_id && reservations[i].reservation_id != -1) {
            found = 1;
            reservations[i].reservation_id = -1;
            save_reservations();
            printf("\n-----------------------------------\n");
            printf("[SUCCESS] Reservation deleted!\n");
            printf("-----------------------------------\n");
            break;
        }
    }
    if (!found) {
        printf("Reservation not found or already deleted.\n");
    }
}

// Book a new reservation
void book_reservation() {
    printf("\n--- Book a Reservation ---\n");
    // Only allow booking for today, tomorrow, and the day after
    time_t now = time(NULL);
    int days[3];
    int months[3];
    int years[3];
    for (int i = 0; i < 3; i++) {
        time_t t = now + i * 24 * 3600;
        struct tm *tm_day = localtime(&t);
        days[i] = tm_day->tm_mday;
        months[i] = tm_day->tm_mon + 1;
        years[i] = tm_day->tm_year + 1900;
    }
    printf("Select a day:\n");
    for (int i = 0; i < 3; i++) {
        printf("%d. %02d/%02d/%d\n", i+1, days[i], months[i], years[i]);
    }
    int day_choice = read_int("");
    if (day_choice < 1 || day_choice > 3) {
        printf("Invalid day selection.\n");
        return;
    }
    int day = days[day_choice-1];
    int month = months[day_choice-1];
    int year = years[day_choice-1];

    printf("Select a time slot:\n1. 8h-10h\n2. 10h-12h\n");
    int slot_choice = read_int("");
    int hour = (slot_choice == 1) ? 8 : 10;
    if (slot_choice != 1 && slot_choice != 2) {
        printf("Invalid slot selection.\n");
        return;
    }

    // Show available rooms for this slot
    show_available_rooms_for_slot(day, month, year, hour);

    // Select room
    int room_id = read_int("Enter room ID: ");

    // Check slot availability
    if (!is_slot_available(room_id, day, month, year, hour)) {
        printf("This slot is already booked for this room.\n");
        return;
    }

    // Proceed to booking
    ReservationRoom new_res;
    new_res.room_id = room_id;
    new_res.date.day = day;
    new_res.date.month = month;
    new_res.date.year = year;
    new_res.date.hour = hour;
    read_string("Enter your email: ", new_res.client_mail, MAX_SIZE);
    // Generate reservation_id
    new_res.reservation_id = reservation_count + 1;
    add_reservation(new_res);
    printf("Reservation successful!\n");
}