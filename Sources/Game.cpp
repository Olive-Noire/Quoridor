#include "../Headers/Game.hpp"

#include <SDL.h>

#include "../Headers/Events.hpp"
#include "../Headers/IA.hpp"

Game::Game() {

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_EVENTS | SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Quoridor", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Screen::width, Screen::height, SDL_WindowFlags::SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RendererFlags::SDL_RENDERER_ACCELERATED);

    SDL_SetRenderDrawBlendMode(renderer, SDL_BlendMode::SDL_BLENDMODE_BLEND);

    turn = Turn::P1;

    for (Player &p : players) p = Player{std::numeric_limits<std::uint8_t>::max(), 0, 0, Team::WHITE};
    players[0] = Player{Case{4, 8}, 10, Team::WHITE};
    players[1] = Player{Case{4, 0}, 10, Team::BLACK};
    // players[2] = Player{Case{0, 4}, 5, Team::ORANGE};
    // players[3] = Player{Case{8, 4}, 5, Team::BLUE};

    commands.select = SelectMode::NONE;

    for (Barrier &b : barriers) b = Barrier{Case{std::numeric_limits<std::uint8_t>::max(), 0}, false};
    
    run = true;

}

Game::~Game() {

    if (renderer != nullptr) SDL_DestroyRenderer(renderer);
    if (window != nullptr) SDL_DestroyWindow(window);
    SDL_Quit();

}

