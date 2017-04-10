/*
 * Copyright (C) 2017 Elysium Project <https://github.com/elysium-project>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>

class ThreadPool
{
public:

    /**
     * @brief ThreadPool allocates memory, use ThreadPool::start() to spawn the threads.
     * @param numThreads the number of threads that will be created.
     */
    ThreadPool(int numThreads);

    ThreadPool() = delete;

    /**
     * @brief start creates and start the treads.
     */
    void start();

    /**
     * @brief processWorkload notify the threads that the workload is ready.
     */
    void processWorkload();

    /**
     * @brief setWorkload set the next workload
     * @param workload
     * @param safe if true, it will wait for previous workload to be done
     */
    void setWorkload(std::vector<std::function<void()>>&& workload, bool safe = false);

    /**
     * @brief waitForFinished
     */
    void waitForFinished();

    /**
     * @brief isStarted
     * @return true if the threads are spawned
     */
    bool isStarted();

    /**
     * @brief size
     * @return the number of threads that are/will be created
     */
    int size();

    /**
     * @brief operator << add a task to the workload
     * NOT threadsafe
     * @param function
     * @return
     */
    ThreadPool& operator<<(std::function<void()> &&function);

    /**
     * @brief operator << add a task to the workload
     * NOT threadsafe
     * @param f
     * @return
     */
    ThreadPool& operator<<(auto f)
    {
        m_workload.emplace_back(wrap(f));
        return *this;
    }

private:
    void waitForWork(int id);


    struct worker{
        worker(auto f) :
            thread(f)
        {
        }

        std::thread thread;
        std::condition_variable waitForFinished;
        bool busy;
        std::mutex mutex;
        std::vector<std::function<void()>>::iterator it;
    };
    using workers_t = std::vector<std::unique_ptr<worker>>;

    workers_t m_workers;
    int m_size;
    std::mutex m_mutex;
    std::condition_variable m_waitForWork;
    std::vector<std::function<void()>> m_workload;
    void workerLoop(int id);
};

std::unique_ptr<ThreadPool> & operator<<(std::unique_ptr<ThreadPool> & tp, auto f)
{
    (*tp) << f;
    return tp;
}

#endif
