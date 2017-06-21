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

#ifndef XMLPLANE_H
#define XMLPLANE_H

#include "XmlWindow.h"

#include <string>

#include <QtWidgets/QWidget>
#include <QtXml/QDomNode>

class XmlWindow;

class XmlPlane /*: public QWidget */
{
/*    Q_OBJECT */
public:
    XmlPlane (QDomNode& node);
    ~XmlPlane(void          );

    QDomNode&                          getNode           (void){return thisNode_ ;}
    QDomNode&                          getCalibNode      (void){return calibNode_;}
    XmlWindow*                         getWindow         (void){return theWindow_;}
    std::pair<std::string,std::string> getChipsNumber    (void);
    std::pair<std::string,std::string> getCellPitches    (void);
    std::string                        getZRotation      (void);
    std::string                        getZPosition      (void);
    std::string                        getCalibPlotsFile (void);
    std::string                        getName           (void);
    std::string                        getId             (void);
    std::string                        getCalibration    (void);
    bool                               useCalibrations   (void);

private:
    QDomNode   thisNode_ ;
    QDomNode   calibNode_;
    XmlWindow* theWindow_;

signals:

public slots:

};

#endif // XMLPLANE_H