void Game::Update() {

    SDL_PollEvent(&event);

    UpdateCommandsKeys(event, commands);
    UpdateCommandsMouse(event, commands);

    if (event.type == SDL_EventType::SDL_QUIT || event.type == SDL_WindowEventID::SDL_WINDOWEVENT_CLOSE) run = false;
    if (event.type == SDL_EventType::SDL_MOUSEWHEEL) commands.flip = !commands.flip;

    if (commands.mouse[SDL_BUTTON_LEFT]) {

        switch (commands.select) {

            case SelectMode::PLAYERS:

            if (players[static_cast<std::size_t>(turn)].pos.x > 0) {

                if (CaseHasPlayer(Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x-1), players[static_cast<std::size_t>(turn)].pos.y}, players) && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x-2), players[static_cast<std::size_t>(turn)].pos.y}, barriers)) {

                    if (players[static_cast<std::size_t>(turn)].pos.x-1 > 0 && AABB(SDL_Rect{commands.mousePos[0], commands.mousePos[1], 1, 1}, SDL_Rect{Screen::width/9*(players[static_cast<std::size_t>(turn)].pos.x-2), Screen::height/9*players[static_cast<std::size_t>(turn)].pos.y, Screen::width/9, Screen::height/9})) {

                        players[static_cast<std::size_t>(turn)].pos.x -= 2;
                        if (turn == Turn::P1) turn = Turn::P2; else turn = Turn::P1;
                        // if (turn == Turn::P4) turn = Turn::P1; else turn = Turn{static_cast<std::int32_t>(turn)+1};
                        commands.select = SelectMode::NONE;
                        break;

                    }

                } else {

                    if (AABB(SDL_Rect{commands.mousePos[0], commands.mousePos[1], 1, 1}, SDL_Rect{Screen::width/9*(players[static_cast<std::size_t>(turn)].pos.x-1), Screen::height/9*players[static_cast<std::size_t>(turn)].pos.y, Screen::width/9, Screen::height/9}) && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x-1), players[static_cast<std::size_t>(turn)].pos.y}, barriers)) {
            
                        players[static_cast<std::size_t>(turn)].pos.x--;
                        if (turn == Turn::P1) turn = Turn::P2; else turn = Turn::P1;
                        // if (turn == Turn::P4) turn = Turn::P1; else turn = Turn{static_cast<std::int32_t>(turn)+1};
                        commands.select = SelectMode::NONE;
                        break;

                    }

                }

            }

            if (players[static_cast<std::size_t>(turn)].pos.x < 9) {

                if (CaseHasPlayer(Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x+1), players[static_cast<std::size_t>(turn)].pos.y}, players) && players[static_cast<std::size_t>(turn)].pos.x+1 < 9 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x+2), players[static_cast<std::size_t>(turn)].pos.y}, barriers)) {

                    if (players[static_cast<std::size_t>(turn)].pos.x+1 < 9 && AABB(SDL_Rect{commands.mousePos[0], commands.mousePos[1], 1, 1}, SDL_Rect{Screen::width/9*(players[static_cast<std::size_t>(turn)].pos.x+2), Screen::height/9*players[static_cast<std::size_t>(turn)].pos.y, Screen::width/9, Screen::height/9})) {

                        players[static_cast<std::size_t>(turn)].pos.x += 2;
                        if (turn == Turn::P1) turn = Turn::P2; else turn = Turn::P1;
                        // if (turn == Turn::P4) turn = Turn::P1; else turn = Turn{static_cast<std::int32_t>(turn)+1};
                        commands.select = SelectMode::NONE;
                        break;
                    
                    }

                } else {

                    if (AABB(SDL_Rect{commands.mousePos[0], commands.mousePos[1], 1, 1}, SDL_Rect{Screen::width/9*(players[static_cast<std::size_t>(turn)].pos.x+1), Screen::height/9*players[static_cast<std::size_t>(turn)].pos.y, Screen::width/9, Screen::height/9}) && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x+1), players[static_cast<std::size_t>(turn)].pos.y}, barriers)) {
            
                        players[static_cast<std::size_t>(turn)].pos.x++;
                        if (turn == Turn::P1) turn = Turn::P2; else turn = Turn::P1;
                        // if (turn == Turn::P4) turn = Turn::P1; else turn = Turn{static_cast<std::int32_t>(turn)+1};
                        commands.select = SelectMode::NONE;
                        break;

                    }

                }
            
            }

            if (players[static_cast<std::size_t>(turn)].pos.y > 0) {

                if (CaseHasPlayer(Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y-1)}, players) && players[static_cast<std::size_t>(turn)].pos.y-1 > 0 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y-2)}, barriers)) {

                    if (players[static_cast<std::size_t>(turn)].pos.y-1 > 0 && AABB(SDL_Rect{commands.mousePos[0], commands.mousePos[1], 1, 1}, SDL_Rect{Screen::width/9*players[static_cast<std::size_t>(turn)].pos.x, Screen::height/9*(players[static_cast<std::size_t>(turn)].pos.y-2), Screen::width/9, Screen::height/9})) {

                        players[static_cast<std::size_t>(turn)].pos.y -= 2;
                        if (turn == Turn::P1) turn = Turn::P2; else turn = Turn::P1;
                        // if (turn == Turn::P4) turn = Turn::P1; else turn = Turn{static_cast<std::int32_t>(turn)+1};
                        commands.select = SelectMode::NONE;
                        break;

                    }

                } else {

                    if (AABB(SDL_Rect{commands.mousePos[0], commands.mousePos[1], 1, 1}, SDL_Rect{Screen::width/9*players[static_cast<std::size_t>(turn)].pos.x, Screen::height/9*(players[static_cast<std::size_t>(turn)].pos.y-1), Screen::width/9, Screen::height/9}) && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y-1)}, barriers)) {
            
                        players[static_cast<std::size_t>(turn)].pos.y--;
                        if (turn == Turn::P1) turn = Turn::P2; else turn = Turn::P1;
                        // if (turn == Turn::P4) turn = Turn::P1; else turn = Turn{static_cast<std::int32_t>(turn)+1};
                        commands.select = SelectMode::NONE;
                        break;

                    }

                }

            }

            if (players[static_cast<std::size_t>(turn)].pos.y < 9) {

                if (CaseHasPlayer(Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y+1)}, players) && players[static_cast<std::size_t>(turn)].pos.y+1 < 9 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y+2)}, barriers)) {

                    if (players[static_cast<std::size_t>(turn)].pos.y+1 < 9 && AABB(SDL_Rect{commands.mousePos[0], commands.mousePos[1], 1, 1}, SDL_Rect{Screen::width/9*players[static_cast<std::size_t>(turn)].pos.x, Screen::height/9*(players[static_cast<std::size_t>(turn)].pos.y+2), Screen::width/9, Screen::height/9})) {

                        players[static_cast<std::size_t>(turn)].pos.y += 2;
                        if (turn == Turn::P1) turn = Turn::P2; else turn = Turn::P1;
                        // if (turn == Turn::P4) turn = Turn::P1; else turn = Turn{static_cast<std::int32_t>(turn)+1};
                        commands.select = SelectMode::NONE;
                        break;

                    }

                } else {

                    if (AABB(SDL_Rect{commands.mousePos[0], commands.mousePos[1], 1, 1}, SDL_Rect{Screen::width/9*players[static_cast<std::size_t>(turn)].pos.x, Screen::height/9*(players[static_cast<std::size_t>(turn)].pos.y+1), Screen::width/9, Screen::height/9}) && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y+1)}, barriers)) {
            
                        players[static_cast<std::size_t>(turn)].pos.y++;
                        if (turn == Turn::P1) turn = Turn::P2; else turn = Turn::P1;
                        // if (turn == Turn::P4) turn = Turn::P1; else turn = Turn{static_cast<std::int32_t>(turn)+1};
                        commands.select = SelectMode::NONE;
                        break;

                    }

                }

            }

            commands.select = SelectMode::NONE;

            break;

            case SelectMode::BARRIERS:
            {

                Barrier mouse{std::uint8_t(commands.mousePos[0]/(Screen::width/9)), std::uint8_t(commands.mousePos[1]/(Screen::width/9)), commands.flip};
                if (mouse.orientation) {

                    mouse.pos.x += (commands.mousePos[0]/(Screen::width/9) == 0);
                    mouse.pos.y -= (commands.mousePos[1]/(Screen::height/9) == 8);

                } else {

                    mouse.pos.x -= (commands.mousePos[0]/(Screen::width/9) == 8);
                    mouse.pos.y += (commands.mousePos[1]/(Screen::height/9) == 0);

                }

                if (players[static_cast<std::size_t>(turn)].barriersCount > 0 && !BarriersCollideInArray(mouse, barriers) && CanAccessLine(players[0].pos, true, &PreviewAddBarrierInArray(barriers, players, mouse)[0]) && CanAccessLine(players[1].pos, false, &PreviewAddBarrierInArray(barriers, players, mouse)[0])) {

                    barriers[20-players[0].barriersCount-players[1].barriersCount-players[2].barriersCount-players[3].barriersCount] = mouse;
                    players[static_cast<std::size_t>(turn)].barriersCount--;
                    if (turn == Turn::P1) turn = Turn::P2; else turn = Turn::P1;
                    // if (turn == Turn::P4) turn = Turn::P1; else turn = Turn{static_cast<std::int32_t>(turn)+1};
                    commands.select = SelectMode::NONE;

                }

            }
            break;

            case SelectMode::NONE:
            if (AABB(SDL_Rect{commands.mousePos[0], commands.mousePos[1], 1, 1}, SDL_Rect{players[static_cast<std::size_t>(turn)].pos.x*Screen::width/9, players[static_cast<std::size_t>(turn)].pos.y*Screen::height/9, Screen::width/9, Screen::height/9})) commands.select = SelectMode::PLAYERS; else commands.select = SelectMode::BARRIERS;
            break;

        }

        commands.mouse[SDL_BUTTON_LEFT] = false;

    }

    if (commands.keys[SDL_KeyCode::SDLK_ESCAPE] || commands.mouse[SDL_BUTTON_RIGHT]) commands.select = SelectMode::NONE;

    event = SDL_Event{};

}

