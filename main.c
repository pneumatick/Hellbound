#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

enum creature_type {God, Human, Cat, Rat, Bug};
int num_creature_types = 5;

typedef struct Player {
    char *name;
    int health;
    int attack;
} Player;

typedef struct Creature {
    char *name;
    enum creature_type type;
    int health;
    int attack;
} Creature;

// Generate an arbitrary number of creatures
Creature **random_creature_generator(int num) {
    Creature **creature_list = malloc(num * sizeof(Creature*));
    for (int i = 0; i < num; i++) {
        Creature *creature = malloc(sizeof(Creature));
        creature->name = "Rando";
        creature->type = rand() % num_creature_types;
        creature->health = rand() % 26;
        creature->attack = rand() % 6;
        creature_list[i] = creature;
	}
	
	return creature_list;
}

// Create the player structure with user input
Player *create_player() {
    int size = 51;
    char *name = malloc(size * sizeof(char));

    fgets(name, size, stdin);
    int newline_index = strcspn(name, "\n");
    name[newline_index] = 0;
    char new_char_buff = '0';
    while (newline_index >= (size - 1) && new_char_buff != '\n') {
        new_char_buff = getchar();
    }

    Player *player = malloc(sizeof(Player));
    player->name = name;
    player->health = rand() % 101;
    player->attack = rand() % 26;

    return player;
}

// Get the number of creatures from the user
int get_num_creatures(Creature **creature_list) {
    char num_creature_buff[4];
    int num_creatures;
    int newline_index;
    char extra_char_buff = '0';

    // Get user input
    fgets(num_creature_buff, sizeof num_creature_buff, stdin);
    newline_index = strcspn(num_creature_buff, "\n");
    num_creatures = atoi(num_creature_buff);	
    printf("\nOkay, summoning %d creatures...\n", num_creatures);

    // Clear stdin if necessary
    while (newline_index >= (sizeof num_creature_buff - 1) && extra_char_buff != '\n') {
        extra_char_buff = getchar();
    }

    return num_creatures;
}

// Only used for testing purposes
void display_creatures(Creature **list, int size) {
    for (int i = 0; i < size; i++) {
        printf("Name: %s\nType: %d\nHealth: %d\nAttack: %d\n", list[i]->name, list[i]->type, list[i]->health, list[i]->attack);
    }
}

// Free each creature in the creature list
void free_creatures(Creature **list, int size) {
    for (int i = 0; i < size; i++) {
        free(list[i]);
    }
    free(list);
}

void player_attack(Player *player, Creature *opponent) {
    printf("\n%s attacked %s for %d damage!\n", player->name, opponent->name, player->attack);
    opponent->health -= player->attack;
    printf("%s now has %d health.\n", opponent->name, opponent->health);
   
}

void player_heal(Player *player) {
    int heal = rand() % 6;
    player->health += heal;
    printf("\n%s healed for %d health. %s's health is now %d.\n", player->name, heal, player->name, player->health);

}

void creature_turn(Player *player, Creature *opponent) {
    if (opponent->health > 0) {
        printf("%s attacked %s for %d damage!\n", opponent->name, player->name, opponent->attack);
        player->health -= opponent->attack;
        printf("%s now has %d health.\n", player->name, player->health);
    }
    // Creature death
    else if (opponent->health <= 0) {
        printf("%s has died.\n", opponent->name);
    }
}

// Initiate a single round of combat
int start_round(Player *player, Creature **creature_list, int num_creatures) {
    Creature *opponent;
    char user_in;
    char extra_char_buff;
    bool player_action;

    // Loop over each creature in the list
    for (int i = 0; i < num_creatures; i++) {
        opponent = creature_list[i];
        printf("\nOpponent: %s\nType: %d\nHealth: %d\nAttack: %d\n", opponent->name, opponent->type, opponent->health, opponent->attack);

        // Combat loop
        while (player->health > 0 && opponent->health > 0 && user_in != 'q') {
            // Player action
            player_action = false;
            printf("\nAction: ");
            user_in = getchar();
            extra_char_buff = getchar();    // Clear buffer of '\n'
            switch (user_in) {
                // Player attack
                case 'a':
                    player_action = true;
                    player_attack(player, opponent);
                    break;
                // Player self-heal
                case 'h':
                    player_action = true;
                    player_heal(player);
                    break;
                // User quit
                case 'q':
                    continue;
                default:
                    printf("Invalid input '%c'\n", user_in);
            }
            // Clear stdin of extra characters
            while (extra_char_buff != '\n') {
                extra_char_buff = getchar();
            }

            // Creature's turn
            if (player_action) {
                creature_turn(player, opponent);
            }

            // Player death
            if (player->health <= 0) {
                printf("%s has died.\n", player->name);
                return 1;
            }
        }

        // Handle user quit
        if (user_in == 'q') {
            printf("\nReturning to Earth...\n");
            return -1;
        }
    }

    return 0;
}

// Ask the user whether or not they want to continue playing if they're alive
int request_new_round() {
    char user_in;
    char extra_char_buff;
    int state;
    bool valid = false;
    while (!valid) {
        printf("Would you like to fight again? (y/n): ");
        user_in = getchar();
        extra_char_buff = getchar();
        switch (user_in) {
            case 'y':
                state = 0;
                valid = true;
                break;
            case 'n':
                state = -1;
                valid = true;
                break;
            default:
                printf("Invalid input \"%c\"\n", user_in);
        }
        while (extra_char_buff != '\n') {
            extra_char_buff = getchar();
        }
    }

    return state;
}

// Main game loop
int game_loop() {
    Player *player;
    Creature **creature_list;
    int num_creatures;
    int user_state = 0;
    int years = 0;

    // Create player
    printf("What was your name, oh wretched soul?: ");
    player = create_player();

    // Specify number of creatures to fight
    printf("\nWelcome to Hell, %s! Your health is %d and your attack is %d.\nHow many creatures would you like to fight? (999 Max): ", player->name, player->health, player->attack);	
    num_creatures = get_num_creatures(creature_list);

    // Summon creatures
    creature_list = random_creature_generator(num_creatures);

    while (user_state == 0) {
        user_state = start_round(player, creature_list, num_creatures);
        free_creatures(creature_list, num_creatures);

        // Request and set up new round if the player is still alive
        if (user_state == 0) {
            years += num_creatures;
            printf("\nCongratulations, %s, you have earned yourself %d Earth years in Heaven.\n", player->name, years);
            user_state = request_new_round();
            if (user_state == 0) {
                printf("How many more creatures would you like to fight?: ");
                num_creatures = get_num_creatures(creature_list);
                creature_list = random_creature_generator(num_creatures);
            }
        }
    }

    // Free remaining memory
    free(player->name);
    free(player);

    return user_state;
}

int main(int argc, char **argv) {
    int user_state;

    // Set random seed
    srand(time(0));

    // Multi-round loop
    user_state = game_loop();

    // Player death message
    if (user_state == 1) {
        printf("\nIt would appear that you have exceeded your means and sought too great of a reward. Your eternal punishment in Hell begins now!\n");
    }

    return 0;
}
