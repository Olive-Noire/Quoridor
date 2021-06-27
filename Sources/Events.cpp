#include "../Headers/Events.hpp"

void UpdateCommandsKeys(const SDL_Event &event, Commands &commands) {

    switch (event.type) {

		case SDL_EventType::SDL_KEYDOWN:
        case SDL_EventType::SDL_KEYUP:

            commands.keys[event.key.keysym.sym] = (event.type == SDL_EventType::SDL_KEYDOWN);

		break;

		default:
		break;

	}

}

void UpdateCommandsMouse(const SDL_Event &event, Commands &commands) {

    switch(event.type) {

        case SDL_EventType::SDL_MOUSEBUTTONDOWN:
        commands.mouse[event.button.button] = true;
        break;

        case SDL_EventType::SDL_MOUSEBUTTONUP:
        commands.mouse[event.button.button] = false;
        break;

        case SDL_EventType::SDL_MOUSEMOTION:
        commands.mousePos[0] = event.motion.x;
        commands.mousePos[1] = event.motion.y;
        break;

        default:
        break;

    }

}