#include "card.hpp"

ariel::Card::Card(Rank rank, Suit suit)
    : rank_(rank), suit_(suit) {}

std::string ariel::Card::toString() const {
    std::string str;
    switch (rank_) {
        case Rank::ACE:
            str += "Ace";
            break;
        case Rank::TWO:
            str += "2";
            break;
        case Rank::THREE:
            str += "3";
            break;
        case Rank::FOUR:
            str += "4";
            break;
        case Rank::FIVE:
            str += "5";
            break;
        case Rank::SIX:
            str += "6";
            break;
        case Rank::SEVEN:
            str += "7";
            break;
        case Rank::EIGHT:
            str += "8";
            break;
        case Rank::NINE:
            str += "9";
            break;
        case Rank::TEN:
            str += "10";
            break;
        case Rank::JACK:
            str += "Jack";
            break;
        case Rank::QUEEN:
            str += "Queen";
            break;
        case Rank::KING:
            str += "King";
            break;
        default:
            break;
    }

    switch (suit_) {
        case Suit::SPADES:
            str += " of Spades";
            break;
        case Suit::HEARTS:
            str += " of Hearts";
            break;
        case Suit::DIAMONDS:
            str += " of Diamonds";
            break;
        case Suit::CLUBS:
            str += " of Clubs";
            break;
        default:
            break;
    }

    return str;
}
