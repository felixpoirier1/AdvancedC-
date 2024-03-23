#include <cstdio>
#include <random> 
#include <thread>
#include <atomic>
#include <iostream>
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>

enum class Hand{
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

enum class Result{
	WIN,
	LOSE,
	DRAW
};

// this is a global variable that will be used to signal the start and end of the game
std::atomic<bool> game_ongoing(false);
// this is a global variable that will be used to keep track of the current round
std::atomic<short> current_round(0);

class Player{
	// the player has a private member variable to store the hand they played
	Hand hand{Hand::EMPTY};
	// the player has a private member variable to store the last round they played
	short round{0};
public:
	std::string name;
	int wins{0};
	// the player has a private member variable to indicate if they are currently playing a hand
    std::atomic_flag is_playing{false};

	Player(std::string n = "Player"): name(n){}
	//copy constructor
	Player(const Player& p): name(p.name), wins(p.wins){}
	void play_hands(){
		// create a random number generator (unique for each player)
		std::random_device rnd;
		std::mt19937 mt(rnd());
		std::uniform_int_distribution<> dist(0, 2);
		while(game_ongoing){
			// if the player has not played the current round, play a hand
			if(round != current_round){
				is_playing.test_and_set();
				hand = static_cast<Hand>(dist(mt));
				++round;
				is_playing.clear();
				// notify the judge that the player has played a hand
				is_playing.notify_one();
			}
		}
	}

	Hand get_hand() const {
		return hand;
	}

};

Result evaluate_game(Player& p1, Player& p2){
		switch(p1.get_hand()){
			case Hand::ROCK:
				if(p2.get_hand() == Hand::SCISSORS)
					return Result::WIN;
				else if(p2.get_hand() == Hand::PAPER)
					return Result::LOSE;
				break;
			case Hand::PAPER:
				if(p2.get_hand() == Hand::ROCK)
				    return Result::WIN;
				else if(p2.get_hand() == Hand::SCISSORS)
					return Result::LOSE;
				break;
			case Hand::SCISSORS:
				if (p2.get_hand() == Hand::PAPER)
					return Result::WIN;
				else if(p2.get_hand() == Hand::ROCK)
					return Result::LOSE;			
				break;
		};
		return Result::DRAW;
}

class Judge{
	Player& player1; // Result objects (returned by play_round()) are in relationship with player1 always
	Player& player2;

public:
	Judge(Player &p1, Player &p2): player1(p1), player2(p2) {}
	Result play_round(){
		std::printf("\tRock-Paper-Scissors Go!\n");
		++current_round;
		// sleep for 10 milliseconds to allow players to play their hands
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		// wait for both players to finish playing their hands
		player1.is_playing.wait(true);
		player2.is_playing.wait(true);
		std::cout << "\t" << player1.name << ": " << handToString(player1.get_hand()) << "  v.s. " <<
		player2.name << ": " << handToString(player2.get_hand()) << std::endl;
		Result result = evaluate_game(player1, player2);
		switch(result){
			case Result::WIN:
				player1.wins++;
				break;
			case Result::LOSE:
				player2.wins++;
				break;
			case Result::DRAW:
				break;
		}
		return result;
	}
};

int main(){
	// create two players and a judge
	Player p1{"George"}, p2{"Felix"};
	Judge judge(p1, p2);

	// start the game
	game_ongoing = true;

	// create two threads, one for each player
	std::thread t1(&Player::play_hands, std::ref(p1));
	std::thread t2(&Player::play_hands, std::ref(p2));

	// play 10 rounds
	for(int i = 0; i < 10; ++i){
		std::cout <<"Playing round " << i+1 << std::endl;
		Result result = judge.play_round();
		std::cout << "\t";
		switch(result){
			case Result::WIN:
				std::cout << p1.name << " wins!"<<std::endl;
				break;
			case Result::LOSE:
				std::cout << p2.name <<" wins!"<<std::endl;
				break;
			case Result::DRAW:
				std::cout<<"It's a draw!"<<std::endl;
				break;
		}
	}
	// end the game
	game_ongoing = false;
	// wait for the threads to finish (should be almost instantaneous)
	t1.join();
	t2.join();

	// print the final result
	if (p1.wins == p2.wins){
		std::cout << "It's a draw!" << std::endl;
	}
	else{
		Player winner = p1.wins > p2.wins ? p1 : p2;
		Player loser = p1.wins < p2.wins ? p1 : p2;
		std::cout << winner.name << " wins the game " << winner.wins << " to " << loser.wins << std::endl;
	}
	return 0;
}
