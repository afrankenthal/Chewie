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

#ifndef CANVASWIDGET_H
#define CANVASWIDGET_H

#include <sstream>
#include <mainwindow.h>
#include <QtWidgets/QWidget>
#include <QtGui/QResizeEvent>
#include "TCanvas.h"
//#include <HanSoloFitter.h>
#include "qrootcanvas.h"

namespace Ui {
    class CanvasWidget;
}

class CanvasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CanvasWidget(QWidget * parent = 0);
            ~CanvasWidget();

    void     divide      (int nx, int ny    ) ;
    void     cd          (int pos           ) ;
    void     clear       (void              ) ;
    void     flush       (void              ) ;
    void     update      (void              ) ;
    void     setTitle    (std::string title ) ;
    void     setLogX     (bool set          ){logX_ = set       ;}
    void     setLogY     (bool set          ){logY_ = set       ;}
    void     setLogZ     (bool set          ){logZ_ = set       ;}

    TCanvas * getCanvas  (void              ) {return theCanvas_;}

private:
    Ui::CanvasWidget  * ui               ;
    TCanvas           * theCanvas_       ;
    HanSoloFitter*      theHanSoloFitter_;
    bool                logX_            ;
    bool                logY_            ;
    bool                logZ_            ;

    QRootCanvas       * canvas_          ;

    std::stringstream   ss_              ;

    void resizeEvent ( QResizeEvent * event ) ;

private slots:
} ;

#endif // CANVASWIDGET_H
