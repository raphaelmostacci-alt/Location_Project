// Fonctions pour la gestion des réservations
#include "includes.h"
#include "reservation_room.h"
#include "room_management.h"
#include "client_management.h"
#include <time.h>

#define RESERVATIONS_DATA_FILE "reservations.dat"
#define ROOMS_DATA_FILE "rooms.dat"

// Global variable to store the currently logged-in user
static char current_user_last_name[MAX_SIZE] = "";
static char current_user_first_name[MAX_SIZE] = "";
static char current_user_email[MAX_SIZE] = "";

// Validate date and time
static int validate_date(int d, int m, int y) {
    if (d < 1 || d > 31 || m < 1 || m > 12 || y < 1000 || y > 9999) return printf("Error: Invalid date.\n"), 0;
    return 1;
}
static int validate_time(int h, int min) {
    if (h < 8 || h > 11 || min < 0 || min > 59 || (h == 11 && min > 0)) return printf("Error: Time 8:00-11:00.\n"), 0;
    return 1;
}

// Helper function to load user reservations
static int load_user_reservations(ReservationRoom all_res[], int all_idx[], int *total) {
    FILE *f = fopen(RESERVATIONS_DATA_FILE, "rb");
    if (!f) return 0;
    
    int user_count = 0;
    *total = 0;
    while (fread(&all_res[*total], sizeof(ReservationRoom), 1, f) == 1) {
        if (strcmp(all_res[*total].client_mail, current_user_email) == 0) {
            all_idx[user_count++] = *total;
        }
        (*total)++;
    }
    fclose(f);
    return user_count;
}

// Display reservations with room names
static void display_reservations(ReservationRoom res[], int idx[], int cnt) {
    FILE *f = fopen(ROOMS_DATA_FILE, "rb");
    printf("%-5s %-20s %-15s %-20s\n%-72s\n", "No.", "Room", "ID", "Date & Time", "------------------------------------------------------------------------");
    for (int i = 0; i < cnt; i++) {
        char nm[MAX_SIZE] = "Unknown";
        if (f) {
            rewind(f);
            room r;
            while (fread(&r, sizeof(room), 1, f) == 1) if (r.id == res[idx[i]].room_id) { strcpy(nm, r.name); break; }
        }
        printf("%-5d %-20s %-15d %02d/%02d/%04d %02d:%02d\n", i+1, nm, res[idx[i]].reservation_id, 
               res[idx[i]].date.day, res[idx[i]].date.month, res[idx[i]].date.year, res[idx[i]].date.hour, res[idx[i]].date.minute);
    }
    if (f) fclose(f);
}

// Show and manage reservations
void show_room_locked_by_client() {
    if (!strlen(current_user_email)) return (void)printf("\n=== My Reservations ===\nError: Login required.\n");
    printf("\n=== My Reservations ===\nUser: %s %s (%s)\n\n", current_user_first_name, current_user_last_name, current_user_email);
    ReservationRoom all[100];
    int idx[100], tot, cnt = load_user_reservations(all, idx, &tot);
    if (!cnt) return (void)printf("No reservations.\n");
    display_reservations(all, idx, cnt);
    printf("\n1. Delete\n0. Return\nChoose: ");
    int act; scanf("%d", &act); while (getchar() != '\n');
    if (act != 1) return;
    printf("\nSelect (1-%d, 0=cancel): ", cnt);
    int ch; scanf("%d", &ch); while (getchar() != '\n');
    if (ch < 1 || ch > cnt) return (void)printf("Cancelled.\n");
    int i = idx[ch-1];
    printf("\nDelete ID %d on %02d/%02d/%04d %02d:%02d? (y/n): ", all[i].reservation_id, 
           all[i].date.day, all[i].date.month, all[i].date.year, all[i].date.hour, all[i].date.minute);
    char c; scanf(" %c", &c); while (getchar() != '\n');
    if (c != 'y' && c != 'Y') return (void)printf("Cancelled.\n");
    FILE *f = fopen(RESERVATIONS_DATA_FILE, "wb");
    if (!f) return (void)printf("Error: Cannot write.\n");
    for (int j = 0; j < tot; j++) if (j != i) fwrite(&all[j], sizeof(ReservationRoom), 1, f);
    fclose(f);
    printf("✓ Deleted!\n");
}

