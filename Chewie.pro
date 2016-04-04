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

HEADERS       	     = include/analyzerdlg.h					  \
                       include/AnalysisManager.h				  \
                       include/Analysis.h					  \
                       include/ANSIColors.h					  \
                       include/CalibrationsManager.h				  \
                       include/CanvasWidget.h					  \
                       include/Charge.h 					  \
                       include/ChargeUniMiB.h 					  \
                       include/CutsUtilities.h  				  \
                       include/CutsUtilities.h  				  \
                       include/Data.h						  \
                       include/Efficiency.h					  \
                       include/EfficiencyUniMiB.h				  \
                       include/EventManager.h					  \
                       include/EventConverter.h 				  \
                       include/fitParamManagerWidget.h  			  \
                       include/HanSoloFitter.h  				  \
                       include/HanSoloTreeBrowser.h				  \
                       include/HistogramWindow.h				  \
                       include/hnavigator.h					  \
                       include/hTreeBrowser.h                                     \
                       include/mainwindow.h					  \
                       include/MessageTools.h					  \
                       include/PixelMatrix.h					  \
                       include/PixelMatrixCut.h 				  \
                       include/PixelMatrixCutButton.h				  \
                       include/PlanesMapping.h  				  \
                       include/qrootcanvas.h                                      \
                       include/Resolution.h					  \
                       include/ResolutionUniMiB.h			          \
                       include/Threader.h					  \
                       include/ThreadUtilities.h				  \
                       include/Tracks.h 					  \
                       include/TracksAfter.h					  \
                       include/Utilities.h					  \
                       include/XmlAnalysis.h					  \
                       include/XmlConverter.h					  \
                       include/XmlGeneral.h					  \
                       include/XmlParser.h					  \
              	       include/XmlPlane.h					  \
              	       include/XmlScan.h					  \
                       include/XmlWindow.h					  \
                       include/Window.h 					  \
                       include/WindowsManager.h 				  \
                       ../Monicelli/include/Detector.h  			  \
                       ../Monicelli/include/Event.h				  \
                       ../Monicelli/include/EventHeader.h			  \
              	       ../Monicelli/include/Geometry.h  			  \
                       ../Monicelli/include/mdisubwindow.h			  \
                       ../Monicelli/include/ROC.h				  \
                       ../Monicelli/plugins/customCheckBox/customCheckBox.h	  \
                       ../Monicelli/plugins/customCheckBox/customComboBox.h	  \
                       ../Monicelli/plugins/customLineEdit/customLineEdit.h	  \
                       ../Monicelli/plugins/customSpinBox/customSpinBox.h	  \
                       ../Monicelli/plugins/customTableWidget/customTableWidget.h \
                       ../Monicelli/plugins/customTextEdit/customTextEdit.h

SOURCES       	     = src/Analysis.cpp 		 			  \
                       src/AnalysisManager.cpp  	 			  \
                       src/analyzerdlg.cpp		 			  \
                       src/CalibrationsManager.cpp	 			  \
                       src/CanvasWidget.cpp		 			  \
                       src/Charge.cpp			 			  \
                       src/ChargeUniMiB.cpp			 		  \
                       src/Data.cpp			 			  \
                       src/Efficiency.cpp		 			  \
                       src/EfficiencyUniMiB.cpp		 			  \
                       src/EventManager.cpp		 			  \
              	       src/EventConverter.cpp		 			  \
                       src/fitParamManagerWidget.cpp	 			  \
                       src/HanSoloFitter.cpp		 			  \
                       src/HanSoloTreeBrowser.cpp	 			  \
                       src/HistogramWindow.cpp  	 			  \
                       src/hnavigator.cpp		 			  \
                       src/hTreeBrowser.cpp                                       \
                       src/mainwindow.cpp		 			  \
                       src/main.cpp			 			  \
                       src/Tracks.cpp			 			  \
                       src/TracksAfter.cpp		 			  \
                       src/Threader.cpp 		 			  \
              	       src/ThreadUtilities.cpp  	 			  \
                       src/PixelMatrix.cpp		 			  \
                       src/PixelMatrixCut.cpp		 			  \
                       src/PixelMatrixCutButton.cpp	 			  \
                       src/PlanesMapping.cpp		 	 		  \
                       src/qrootcanvas.cpp                                        \
                       src/Resolution.cpp		 			  \
                       src/ResolutionUniMiB.cpp		 			  \
                       src/Utilities.cpp		 			  \
                       src/XmlAnalysis.cpp		 			  \
                       src/XmlConverter.cpp		 			  \
                       src/XmlGeneral.cpp		 			  \
              	       src/XmlWindow.cpp		 			  \
                       src/XmlParser.cpp		 			  \
                       src/XmlPlane.cpp 		 			  \
                       src/XmlScan.cpp  		 			  \
                       src/Window.cpp			 			  \
                       src/WindowsManager.cpp		 			  \
                       tmp/EventDict.C  		 			  \
              	       tmp/EventHeaderDict.C		 			  \
                       ../Monicelli/src/Detector.cpp	 			  \
                       ../Monicelli/src/Event.cpp	 			  \
                       ../Monicelli/src/EventHeader.cpp  			  \
              	       ../Monicelli/src/Geometry.cpp	 			  \
                       ../Monicelli/src/mdisubwindow.cpp 			  \
                       ../Monicelli/src/ROC.cpp

RESOURCES    	     = Chewie.qrc

DEFINES      	     = USE_QT

INCLUDEPATH  	    += ./include						  \
             	       ../Monicelli						  \
             	       ../Monicelli/include					  \
                       ../Monicelli/plugins/customCheckBox			  \
                       ../Monicelli/plugins/customComboBox			  \
                       ../Monicelli/plugins/customLineEdit			  \
             	       ../Monicelli/plugins/customTextEdit			  \
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
                       -lcustomCheckBox 					  \
                       -lcustomComboBox 					  \
                       -lcustomLineEdit 					  \
             	       -lcustomTextEdit 					  \
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

header.depends       = ../Monicelli/include/EventHeader.h                         \

header.target        = tmp/EventHeaderDict.C

ROOTVERSION          = $$(ROOTVER)

contains(ROOTVERSION, "FIVE") {
 message("Setting up Makefile for ROOT5")  
 header.commands     = @echo "------ ROOT5 header ----------"                  && \
                       rootcint -f tmp/EventHeaderDict.C                          \
                                -c ../Monicelli/include/EventHeader.h+
} else {
 message("Setting up Makefile for ROOT6")  
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

contains(ROOTVERSION, "FIVE") {
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
                                   ../Monicelli/include/ROC.h+                 && \
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

extraclean.commands  = rm -rf tmp/*                                               \
                       MakefileExpress                                            

clean.depends        = extraclean
QMAKE_EXTRA_TARGETS += clean extraclean

unix:!macx:!symbian: LIBS += -lTreePlayer

