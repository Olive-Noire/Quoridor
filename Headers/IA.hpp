#ifndef DEF_IA_HPP
#define DEF_IA_HPP

#include <SDL_rect.h>
#include <array>
#include <deque>

#include "./Player.hpp"

bool DequeHasCase(const std::deque<Case> &contener, Case c);
bool AABB(const SDL_Rect &a, const SDL_Rect &b);
bool CaseHasPlayer(Case c, const Player players[4]);
bool CasesHasBarrier(Case a, Case b, const Barrier barriers[20]);
bool BarrierCollide(const Barrier &a, const Barrier &b);
bool BarriersCollideInArray(const Barrier &b, const Barrier barriers[20]);
bool CanAccessCases(Case from, Case To, const Barrier barriers[20]);
bool CanAccessLine(Case c, bool line, const Barrier barriers[20]);

std::array<Barrier, 20> PreviewAddBarrierInArray(const Barrier barriers[20], const Player players[4], const Barrier &add);

#endif // DEF_IA_HPP