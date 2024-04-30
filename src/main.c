#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

// Constants
#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768
#define NUM_OF_COLUMNS 7
#define COLUMN_WIDTH (SCREEN_WIDTH / NUM_OF_COLUMNS)
#define FOUNDATION_WIDTH 100
#define FOUNDATION_HEIGHT 140

// Definition of struct for Card
struct ListCard {
    char value;
    char suit;
    bool faceDown;
    bool existsInGame;
    struct ListCard* next;
    struct ListCard* prev;
    bool exists;
};

// Structure of Linked List
typedef struct {
    int size;
    struct ListCard* tail;
    struct ListCard* head;
} Linked_list;

// Global SDL variables
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* cardTextures[53]; // 52 cards + 1 card back

// Function Prototypes
Linked_list* createLinkedList();
void renderCard(struct ListCard* card, int x, int y);
void renderGame(Linked_list* columns[NUM_OF_COLUMNS], Linked_list* foundations[4]);
void handleMouseClick(int x, int y, Linked_list* columns[NUM_OF_COLUMNS], Linked_list* foundations[4]);
void initializeGame(Linked_list* columns[NUM_OF_COLUMNS], Linked_list* foundations[4]);
void addCardToList(Linked_list* list, char value, char suit);

// Helper function to add a card to a linked list
void addCardToList(Linked_list* list, char value, char suit) {
    struct ListCard* newCard = (struct ListCard*)malloc(sizeof(struct ListCard));
    if (newCard != NULL) {
        newCard->value = value;  // Set card value
        newCard->suit = suit;    // Set card suit
        newCard->faceDown = false;  // Card is face up by default
        newCard->existsInGame = true;
        newCard->next = NULL;  // No next card yet
        newCard->prev = list->tail;  // Set previous card to current tail
        newCard->exists = true;  // Card exists in game

        if (list->tail != NULL) {  // If there is a tail card, link it to the new card
            list->tail->next = newCard;
        } else {  // If the list is empty, new card is also the head
            list->head = newCard;
        }
        list->tail = newCard;  // Update the tail to the new card
        list->size++;  // Increase the size of the list
    } else {
        printf("Failed to allocate memory for new card\n");
    }
}


// Load texture function with debug messages
SDL_Texture* loadTexture(const char* path) {
    printf("Loading texture from path: %s\n", path);
    SDL_Surface* loadedSurface = IMG_Load(path);
    if (loadedSurface == NULL) {
        printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
        return NULL;
    }

    SDL_Texture* newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
    SDL_FreeSurface(loadedSurface);
    if (newTexture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", path, SDL_GetError());
    } else {
        printf("Texture loaded successfully!\n");
    }
    return newTexture;
}

void loadCardTextures() {
    char path[100];
    char suits[] = {'H', 'D', 'C', 'S'};
    char values[] = {'A', '2', '3', '4', '5', '6', '7', '8', '9', 'T', 'J', 'Q', 'K'};
    int index = 0;
    for (int s = 0; s < 4; s++) {
        for (int v = 0; v < 13; v++) {
            sprintf(path, "/Users/kasparasm/Desktop/2. semester/Maskinnær projekt/maskin-projekt2/resources/%c%c.png", values[v], suits[s]);
            printf("Attempting to load texture from: %s\n", path);
            cardTextures[index++] = loadTexture(path);
        }
    }
    cardTextures[52] = loadTexture("/Users/kasparasm/Desktop/2. semester/Maskinnær projekt/maskin-projekt2/resources/back.png");
}

void renderCard(struct ListCard* card, int x, int y) {
    if (card == NULL || renderer == NULL || window == NULL) {
        printf("Rendering issue: NULL pointers involved.\n");
        return;
    }
    SDL_Texture* texture = card->faceDown ? cardTextures[52] : cardTextures[(card->value - 'A') + (card->suit - 'C') * 13];
    SDL_Rect destRect = {x, y, 100, 140};
    SDL_RenderCopy(renderer, texture, NULL, &destRect);
}

void renderGame(Linked_list* columns[NUM_OF_COLUMNS], Linked_list* foundations[4]) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Set draw color to black
    SDL_RenderClear(renderer); // Clear the screen with black

    for (int i = 0; i < NUM_OF_COLUMNS; i++) {
        int colX = i * COLUMN_WIDTH;
        int colY = 0;
        Linked_list* column = columns[i];
        for (struct ListCard* card = column->head; card != NULL; card = card->next) {
            renderCard(card, colX, colY);
            colY += 35; // Stack cards vertically
        }
    }

    for (int i = 0; i < 4; i++) {
        int fndX = SCREEN_WIDTH - FOUNDATION_WIDTH;
        int fndY = i * FOUNDATION_HEIGHT;
        Linked_list* foundation = foundations[i];
        if (foundation->tail != NULL) {
            renderCard(foundation->tail, fndX, fndY);
        }
    }

    SDL_RenderPresent(renderer); // Update the screen
    printf("Game rendered successfully!\n");
}

void handleMouseClick(int x, int y, Linked_list* columns[NUM_OF_COLUMNS], Linked_list* foundations[4]) {
    printf("Mouse clicked at (%d, %d)\n", x, y);
}

void initializeGame(Linked_list* columns[NUM_OF_COLUMNS], Linked_list* foundations[4]) {
    for (int i = 0; i < NUM_OF_COLUMNS; i++) {
        columns[i] = createLinkedList();
        addCardToList(columns[i], 'A' + i % 13, 'H' + i % 4);  // Add cards with different values and suits
    }
    for (int i = 0; i < 4; i++) {
        foundations[i] = createLinkedList();
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Yukon Solitaire", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    loadCardTextures();
    Linked_list* columns[NUM_OF_COLUMNS];
    Linked_list* foundations[4];
    initializeGame(columns, foundations);

    bool quit = false;
    SDL_Event e;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                handleMouseClick(x, y, columns, foundations);
            }
        }

        renderGame(columns, foundations);

        // Delay to prevent high CPU usage
        SDL_Delay(10);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
