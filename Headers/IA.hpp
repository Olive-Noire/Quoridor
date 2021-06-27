#ifndef DEF_IA_HPP
#define DEF_IA_HPP

#include <SDL_rect.h>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include <cmath>
#include <deque>

#include "./Player.hpp"

template<typename Type>
inline bool DequeHasValue(const std::deque<Type> &contener, Type value) {

    return std::find(contener.begin(), contener.end(), value) != contener.end();

}

inline bool AABB(const SDL_Rect &a, const SDL_Rect &b) {

    return a.x+a.w >= b.x && a.x+a.w <= b.x+b.w && a.y+a.h >= b.y && a.y+a.h <= b.y+b.h;

}

template<std::size_t Size>
inline bool CaseHasPlayer(Case c, const Player players[Size]) {

    for (std::size_t i{0}; i < Size; i++) if (players[i].pos.x == c.x && players[i].pos.y == c.y) return true;
    return false;

}

template<std::size_t Size>
inline bool CasesHasBarrier(Case a, Case b, const Barrier barriers[Size]) {

    for (std::size_t i{0}; i < Size; i++) {

        if ((barriers[i].pos.x > a.x && barriers[i].pos.x <= b.x && barriers[i].orientation && a.y == b.y && (std::abs(barriers[i].pos.y-a.y) <= 1 && barriers[i].pos.y <= a.y)) ||
            (barriers[i].pos.y > a.y && barriers[i].pos.y <= b.y && !barriers[i].orientation && a.x == b.x && (std::abs(barriers[i].pos.x-a.x) <= 1 && barriers[i].pos.x <= a.x)) ||
            (barriers[i].pos.x <= a.x && barriers[i].pos.x > b.x && barriers[i].orientation && a.y == b.y && (std::abs(barriers[i].pos.y-a.y) <= 1 && barriers[i].pos.y <= a.y)) ||
            (barriers[i].pos.y <= a.y && barriers[i].pos.y > b.y && !barriers[i].orientation && a.x == b.x && (std::abs(barriers[i].pos.x-a.x) <= 1 && barriers[i].pos.x <= a.x))
        
        ) return true;

    }

    return false;

}

inline bool BarrierCollide(const Barrier &a, const Barrier &b) {

    if (a.orientation == b.orientation) {

        if (a.pos.x == b.pos.x && a.pos.y == b.pos.y) return true;
        
        if (a.orientation) {
            
            if (a.pos.x == b.pos.x && std::abs(a.pos.y-b.pos.y) == 1) return true;
            
        } else if (a.pos.y == b.pos.y && std::abs(a.pos.x-b.pos.x) == 1) return true;

    } else {

        if (!a.orientation && b.pos.y == a.pos.y-1 && a.pos.x == b.pos.x-1) return true;
        if (!b.orientation && a.pos.y == b.pos.y-1 && b.pos.x == a.pos.x-1) return true;

    }

    return false;

}

template<std::size_t Size>
inline bool BarriersCollideInArray(const Barrier &b, const Barrier barriers[Size]) {

    for (std::size_t i{0}; i < Size; i++) if (BarrierCollide(b, barriers[i])) return true;
    return false;

}

template<std::size_t Size>
inline bool CanAccessCases(Case from, Case To, const Barrier barriers[Size]) {

    std::deque<Case> checks, pass;
    checks.push_front(from);

    while (!checks.empty()) {

        if (checks.back() == To) return true;

        Case c{static_cast<std::uint8_t>(checks.back().x-1), checks.back().y};

        if (checks.back().x > 0 && !CasesHasBarrier<Size>(checks.back(), c, barriers) && !DequeHasValue(checks, c) && !DequeHasValue(pass, c)) checks.push_front(c);
        c.x+=2;
        if (checks.back().x < 8 && !CasesHasBarrier<Size>(checks.back(), c, barriers) && !DequeHasValue(checks, c) && !DequeHasValue(pass, c)) checks.push_front(c);
        c.x--;
        c.y--;
        if (checks.back().y > 0 && !CasesHasBarrier<Size>(checks.back(), c, barriers) && !DequeHasValue(checks, c) && !DequeHasValue(pass, c)) checks.push_front(c);
        c.y+=2;
        if (checks.back().y < 8 && !CasesHasBarrier<Size>(checks.back(), c, barriers) && !DequeHasValue(checks, c) && !DequeHasValue(pass, c)) checks.push_front(c);

        pass.push_front(checks.back());
        checks.pop_back();

    }

    return false;

}

template<std::size_t Size>
inline bool CanAccessLine(Case c, bool line, const Barrier barriers[Size]) {

    for (std::uint8_t i{0}; i < 9; i++) if (CanAccessCases<Size>(c, Case{i, static_cast<std::uint8_t>(8*(!line))}, barriers)) return true;
    return false;

}

template<std::size_t BarriersSize, std::size_t PlayersSize>
std::array<Barrier, BarriersSize> PreviewAddBarrierInArray(const Barrier barriers[BarriersSize], const Player players[PlayersSize], const Barrier &add) {

    std::uint8_t barriersCount{0};
    for (std::size_t i{0}; i < PlayersSize; i++) barriersCount += players[i].barriersCount;

    std::array<Barrier, BarriersSize> result;
    for (std::size_t i{0}; i < BarriersSize; i++) result[i] = barriers[i];

    if (barriersCount > 0) result[BarriersSize-barriersCount] = add;

    return result;

}

#endif // DEF_IA_HPP