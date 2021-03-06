#/*===============================================================================
# * Chewie: the FERMILAB MTEST telescope and DUT anaysis tool
# * 
# * Copyright (C) 2014 
# *
# * Authors:
# *
# * Mauro Dinardo      (Universita' Bicocca) 
# * Dario Menasce      (INFN) 
# * Jennifer Ngadiuba  (INFN)
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

# Initialization recipe:

# On hal9000:
# scl enable devtoolset-2 tcsh
# source setup-6.csh

# On cip:
# source setup-6.csh

# To switch from one to the other:
# qmake
# make [-f MakefileAll] distclean
# qmake && make [-f MakefileAll]

export BASEDATADIR=/nfs/cms/mc1/as2872/TB_Data

#===== Local directories
export CHEWIEDIR=`pwd`
export CHEWIEDATADIR=$BASEDATADIR/MonicelliOutput
export CHEWIEINPUTDIR=$BASEDATADIR/ChewieInput
export CHEWIEOUTPUTDIR=$BASEDATADIR/ChewieOutput
export CHEWIEXMLDIR=$CHEWIEDIR/xml

export MonicelliDir=`cd ../Monicelli_Feb/; pwd; cd $CHEWIEDIR`
export Monicelli_CalSample_Dir=$BASEDATADIR/Calibrations  

#===== Location of the ROOT components
export ROOTVER=SIX
export ROOTSYS=/nfs/cms/mc1/as2872/TB_Software/root-6.06.06-build
export ROOTINC=$ROOTSYS/include
export ROOTLIB=$ROOTSYS/lib

#===== Location of the Qt components
export QTDIR=/nfs/opt/qt-5.7.1/5.7/gcc_64
export QTCREATORDIR=/nfs/opt/qt-5.7.1/Tools/QtCreator
export QTLIB=$QTDIR/lib
export QTINC=$QTDIR/include

#===== Location of the BOOST components
export BOOSTINC=/usr/include/boost
export BOOSTLIB=/usr/lib

#===== Location of the XERCES components
export XERCESCINC=/nfs/cms/mc1/as2872/local/include
export XERCESCLIB=/nfs/cms/mc1/as2872/local/lib
  
#===== C++ flags
export CPLUSPLUSFLAGS=-std=c++11

export LD_LIBRARY_PATH=/usr/lib64

alias qtcreator=$QTCREATORDIR/bin/qtcreator


#===== Final PATH definitions
export PATH=$ROOTSYS/bin:$QTDIR/bin:$QTCREATORDIR/bin:$PATH
export LD_LIBRARY_PATH=$ROOTLIB:$BOOSTLIB:$QTDIR/lib:$QTCREATORDIR/$XERCESCLIB/:$MonicelliDir/plugins/libs/:$MonicelliDir/test/customWidgets/libs:$LD_LIBRARY_PATH
