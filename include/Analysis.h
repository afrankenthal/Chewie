#ifndef _Analysis_h_
#define _Analysis_h_ 

#include "Threader.h"

#include <vector>
#include <string>
#include <map>

class AnalysisManager;
class TFile;
class TTree;
class Data;

class Analysis : public Threader
{
public:
    Analysis (AnalysisManager* analysisManager,
              int nOfThreads = 1               );
    ~Analysis(void                             );

    virtual void beginJob               (void                                                 ){}
    virtual void analyze                (const Data&, int                                     ){}
    virtual void endJob                 (void                                                 ){}
    virtual int  runAnalysis            (void                                                 );

    virtual void setCutsFormula         (std::map<std::string,std::string>,std::vector<TTree*>){}
    virtual bool passStandardCuts       (int , const Data&                                    ) = 0;
    virtual bool passBadPlanesCut       (int , const Data&                                    ) = 0;
            void setDoNotDelete         (bool flag                                            ){fDoNotDelete_ = flag;}
    virtual void getInFile              (TFile *                                              ){}

            void insertRunNumberEntries (int runNumber, int entries                           );

            //void setAsymm_par           (std::vector< matr_asim >&    Asymm_par               ){fMatrix_asim_ = Asymm_par;}
            //void getAsymm_par           (std::vector< matr_asim >&    Asymm_par               ){Asymm_par = fMatrix_asim_;}


protected:
    AnalysisManager*        theAnalysisManager_ ;
    bool                    fDoNotDelete_       ;
    std::map<int,int>       runNumberEntries_   ;
    //std::vector<matr_asim>  fMatrix_asim_       ;


private:
    int execute(int threadNumber);
};

#endif
