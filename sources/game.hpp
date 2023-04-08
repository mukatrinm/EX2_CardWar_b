#pragma once

#include <vector>

#include "player.hpp"

enum class GameStatus {
    NOT_STARTED,  // in case game didn't start or one of the players already in another game or he's playing with himself.
    STARTED,
    FINISHED,
};

namespace ariel {
class Game {
   private:
    std::vector<Card> deck_;
    Player& player1_;
    Player& player2_;
    GameStatus game_status_ = GameStatus::NOT_STARTED;

   public:
    Game(Player& p1, Player& p2);

    void playTurn();
    void printLastTurn();
    void playAll();
    void printWiner();
    void printLog();
    void printStats();
};
}  // namespace ariel
