#include "PixelMatrixCut.h"
#include "MessageTools.h"

PixelMatrixCut::PixelMatrixCut(QWidget *parent) :
    QWidget(parent)
{
    Ncols_ = 52;
    Nrows_ = 80;
    cellWidthX_ = 9;
    cellWidthY_ = 6;
    borderX_   = 2 ;
    borderY_   = 1 ;
    isCtrlPressed_ = false;

    int r = 200;
    int g = 0;
    int b = 200;
    palette_[0] = QColor(r,g,b);//violet
    for (int i1 = 1; i1 < 5; ++i1)
    {
        r = r - 40;
        if (r<0) r = 0;
        b = b + 11;
        if (b>255) b = 255;
        palette_[i1] = QColor(r,g,b);
    }//blue
    r = 0;
    for (int i2 = 5; i2 < 27; ++i2)
    {
        g = g + 12;
        if (g>255) g = 255;
        b = b - 12;
        if (b<0) b = 0;
        palette_[i2] = QColor(r,g,b);
    }//green
    b = 0;
    for (int i3 = 27; i3 < 33; ++i3)
    {
        r = r + 42;
        if (r>255) r = 255;
        g = g - 1;
        if (g<0) g = 0;
        palette_[i3] = QColor(r,g,b);
    }//yellow
    for (int i4 = 33; i4 < 50; ++i4)
    {
        r = r + 2;
        if (r>255) r = 255;
        g = g - 16;
        if (g<0) g = 0;
        palette_[i4] = QColor(r,g,b);
    }//red

}

PixelMatrixCut::~PixelMatrixCut()
{
    for (std::map<std::pair<int,int>, PixelMatrixCutButton*>::iterator it = pixelButtons_.begin(); it != pixelButtons_.end(); ++it)
    {
        delete it->second;
    }
}

void PixelMatrixCut::selectWholeCol()
{
    int col = static_cast<PixelMatrixCutButton*>(sender())->getCol();
    bool sel = static_cast<PixelMatrixCutButton*>(sender())->getIsSelected();
    for (int j = 0; j < Nrows_; ++j)
    {
        pixelButtons_[std::pair<int,int>(col,j)]->clickIt(sel);
    }
}

void PixelMatrixCut::selectWholeRow()
{
    int row = static_cast<PixelMatrixCutButton*>(sender())->getRow();
    bool sel = static_cast<PixelMatrixCutButton*>(sender())->getIsSelected();
    for (int i = 0; i < Ncols_; ++i)
    {
        pixelButtons_[std::pair<int,int>(i,row)]->clickIt(sel);
    }
}

void PixelMatrixCut::init()
{
    for (int i = 0; i < Ncols_; ++i)
    {
        for (int j = 0; j < Nrows_; ++j)
        {
            pixelButtons_[std::pair<int,int>(i,j)] = new PixelMatrixCutButton (this, i, j, 20, 10);
            pixelButtons_[std::pair<int,int>(i,j)]->init();
            selectedPixels_[std::pair<int,int>(i,j)] = true;
        }
        pixelButtons_[std::pair<int,int>(i,-1)] = new PixelMatrixCutButton (this, i, -1, 20, 8);
        pixelButtons_[std::pair<int,int>(i,-1)]->init();
        connect(pixelButtons_[std::pair<int,int>(i,-1)], SIGNAL(clicked()), this, SLOT(selectWholeCol()));
//        PixelMatrixCutButton * cacca = new PixelMatrixCutButton (this, i, -1, 20, 8);
//        cacca->init();
//        connect(cacca, SIGNAL(selectRow), this, SLOT(selectWholeRow));
    }
    for (int j = 0; j < Nrows_; ++j)
    {
       pixelButtons_[std::pair<int,int>(-1,j)] = new PixelMatrixCutButton (this, -1, j, 14, 10);
       pixelButtons_[std::pair<int,int>(-1,j)]->init();
       connect((QObject*)pixelButtons_[std::pair<int,int>(-1,j)], SIGNAL(clicked()), this, SLOT(selectWholeRow()));
    }
}

std::map<std::pair<int, int>, bool> PixelMatrixCut::getSelectedPixels()
{
    for (std::map<std::pair<int,int>, PixelMatrixCutButton*>::iterator it = pixelButtons_.begin(); it != pixelButtons_.end(); ++it)
    {
        selectedPixels_[it->first] = it->second->getIsSelected();
    }

    return selectedPixels_;
}

void PixelMatrixCut::setReferenceHistogram(TH2F *histo)
{
    if (histo)
    {
        theHisto_ = histo;

        QColor white = QColor(255,255,255);

        int ix, iy, valScale;
        float val;
        double min = theHisto_->GetMinimum();
        double max = theHisto_->GetMaximum();
        for (int i = 0; i < Ncols_; ++i)
        {
            for (int j = 0; j < Nrows_; ++j)
            {
                ix = theHisto_->GetXaxis()->FindBin((float)(i+0.5));
                iy = theHisto_->GetYaxis()->FindBin((float)(j+0.5));
                if (theHisto_->GetBinContent(ix,iy) <= 0)
                    pixelButtons_[std::pair<int,int>(i,j)]->setActiveColor(white);
                else
                {
                    val = ((float)theHisto_->GetBinContent(ix,iy)-min)/(max-min);
                    valScale = val*49;
                    pixelButtons_[std::pair<int,int>(i,j)]->setActiveColor(palette_[valScale]);
                }
            }
        }
    }
    else
    {
        for (int i = 0; i < Ncols_; ++i)
        {
            for (int j = 0; j < Nrows_; ++j)
            {
                    pixelButtons_[std::pair<int,int>(i,j)]->setActiveColor(QColor(0,255,0,255));//green

            }
        }
    }
//    emit cazzo();
}

void PixelMatrixCut::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()) {
        case Qt::Key_Control: {
            isCtrlPressed_ = true;
            std::cout << "ctrl got pressed\n";
            break;
        }
    default:
            break;
        }
}

void PixelMatrixCut::keyReleaseEvent(QKeyEvent *e)
{
    switch(e->key()) {
        case Qt::Key_Control: {
            if (isCtrlPressed_)
                isCtrlPressed_ = false;
            std::cout << "ctrl got released\n";
            break;
        }
    default:
            break;
        }
}
