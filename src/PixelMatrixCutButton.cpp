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

#include "PixelMatrixCutButton.h"
#include "MessageTools.h"

PixelMatrixCutButton::PixelMatrixCutButton(QWidget *parent, int posX, int posY, int traslX, int traslY, bool isActive):
    QPushButton(parent)
{
    row_ = posY;
    col_ = posX;
    this->setGeometry(posX*14 + traslX, posY*7 + traslY, 14, 7);

    activeColor_ = QColor(0,255,0,255);
    isSelected_ = isActive;

    connect(this, SIGNAL(clicked()), this, SLOT(on_theBotton_clicked()));
}

int PixelMatrixCutButton::init()
{
    QPalette palette;

    if (isSelected_)
    {
        QBrush brush7(activeColor_);
        palette.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        this->setAutoFillBackground(true);
        this->setPalette(palette);

        this->setEnabled(true);
    }
    else
    {
        QBrush brush7(QColor(75, 75, 75));
        palette.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        this->setAutoFillBackground(true);
        this->setPalette(palette);

        this->setEnabled(true);
    }
    this->show();

    return 0;
}

/*void PixelMatrixCutButton::clicked(bool checked)
{
    isSelected_ = checked;
    on_theBotton_clicked();
}*/

void PixelMatrixCutButton::on_theBotton_clicked()
{
    QPalette palette;

    if (!isSelected_)
    {
        QBrush brush7(activeColor_);
        palette.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        this->setAutoFillBackground(true);
        this->setPalette(palette);

        isSelected_ = true;
    }
    else
    {
        QBrush brush7(QColor(75, 75, 75));
        palette.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        this->setAutoFillBackground(true);
        this->setPalette(palette);
//        this->set

        isSelected_ = false;
    }

//    std::stringstream ss;
//    ss << "I am at row " << row_ << " col " << col_;
//    STDLINE(ss.str(), ACWhite);
/*
    if (row_ < 0)
    {
        emit selectCol();
        STDLINE("Col selected!", ACYellow);
    }
    if (col_ < 0)
    {
        emit selectRow();
        STDLINE("Row selected!", ACYellow);
    }*/
}

void PixelMatrixCutButton::clickIt()
{
//    on_theBotton_clicked();
    QPalette palette;

    if (!isSelected_)
    {
        QBrush brush7(activeColor_);
        palette.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        this->setAutoFillBackground(true);
        this->setPalette(palette);

        isSelected_ = true;
    }
    else
    {
        QBrush brush7(QColor(75, 75, 75));
        palette.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        this->setAutoFillBackground(true);
        this->setPalette(palette);
//        this->set

        isSelected_ = false;
    }
}

void PixelMatrixCutButton::clickIt(bool click)
{
//    on_theBotton_clicked();
    QPalette palette;

    if (click)
    {
        QBrush brush7(activeColor_);
        palette.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        this->setAutoFillBackground(true);
        this->setPalette(palette);

        isSelected_ = true;
    }
    else
    {
        QBrush brush7(QColor(75, 75, 75));
        palette.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        this->setAutoFillBackground(true);
        this->setPalette(palette);
//        this->set

        isSelected_ = false;
    }
}

void PixelMatrixCutButton::refreshIt()
{
    QPalette palette;

    if (isSelected_)
    {
        QBrush brush7(activeColor_);
        palette.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        this->setAutoFillBackground(true);
        this->setPalette(palette);
    }
    else
    {
        QBrush brush7(QColor(75, 75, 75));
        palette.setBrush(QPalette::Active, QPalette::Window, brush7);
        palette.setBrush(QPalette::Active, QPalette::Button, brush7);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush7);
        this->setAutoFillBackground(true);
        this->setPalette(palette);
    }
}

void PixelMatrixCutButton::setActiveColor(QColor color)
{
    activeColor_ = color;
    refreshIt();
}
