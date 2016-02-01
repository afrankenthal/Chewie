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

#include "fitManagerListWidgetItem.h"

#include "MessageTools.h"

#include <iostream>

//===========================================================================
FitManagerListWidgetItem::FitManagerListWidgetItem(QListWidgetItem *parent) :
    QListWidgetItem(*parent)
//  ,ui(new Ui::FitParamManagerWidget)

{
//    this->setGeometry(5, 5, 206, 101);

    theFitManager_ = new FitParamManagerWidget;
    theFitManager_->setGeometry(5, 5, 206, 101);
    theFitManager_->show();

//    ui->setupUi(this);
}

//===========================================================================
FitManagerListWidgetItem::~FitManagerListWidgetItem()
{
    //  STDLINE("Dtor",ACRed) ;
    //  delete ui;

    delete theFitManager_;
}
