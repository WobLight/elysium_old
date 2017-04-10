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

#include "ThreadPool.h"
#include "Log.h"

ThreadPool::ThreadPool(int numThreads) :
    m_size(numThreads), m_dirty(false)
{
    m_workers.reserve(m_size);
}

void ThreadPool::start()
{
    if (!m_workers.empty())
        return;
    for (int i = 0; i < m_size; i++)
        m_workers.emplace_back(new worker([this,i](){this->workerLoop(i);}));
    m_waitForWork.notify_all();
}

void ThreadPool::processWorkload()
{
    if (m_workload.empty())
        return;
    m_dirty = true;
    for (int i = 0; i < m_size; i++)
    {
        m_workers[i]->it = m_workload.begin() + i;
        m_workers[i]->busy = true;
    }
    m_waitForWork.notify_all();
}

void ThreadPool::setWorkload(std::vector<std::function<void ()> > &workload, bool safe)
{
    if (workload.empty())
        return;
    if (safe)
        waitForFinished();

    m_workload = workload;
    processWorkload();
}

void ThreadPool::setWorkload(std::vector<std::function<void()> > &&workload, bool safe)
{
    if (workload.empty())
        return;
    if (safe)
        waitForFinished();

    m_workload = std::move(workload);
    processWorkload();
}

void ThreadPool::waitForFinished()
{
    for (workers_t::iterator it = m_workers.begin(); it < m_workers.end(); it++)
    {
        std::unique_lock<std::mutex> lock((*it)->mutex);
        while ((*it)->busy)
            (*it)->waitForFinished.wait(lock);
    }
}

bool ThreadPool::isStarted()
{
    return !m_workers.empty();
}

int ThreadPool::size()
{
    return m_size;
}

void ThreadPool::waitForWork(int id)
{
    std::unique_lock<std::mutex> lock(m_mutex); //locked!
    while(!m_workers[id]->busy) //wait for work
        m_waitForWork.wait(lock);
}

ThreadPool &ThreadPool::operator<<(std::function<void()> packaged_task)
{
    if (m_dirty) m_workload.clear();
    m_workload.emplace_back(std::move(packaged_task));
    return *this;
}

void ThreadPool::workerLoop(int id){ // WORKER THREAD LOOP
    {
        std::unique_lock<std::mutex> lock(m_mutex); //locked!
        while(m_size != m_workers.size())
            m_waitForWork.wait(lock);
    } //destroy lock
    while(true)
    {
        waitForWork(id);
        while (m_workers[id]->it < m_workload.end())
        {
            (*m_workers[id]->it)(); // do work
            m_workers[id]->it += m_size;
        }
        m_workers[id]->mutex.lock(); //locked!
        m_workers[id]->busy = false;
        m_workers[id]->waitForFinished.notify_all();
        m_workers[id]->mutex.unlock(); //locked!
    }
}
