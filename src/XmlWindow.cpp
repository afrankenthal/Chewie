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

#include "XmlWindow.h"
#include "MessageTools.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
XmlWindow::XmlWindow(QDomNode& node)
{
    thisNode_ = node;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void XmlWindow::setRemovedPixels(void)
{
    QDomNodeList removedPixels = thisNode_.toElement().elementsByTagName("RemovedPixel");

    std::map<std::string,int> removedPixel;

    for(int p=0;p<removedPixels.size();p++)
    {
        removedPixel["row"] = atoi(removedPixels.at(p).toElement().attribute("Row").toStdString().c_str());
        removedPixel["col"] = atoi(removedPixels.at(p).toElement().attribute("Col").toStdString().c_str());
        removedPixelsVec_.push_back(removedPixel);
    }
}
