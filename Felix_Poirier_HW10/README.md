# Homework 10

GITHUB LINK: https://github.com/felixpoirier1/AdvancedCPP/tree/main/Felix_Poirier_HW10
## Question 1

Here is the command I use to compile the program (I am using C++20):
```bash
g++ -std=c++20 rock_paper_scissors.cpp -o rock_paper_scissors
```

In this question, I wrote a class-based program, where I define both the `Player` and `Judge` classes. Two `Player` based objects are then assigned an individual thread on their `play_hands()` method. 
A single `Judge` object is then created, and the `play_round` method from the `Judge` object runs on the main thread 10 times as required.
The way in which parallelism is achieved is by using an atomic count variable (`atomic<int>`) to keep track of the number of rounds played. This variable is incremented by the `play_round` method, and the `play_hands` method of the `Player` class awaits a difference between the internal `round` variable and the atomic count variable before proceeding to the next round.
The judge thread knows when a round is over when both players' internal `std::atomic_flag is_playing` is set to false. 

An issue I found with my program is the `play_round` method in the `Judge` class. I had to add a 10 millisecond sleep to allow the players to play their hands before the judge checks if the players are done playing. This is because the judge thread is running on the main thread, and the player threads are running on separate threads. The judge thread would check if the players are done playing before the players even had a chance to play their hands. This is why I added the sleep to allow the players to play their hands before the judge checks if they are done playing. I am not sure if this is the best way to solve this issue, it feels like a suboptimal solution. Advice on how to solve this issue would be appreciated!

```cpp
std::printf("\tRock-Paper-Scissors Go!\n");
++current_round;
// sleep for 10 milliseconds to allow players to play their hands
std::this_thread::sleep_for(std::chrono::milliseconds(10));
// wait for both players to finish playing their hands
player1.is_playing.wait(true);
player2.is_playing.wait(true);
```

## Question 2

*What is something you would like to see added to C++?*

This is a pretty silly request, but oftenhand when I write more complex programs I like to have configuration files. When using Python, I can usually simply use the built-in `json` library, or `pyYAML` if I feel fancy. However in C++, there's no simple tool that allows me to read and write to these hierarchical types of files. It would be nice to have a header that gives developers the ability to read these files and store their contents in some map. Obviously this object would be statically-typed:

```c++
class JSON{
    std::hashmap<auto, std::any> data;
    std::string filename;
    Permission permission;
public:
    JSON(std::string filename, Permission permission): filename(filename), permission(permission) {}
    std::hashmap<auto, std::any> read(); //some recursive function that would return the contents of the file
    void write(std::hashmap<auto, std::any>); //some function that would write the file according to a template
    // etc.
};
```

I like the fact that C++ allows for a lot of flexibility for I/O operations with files, but for this kind of task, often writing the program that interprets configuration files, feels like reinventing the wheel. 

Additionnaly a library like **CLI11** should be in the standard template library. For the same reasons as the configuration file problem stated above.
