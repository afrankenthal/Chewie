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

#include <QApplication>
#include <QFile>
#include <QPlastiqueStyle>
#include "MessageTools.h"
#include <sstream>
#include "mainwindow.h"

#include <TApplication.h>
#include <TROOT.h>

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(Chewie);

    system("clear") ;

    TApplication rootapp("Simple Qt ROOT Application", &argc, argv);

    std::stringstream ss_ ;

    QApplication::setStyle(new QPlastiqueStyle()) ;
    QApplication app(argc, argv);

    char* envVariables;
    bool missingEnvVariable = false;

    envVariables = getenv("CHEWIEDIR");
    if(envVariables == 0)
    {
        FATAL("The 'CHEWIEDIR' environment variable is not defined",ACYellow);
        missingEnvVariable = true;
    }
    envVariables = getenv("CHEWIEDATADIR");
    if(envVariables == 0)
    {
        FATAL("The 'CHEWIEDATADIR' environment variable is not defined",ACYellow);
        missingEnvVariable = true;
    }
    envVariables = getenv("CHEWIEOUTPUTDIR");
    if(envVariables == 0)
    {
        FATAL("The 'CHEWIEOUTPUTDIR' environment variable is not defined",ACYellow);
        missingEnvVariable = true;
    }
    else
    {
        QFile tmpFile(envVariables) ;
        if( !tmpFile.exists() )
        {
            ss_.str("") ; ss_ << "The " << envVariables << " directory (CHEWIEOUTPUTDIR) does not exist" ;
            FATAL(ss_.str(),ACRed) ;
            FATAL("Please create one...",ACCyan) ;
            missingEnvVariable = true;
        }
    }
    envVariables = getenv("CHEWIEXMLDIR");
    if(envVariables == 0)
    {
        FATAL("The 'CHEWIEXMLDIR' environment variable is not defined",ACYellow);
        missingEnvVariable = true;
    }
    if(missingEnvVariable)
        {
        app.closeAllWindows() ;
        app.quit() ;
        exit(0);
    }

    MainWindow mainWin;
    mainWin.show();

    std::string color = std::string(ACYellow)+std::string(ACBold)+std::string(ACReverse) ;
    STDLINE("",color);
    STDLINE("+--------------------------------------------------+",color);
    STDLINE("|                                                  |",color);
    STDLINE("|       W e l c o m e   t o   C h e w i e          |",color);
    STDLINE("|                                                  |",color);
    STDLINE("|        The MTEST pixel-telescope tracks          |",color);
    STDLINE("|             analysis code framework              |",color);
    STDLINE("|    at Fermi National Accelerator Laboratory      |",color);
    STDLINE("|                                                  |",color);
    STDLINE("|        D. Menasce, L. Moroni, S. Terzo           |",color);
    STDLINE("|      J. Ngadiuba, L. Uplegger, L. Vigani         |",color);
    STDLINE("|                                                  |",color);
    STDLINE("+--------------------------------------------------+",color);
    STDLINE("",color);

    gROOT->SetBatch(true);
    return app.exec();
}
