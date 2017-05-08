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

#ifndef XMLPARSER_H
#define XMLPARSER_H

#include "XmlPlane.h"

#include <sstream>
#include <map>
#include <string>

#include <QDomNode>
#include <QString>

class QDomElement ;
class QFile       ;
class QDomDocument;

class XmlConverter;
class XmlGeneral  ;
class XmlAnalysis ;
class XmlPlane    ;
class XmlScan     ;

class XmlParser
{
public:
    typedef std::map< int, std::pair<std::string,XmlAnalysis*> > analysesDef_ ;

                                    XmlParser     (void            );
                                   ~XmlParser     (void            );

    void                            destroy       (void            );
    bool                            parseDocument (QString fileName);
    void                            save          (QString fileName);

    QDomDocument*                   getDocument   (void            ){return document_        ;}
    XmlConverter*                   getConverter  (void            ){return theConverter_    ;}
    XmlGeneral*                     getGeneral    (void            ){return theGeneral_      ;}
    XmlScan*                        getScan       (void            ){return theScan_         ;}
    unsigned int                    getNPlanes    (void            ){return thePlanes_.size();}
    QDomNodeList&                   getPlanesNode (void            ){return planes_          ;}
    std::map<std::string,XmlPlane*> getPlanes     (void            ){return thePlanes_       ;}
    analysesDef_                    getAnalyses   (void            ){return theAnalyses_     ;}
    XmlAnalysis *                   getAnalysesFromString (std::string analysisName);

private:
    QDomDocument*                   document_    ;
    QDomNode                        rootNode_    ;
    QDomNodeList                    planes_      ;

    XmlConverter*                   theConverter_;
    XmlGeneral*                     theGeneral_  ;
    XmlScan*                        theScan_     ;
    std::map<std::string,XmlPlane*> thePlanes_   ;
    analysesDef_                    theAnalyses_ ;

    std::stringstream               ss_          ;
};

#endif // XMLPARSER_H
