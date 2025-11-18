// Functions for reservation management
#include "includes.h"
#include "reservation_room.h"
#include "useful_fonction.h"
#include "room_management.h"


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
    FILE *f = fopen("reservations.dat", "rb");
    if (!f) return;
    ReservationRoom temp;
    reservation_count = 0;
    reservations = NULL;
    while (fread(&temp, sizeof(ReservationRoom), 1, f) == 1) {
        reservations = realloc(reservations, (reservation_count + 1) * sizeof(ReservationRoom));
        reservations[reservation_count++] = temp;
    }
    fclose(f);
}

// Save reservations to file
void save_reservations() {
    FILE *f = fopen("reservations.dat", "wb");
    if (!f) return;
    for (int i = 0; i < reservation_count; i++) {
        fwrite(&reservations[i], sizeof(ReservationRoom), 1, f);
    }
    fclose(f);
}

// Add a reservation
void add_reservation(ReservationRoom new_res) {
    reservations = realloc(reservations, (reservation_count + 1) * sizeof(ReservationRoom));
    reservations[reservation_count++] = new_res;
    save_reservations();
}

// Free reservation memory
void free_reservations() {
    free(reservations);
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

// Main reservation menu (harmonized)
void reservation_room_menu() 
{
	int quit = 0;
    load_reservations();
    while (!quit) {
        printf("\n==== Room Reservation Menu ====\n");
        printf("1. Show my reservations\n");
        printf("2. Book a reservation\n");
        printf("3. Return to previous menu\n");
        printf("4. Exit Application\n");
        printf("Choose an option: ");
        int choice = 0;
        scanf("%d", &choice);
        getchar();
        switch (choice) {
            case 1:
                show_room_locked_by_client();
                break;
            case 2:
                book_reservation();
                break;
            case 3:
                free_reservations();
                return_menu();
                quit = 1;
                break;
            case 4:
                free_reservations();
                exit_application();
                break;
            default:
                printf("Invalid option. Try again.\n");
                break;
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
        printf("No reservations found for this user.\n");
        free(indices);
        return_menu();
        return;
    }
    int choice;
    printf("\nSelect a reservation to edit/delete (1-%d), or 0 to return: ", count);
    scanf("%d", &choice);
    getchar();
    if (choice < 1 || choice > count) {
        free(indices);
        return_menu();
        return;
    }
    int idx = indices[choice - 1];
    printf("\n-----------------------------------\n");
    printf("Selected reservation:\n");
    printf("Room: %s\nDate: %02d/%02d/%04d %02d:00\n",
           get_room_name(reservations[idx].room_id), reservations[idx].date.day, reservations[idx].date.month, reservations[idx].date.year, reservations[idx].date.hour);
    printf("-----------------------------------\n");
    int action;
    printf("1. Edit\n2. Delete\nChoose action: ");
    scanf("%d", &action);
    getchar();
    if (action == 1) {
        edit_reservation_id(reservations[idx].reservation_id);
    } else if (action == 2) {
        delete_reservation_id(reservations[idx].reservation_id);
    } else {
        printf("Invalid action.\n");
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
            int new_day, new_month, new_year, new_hour, new_minute = 0;
            scanf("%d %d %d", &new_day, &new_month, &new_year);
            printf("Enter new time (HH MM): ");
            scanf("%d %d", &new_hour, &new_minute);
            if (new_hour < 8 || new_hour > 11) {
                printf("\n-----------------------------------\n");
                printf("[ERREUR] L'heure doit être comprise entre 8h et 11h.\n");
                printf("-----------------------------------\n");
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
                printf("\n-----------------------------------\n");
                printf("[ERREUR] Impossible de réserver dans le passé.\n");
                printf("-----------------------------------\n");
                break;
            }
            if (diff_days > 6.99) {
                printf("\n-----------------------------------\n");
                printf("[ERREUR] Impossible de réserver au-delà de 7 jours à partir d'aujourd'hui.\n");
                printf("-----------------------------------\n");
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
                printf("\n-----------------------------------\n");
                printf("[ERREUR] Ce créneau est déjà réservé pour cette salle et cette date.\n");
                printf("-----------------------------------\n");
                break;
            }
            reservations[i].date.day = new_day;
            reservations[i].date.month = new_month;
            reservations[i].date.year = new_year;
            reservations[i].date.hour = new_hour;
            reservations[i].date.minute = new_minute;
            save_reservations();
            printf("\n-----------------------------------\n");
            printf("[SUCCÈS] Réservation modifiée !\n");
            printf("-----------------------------------\n");
            break;
        }
    }
    if (!found) {
        printf("Reservation not found or already deleted.\n");
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
    printf("\n--- Book a Reservation (Search Available) ---\n");
    FILE *room_file = fopen("rooms.dat", "rb");
    if (!room_file) {
        printf("[ERROR] Unable to open rooms.dat.\n");
        return_menu();
        return;
    }
    room_user r;
    int found = 0;
    printf("\nID   Name                Capacity   Available Slots\n");
    printf("---------------------------------------------------\n");
    time_t now = time(NULL);
    struct tm today;
    localtime_r(&now, &today);
    while (fread(&r, sizeof(room_user), 1, room_file) == 1) {
        if (r.available) {
            for (int offset = 0; offset < 7; offset++) {
                struct tm slot_date = today;
                slot_date.tm_mday += offset;
                mktime(&slot_date); // normalise la date
                int wday = slot_date.tm_wday; // 0=dimanche, 1=lundi, ..., 6=samedi
                if (wday == 0 || wday == 6) continue; // skip samedi/dimanche
                int d = slot_date.tm_mday;
                int m = slot_date.tm_mon + 1;
                int y = slot_date.tm_year + 1900;
                for (int h = 8; h <= 11; h++) {
                    int slot_free = 1;
                    for (int i = 0; i < reservation_count; i++) {
                        if (reservations[i].room_id == r.id && reservations[i].date.day == d && reservations[i].date.month == m && reservations[i].date.year == y && reservations[i].date.hour == h && reservations[i].reservation_id != -1) {
                            slot_free = 0;
                            break;
                        }
                    }
                    if (slot_free) {
                        printf("%-4d %-20s %-9d %02d:00 %02d/%02d/%04d\n", r.id, r.name, r.capacity, h, d, m, y);
                        found = 1;
                    }
                }
            }
        }
    }
    fclose(room_file);
    if (!found) {
        printf("No available rooms or slots found.\n");
        return_menu();
        return;
    }
    int room_id, hour, day, month, year;
    printf("\nEntrez l'ID de la salle à réserver : ");
    scanf("%d", &room_id);
    printf("Entrez l'heure à réserver (8-11) : ");
    scanf("%d", &hour);
    if (hour < 8 || hour > 11) {
        printf("\n-----------------------------------\n");
        printf("[ERREUR] L'heure doit être comprise entre 8h et 11h.\n");
        printf("-----------------------------------\n");
        return_menu();
        return;
    }
    int minute = 0;
    printf("Entrez la date (JJ MM AAAA) : ");
    scanf("%d %d %d", &day, &month, &year);
    struct tm input_date = {0};
    input_date.tm_mday = day;
    input_date.tm_mon = month - 1;
    input_date.tm_year = year - 1900;
    input_date.tm_hour = hour;
    input_date.tm_min = minute;
    input_date.tm_sec = 0;
    time_t input_time = mktime(&input_date);
    double diff_days = difftime(input_time, now) / (60 * 60 * 24);
    if (diff_days < 0) {
        printf("\n-----------------------------------\n");
        printf("[ERREUR] Impossible de réserver dans le passé.\n");
        printf("-----------------------------------\n");
        return_menu();
        return;
    }
    if (diff_days > 6.99) {
        printf("\n-----------------------------------\n");
        printf("[ERREUR] Impossible de réserver au-delà de 7 jours à partir d'aujourd'hui.\n");
        printf("-----------------------------------\n");
        return_menu();
        return;
    }
    getchar();
    int slot_free = 1;
    for (int i = 0; i < reservation_count; i++) {
        if (reservations[i].room_id == room_id && reservations[i].date.day == day && reservations[i].date.month == month && reservations[i].date.year == year && reservations[i].date.hour == hour && reservations[i].reservation_id != -1) {
            slot_free = 0;
            break;
        }
    }
    if (!slot_free) {
        printf("\n-----------------------------------\n");
        printf("[ERREUR] Ce créneau est déjà réservé pour cette salle et cette date.\n");
        printf("-----------------------------------\n");
        return_menu();
        return;
    }
    ReservationRoom res;
    strcpy(res.client_mail, current_user_email);
    res.room_id = room_id;
    res.reservation_id = (int)time(NULL);
    res.date.day = day;
    res.date.month = month;
    res.date.year = year;
    res.date.hour = hour;
    res.date.minute = minute;
    add_reservation(res);
    printf("\n-----------------------------------\n");
    printf("[SUCCÈS] Réservation créée !\n");
    printf("-----------------------------------\n");
    return_menu();
}

