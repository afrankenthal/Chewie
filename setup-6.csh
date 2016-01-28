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

if(${HOSTNAME} == "ftbftracker01.fnal.gov") then

  setenv MonicelliDir              ../Monicelli
  setenv Monicelli_CalSample_Dir   ${BASEDATADIR}/Calibrations  

  #===== Local directories
  setenv CHEWIEDIR		   `pwd`
  setenv CHEWIEDATADIR		   ${BASEDATADIR}/MonicelliOutput
  setenv CHEWIEINPUTDIR            ${BASEDATADIR}/ChewieInput
  setenv CHEWIEOUTPUTDIR           ${BASEDATADIR}/ChewieOutput
  setenv CHEWIEXMLDIR              $CHEWIEDIR/xml
  
  #===== Location of the ROOT components
  setenv ROOTSYS		   /opt/local/root

  #===== Location of the Qt components
  setenv QTDIR  		   /opt/local/qt
  setenv QTCREATORDIR		   /opt/local/qtcreator

  #===== Location of the BOOST components
  setenv BOOSTINC		   /usr/local/include/boost
  setenv BOOSTLIB		   /usr/local/lib

  #===== Location of the XERCES components
  setenv XERCESCINC		   /opt/local/xerces/include
  setenv XERCESCLIB		   /opt/local/xerces/lib
  
  setenv LD_LIBRARY_PATH           /usr/lib64

else if(${HOSTNAME} == "hal9000.mib.infn.it") then

  if      ( ${USERNAME} == "menasce" ) then
   setenv BASEDATADIR ../data/2015_10_October
  else if ( ${USERNAME} == "dinardo" ) then
   setenv BASEDATADIR /raid2/data1/user/gr1/e831/dinardo/TestBeamData2015
  endif

  setenv MonicelliDir              ../Monicelli/
  setenv Monicelli_CalSample_Dir   ${BASEDATADIR}/Calibrations  

  #===== Local directories
  setenv CHEWIEDIR		   `pwd`
  setenv CHEWIEDATADIR		   ${BASEDATADIR}/MonicelliOutput
  setenv CHEWIEINPUTDIR            ${BASEDATADIR}/ChewieInput
  setenv CHEWIEOUTPUTDIR           ${BASEDATADIR}/ChewieOutput
  setenv CHEWIEXMLDIR              ${CHEWIEDIR}/xml
  
  #===== Compiler specs
  setenv CPLUSPLUSFLAGS            -std=c++11

  #===== Location of the ROOT components
  setenv ROOTSYS		  /opt/local/root
  setenv ROOTINC		  $ROOTSYS/include
  setenv ROOTLIB		  $ROOTSYS/lib

  #===== Location of the Qt components
  setenv QTDIR  		  /opt/local/qt
  setenv QTCREATORDIR		  /opt/local/qtcreator-2.7.1/bin/
  setenv QTROOTSYSDIR		  `pwd`/qtrootsys
  setenv QTROOT_INCLUDE_PATH	  $ROOTSYS/include

  #===== Location of the BOOST components
  setenv BOOSTINC		   /usr/local/include/boost
  setenv BOOSTLIB		   /usr/local/lib

  #===== Location of the XERCES components
  setenv XERCESCINC		  /opt/local/xerces/include
  setenv XERCESCLIB		  /opt/local/xerces/lib
  
  setenv LD_LIBRARY_PATH          /usr/lib64

  alias qtcreator ${QTCREATORDIR}/qtcreator

else if(${HOSTNAME} == "ftbftracker01.fnal.gov"  || 
        ${HOSTNAME} == "ftbftracker02.fnal.gov"  || 
	${HOSTNAME} == "rulinux01.dhcp.fnal.gov" || 
	${HOSTNAME} == "rulinux02.dhcp.fnal.gov" || 
	${HOSTNAME} == "rulinux03.dhcp.fnal.gov") then

  setenv MonicelliDir              ../Monicelli
  setenv Monicelli_CalSample_Dir   ${BASEDATADIR}/Calibrations  

  #===== Local directories
  setenv CHEWIEDIR		   `pwd`
  setenv CHEWIEDATADIR		   $CHEWIEDIR/../MonicelliOutput
  setenv CHEWIEINPUTDIR            $CHEWIEDIR/../ChewieInput
  setenv CHEWIEOUTPUTDIR           $CHEWIEDIR/../ChewieOutput
  setenv CHEWIEXMLDIR              $CHEWIEDIR/xml
  
  #===== Location of the ROOT components
  setenv ROOTSYS		  /opt/local/root

  #===== Location of the Qt components
  setenv QTDIR  		  /opt/local/qt
  setenv QTCREATORDIR		  /opt/local/qtcreator
  setenv QTROOTSYSDIR		  `pwd`/qtrootsys
  setenv QTROOT_INCLUDE_PATH	  $ROOTSYS/include

  #===== Location of the BOOST components
  setenv BOOSTINC		  /usr/include
  setenv BOOSTLIB		  /usr/lib64

  #===== Location of the XERCES components
  setenv XERCESCINC		  /opt/local/xerces/include
  setenv XERCESCLIB		  /opt/local/xerces/lib
  
  setenv LD_LIBRARY_PATH          /usr/lib64
  
else if(${HOSTNAME} == "hercules.mib.infn.it" ) then

  #===== Local directories
  setenv MonicelliDir              ../Monicelli
  setenv Monicelli_CalSample_Dir   ../calibrations

  setenv CHEWIEDIR		   `pwd`
  setenv CHEWIEDATADIR		   $CHEWIEDIR/../MonicelliOutput
  setenv CHEWIEINPUTDIR            $CHEWIEDIR/../ChewieInput
  setenv CHEWIEOUTPUTDIR           $CHEWIEDIR/../ChewieOutput
  setenv CHEWIEXMLDIR              $CHEWIEDIR/xml

  #===== Location of the ROOT components
  setenv ROOTSYS		   /gwterax3/local/root_v5.27.06-qtroot

  #===== Location of the Qt components
  setenv QTDIR  		   /usr/local/Trolltech/Qt-4.7.0/
  setenv QTCREATORDIR		   /usr/local/qtcreator
  setenv QTROOTSYSDIR		   `pwd`/qtrootsys
  setenv QTROOT_INCLUDE_PATH	   $ROOTSYS/include

  #===== Location of the BOOST components
  setenv BOOSTINC		   /usr/include/boost/
  setenv BOOSTLIB		   /usr/lib64/

  #===== Location of the XERCES components
  setenv XERCESCINC		   /usr/include/xercesc/ 
  setenv XERCESCLIB		   /usr/lib64/

  source                           /gwpool/initcms/cvs-hercules.csh

endif

#===== Final PATH definitions
setenv PATH		${ROOTSYS}/bin:$QTDIR/bin:$QTCREATORDIR/bin:$PATH
setenv LD_LIBRARY_PATH  ${ROOTLIB}:${BOOSTLIB}:$QTDIR/lib:$QTCREATORDIR/lib:$XERCESCLIB/:$CHEWIEDIR/../Monicelli/plugins/libs/:$LD_LIBRARY_PATH

#----------------------------------------------------------------------------------------------------
