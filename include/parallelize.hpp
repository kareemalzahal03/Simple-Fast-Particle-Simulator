#pragma once
#include <condition_variable>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class Parallelize { 
public:

    Parallelize(int threadCount = std::thread::hardware_concurrency());
    ~Parallelize();

    template <typename Func>
    void operator() (Func f) {
        
        func = std::move(f);
        runThreads();
    }

private:

    void threadLoop(int threadIndex);
    void runThreads();

    std::function<void(int, int)> func;
    bool stopped;
    std::mutex m;
    std::condition_variable cv;
    bool allThreadsComplete;
    const int threadCount;
    int threadsCompletedCount;
    std::vector<std::thread> threads;
    std::vector<bool> isThreadComplete;
};
