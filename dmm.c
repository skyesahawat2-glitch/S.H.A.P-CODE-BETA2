#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    char name[20];
    int is_locked;
} Door;

// Function to draw the ASCII Floor Plan
void draw_map(Door doors[]) {
    printf("\033[1;34m      FACILITY FLOOR PLAN (TOP VIEW)\033[0m\n");
    printf("      +-------------------------------+\n");
    printf("      |           [SERVER]            |\n");
    printf("      |             %s               |\n", doors[1].is_locked ? "\033[1;31m[X]\033[0m" : "\033[1;32m[O]\033[0m");
    printf("      |    +---------+     +---------+|\n");
    printf("      |    |         |     |  [LAB]  ||\n");
    printf(" %s | [LOBBY] |     |   %s   ||\n", doors[0].is_locked ? "\033[1;31m[X]\033[0m" : "\033[1;32m[O]\033[0m", doors[2].is_locked ? "\033[1;31m[X]\033[0m" : "\033[1;32m[O]\033[0m");
    printf("      |    |         |     |         ||\n");
    printf("      |    +---------+     +---------+|\n");
    printf("      |             %s               |\n", doors[3].is_locked ? "\033[1;31m[X]\033[0m" : "\033[1;32m[O]\033[0m");
    printf("      |           [EXIT]              |\n");
    printf("      +-------------------------------+\n");
}

// NEW: Function to draw the Status Terminal / Diagnostics
void draw_status_terminal(int unlocked_count) {
    float integrity = 100.0 - (unlocked_count * 25.0);
    
    printf("\033[1;30m>> DIAGNOSTIC TERMINAL <<\033[0m\n");
    printf("\033[0;37m[ AUTH ] Protocol: AES-256-GCM encrypted\n");
    printf("[ SYS  ] Network Load: %d.42 kbps\n", 10 + (rand() % 5));
    printf("[ SEC  ] Perimeter Integrity: \033[1;%dm%.1f%%\033[0m\n", (integrity < 50 ? 31 : 32), integrity);
    printf("[ LOG  ] Pulse: 0x%X... OK\033[0m\n", rand() % 0xFFFF);
}

void draw_ui(Door doors[], int count, const char* status_msg) {
    int i;
    int unlocked = 0;
    
    // Count unlocked doors for the integrity status
    for(i = 0; i < count; i++) if(!doors[i].is_locked) unlocked++;

    printf("\033[H\033[J"); // Clear Screen

    printf("\033[1;36m================================================\033[0m\n");
    printf("\033[1;36m   AEGIS ZERO | COMMAND & CONTROL CENTER        \033[0m\n");
    printf("\033[1;36m================================================\033[0m\n\n");

    draw_map(doors);
    printf("\n");

    for (i = 0; i < count; i++) {
        printf(" [%d] %-18s ", i + 1, doors[i].name);
        if (doors[i].is_locked) printf("\033[1;31m[ LOCKED ]\033[0m\n");
        else printf("\033[1;32m[  OPEN  ]\033[0m\n");
    }

    printf("\n\033[1;34m------------------------------------------------\033[0m\n");
    draw_status_terminal(unlocked);
    printf("\033[1;34m------------------------------------------------\033[0m\n");
    
    if (status_msg != NULL) printf("\n \033[1;35mLAST EVENT:\033[0m %s\n", status_msg);
    printf("\n [M] LOCKDOWN | [O] OPEN ALL | [0] EXIT | CMD > ");
}

int main() {
    int i; 
    char input;
    const char* current_log = "System check complete. Map online.";
    Door doors[4] = {{"Main Entrance", 1}, {"Server Vault", 1}, {"Research Lab", 1}, {"Back Exit", 1}};

    srand(time(NULL)); // Initialize random for the terminal pulses

    while (1) {
        draw_ui(doors, 4, current_log);
        input = getchar();
        while (getchar() != '\n'); 

        if (input == '0') break;
        if (input == 'm' || input == 'M') {
            for (i = 0; i < 4; i++) doors[i].is_locked = 1;
            current_log = "\033[1;31m!!! MANUAL LOCKDOWN ENGAGED !!!\033[0m";
        } 
        else if (input == 'o' || input == 'O') {
            for (i = 0; i < 4; i++) doors[i].is_locked = 0;
            current_log = "\033[1;32m*** EMERGENCY BYPASS ACTIVE ***\033[0m";
        }
        else if (input >= '1' && input <= '4') {
            int idx = input - '1';
            doors[idx].is_locked = !doors[idx].is_locked;
            current_log = doors[idx].is_locked ? "Sector Re-secured." : "Access Authorized.";
        } 
    }
    return 0;
}
