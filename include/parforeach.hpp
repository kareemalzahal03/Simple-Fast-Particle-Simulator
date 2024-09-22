#pragma once
#include <condition_variable>
#include <algorithm>
#include <functional>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

/*
The ParForEach class is a thread-based utility designed to parallelize
operations over a standard C++ vector of any type (std::vector<T>). It
is particularly useful when you need to apply a function to each element
of a large vector using multiple threads for performance gains, especially
in scenarios involving large datasets or time-consuming operations on
each element.
*/

template <typename T>
class ParForEach { 
public:
    
    ParForEach(std::vector<T>& v, unsigned int threadCount = std::thread::hardware_concurrency()):
        stopped(false), isThreadComplete(threadCount, true) {

        if (threadCount == 0) throw std::runtime_error("Need positive threadCount");

        for (int threadIndex = 0; threadIndex < threadCount; ++threadIndex) {
        threads.emplace_back([this, threadIndex, threadCount, &v] {
        while (true) {

            // Wait until for each called
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [this, threadIndex] 
                { return !isThreadComplete[threadIndex] || stopped; });
            if (stopped) return;
            lock.unlock();

            // Call Func on Range
            std::size_t size = v.size();
            std::size_t partSize = size / threadCount + (size % threadCount != 0);
            std::size_t start = threadIndex * partSize;
            std::size_t end = std::min(start + partSize, size);
            if (start <= size)
                std::for_each(v.begin()+start, v.begin()+end, func);

            // Notify main thread when done
            lock.lock();
            isThreadComplete[threadIndex] = true;
            allThreadsComplete = (threadCount == ++threadsCompletedCount);
            cv.notify_all();

        }});}
    }

    ParForEach(ParForEach& other) = delete;

    ~ParForEach() {

        stopped = true;
        cv.notify_all();
        for (std::thread& t : threads) t.join();
    }

    template <typename Func>
    void operator() (Func f) {
        
        // Set function
        func = f;

        // Reset
        isThreadComplete.flip();
        threadsCompletedCount = 0;
        allThreadsComplete = false;

        // Notify all threads to run func
        cv.notify_all();

        // Wait for all threads to complete
        {
            std::unique_lock<std::mutex> lock(m);
            cv.wait(lock, [this] { return allThreadsComplete; });
        }
    }

private:

    std::function<void(T&)> func;
    bool stopped;
    std::mutex m;
    std::condition_variable cv;
    bool allThreadsComplete;
    int threadsCompletedCount;
    std::vector<std::thread> threads;
    std::vector<bool> isThreadComplete;
};
