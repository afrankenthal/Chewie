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
