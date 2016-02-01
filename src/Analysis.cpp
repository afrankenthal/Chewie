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

#include "Analysis.h"
#include "AnalysisManager.h"
#include "Data.h"
#include "MessageTools.h"

#include <TThread.h>

#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////
Analysis::Analysis(AnalysisManager* analysisManager, int nOfThreads) : Threader(nOfThreads)
  , theAnalysisManager_(analysisManager)
  , fDoNotDelete_(false)
{
}

///////////////////////////////////////////////////////////////////////////////////////////
Analysis::~Analysis()
{
    stopThreads();
}

///////////////////////////////////////////////////////////////////////////////////////////
int Analysis::execute(int threadNumber)
{
    int entry;
    Data& data = theAnalysisManager_->getCurrentData(&entry,threadNumber);
    if(entry >= 0)
        analyze(data,threadNumber);

    return entry;
}

///////////////////////////////////////////////////////////////////////////////////////////
int Analysis::runAnalysis()
{
    startThreads();
//    STDLINE("Waiting for threads to be done...",ACPurple);
    join();
//    STDLINE("Done with threads, stopping them...",ACPurple);
    stopThreads();
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////
void Analysis::insertRunNumberEntries(int runNumber, int entries)
{
    runNumberEntries_[runNumber] = entries;
}
