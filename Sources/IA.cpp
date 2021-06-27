#include "../Headers/IA.hpp"

#include <algorithm>
#include <cstdint>
#include <cmath>

bool DequeHasCase(const std::deque<Case> &contener, Case c) {

    return std::find(contener.begin(), contener.end(), c) != contener.end();

}

bool AABB(const SDL_Rect &a, const SDL_Rect &b) {

    return a.x+a.w >= b.x && a.x+a.w <= b.x+b.w && a.y+a.h >= b.y && a.y+a.h <= b.y+b.h;

}

bool CaseHasPlayer(Case c, const Player players[4]) {

    for (std::size_t i{0}; i < 4; i++) if (players[i].pos.x == c.x && players[i].pos.y == c.y) return true;
    return false;

}

bool CasesHasBarrier(Case a, Case b, const Barrier barriers[20]) {

    for (std::size_t i{0}; i < 20; i++) {

        if ((barriers[i].pos.x > a.x && barriers[i].pos.x <= b.x && barriers[i].orientation && a.y == b.y && (std::abs(barriers[i].pos.y-a.y) <= 1 && barriers[i].pos.y <= a.y)) ||
            (barriers[i].pos.y > a.y && barriers[i].pos.y <= b.y && !barriers[i].orientation && a.x == b.x && (std::abs(barriers[i].pos.x-a.x) <= 1 && barriers[i].pos.x <= a.x)) ||
            (barriers[i].pos.x <= a.x && barriers[i].pos.x > b.x && barriers[i].orientation && a.y == b.y && (std::abs(barriers[i].pos.y-a.y) <= 1 && barriers[i].pos.y <= a.y)) ||
            (barriers[i].pos.y <= a.y && barriers[i].pos.y > b.y && !barriers[i].orientation && a.x == b.x && (std::abs(barriers[i].pos.x-a.x) <= 1 && barriers[i].pos.x <= a.x))
        
        ) return true;

    }

    return false;

}

bool BarrierCollide(const Barrier &a, const Barrier &b) {

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

bool BarriersCollideInArray(const Barrier &b, const Barrier barriers[20]) {

    for (std::size_t i{0}; i < 20; i++) if (BarrierCollide(b, barriers[i])) return true;
    return false;

}

bool CanAccessCases(Case from, Case To, const Barrier barriers[20]) {

    std::deque<Case> checks, pass;
    checks.push_front(from);

    while (!checks.empty()) {

        if (checks.back() == To) return true;

        Case c{static_cast<std::uint8_t>(checks.back().x-1), checks.back().y};

        if (checks.back().x > 0 && !CasesHasBarrier(checks.back(), c, barriers) && !DequeHasCase(checks, c) && !DequeHasCase(pass, c)) checks.push_front(c);
        c.x+=2;
        if (checks.back().x < 8 && !CasesHasBarrier(checks.back(), c, barriers) && !DequeHasCase(checks, c) && !DequeHasCase(pass, c)) checks.push_front(c);
        c.x--;
        c.y--;
        if (checks.back().y > 0 && !CasesHasBarrier(checks.back(), c, barriers) && !DequeHasCase(checks, c) && !DequeHasCase(pass, c)) checks.push_front(c);
        c.y+=2;
        if (checks.back().y < 8 && !CasesHasBarrier(checks.back(), c, barriers) && !DequeHasCase(checks, c) && !DequeHasCase(pass, c)) checks.push_front(c);

        pass.push_front(checks.back());
        checks.pop_back();

    }

    return false;

}

bool CanAccessLine(Case c, bool line, const Barrier barriers[20]) {

    for (std::uint8_t i{0}; i < 9; i++) if (CanAccessCases(c, Case{i, static_cast<std::uint8_t>(8*(!line))}, barriers)) return true;
    return false;

}

std::array<Barrier, 20> PreviewAddBarrierInArray(const Barrier barriers[20], const Player players[4], const Barrier &add) {

    std::uint8_t barriersCount{0};
    for (std::size_t i{0}; i < 4; i++) barriersCount += players[i].barriersCount;

    std::array<Barrier, 20> result;
    for (std::size_t i{0}; i < 20; i++) result[i] = barriers[i];

    if (barriersCount > 0) result[20-barriersCount] = add;

    return result;

}