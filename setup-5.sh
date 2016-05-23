#/*===============================================================================
# * Monicelli: the FERMILAB MTEST geometry builder and track reconstruction tool
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

export BASEDATADIR=../data/2015_10_October

if [ ${HOSTNAME} == "hal9000.mib.infn.it" ]; then

  if [ ${USER} == "menasce" ]; then
      export BASEDATADIR=/user/gr1/e831/menasce/MonicelliNew/data/2015_10_October
  elif [ ${USER} == "dinardo" ]; then
      export BASEDATADIR=/raid2/data1/user/gr1/e831/dinardo/TestBeamData2015
  fi

  #===== Local directories
  export CHEWIEDIR=`pwd`
  export CHEWIEDATADIR=${BASEDATADIR}/MonicelliOutput
  export CHEWIEINPUTDIR=${BASEDATADIR}/ChewieInput
  export CHEWIEOUTPUTDIR=${BASEDATADIR}/ChewieOutput
  export CHEWIEXMLDIR=${CHEWIEDIR}/xml
  
  export MonicelliDir=`cd ../Monicelli/; pwd; cd $CHEWIEDIR`
  export Monicelli_CalSample_Dir=${BASEDATADIR}/Calibrations  

  #===== Location of the ROOT components
  export ROOTVER=FIVE
  export ROOTSYS=/user/service/root-5.28.00g/
  export ROOTINC=$ROOTSYS/include/root
  export ROOTLIB=$ROOTSYS/lib/root

  #===== Location of the Qt components
  export QTDIR=/usr/local/Trolltech/Qt-4.8.5/
  export QTCREATORDIR=/opt/local/qtcreator-2.7.1/bin/
  export QTROOTSYSDIR=`pwd`/qtrootsys
  export QTROOT_INCLUDE_PATH=$ROOTSYS/include

  #===== Location of the BOOST components
  export BOOSTINC=/usr/local/include/boost
  export BOOSTLIB=/usr/local/lib

  #===== Location of the XERCES components
  export XERCESCINC=/opt/local/xerces/include
  export XERCESCLIB=/opt/local/xerces/lib
  
  #===== C++ flags
  export CPLUSPLUSFLAGS=-std=c++0x

  export LD_LIBRARY_PATH	  
  
  alias qtcreator=${QTCREATORDIR}/qtcreator

fi

#===== Final PATH definitions
export PATH=${ROOTSYS}/bin:${QTDIR}/bin:${QTCREATORDIR}/bin:${PATH}
export LD_LIBRARY_PATH=${ROOTLIB}:${BOOSTLIB}:${QTDIR}/lib:${QTCREATORDIR}/${XERCESCLIB}/:${MonicelliDir}/plugins/libs/:${MonicelliDir}/test/customWidgets/libs:${LD_LIBRARY_PATH}
