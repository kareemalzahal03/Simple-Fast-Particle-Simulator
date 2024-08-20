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
        
        func = f;
        runThreads();
    }

    template <typename Iterable, typename Function>
    void for_each(Iterable& iterable, Function f) {

        func = [&] (int threadIndex, int threadCount) {

            auto begin = iterable.begin();
            auto end = iterable.end();

            int chunkSize = iterable.size() / threadCount + 1;

            for (int x = threadIndex * chunkSize; x and begin != end; --x)
                ++begin;

            for (int x = chunkSize; x and begin != end; --x) {
                f(*begin);
                ++begin;
            }
        };

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
