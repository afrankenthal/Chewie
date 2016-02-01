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

#ifndef XMLWINDOW_H
#define XMLWINDOW_H

#include <QWidget>
#include <QDomNode>

#include <vector>
#include <map>
#include <string>

class XmlWindow : public QWidget
{
    Q_OBJECT
public:
    XmlWindow (QDomNode & node);
    ~XmlWindow(void){;}

    void        setRemovedPixels  (void);

    QDomNode&   getNode           (void){return thisNode_ ;}
    std::string getLowerCol       (void){return thisNode_.toElement().elementsByTagName("LowerCol").at(0).toElement().text().toStdString() ;}
    std::string getHigherCol      (void){return thisNode_.toElement().elementsByTagName("HigherCol").at(0).toElement().text().toStdString();}
    std::string getLowerRow       (void){return thisNode_.toElement().elementsByTagName("LowerRow").at(0).toElement().text().toStdString() ;}
    std::string getHigherRow      (void){return thisNode_.toElement().elementsByTagName("HigherRow").at(0).toElement().text().toStdString();}

    std::vector< std::map<std::string,int> >& getRemovedPixels(void){return removedPixelsVec_;}

private:
    QDomNode                                 thisNode_        ;
    std::vector< std::map<std::string,int> > removedPixelsVec_;

signals:

public slots:

};

#endif // XMLWINDOW_H
