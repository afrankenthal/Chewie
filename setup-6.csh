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

# scl enable devtoolset-2 tcsh

setenv BASEDATADIR ../data/2015_10_October

if(${HOSTNAME} == "hal9000.mib.infn.it") then

  if	  ( ${USER} == "menasce" ) then
   setenv BASEDATADIR ../data/2015_10_October
  else if ( ${USER} == "dinardo" ) then
   setenv BASEDATADIR /raid2/data1/user/gr1/e831/dinardo/TestBeamData2015
  endif

  setenv MonicelliDir             ../Monicelli/
  setenv Monicelli_CalSample_Dir  ${BASEDATADIR}/Calibrations  

  #===== Local directories
  setenv CHEWIEDIR		  `pwd`
  setenv CHEWIEDATADIR  	  ${BASEDATADIR}/MonicelliOutput
  setenv CHEWIEINPUTDIR 	  ${BASEDATADIR}/ChewieInput
  setenv CHEWIEOUTPUTDIR	  ${BASEDATADIR}/ChewieOutput
  setenv CHEWIEXMLDIR		  ${CHEWIEDIR}/xml

  #===== Location of the ROOT components
  setenv ROOTVER                  SIX
  setenv ROOTSYS		  /user/service/root-6.04.10/
  setenv ROOTINC		  $ROOTSYS/include/root
  setenv ROOTLIB		  $ROOTSYS/lib/root

  #===== Location of the Qt components
  setenv QTDIR  		  /usr/local/Trolltech/Qt-4.8.5
  setenv QTCREATORDIR		  /opt/local/qtcreator-2.7.1/bin/
  setenv QTROOTSYSDIR		  `pwd`/qtrootsys
  setenv QTROOT_INCLUDE_PATH	  $ROOTSYS/include

  #===== Location of the BOOST components
  setenv BOOSTINC		  /usr/local/include/boost
  setenv BOOSTLIB		  /usr/local/lib

  #===== Location of the XERCES components
  setenv XERCESCINC		  /opt/local/xerces/include
  setenv XERCESCLIB		  /opt/local/xerces/lib
  
  #===== C++ flags
  setenv CPLUSPLUSFLAGS           -std=c++11

  setenv LD_LIBRARY_PATH          /usr/lib64

  alias qtcreator ${QTCREATORDIR}/qtcreator

endif

#===== Final PATH definitions
setenv PATH		${ROOTSYS}/bin:${QTDIR}/bin:${QTCREATORDIR}/bin:${PATH}
setenv LD_LIBRARY_PATH  ${ROOTLIB}:${BOOSTLIB}:${QTDIR}/lib:${QTCREATORDIR}/${XERCESCLIB}/:${MonicelliDir}/plugins/libs/:${MonicelliDir}/test/customWidgets/libs:${LD_LIBRARY_PATH}
