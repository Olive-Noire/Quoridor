#include <SDL_timer.h>

#include "../Headers/Game.hpp"

int main() {

    Game quoridor;

    constexpr std::uint32_t frameDelay{1000/120};
    std::uint32_t frameStart{0}, frameTime{0};

    while (quoridor.run) {

        frameStart = SDL_GetTicks();

        quoridor.Update();
        quoridor.Render();

        frameTime = SDL_GetTicks()-frameStart;
        if (frameDelay > frameTime) SDL_Delay(frameDelay-frameTime);

    }

    return 0;

}

// Command : g++ -m32 "./Code/Sources/*.cpp" -o "./bin/Quoridor" -std=c++17 -s -I "./include/" -L "./lib/" -lSDL2main -lSDL2 -Wall -Wextra -pedantic-errors -mwindows -DNDEBUG