// Book a reservation
void book_reservation() {
    if (!strlen(current_user_email)) return (void)printf("\n=== Book Reservation ===\nError: Login required.\n");
    printf("\n=== Book Reservation ===\n\n%-5s %-20s %-10s\n%-36s\n", "ID", "Name", "Size", "------------------------------------");
    FILE *f = fopen(ROOMS_DATA_FILE, "rb");
    room r; int ok = 0;
    if (f) { while (fread(&r, sizeof(room), 1, f) == 1) if (r.available) { printf("%-5d %-20s %-10d\n", r.id, r.name, r.size); ok = 1; } fclose(f); }
    if (!ok) return (void)printf("No rooms available.\n");
    
    ReservationRoom res;
    strcpy(res.client_mail, current_user_email);
    int max = 0;
    f = fopen(RESERVATIONS_DATA_FILE, "rb");
    if (f) { ReservationRoom tmp; while (fread(&tmp, sizeof(ReservationRoom), 1, f) == 1) if (tmp.reservation_id > max) max = tmp.reservation_id; fclose(f); }
    res.reservation_id = max + 1;
    
    printf("\nBooking: %s %s | ID: %d\nRoom ID: ", current_user_first_name, current_user_last_name, res.reservation_id);
    scanf("%d", &res.room_id); while (getchar() != '\n');
    printf("Date (day month year): ");
    char b[100]; fgets(b, 100, stdin);
    sscanf(b, "%d %d %d", &res.date.day, &res.date.month, &res.date.year);
    if (!validate_date(res.date.day, res.date.month, res.date.year)) return;
    
    time_t n = time(NULL); struct tm *t = localtime(&n);
    int td = (t->tm_year+1900)*10000 + (t->tm_mon+1)*100 + t->tm_mday;
    int bk = res.date.year*10000 + res.date.month*100 + res.date.day;
    if (bk < td) return (void)printf("Error: Past date.\n");
    
    while (1) { printf("Time (hour minute) 8:00-11:00: "); fgets(b, 100, stdin); sscanf(b, "%d %d", &res.date.hour, &res.date.minute); if (validate_time(res.date.hour, res.date.minute)) break; }
    
    f = fopen(ROOMS_DATA_FILE, "rb");
    if (!f) return (void)printf("Error: Cannot open rooms.dat\n");
    ok = 0;
    while (fread(&r, sizeof(room), 1, f) == 1) {
        if (r.id == res.room_id) { ok = 1; if (!r.available) { fclose(f); return (void)printf("Error: Room unavailable.\n"); } break; }
    }
    fclose(f);
    if (!ok) return (void)printf("Error: Room ID %d not found.\n", res.room_id);
    
    f = fopen(RESERVATIONS_DATA_FILE, "ab");
    if (!f) return (void)printf("Error: Cannot save.\n");
    fwrite(&res, sizeof(ReservationRoom), 1, f); fclose(f);
    printf("✓ Reserved! Room:%d | ID:%d | %02d/%02d/%04d %02d:%02d-%02d:%02d (1h)\n", 
           res.room_id, res.reservation_id, res.date.day, res.date.month, res.date.year, res.date.hour, res.date.minute, res.date.hour+1, res.date.minute);
}

// Main menu
void reservation_room_menu() {
    printf("\n=== Login ===\nLast name: ");
    fgets(current_user_last_name, MAX_SIZE, stdin);
    current_user_last_name[strcspn(current_user_last_name, "\n")] = '\0';
    printf("First name: ");
    fgets(current_user_first_name, MAX_SIZE, stdin);
    current_user_first_name[strcspn(current_user_first_name, "\n")] = '\0';
    
    client_user c;
    if (!get_client_info(current_user_last_name, current_user_first_name, &c)) return (void)printf("Error: Client not found.\n");
    strcpy(current_user_email, c.mail);
    printf("\n✓ Logged in: %s %s (%s)\n", current_user_first_name, current_user_last_name, current_user_email);
    
    int ch;
    while (1) {
        printf("\n╔════════════════════════════════════╗\n║   RESERVATION MENU                 ║\n╠════════════════════════════════════╣\n║ 1. My reservations                 ║\n║ 2. Book reservation                ║\n║ 3. Exit                            ║\n╚════════════════════════════════════╝\nOption: ");
        scanf("%d", &ch); getchar();
        if (ch == 1) show_room_locked_by_client();
        else if (ch == 2) book_reservation();
        else if (ch == 3) return (void)printf("Exiting...\n");
        else printf("Invalid.\n");
    }
}
