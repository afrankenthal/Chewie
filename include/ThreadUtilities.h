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

#ifndef THREADUTILITIES_H
#define THREADUTILITIES_H

#include <string>
#include <TTree.h>
#include <TChain.h>
#include <TDirectory.h>
#include <iostream>

#define NEW_THREADED(x)  (ThreadUtilities::newThreadedObject(new x,Threader::nOfThreads_))
#define CLONE_THREADED(x)(ThreadUtilities::newThreadedObject(x,Threader::nOfThreads_))
#define THREADED(x)      (ThreadUtilities::getThreadedObject(x,threadNumber))
#define ADD_THREADED(x)  (ThreadUtilities::addThreadedObject(x,Threader::nOfThreads_))
#define ADD_THREADED_CHAIN(x,y) (ThreadUtilities::addThreadedObjectInChain(x, y, Threader::nOfThreads_))
#define MERGE_THREADED(x,y)     (ThreadUtilities::mergeThreadedObject(x,y,Threader::nOfThreads_))

//class TTree;

//////////////////////////////////////////////////////////////////////////
//ATTENTION These MACROs ASSUME that nThreads is Threader::nOfThreads_ variable with EXACTLY this name
//////////////////////////////////////////////////////////////////////////

namespace ThreadUtilities
{

std::string itoa(int integer);
char* sput_i(int integer, char *string);

////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* newThreadedObject(T* object, int nThreads)
{
    for(int t=0; t<nThreads; t++)
        ((T*)(object->Clone()))->SetName(std::string((object->GetName())+itoa(t)).c_str());
    return object;
}

////////////////////////////////////////////////////////////////////////////////////
template<class T>
T* getThreadedObject(T* object, int t)
{
    return (T*)(object->GetDirectory()->FindObjectAny(std::string((object->GetName())+itoa(t)).c_str()));
}

////////////////////////////////////////////////////////////////////////////////////
template<class T>
void addThreadedObject(T* object, int nThreads)
{
    T* tmp;
    for(int threadNumber=0; threadNumber<nThreads; threadNumber++)
    {
        tmp = THREADED(object);
        object->Add(tmp);
        tmp->Delete();
    }

}

template<class T>
void addThreadedObjectInChain (T* object, TChain * chain, int nThreads)
{
    T* tmp;
    for (int threadNumber = 0; threadNumber<nThreads; threadNumber++) {
        tmp = THREADED(object);
        chain->Add(object->GetName());
        tmp->Delete();
    }
}

template<class T>
void mergeThreadedObject (T * object, TList * list, int nThreads) {
    T* tmp;
    for (int threadNumber = 0; threadNumber<nThreads; threadNumber++) {
        tmp = THREADED(object);
        list->Add((TObject*)tmp);
        tmp->Delete();
    }
}

/*template<>
void addThreadedObject(TTree* object, int nThreads)
{
    TTree* tmp;
    for(int t=0; t<nThreads; t++)
    {
        tmp = THREADED(object,t);
        object->CopyEntries(tmp);
        tmp->Delete();
    }
}

std::string itoa(int integer)
{
    char asciiVal[8];
    *sput_i(integer, asciiVal) = '\0';
    return std::string(asciiVal);
}

char* sput_i(int integer, char *string)
{
    if (integer / 10 != 0)
    {
        string = sput_i(integer / 10, string);
    }
    *string++ = (char)('0' + integer % 10);
    return string;
}*/


}

#endif // THREADUTILITIES_H
