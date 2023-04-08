#pragma once

#include <stack>
#include <string>

#include "card.hpp"

namespace ariel {
class Player {
   private:
    size_t player_id_;  // TODO: implemet this
    static size_t next_id_;
    std::string player_name_;
    std::stack<Card> stack_;
    int cards_taken_;
    bool player_in_game_;

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

    bool operator==(const Player& other) const {
        return player_id_ == other.player_id_;
    }

    bool operator!=(const Player& other) const {
        return player_id_ != other.player_id_;
    }
};
}  // namespace ariel
