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

#ifndef FITPARAMMANAGERWIDGET_H
#define FITPARAMMANAGERWIDGET_H

#include <sstream>
#include <string.h>

#include <QWidget>
#include <QListWidgetItem>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QLabel>
#include <QCheckBox>

namespace Ui {
    class FitParamManagerWidget;
}

class FitParamManagerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FitParamManagerWidget (QWidget *parent = 0);
            ~FitParamManagerWidget();

    void setParName   (std::string paramName) {parName  ->setText((QString)paramName.c_str());}
    void setParValue  (double p             ) {parValue ->setValue(p)                        ;}
    void setParLimInf (double p             ) {parLimInf->setValue(p)                        ;}
    void setParLimSup (double p             ) {parLimSup->setValue(p)                        ;}
    void setParFixed  (bool   f             );

    void setParPrecision (int decimals          );
    void setParStep      (double step           );
    void setParLimits    (double min, double max);

    std::string getParName   () {return parName  ->text() .toStdString();}
    double      getParValue  () {return parValue ->value()              ;}
    double      getParLimInf () {return parLimInf->value()              ;}
    double      getParLimSup () {return parLimSup->value()              ;}
    bool        isParFixed   () {return fixPar->isChecked()             ;}

    QCheckBox * getParFixed  () {return fixPar                          ;}

private:
    Ui::FitParamManagerWidget  * ui;
    QLineEdit                  * parName;
    QDoubleSpinBox             * parValue;
    QDoubleSpinBox             * parLimInf;
    QDoubleSpinBox             * parLimSup;
    QLabel                     * parSetValue;
    QLabel                     * parSetLimits;
    QCheckBox                  * fixPar;

    std::stringstream   ss_ ;

private slots:
        void on_fixPar_clicked(void);
} ;

#endif // FITPARAMMANAGERWIDGET_H
