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

#ifndef XMLANALYSIS_H
#define XMLANALYSIS_H

#include <QWidget>
#include <QDomNode>

#include "MessageTools.h"

#include <string>
#include <map>

class XmlAnalysis /*: public QWidget */
{
/*    Q_OBJECT */
public:
    XmlAnalysis (QDomNode&  node);
    ~XmlAnalysis(void           ){;}

    QDomNode&                          getNode                 (void                ){return thisNode_;}
    QDomNode&                          getCutNode              (std::string cutType );
    std::string                        getType                 (void                );
    std::string                        getCut                  (std::string cutType );
    std::map<std::string,std::string>  getCutsList             (void                ){return cutsList_;}
    std::string                        getMinHits              (void                );
    std::pair<bool,bool>               isParToLimit            (int         parID   );
    std::pair<std::string,std::string> getParLimits            (int         parID   );
    std::pair<double,double>           getParLimits            (int parID, bool flag);
    int                                getPixelMinimumCharge   (void                );
    int                                getPixelMaximumCharge   (void                );
    int                                getClusterMinimumCharge (void                );
    int                                getClusterMaximumCharge (void                );
    int                                getPriority             (void                );
    int                                getBadPlanesCut         (void                );
    bool                               isToRun                 (void                );
    bool                               doFits                  (void                );
    bool                               applyStandardCuts       (void                );
    bool                               excludeBadPlanes        (void                );

    void                               setCutsList             (void                );

private:
    QDomNode                          thisNode_;
    QDomNode                          cutNode_;
    std::map<std::string,std::string> cutsList_;

signals:
    
public slots:
    
};

#endif // XMLANALYSIS_H
