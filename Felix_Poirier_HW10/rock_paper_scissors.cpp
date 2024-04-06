#include <cstdio>
#include <random> 
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

enum class Hand {
    ROCK,
    PAPER,
    SCISSORS,
    EMPTY
};

std::string handToString(Hand hand) {
    switch (hand) {
        case Hand::ROCK:
            return "\U0001FAA8";
        case Hand::PAPER:
            return "\U0001F4C3";
        case Hand::SCISSORS:
            return "\u2702";
        case Hand::EMPTY:
            return "EMPTY";
        default:
            return "UNKNOWN";
    }
}

enum class Result {
    WIN,
    LOSE,
    DRAW
};

std::atomic<bool> game_ongoing(false);
std::atomic<short> current_round(0);

class Player {
    Hand hand{Hand::EMPTY};
    short round{0};

public:
    std::string name;
    int wins{0};
    std::atomic_flag is_playing = ATOMIC_FLAG_INIT;

    Player(std::string n = "Player") : name(n) {}
    Player(const Player& p) : name(p.name), wins(p.wins) {}

    void play_hands() {
        std::random_device rnd;
        std::mt19937 mt(rnd());
        std::uniform_int_distribution<> dist(0, 2);

        while (game_ongoing) {
            if (round != current_round) {
				std::cout << name << " is playing round " << current_round << std::endl;
                is_playing.test_and_set();
                hand = static_cast<Hand>(dist(mt));
                ++round;
                is_playing.clear();
            }

        }
    }

    Hand get_hand() const {
        return hand;
    }
};

Result evaluate_game(Player& p1, Player& p2) {
    switch (p1.get_hand()) {
        case Hand::ROCK:
            if (p2.get_hand() == Hand::SCISSORS)
                return Result::WIN;
            else if (p2.get_hand() == Hand::PAPER)
                return Result::LOSE;
            break;
        case Hand::PAPER:
            if (p2.get_hand() == Hand::ROCK)
                return Result::WIN;
            else if (p2.get_hand() == Hand::SCISSORS)
                return Result::LOSE;
            break;
        case Hand::SCISSORS:
            if (p2.get_hand() == Hand::PAPER)
                return Result::WIN;
            else if (p2.get_hand() == Hand::ROCK)
                return Result::LOSE;
            break;
    };
    return Result::DRAW;
}

class Judge {
    Player& player1;
    Player& player2;

public:
    Judge(Player& p1, Player& p2) : player1(p1), player2(p2) {}

    Result play_round() {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        player1.is_playing.wait(true);
        player2.is_playing.wait(true);
        std::cout << "\t" << player1.name << ": " << handToString(player1.get_hand()) << "  vs. " <<
            player2.name << ": " << handToString(player2.get_hand()) << std::endl;
        return evaluate_game(player1, player2);
    }
};

int main() {
    Player p1{"George"}, p2{"Felix"};
    Judge judge(p1, p2);

    game_ongoing = true;

    std::thread t1(&Player::play_hands, std::ref(p1));
    std::thread t2(&Player::play_hands, std::ref(p2));

    for (int i = 0; i < 10; ++i) {
        std::cout <<"Playing round " << i+1 << std::endl;
        Result result = judge.play_round();
        std::cout << "\t";
        switch(result) {
            case Result::WIN:
                std::cout << p1.name << " wins!" << std::endl;
                ++p1.wins;
                break;
            case Result::LOSE:
                std::cout << p2.name << " wins!" << std::endl;
                ++p2.wins;
                break;
            case Result::DRAW:
                std::cout << "It's a draw!" << std::endl;
                break;
        }
        ++current_round;
    }

    game_ongoing = false;

    t1.join();
    t2.join();

    if (p1.wins == p2.wins) {
        std::cout << "It's a draw!" << std::endl;
    } else {
        Player& winner = (p1.wins > p2.wins) ? p1 : p2;
        Player& loser = (p1.wins < p2.wins) ? p1 : p2;
        std::cout << winner.name << " wins the game " << winner.wins << " to " << loser.wins << std::endl;
    }

    return 0;
}


