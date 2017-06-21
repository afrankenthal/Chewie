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
# 
# On hal9000:
# scl enable devtoolset-2 tcsh
# source setup-6.csh
#
# On cip:
# source setup-6.csh
#
# To switch from one to the other:
# qmake
# make [-f MakefileAll] distclean 
# qmake && make [-f MakefileAll] 

setenv BASEDATADIR ../data/2015_10_October

echo "This is ${HOSTNAME}";

#===============   Personalized configuration on this computer   ==============================================
if(${HOSTNAME} == "hal9000.mib.infn.it") then

  echo "Compiling on ${HOSTNAME}";

  if	  ( ${USER} == "menasce" ) then
   setenv BASEDATADIR /user/gr1/e831/menasce/MonicelliNew/data/2015_10_October
  else if ( ${USER} == "dinardo" ) then
   setenv BASEDATADIR /raid2/data1/user/gr1/e831/dinardo/TestBeamFBKPlanarBeforeRad_Batch01
  endif

  #===== Local directories
  setenv CHEWIEDIR		  `pwd`
  setenv CHEWIEDATADIR  	  ${BASEDATADIR}/MonicelliOutput
  setenv CHEWIEINPUTDIR 	  ${BASEDATADIR}/ChewieInput
  setenv CHEWIEOUTPUTDIR	  ${BASEDATADIR}/ChewieOutput
  setenv CHEWIEXMLDIR		  ${CHEWIEDIR}/xml

  setenv MonicelliDir             `cd ../Monicelli/; pwd; cd $CHEWIEDIR`
  setenv Monicelli_CalSample_Dir  ${BASEDATADIR}/Calibrations  

  #===== Location of the ROOT components
  setenv ROOTVER                  SIX
  setenv ROOTSYS		  /user/service/root-6.04.10/
  setenv ROOTINC		  $ROOTSYS/include/root
  setenv ROOTLIB		  $ROOTSYS/lib/root

  #===== Location of the Qt components
  setenv QTDIR  		  /user/service/Qt-5.6.2/5.6/gcc_64/
  setenv QTCREATORDIR		  /raid1/menasce/qt-creator-opensource-src-4.3.0

  #===== Location of the BOOST components
  setenv BOOSTINC		  /usr/local/include/boost
  setenv BOOSTLIB		  /usr/local/lib

  #===== Location of the XERCES components
  setenv XERCESCINC		  /opt/local/xerces/include
  setenv XERCESCLIB		  /opt/local/xerces/lib
  
  #===== C++ flags
  setenv CPLUSPLUSFLAGS           -std=c++11

  setenv LD_LIBRARY_PATH          /usr/lib64

#  alias qtcreator ${QTCREATORDIR}/qtcreator

endif
#============================================================================
if(${HOSTNAME} == "cip.mib.infn.it") then

  echo "Compiling on ${HOSTNAME}";
  
  if(     ${USER} == "menasce") then
   setenv BASEDATADIR /raid1/menasce/AnalysisTBF/data/TestBeamData_PlanarRadBatch01_PlanarRadBatch02_3DBatch01/
  else if(${USER} == "dinardo") then
   setenv BASEDATADIR /raid2/data1/vtx1/dinardo/TestBeamFBKPlanarBeforeRad_Batch01
  else if(${USER} == "dzuolo") then
   setenv BASEDATADIR ../data/TestBeamFBKPlanarAfterRad_Batch01_02
  endif
  
  setenv PATH /opt/rh/devtoolset-2/root/usr/bin:/bin:/usr/bin:/usr/local/bin:/usr/printmi/bin

  #===== Local directories
  setenv CHEWIEDIR		   `pwd`
  setenv CHEWIEDATADIR  	   ${BASEDATADIR}/MonicelliOutput
  setenv CHEWIEINPUTDIR 	   ${BASEDATADIR}/ChewieInput
  setenv CHEWIEOUTPUTDIR	   ${BASEDATADIR}/ChewieOutput
  setenv CHEWIEXMLDIR		   ${CHEWIEDIR}/xml

  setenv MonicelliDir             `cd ../Monicelli/; pwd; cd $CHEWIEDIR`
  setenv Monicelli_CalSample_Dir  ${BASEDATADIR}/Calibrations  

  #===== Location of the ROOT components
  setenv ROOTVER                   SIX
  setenv ROOTSYS		   /usr
  setenv ROOTINC		   $ROOTSYS/include/root
  setenv ROOTLIB		   $ROOTSYS/lib64/root

  #===== Location of the Qt components
  setenv QTDIR  		   /user/service/Qt-5.6.2/5.6/gcc_64
  setenv QTCREATORDIR		   /user/service/QtCreator-4.3.0/

  #===== Location of the BOOST components
  setenv BOOSTINC		   /usr/local/include/boost
  setenv BOOSTLIB		   /usr/local/lib

  #===== Location of the XERCES components
  setenv XERCESCINC		   /opt/local/xerces/include
  setenv XERCESCLIB		   /opt/local/xerces/lib
  
  #===== C++ flags
  setenv CPLUSPLUSFLAGS            -std=c++11

  setenv LD_LIBRARY_PATH	   
  
#  alias  qtcreator                 ${QTCREATORDIR}/qtcreator
    
endif


#===== Final PATH definitions
setenv PATH		${ROOTSYS}/bin:${QTDIR}/bin:${QTCREATORDIR}/bin:${PATH}
setenv LD_LIBRARY_PATH  ${ROOTLIB}:${BOOSTLIB}:${QTDIR}/lib:${QTCREATORDIR}/${XERCESCLIB}/:${MonicelliDir}/plugins/libs/:${MonicelliDir}/customWidgets/libs:${LD_LIBRARY_PATH}
