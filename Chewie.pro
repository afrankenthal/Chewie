#/*===============================================================================
# * Monicelli: the FERMILAB MTEST geometry builder and track reconstruction tool
# * 
# * Copyright (C) 2014 
# *
# * Authors:
# *
# * Dario Menasce      (INFN) 
# * Luigi Moroni       (INFN)
# * Jennifer Ngadiuba  (INFN)
# * Stefano Terzo      (INFN)
# * Lorenzo Uplegger   (FNAL)
# * Luigi Vigani       (INFN)
# *
# * INFN: Piazza della Scienza 3, Edificio U2, Milano, Italy 20126
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 3 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program.  If not, see <http://www.gnu.org/licenses/>.
# ================================================================================*/

include("$(ROOTINC)/rootcint.pri")

QMAKE_CFLAGS_RELEASE   = -O3 $(CPLUSPLUSFLAGS)
QMAKE_CXXFLAGS_RELEASE = -g  $(CPLUSPLUSFLAGS)

target.path   	     = ./
sources.path  	     = ./

HEADERS       	     = include/mainwindow.h					  \
              	       include/MessageTools.h					  \
              	       include/ANSIColors.h					  \
              	       include/AnalysisManager.h				  \
              	       include/Data.h						  \
              	       include/Analysis.h					  \
              	       include/Threader.h					  \
              	       include/Efficiency.h					  \
              	       include/EventManager.h					  \
              	       include/EventConverter.h 				  \
              	       include/HistogramWindow.h				  \
              	       include/Window.h 					  \
              	       include/ThreadUtilities.h				  \
              	       include/CutsUtilities.h  				  \
              	       include/CutsUtilities.h  				  \
              	       include/Charge.h 					  \
              	       include/XmlParser.h					  \
              	       include/analyzerdlg.h					  \
              	       include/hnavigator.h					  \
              	       include/PlanesMapping.h  				  \
              	       include/XmlAnalysis.h					  \
              	       include/XmlGeneral.h					  \
              	       include/WindowsManager.h 				  \
              	       include/XmlWindow.h					  \
              	       include/XmlPlane.h					  \
              	       include/Resolution.h					  \
              	       include/XmlConverter.h					  \
              	       include/Utilities.h					  \
              	       include/XmlScan.h					  \
              	       include/CanvasWidget.h					  \
              	       include/Tracks.h 					  \
              	       include/CalibrationsManager.h				  \
              	       ../Monicelli/include/EventHeader.h			  \
              	       ../Monicelli/include/Event.h				  \
              	       ../Monicelli/include/Geometry.h  			  \
              	       ../Monicelli/include/Detector.h  			  \
              	       ../Monicelli/include/ROC.h				  \
              	       ../Monicelli/include/mdisubwindow.h			  \
              	       ../Monicelli/plugins/customTextEdit/customTextEdit.h	  \
              	       ../Monicelli/plugins/customLineEdit/customLineEdit.h	  \
              	       ../Monicelli/plugins/customCheckBox/customCheckBox.h	  \
              	       ../Monicelli/plugins/customSpinBox/customSpinBox.h	  \
              	       ../Monicelli/plugins/customTableWidget/customTableWidget.h \
    		       include/PixelMatrix.h					  \
    		       include/TracksAfter.h					  \
    		       include/fitParamManagerWidget.h  			  \
    		       include/PixelMatrixCut.h 				  \
    		       include/PixelMatrixCutButton.h				  \
    		       include/HanSoloFitter.h  				  \
    		       include/HanSoloTreeBrowser.h				  \
                       include/hTreeBrowser.h                                     \
                       include/qrootcanvas.h

SOURCES       	     = src/EventManager.cpp		 			  \
              	       src/EventConverter.cpp		 			  \
              	       src/Data.cpp			 			  \
              	       src/AnalysisManager.cpp  	 			  \
              	       src/WindowsManager.cpp		 			  \
              	       src/Window.cpp			 			  \
              	       src/HistogramWindow.cpp  	 			  \
              	       src/Analysis.cpp 		 			  \
              	       src/Efficiency.cpp		 			  \
              	       src/Charge.cpp			 			  \
              	       src/Threader.cpp 		 			  \
              	       src/ThreadUtilities.cpp  	 			  \
                       src/PlanesMapping.cpp		 	 		  \
              	       src/XmlAnalysis.cpp		 			  \
              	       src/XmlGeneral.cpp		 			  \
              	       src/analyzerdlg.cpp		 			  \
              	       src/hnavigator.cpp		 			  \
              	       src/mainwindow.cpp		 			  \
              	       src/main.cpp			 			  \
              	       src/XmlWindow.cpp		 			  \
              	       src/XmlPlane.cpp 		 			  \
              	       src/XmlParser.cpp		 			  \
              	       src/Resolution.cpp		 			  \
              	       src/XmlConverter.cpp		 			  \
              	       src/Utilities.cpp		 			  \
              	       src/CanvasWidget.cpp		 			  \
              	       src/Tracks.cpp			 			  \
              	       src/CalibrationsManager.cpp	 			  \
              	       tmp/EventDict.C  		 			  \
              	       tmp/EventHeaderDict.C		 			  \
              	       ../Monicelli/src/EventHeader.cpp  			  \
              	       ../Monicelli/src/Event.cpp	 			  \
              	       ../Monicelli/src/Geometry.cpp	 			  \
              	       ../Monicelli/src/Detector.cpp	 			  \
              	       ../Monicelli/src/ROC.cpp 	 			  \
              	       ../Monicelli/src/mdisubwindow.cpp 			  \
              	       src/XmlScan.cpp  		 			  \
    		       src/PixelMatrix.cpp		 			  \
    		       src/TracksAfter.cpp		 			  \
    		       src/fitParamManagerWidget.cpp	 			  \
    		       src/PixelMatrixCut.cpp		 			  \
    		       src/PixelMatrixCutButton.cpp	 			  \
    		       src/HanSoloFitter.cpp		 			  \
    		       src/HanSoloTreeBrowser.cpp	 			  \
                       src/hTreeBrowser.cpp                                       \
                       src/qrootcanvas.cpp

