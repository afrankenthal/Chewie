#----------------------------------------------------------------------------------------------------
# Author: J. Ngadiuba, L. Uplegger

# The following script sets up the environment to run qt and Monicelli on local machines.
# Once you have fetched this copy from the cvs repository, make a copy (name it something
# like mySetupQt.csh) and modify the copy to suit your needs. Please do NOT put bac in cvs  
# a modified copy of the original setupQt.csh if you have by mistake accidentally edited it.
#
# Before running Monicelli, source YOUR own, PRIVATE **copy** of this file:
setenv BASEDATADIR /home/data/TestBeam/2014_04_April
#===============   rulinux01.dhcp.fnal.gov   ==============================================
if(${HOSTNAME} == "rulinux03.dhcp.fnal.gov") then

  setenv MonicelliDir              ../Monicelli

  #===== Local directories
  setenv CHEWIEDIR		   `pwd`
  setenv CHEWIEDATADIR		   $CHEWIEDIR/../MonicelliStripOutput
  setenv CHEWIEINPUTDIR            $CHEWIEDIR/../ChewieInput
  setenv CHEWIEOUTPUTDIR           $CHEWIEDIR/../ChewieOutput
  setenv CHEWIEXMLDIR              $CHEWIEDIR/xml
  
  #===== Location of the ROOT components
  setenv ROOTSYS		   /opt/local/root

  #===== Location of the Qt components
  setenv QTDIR  		   /opt/local/qt
  setenv QTCREATORDIR		   /opt/local/qtcreator

  #===== Location of the BOOST components
  setenv BOOSTINC		   /usr/include
  setenv BOOSTLIB		   /usr/lib64

  #===== Location of the XERCES components
  setenv XERCESCINC		   /opt/local/xerces/include
  setenv XERCESCLIB		   /opt/local/xerces/lib
  
  setenv LD_LIBRARY_PATH           /usr/lib64
  
endif

#===============   rulinux02.dhcp.fnal.gov   ==============================================
if(${HOSTNAME} == "rulinux02.dhcp.fnal.gov") then

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
  
endif

#===============   hercules.mib.infn.it   ==============================================
if(${HOSTNAME} == "hercules.mib.infn.it" ) then

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
setenv PATH			$ROOTSYS/bin:$QTDIR/bin:$QTCREATORDIR/bin:$PATH
setenv LD_LIBRARY_PATH          $ROOTSYS/lib:$QTDIR/lib:$QTCREATORDIR/lib:$XERCESCLIB/:$CHEWIEDIR/../Monicelli/plugins/libs/:$LD_LIBRARY_PATH

#----------------------------------------------------------------------------------------------------
