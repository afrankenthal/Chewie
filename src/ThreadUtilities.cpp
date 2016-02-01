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

#include "ThreadUtilities.h"

////////////////////////////////////////////////////////////////////////////////////
//template<>
void addThreadedObject(TTree* object, int nThreads)
{
    TTree* tmp;
    std::string name = object->GetName();
    for(int threadNumber=0; threadNumber<nThreads; threadNumber++)
    {
        tmp = THREADED(object);
        if(threadNumber==0)
        {
            object->Delete();
            object = tmp;
            object->SetName(name.c_str());
        }
        else
        {
            object->CopyEntries(tmp);
            tmp->Delete();
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
std::string ThreadUtilities::itoa(int integer)
{
    char asciiVal[8];
    *sput_i(integer, asciiVal) = '\0';
    return std::string(asciiVal);
}

////////////////////////////////////////////////////////////////////////////////////
char* ThreadUtilities::sput_i(int integer, char *string)
{
    if (integer / 10 != 0)
    {
        string = sput_i(integer / 10, string);
    }
    *string++ = (char)('0' + integer % 10);
    return string;
}