void Game::Render() const {

    SDL_RenderClear(renderer);

    if (commands.select == SelectMode::PLAYERS) {

        SDL_SetRenderDrawColor(renderer, std::uint8_t{255}, std::uint8_t{0}, std::uint8_t{0}, std::uint8_t{255});
        if (players[static_cast<std::size_t>(turn)].pos.x > 0 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x-1), players[static_cast<std::size_t>(turn)].pos.y}, barriers)) {

            SDL_Rect rect{Screen::width/9*(players[static_cast<std::size_t>(turn)].pos.x-1), Screen::height/9*players[static_cast<std::size_t>(turn)].pos.y, Screen::width/9, Screen::height/9};
            if (CaseHasPlayer(Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x-1), players[static_cast<std::size_t>(turn)].pos.y}, players) && players[static_cast<std::size_t>(turn)].pos.x-1 > 0 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x-2), players[static_cast<std::size_t>(turn)].pos.y}, barriers)) rect.x -= Screen::width/9;

            SDL_RenderFillRect(renderer, &rect);

        }

        if (players[static_cast<std::size_t>(turn)].pos.x < 9 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x+1), players[static_cast<std::size_t>(turn)].pos.y}, barriers)) {
            
            SDL_Rect rect{Screen::width/9*(players[static_cast<std::size_t>(turn)].pos.x+1), Screen::height/9*players[static_cast<std::size_t>(turn)].pos.y, Screen::width/9, Screen::height/9};
            if (CaseHasPlayer(Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x+1), players[static_cast<std::size_t>(turn)].pos.y}, players) && players[static_cast<std::size_t>(turn)].pos.x+1 < 9 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.x+2), players[static_cast<std::size_t>(turn)].pos.y}, barriers)) rect.x += Screen::width/9;

            SDL_RenderFillRect(renderer, &rect);

        }

        if (players[static_cast<std::size_t>(turn)].pos.y > 0 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y-1)}, barriers)) {
            
            SDL_Rect rect{Screen::width/9*players[static_cast<std::size_t>(turn)].pos.x, Screen::height/9*(players[static_cast<std::size_t>(turn)].pos.y-1), Screen::width/9, Screen::height/9};
            if (CaseHasPlayer(Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y-1)}, players) && players[static_cast<std::size_t>(turn)].pos.y-1 > 0 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y-2)}, barriers)) rect.y -= Screen::height/9;

            SDL_RenderFillRect(renderer, &rect);

        }

        if (players[static_cast<std::size_t>(turn)].pos.y < 9 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y+1)}, barriers)) {
            
            SDL_Rect rect{Screen::width/9*players[static_cast<std::size_t>(turn)].pos.x, Screen::height/9*(players[static_cast<std::size_t>(turn)].pos.y+1), Screen::width/9, Screen::height/9};
            if (CaseHasPlayer(Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y+1)}, players) && players[static_cast<std::size_t>(turn)].pos.y+1 < 9 && !CasesHasBarrier(players[static_cast<std::size_t>(turn)].pos, Case{players[static_cast<std::size_t>(turn)].pos.x, static_cast<std::uint8_t>(players[static_cast<std::size_t>(turn)].pos.y+2)}, barriers)) rect.y += Screen::height/9;

            SDL_RenderFillRect(renderer, &rect);

        }

    }

    SDL_SetRenderDrawColor(renderer, std::uint8_t{0}, std::uint8_t{0}, std::uint8_t{0}, std::uint8_t{255});
    for (unsigned char x{0}; x < 10; x++) SDL_RenderDrawLine(renderer, 0, Screen::height/9*x-1*(x == 9), Screen::width, Screen::height/9*x-1*(x == 9));
    for (unsigned char y{0}; y < 10; y++) SDL_RenderDrawLine(renderer, Screen::width/9*y-1*(y == 9), 0, Screen::width/9*y-1*(y == 9), Screen::height);

    for (const Player &p : players) {

        switch (p.team) {

            case Team::WHITE:
            SDL_SetRenderDrawColor(renderer, std::uint8_t{255}, std::uint8_t{255}, std::uint8_t{255}, std::uint8_t{255});
            break;

            case Team::BLACK:
            SDL_SetRenderDrawColor(renderer, std::uint8_t{0}, std::uint8_t{0}, std::uint8_t{0}, std::uint8_t{255});
            break;

            case Team::ORANGE:
            SDL_SetRenderDrawColor(renderer, std::uint8_t{255}, std::uint8_t{255}, std::uint8_t{0}, std::uint8_t{255});
            break;

            case Team::BLUE:
            SDL_SetRenderDrawColor(renderer, std::uint8_t{0}, std::uint8_t{0}, std::uint8_t{255}, std::uint8_t{255});
            break;

        }

        SDL_Rect rect{Screen::width/9*p.pos.x+1, Screen::height/9*p.pos.y+1, Screen::width/9-1, Screen::height/9-1};
        SDL_RenderFillRect(renderer, &rect);

    }

    SDL_SetRenderDrawColor(renderer, std::uint8_t{245}, std::uint8_t{160}, std::uint8_t{65}, std::uint8_t{255});

    for (const Barrier &b : barriers) {

        SDL_Rect rect{Screen::width/9*b.pos.x, Screen::height/9*b.pos.y, 0, 0};

        if (b.orientation) {

            rect.h = Screen::height/9*2;
            rect.w = Screen::width/45;

            rect.x -= Screen::width/90;

        } else {

            rect.w = Screen::width/9*2;
            rect.h = Screen::height/45;

            rect.y -= Screen::height/90;

        } 

        SDL_RenderFillRect(renderer, &rect);

    }

    if (commands.select == SelectMode::BARRIERS) {

        if (players[static_cast<std::size_t>(turn)].barriersCount > 0) SDL_SetRenderDrawColor(renderer, std::uint8_t{245}, std::uint8_t{160}, std::uint8_t{65}, std::uint8_t{100}); else SDL_SetRenderDrawColor(renderer, std::uint8_t{255}, std::uint8_t{0}, std::uint8_t{0}, std::uint8_t{100});

        Barrier preview{std::uint8_t(commands.mousePos[0]/(Screen::width/9)), std::uint8_t(commands.mousePos[1]/(Screen::width/9)), commands.flip};
        if (preview.orientation) {

            preview.pos.x += (commands.mousePos[0]/(Screen::width/9) == 0);
            preview.pos.y -= (commands.mousePos[1]/(Screen::height/9) == 8);

        } else {

            preview.pos.x -= (commands.mousePos[0]/(Screen::width/9) == 8);
            preview.pos.y += (commands.mousePos[1]/(Screen::height/9) == 0);

        }

        if (BarriersCollideInArray(preview, barriers) || !CanAccessLine(players[0].pos, true, &PreviewAddBarrierInArray(barriers, players, preview)[0]) || !CanAccessLine(players[1].pos, false, &PreviewAddBarrierInArray(barriers, players, preview)[0])) SDL_SetRenderDrawColor(renderer, std::uint8_t{255}, std::uint8_t{0}, std::uint8_t{0}, std::uint8_t{100});

        SDL_Rect rect{Screen::width/9*preview.pos.x, Screen::height/9*preview.pos.y, 0, 0};

        if (commands.flip) {

            rect.h = Screen::height/9*2;
            rect.w = Screen::width/45;

            rect.x -= Screen::width/90;

        } else {

            rect.w = Screen::width/9*2;
            rect.h = Screen::height/45;

            rect.y -= Screen::height/90;

        }

        SDL_RenderFillRect(renderer, &rect);

    }

    SDL_SetRenderDrawColor(renderer, std::uint8_t{140}, std::uint8_t{0}, std::uint8_t{50}, std::uint8_t{255});
    SDL_RenderPresent(renderer);

}