#pragma once

#include <stack>
#include <string>

#include "card.hpp"

namespace ariel {
class Player {
   private:
    std::string player_name_;
    std::stack<Card> stack_;
    int cards_taken_;
    bool player_in_game_;

    int rounds_won_;
    int num_of_ties_;

   public:
    Player(std::string name);
    void reset();
    void addCard(const Card& card);
    void incCardsTaken();
    Card drawCard();
    int stacksize() const;
    int cardesTaken();
    void startGame();
    void finishGame();
    bool isInGame();
    std::string getName();
    void wins(int cards_taken);
    void tie();
    int getNumOfTies();
    int getNumOfRoundsWon();

    bool operator==(const Player& other) const {
        return player_name_ == other.player_name_;
    }

    bool operator!=(const Player& other) const {
        return player_name_ != other.player_name_;
    }
};
}  // namespace ariel
