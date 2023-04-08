#pragma once

#include <string>
#include <vector>

#include "player.hpp"

enum class GameStatus {
    NOT_STARTED,  // in case game didn't start or one of the players already in another game or he's playing with himself.
    STARTED,
    FINISHED,
};

enum class Winner {
    PLAYER_1,  // in case game didn't start or one of the players already in another game or he's playing with himself.
    PLAYER_2,
    TIE,
};

namespace ariel {
class Game {
   private:
    std::vector<Card> deck_;
    Player& player1_;
    Player& player2_;
    GameStatus game_status_ = GameStatus::NOT_STARTED;

    std::vector<std::string> log_;
    std::string turn_log_;

    void war(Card& card1, Card& card2);
    void endGame(Winner winner);

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
