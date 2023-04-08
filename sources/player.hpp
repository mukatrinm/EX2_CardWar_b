#pragma once

#include <stack>
#include <string>

#include "card.hpp"

namespace ariel {
class Player {
   private:
    size_t player_id_;  // TODO: implemet this
    std::string player_name_;
    std::stack<Card> stack_;
    bool player_in_game_ = false;

   public:
    Player(std::string name);

    void addCard(const Card& card);
    int stacksize() const;
    int cardesTaken();
    void startGame();
    void finishGame();
    bool isInGame();

    bool operator==(const Player& other) const {
        return player_id_ == other.player_id_;
    }

    bool operator!=(const Player& other) const {
        return player_id_ != other.player_id_;
    }
};
}  // namespace ariel
