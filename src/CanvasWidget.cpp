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

#include "CanvasWidget.h"
#include "uiFiles/ui_canvaswidget.h"

#include "MessageTools.h"

#include <TROOT.h>
#include "TH1D.h"
#include "TList.h"
#include <iostream>

//===========================================================================
CanvasWidget::CanvasWidget(QWidget * parent) :
    QWidget(parent)
  ,ui(new Ui::CanvasWidget)
  , logX_(false)
  , logY_(false)
  , logZ_(false)

{
    ui->setupUi(this);
    canvas_ = new QRootCanvas(this,"") ;
}

//===========================================================================
CanvasWidget::~CanvasWidget()
{
    //  STDLINE("Dtor",ACRed) ;
     //delete ui;

}

//===========================================================================
void CanvasWidget::divide( int nx, int ny )
{
    //std::cout << "ui->theCanvas:" << ui->theCanvas << std::endl;
    canvas_->GetCanvas()->Clear() ;
    canvas_->GetCanvas()->Divide(nx, ny, 0.001, 0.001) ;
    canvas_->GetCanvas()->Modified() ;
    canvas_->GetCanvas()->Update();
}

//===========================================================================
void CanvasWidget::cd( int pos )
{
    canvas_->GetCanvas()->cd(pos) ;
}

//===========================================================================
void CanvasWidget::flush( void )
{
    int tot = 0 ;
    TObject * obj = NULL ;
    TIter next(canvas_->GetCanvas()->GetListOfPrimitives()) ;
    while((obj = next()))
    {
        if( obj->InheritsFrom(TVirtualPad::Class()))
        {
            canvas_->GetCanvas()->SetClickSelectedPad((TPad*)(obj)) ;
            canvas_->GetCanvas()->SetClickSelected((obj)) ;
            tot++ ;
            canvas_->GetCanvas()->cd(tot) ;
            gPad->SetLogx(logX_);
            gPad->SetLogy(logY_);
            gPad->SetLogz(logZ_);
            gPad->Modified();
            gPad->Update();
            canvas_->GetCanvas()->Modified() ;
            canvas_->GetCanvas()->Update();


            //      ss_.str("") ;
            //      ss_ << "Working on pad " << tot ;
            //      STDLINE(ss_.str(),ACWhite) ;
            //      sleep(1) ;
        }
    }
}

//===========================================================================
void CanvasWidget::update( void )
{
    canvas_->GetCanvas()->Modified() ;
    canvas_->GetCanvas()->Update();
}

//===========================================================================
void CanvasWidget::clear( )
{
    canvas_->GetCanvas()->Clear();
    canvas_->GetCanvas()->Modified() ;
    canvas_->GetCanvas()->Update();
}

//===========================================================================
void CanvasWidget::resizeEvent ( QResizeEvent * re )
{
    int margin = 0 ;
    QSize newSize(re->size().width()-margin, re->size().height()-margin); // Allow room for margins
    canvas_->resize(newSize) ;
    canvas_->GetCanvas()->Modified() ;
    canvas_->GetCanvas()->Update();
}

//===========================================================================
void CanvasWidget::setTitle(std::string title)
{
    canvas_->setWindowTitle(QApplication::translate("CanvasWidget", title.c_str(), 0, QApplication::UnicodeUTF8));
}
