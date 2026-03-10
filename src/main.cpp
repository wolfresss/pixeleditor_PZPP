#include <iostream>
#include <SDL3/SDL.h>

int main() {


    SDL_Window* window = SDL_CreateWindow("Pixel Editor", 600, 600, 0);
    if (!window) {
        std::cerr << "Nie udało się stworzyć okna: " << SDL_GetError() << "\n";
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    quit = true;
                    break;
                case SDL_EVENT_KEY_DOWN:
                    std::cout << "Klawisz naciśnięty, kod: " << event.key.key << "\n";
                    if (event.key.key == SDLK_ESCAPE) {
                    }
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    std::cout << "Kliknięto przycisk myszy: " << (int)event.button.button
                              << " w pozycji (" << event.button.x << ", " << event.button.y << ")\n";
                    break;
                case SDL_EVENT_MOUSE_MOTION:
                    std::cout << "Ruch myszy: (" << event.motion.x << ", " << event.motion.y << ")\n";
                    break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}