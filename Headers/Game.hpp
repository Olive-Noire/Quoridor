#ifndef DEF_GAME_HPP
#define DEF_GAME_HPP

#include <unordered_map>
#include <SDL_events.h>
#include <SDL_render.h>

#include "./Player.hpp"

enum class Turn{P1=0, P2, P3, P4};
enum class SelectMode{NONE=0, PLAYERS, BARRIERS};

namespace Screen {

    constexpr std::uint16_t width{504}, height{504};

}

struct Commands {

    bool flip{false};

    std::uint16_t mousePos[2];
    SelectMode select;

    std::unordered_map<unsigned char, bool> mouse;
    std::unordered_map<SDL_Keycode, bool> keys;

};

class Game {

    public:

    Game();
    ~Game();

    void Update();
    void Render() const;

    bool run;

    Commands commands;
    Player players[4];
    Barrier barriers[20];

    private:

    Turn turn;

    SDL_Event event;
    SDL_Renderer* renderer;
    SDL_Window* window;

};

#endif // DEF_GAME_HPP