RESOURCES    	     = Chewie.qrc

DEFINES      	     = USE_QT

INCLUDEPATH  	    += ./include						  \
             	       ../Monicelli						  \
             	       ../Monicelli/include					  \
             	       ../Monicelli/plugins/customLineEdit			  \
             	       ../Monicelli/plugins/customTextEdit			  \
             	       ../Monicelli/plugins/customCheckBox			  \
             	       ../Monicelli/plugins/customSpinBox			  \
             	       ../Monicelli/plugins/customTableWidget			  \
             	       -pthread $(ROOTINC)			                  \
             	       $(QTDIR)/include/QtXml					  \
             	       $(XERCESCINC)						  \
             	       $(BOOSTINC)/include/boost/

LIBS         	    += -lTreePlayer						  \

LIBS         	    += -L$(ROOTLIB)						  \

LIBS         	    += -L$(BOOSTLIB)						  \
             	       -lboost_regex						  \
          	       -lboost_system  					          \
             	       -lboost_filesystem     


LIBS         	    += -L../Monicelli/plugins/libs				  \
             	       -lcustomLineEdit 					  \
             	       -lcustomTextEdit 					  \
             	       -lcustomCheckBox 					  \
             	       -lcustomSpinBox  					  \
             	       -lcustomTableWidget					  \

# install
sources.files        = $$SOURCES   						  \
                       $$HEADERS   						  \
		       $$RESOURCES 						  \
		       $$FORMS     						  \
		       Chewie.pro  						  \
		       images
 
INSTALLS            += target sources

header.depends       = ../Monicelli/include/EventHeader.h

header.target        = tmp/EventHeaderDict.C

ROOTVER              = $(ROOTVER)

contains(ROOTVER, FIVE) {
 header.commands     = @echo "------ ROOT5 header ----------"                  && \
                       rootcint -f tmp/EventHeaderDict.C                          \
                                -c ../Monicelli/include/EventHeader.h+
} else {
 header.commands     = @echo "------ ROOT6 header ----------"                  && \
                       rootcint -f tmp/EventHeaderDict.C                          \
                                -c ../Monicelli/include/EventHeader.h+         && \
                       cp tmp/*.pcm .
}

trees.depends        = ../Monicelli/include/Event.h                               \
                       ../Monicelli/include/Geometry.h                            \
                       ../Monicelli/include/Detector.h                            \
                       ../Monicelli/include/ROC.h

trees.target         = tmp/EventDict.C

contains(ROOTVER, FIVE) {
 trees.commands      = @echo "------ ROOT5 commands --------"                  && \
                       rootcint -f tmp/EventDict.C                                \
                                -c ../Monicelli/include/Event.h+                  \
                                   ../Monicelli/include/Geometry.h+               \
                                   ../Monicelli/include/Detector.h+               \
                                   ../Monicelli/include/ROC.h+
} else {
 trees.commands      = @echo "------ ROOT6 commands --------"                  && \
                       rootcint -f tmp/EventDict.C                                \
                                -c ../Monicelli/include/Event.h+                  \
                                   ../Monicelli/include/Geometry.h+               \
                                   ../Monicelli/include/Detector.h+               \
                                   include/ROC.h+                              && \
                       cp tmp/*.pcm .
}

QMAKE_EXTRA_TARGETS += trees
QMAKE_EXTRA_TARGETS += header

FORMS 		    += uiFiles/analyzerdlg.ui   				  \
                       uiFiles/hnavigator.ui    				  \
                       uiFiles/canvaswidget.ui  				  \
                       uiFiles/hanSoloFitter.ui

MOC_DIR             += mocFiles
UI_DIR              += uiFiles
OBJECTS_DIR         += objFiles

QT                  *= xml

DEPENDPATH          += . src include ../Monicelli/plugins/customTableWidget

unix:!macx:!symbian: LIBS += -lTreePlayer

