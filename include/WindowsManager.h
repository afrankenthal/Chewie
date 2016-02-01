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

#ifndef WINDOWSMANAGER_H
#define WINDOWSMANAGER_H

#include "Analysis.h"

#include <string>
#include <vector>
#include <iostream>


class Window         ;
class PlanesMapping  ;
class AnalysisManager;
class XmlParser      ;

class WindowsManager : public Analysis
{
public:
    WindowsManager (AnalysisManager* analysisManager = 0, int nOfThreads = 1);
    ~WindowsManager(void                            );

    void beginJob            (void                              );
    void analyze             (const Data& data, int threadNumber);
    void endJob              (void                              );
    virtual bool passStandardCuts (int , const Data&            ){return true;}
    virtual bool passBadPlanesCut (int , const Data&            ){return true;} //fake


    void getInFile           (TFile *                           ){}

    void init                (std::string type, int maxEntries = -1);
    void destroy             (void               );

    const Window* getWindow  (std::string plane  ) const;
    const Window* getWindow  (int plane          ) const;

private:

    std::vector<Window*> windows_           ;
    AnalysisManager*     theAnalysisManager_;
    PlanesMapping*       thePlanesMapping_  ;
    XmlParser*           theXmlParser_      ;
};

#endif // WINDOWSMANAGER_H
