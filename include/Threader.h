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

#ifndef _Threader_h_
#define _Threader_h_ 

#include <vector>

class TThread;
class ThreadArgs;

class Threader
{
 public:
  Threader(int nOfThreads = 1);
  virtual ~Threader();
  static void* thread0(void* arg); // function running as threads

  virtual int execute(int threadNumber);// user functions called from thread0

  virtual int startThreads(void);// launch all threads
  virtual int stopThreads (void);// stop all threads
  virtual int join        (void);// wait for threads to be done
 
  void setNumberOfThreads (int numberOfThreads);
  int  getNumberOfThreads (void               ){return nOfThreads_;}

  bool getThreadsRun      (void               ){return threadsRun_;}

 protected:
  int  nOfThreads_;//WARNING: You can't change this name since it used in the MACROS defined in ThreadedUtilities!!!!!
  bool threadsRun_;// flags for quick abort of loops within threads
                   // may also use different flags for each thread

 private:
  std::vector<TThread*>    pThreads_;    // Thread pointer
  std::vector<ThreadArgs*> pThreadArgs_; // Thread arguments
};

///////////////////////////////////////////////////////////////////////////////////
class ThreadArgs{
 public:
  ThreadArgs(Threader* threader, int threadNumber) : theThreader_ (threader)
                                                   , threadNumber_(threadNumber)
                                                   , funcRunning_ (false)
						   {;}
  ~ThreadArgs(){;}
  Threader* getThreader    (void        ){return theThreader_;}
  int	    getThreadNumber(void        ){return threadNumber_;}
  void	    setFuncRunning (bool running){funcRunning_ = running;}
  bool	    getFuncRunning (void        ){return funcRunning_;}
 private:	
  Threader* theThreader_;
  int	    threadNumber_;
  bool	    funcRunning_;
};

#endif
