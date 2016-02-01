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

#ifndef _AnalysisManager_h_
#define _AnalysisManager_h_ 

#include "Data.h"

#include "TTree.h"

#include <QSpinBox>

#include <string>
#include <map>
#include <vector>
#include <algorithm>

class TFile              ;
class Analysis           ;
class HistogramWindow    ;
class WindowsManager     ;
class CalibrationsManager;
class XmlParser          ;

class AnalysisManager
{
public:
    AnalysisManager ();//TTree* tree = 0);
    ~AnalysisManager(void);

    void                  setCutsFormula         (std::string     analysisName,
                                                  std::map<std::string,std::string> cutsList);
    void                  setMaxNumberOfThreads  (int             value            );
    void                  setMaxEvents           (int             maxEvents        ){maxEvents_ = maxEvents;}
    void                  setThreadEvents        (void                             );
    void                  setFileNameToTreePos   (void                             );
    void                  setConfiguration       (XmlParser*      xmlParser        ){theXmlParser_      = xmlParser     ;}
    void                  setCurrentEntry        (int             entry            ){currentEntry_      = entry         ;}
    void                  setOutputFileName      (std::string     fileName         ){outFileName_       = fileName      ;}

    int                   getCompletionStatus    (void                             );
    std::string           getCurrentOperation    (void                             ){return currentOperation_;}
    Data&                 getCurrentData         (int*            entry,
                                                  unsigned int    threadNumber     );
    int                   getCurrentEntry        (void                             );
    int                   getNumberOfThreads     (void                             ){return nOfThreads_                 ;}
    const WindowsManager* getWindowsManager      (void                             );
    TFile*                getOutputFile          (void                             ){return outFile_                    ;}
    const std::string&    getOutputFileName      (void                             ){return outFileName_                ;}
    int                   getTotalEvents         (void                             ){return totalEvents_                ;}
    std::vector<int>      getEventsForRun        (void                             );
    int                   calculateTotalEvents   (void                             );
    int                   getMaxEvents           (void                             ){return maxEvents_                  ;}
    XmlParser*            getXmlParser           (void                             ){return theXmlParser_               ;}
    CalibrationsManager*  getCalibrationsManager (void                             ){return theCalibrationsManager_     ;}
    bool                  isFinished             (void                             ){return isFinished_                 ;}
    TFile*                getOutFile_            (void                             ){return outFile_                    ;}


    int                   initializeTree         (std::string     fileName                  );
    int                   initializeTrees        (void                                      );
    void                  setupCalibrations      (int             dut                       );
    Analysis*             addAnalysis            (int analysisPriority, std::string analysis);
    void                  removeAnalysis         (std::string     analysis                  );
    void                  resetAnalyses          (void                                      );
    void                  startSequence          (void                                      );
    void                  stopSequence           (void                                      );
    void                  destroyWindow          (void                                      );
    void                  setListOfRun           (Analysis *analysis                        );

    void                  setInFilesList         (std::vector<std::string> fileList);
    void                  addInFile              (std::string     fileName         );
    void                  clearFilesList         (void                             );
    void                  resetThreadedEntries   (void                             );
    void                  clearThreadedVariables (void                             );
    void                  openInFile             (std::string     fileName         );
    void                  openInFiles            (void                             );
    void                  closeInFile            (std::string     fileName         );
    void                  closeInFiles           (void                             );
    int                   openOutFile            (std::string     fileName = "",
                                                  std::string     mode = "RECREATE");
    void                  closeOutFile           (void                             );
    bool                  isOutFileOpen          (void                             );
    void                  writeOutFile           (void                             );
    void                  mkdir                  (std::string     dirName          );
    void                  cd                     (std::string     dirName          );

    bool                  sorting                (std::string a, std::string b     );
    int                   findAnalysis           (std::string analysisName         );


    std::string           getPriorityFromUi      (std::string analysisName, int newPriority);
//    void                  preventFalseOrder      ();



private:
    std::map<std::string,TFile*>                      inFilesList_        ;
    std::map<std::string,TTree*>                      treeFilesList_      ;
    std::map<std::string, std::pair<int, int> >       fileNameToTreePos_  ;

    std::vector<unsigned int>                         currentTree_           ;
    std::map<int, std::map<int,TTree*> >              threadedTrees_         ;
    std::map<int, std::map<int, Data> >               threadedData_          ;
    std::map<int, std::map<int, int> >                threadedCurrentEntries_;
    std::map<int, std::map<int, int> >                threadedMaxEntries_    ;

    unsigned int                                      nOfThreads_            ;
    unsigned int                                      maxNOfThreads_         ;
    int                                               maxEvents_             ;
    int                                               currentEntry_          ;
    std::string                                       outFileName_           ;
    TFile*                                            outFile_               ;
    Data                                              aData_                 ;
    CalibrationsManager*                              theCalibrationsManager_;
    XmlParser*                                        theXmlParser_          ;
    std::map< int, std::pair<std::string,Analysis*> > analyses_              ;
    std::vector<std::string>                          analysesOrder_         ;
    int                                               totalEvents_           ;
    int                                               totalEventsToProcess_  ;
    bool                                              abort_                 ;
    bool                                              isFinished_            ;
    int                                               completionStatus_      ;
    int                                               completionStatusBegin_ ;
    int                                               completionStatusEnd_   ;
    std::string                                       currentOperation_      ;


};

#endif
