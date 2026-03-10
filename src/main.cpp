#include <iostream>
#include <SDL3/SDL.h>


// NOTE:
// Wymaga SDL3 i odpowiedniego CMake.
int main()
{

    SDL_Window* window = SDL_CreateWindow(
        "Pixel Editor",
        1200,
        600,
        0
    );

    if (!window) {
        std::cerr << "Nie udało się stworzyć okna: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    SDL_Delay(3000);

    SDL_DestroyWindow(window);
    SDL_Quit();
}