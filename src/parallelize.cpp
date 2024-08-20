#include "parallelize.hpp"

Parallelize::Parallelize(int threadCount)
    : threadCount(threadCount), stopped(false), isThreadComplete(threadCount, true) {

    for (int i = 0; i < threadCount; ++i) {
        threads.emplace_back([this, i] { this->threadLoop(i); });
    }
}

Parallelize::~Parallelize() {
    {
        std::unique_lock<std::mutex> lock(m);
        stopped = true;
    }
    cv.notify_all();
    for (std::thread& t : threads) t.join();
}

void Parallelize::threadLoop(int threadIndex) {
    while (true) {

        // Wait until func called
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this, threadIndex] 
            { return !isThreadComplete[threadIndex] || stopped; });
        if (stopped) return;
        lock.unlock();

        // Call Func
        if (func) func(threadIndex, threadCount);

        // Notify main thread when done
        lock.lock();
        allThreadsComplete = (threadCount == ++threadsCompletedCount);
        isThreadComplete[threadIndex] = true;
        cv.notify_all();
    }
}

void Parallelize::runThreads() {
    // Notify all threads to run func
    {
        std::lock_guard<std::mutex> lock(m);
        for (int i = 0; i < threadCount; ++i) isThreadComplete[i] = false;
        threadsCompletedCount = 0;
        allThreadsComplete = false;
    }

    cv.notify_all();

    // Wait for all threads to complete
    {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this] { return allThreadsComplete; });
    }
}
