#ifndef PIXELMATRIXCUT_H
#define PIXELMATRIXCUT_H

#include <QWidget>
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
//    void cazzo();

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
