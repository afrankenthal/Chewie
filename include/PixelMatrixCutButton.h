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

#ifndef PIXELMATRIXCUTBUTTON_H
#define PIXELMATRIXCUTBUTTON_H

#include "QPushButton"
#include <QWidget>
#include "QPaintEvent"
#include <iostream>

class PixelMatrixCut;
//class QPushButton;

class PixelMatrixCutButton : public QPushButton
{
    Q_OBJECT

    friend class PixelMatrixCut;

public:

    PixelMatrixCutButton(QWidget *parent = 0, int posX = 0, int posY = 0, int traslX = 0, int traslY = 0, bool isActive = true);
    ~PixelMatrixCutButton() {;}

    int init ();

    int  getRow        () {return row_       ;}
    int  getCol        () {return col_       ;}
    bool getIsSelected () {return isSelected_;}

    void setActiveColor (QColor color);

    void clickIt   ();
    void clickIt   (bool click);
    void refreshIt ();

signals:
//    void selectRow();// {return row_;}
//    void selectCol();// {return col_;}

private slots:

    void on_theBotton_clicked();

private:

    int row_;
    int col_;
    bool isSelected_;
    QColor activeColor_;

};

#endif // PIXELMATRIXCUTBUTTON_H
