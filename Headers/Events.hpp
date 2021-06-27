#ifndef DEF_EVENTS_HPP
#define DEF_EVENTS_HPP

#include "./Game.hpp"

void UpdateCommandsKeys(const SDL_Event &event, Commands &commands);
void UpdateCommandsMouse(const SDL_Event &event, Commands &commands);

#endif // DEF_EVENTS_HPP