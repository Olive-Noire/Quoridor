#ifndef DEF_PLAYER_HPP
#define DEF_PLAYER_HPP

#include <cstdint>

enum class Team{WHITE=0, BLACK, ORANGE, BLUE};

struct Case {

    std::uint8_t x, y;

    friend bool operator==(const Case &l, const Case &r) { return l.x == r.x && l.y == r.y; }

};

struct Barrier {

    public:

    Case pos;
    bool orientation;

};

struct Player {

    public:

    Case pos;
    std::uint8_t barriersCount;
    Team team;

};

#endif // DEF_PLAYER_HPP