#include <condition_variable>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
#include <functional>

class Parallelize { 
public:

    Parallelize (
        int threadCount = std::thread::hardware_concurrency())
        : threadCount(threadCount), stopped(false), isThreadComplete(threadCount, true) {

        for (int i = 0; i < threadCount; ++i) {
            threads.emplace_back([this, i] { this->threadLoop(i); });
        }
    }

    ~Parallelize() {
        {
            std::unique_lock<std::mutex> lock(m);
            stopped = true;
        }
        cv.notify_all();
        for (std::thread& t : threads) t.join();
    }

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

            for (int x = threadIndex; x && begin != end; --x)
                ++begin;

            while (begin != end) {
                f(*begin);

                for (int x = threadCount; x && begin != end; --x)
                    ++begin;
            }
        };

        runThreads();
    }

private:

    void threadLoop(int threadIndex) {
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

    void runThreads() {

        // Notify all threads to run func
        {
            std::lock_guard<std::mutex> lock(m);
            for (int i = 0; i < threadCount; ++i) { isThreadComplete[i] = false; }
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

    std::function<void(int,int)> func;
    bool stopped;
    std::mutex m;
    std::condition_variable cv;
    bool allThreadsComplete;
    const int threadCount;
    int threadsCompletedCount;
    std::vector<std::thread> threads;
    std::vector<bool> isThreadComplete;
};

int main() {
    Parallelize parallelize(5);

    std::cout << "Launching threads..." << std::endl;

    parallelize([] (int threadIndex, int threadCount) {

        // Simulate work being done
        std::this_thread::sleep_for(std::chrono::seconds(threadIndex/2));
        std::cout << "Thread " + std::to_string(threadIndex) + " has completed.\n";
    });

    std::cout << "All threads have completed their tasks.\n";

    return 0;
}

// int main() {

//     Parallelize parallelize;
//     std::vector<int> myvector(1000000, 0);

//     using namespace std::chrono_literals; 
//     const auto start = std::chrono::high_resolution_clock::now();

//     // parallelize.for_each(myvector,[](int& i){
//     //     i += 3;
//     // });

//     for (int x = 0; x < myvector.size(); ++x) {
//         myvector[x] += 3;
//     }

//     // for (int& i : myvector) {
//     //     i += 3;
//     // }

//     const auto end = std::chrono::high_resolution_clock::now();
//     const std::chrono::duration<double, std::milli> elapsed = end - start;
//     std::cout << "Waited " << elapsed.count() << " seconds." << '\n';

//     return 0;
// }
