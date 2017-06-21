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

#ifndef PIXELMATRIXCUT_H
#define PIXELMATRIXCUT_H

#include <QtWidgets/QWidget>
#include <map>
#include "PixelMatrixCutButton.h"
#include "TH2F.h"


class PixelMatrixCut : public QWidget
{

    Q_OBJECT

//    friend class PixelMatrixCutButton;

public:

    explicit PixelMatrixCut(QWidget *parent = 0);
    ~PixelMatrixCut ();

    void init ();

    std::map<std::pair<int, int>, bool> getSelectedPixels ();

    void setReferenceHistogram (TH2F * histo);

//    int getRowSelected () {return rowSelected_;}
//    int getColSelected () {return colSelected_;}
        
public slots:

    void selectWholeRow();
    void selectWholeCol();

signals:
//    void test();

private:

    void keyPressEvent(QKeyEvent * e);
    void keyReleaseEvent(QKeyEvent * e);

    int Ncols_;
    int Nrows_;
    int cellWidthX_;
    int cellWidthY_;
    int borderX_;
    int borderY_;
    bool isCtrlPressed_;

    TH2F * theHisto_;
    QColor palette_[50];

    std::map<std::pair<int, int>, PixelMatrixCutButton*> pixelButtons_  ;
    std::map<std::pair<int, int>, bool>                  selectedPixels_;

};

#endif // PIXELMATRIXCUT_H
