#include <thread>
#include <condition_variable>
#include <iostream>
#include <chrono>
#include <atomic>

std::condition_variable_any cv;
std::atomic<bool> start_threads(false);
std::atomic<bool> exit_program(false);
std::atomic<bool> worker1_running(false);
std::atomic<bool> worker2_running(false);

void worker_thread(std::string name, std::atomic<bool>& worker_running) {
    while (true) {
        while (!start_threads && !exit_program) {
            std::this_thread::yield(); // Let other threads run
        }

        if (exit_program) {
            break;
        }
		
		worker_running = true;
        std::cout << name << " Worker thread is processing data\n";
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << name << " Worker thread is done\n";

        worker_running = false;
        while (!start_threads && !exit_program) {
            std::this_thread::yield(); // Let other threads run
        }
    }
}

int main() {
    std::thread worker1(worker_thread, "1", std::ref(worker1_running));
    std::thread worker2(worker_thread, "2", std::ref(worker2_running));

    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    for (int i = 0; i < 10; i++) {
        std::cout << "Main thread is running the worker threads\n";
        start_threads = true;
        cv.notify_all();
        
		worker1_running.wait(true);
		worker2_running.wait(true);
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate work in the main thread
		
        start_threads = false;
        cv.notify_all();
    }

    exit_program = true;
    cv.notify_all();

    worker1.join();
    worker2.join();

    return 0;
}


