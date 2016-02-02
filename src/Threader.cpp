/*===============================================================================
 * Chewie: the FERMILAB MTEST telescope and DUT anaysis tool
 * 
 * Copyright (C) 2014 
 *
 * Authors:
 *
 * Mauro Dinardo      (Universita' Bicocca) 
 * Dario Menasce      (INFN) 
 * Jennifer Ngadiuba  (INFN)
 * Lorenzo Uplegger   (FNAL)
 * Luigi Vigani       (INFN)
 *
 * INFN: Piazza della Scienza 3, Edificio U2, Milano, Italy 20126
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 ================================================================================*/

#include "Threader.h"
#include "MessageTools.h"

#include <TThread.h>

#include <iostream>
#include <sstream>

///////////////////////////////////////////////////////////////////////////////////////////
Threader::Threader(int nOfThreads) :
    nOfThreads_(nOfThreads)
  ,threadsRun_(false)
{
    setNumberOfThreads(nOfThreads);
}

///////////////////////////////////////////////////////////////////////////////////////////
Threader::~Threader()
{
    stopThreads();
}

///////////////////////////////////////////////////////////////////////////////////////////
void Threader::setNumberOfThreads(int numberOfThreads)
{
    stopThreads();
    pThreads_   .clear();
    pThreadArgs_.clear();
    nOfThreads_ = numberOfThreads;
    for(int t=0; t<nOfThreads_; t++)
    {
        pThreads_   .push_back(0);
        pThreadArgs_.push_back(0);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////
void* Threader::thread0(void* arg)
{
    ThreadArgs* threadArgs = (ThreadArgs*) arg;
    Threader* inst = threadArgs->getThreader();
    threadArgs->setFuncRunning(true);
    while(inst->getThreadsRun() && inst->execute(threadArgs->getThreadNumber()) >= 0){}
    threadArgs->setFuncRunning(false);
    return 0;
}

///////////////////////////////////////////////////////////////////
// default user functions running within threaded methods, overwrite these:
int Threader::execute(int threadNumber)
{ 
    TThread::Lock();
    std::cout << __PRETTY_FUNCTION__ << "\tDefault method " << std::hex << threadNumber << " running, please overwrite in derived class" << std::dec << std::endl;
    TThread::UnLock();
    gSystem->Sleep(1000);
    return true;
}

///////////////////////////////////////////////////////////////////
int Threader::startThreads()
{
    // start all threads
    //gDebug = 1;
    threadsRun_ = true;

    for(int t=0; t<nOfThreads_; t++)
    {
        if(!(pThreads_[t]))
        {
            std::stringstream threadName;
            threadName << "Thread: " << t;
            pThreadArgs_[t] = new ThreadArgs(this,t);
            pThreads_   [t] = new TThread(threadName.str().c_str(), thread0, (void*) pThreadArgs_[t]);
            if(pThreads_[t])
                pThreads_[t]->Run();
            else
                return -1;
            STDLINE(threadName.str()+" is running...",ACWhite);
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////
int Threader::stopThreads()
{
    // stop all active threads
    if(!threadsRun_)
        return 0;

    threadsRun_= false;	// aborting flag

    for(int t=0; t<nOfThreads_; t++)
    {
        if( pThreads_[t] )
        {
            int timeout = 0;
            while(pThreadArgs_[t]->getFuncRunning() && timeout < 200)
            {
                timeout++;
                gSystem->Sleep(500); // wait a while for threads to halt
            }
            TThread::Delete(pThreads_[t]);
            delete pThreads_[t];
            pThreads_[t] = 0;
            delete pThreadArgs_[t];
            pThreadArgs_[t] = 0;
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////
int Threader::join()
{
    for(int t=0; t<nOfThreads_; t++)
        if( pThreads_[t] )
        {
            //Uncommenting these lines should make the join part work
            //while(pThreads_[t]->GetState() == 2)
            //{
            ////    STDLINE(pThreads_[t]->GetState(),ACGreen);
            //    usleep(500000);
            //}
            ////STDLINE(pThreads_[t]->GetState(),ACGreen);
            pThreads_[t]->Join();
            //STDLINE(pThreads_[t]->GetState(),ACRed);
        }
    return 0;
}
