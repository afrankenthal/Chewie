#----------------------------------------------------------------------------------------------------
# Author: J. Ngadiuba, L. Uplegger

# The following script sets up the environment to run qt and Monicelli on local machines.
# Once you have fetched this copy from the cvs repository, make a copy (name it something
# like mySetupQt.csh) and modify the copy to suit your needs. Please do NOT put bac in cvs  
# a modified copy of the original setupQt.csh if you have by mistake accidentally edited it.
#
# Before running Monicelli, source YOUR own, PRIVATE **copy** of this file:

if [ ${HOSTNAME} = "hal9000.mib.infn.it" ]; then

    if [ ${USER} = "menasce" ]; then
	export BASEDATADIR=../data/2015_10_October
    elif [ ${USER} = "dinardo" ]; then
	export BASEDATADIR=/raid2/data1/user/gr1/e831/dinardo/TestBeamData2015
    fi

    export ROOTVER='FIVE'
    export ROOTSYS=/user/service/root-5.28.00g
    export QTDIR=/usr/local/Trolltech/Qt-4.8.0
    export QTCREATORDIR=/opt/local/qtcreator
    export XERCESDIR=/opt/local/xerces

elif [ ${HOSTNAME} = "rulinux03.dhcp.fnal.gov" ] || [ ${HOSTNAME} = "rulinux04.dhcp.fnal.gov" ] || [${HOSTNAME} = "ftbftracker02.fnal.gov" ]; then
    export BASEDATADIR=/data/TestBeam/2016_05_May
    export ROOTVER='FIVE'
    export ROOTSYS=/opt/local/root
    export QTDIR=/opt/local/qt
    export QTCREATORDIR=/opt/local/qtcreator
    export XERCESDIR=/opt/local/xerces
fi

export MonicelliDir="../Monicelli"
  
#===== Local directories
export CHEWIEDIR=`pwd`
export CHEWIEDATADIR=${BASEDATADIR}/MonicelliOutput
export CHEWIEINPUTDIR=${BASEDATADIR}/ChewieInput
export CHEWIEOUTPUTDIR=${BASEDATADIR}/ChewieOutput
export CHEWIEXMLDIR=${CHEWIEDIR}/xml

#===== Location of the ROOT components
export ROOTINC=${ROOTSYS}/include
export ROOTLIB=${ROOTSYS}/lib

#===== Location of the Qt components
export QTROOTSYSDIR="`pwd`/qtrootsys"
export QTROOT_INCLUDE_PATH="$ROOTSYS/include"

#===== Location of the BOOST components
export BOOSTINC="/usr/include"
export BOOSTLIB="/usr/lib64"

#===== Location of the XERCES components
export XERCESCINC=${XERCESDIR}/include
export XERCESCLIB=${XERCESDIR}/lib

export LD_LIBRARY_PATH="/usr/lib64"

#===== Final PATH definitions
export PATH=${ROOTSYS}/bin:${QTDIR}/bin:${QTCREATORDIR}/bin:${PATH}
export LD_LIBRARY_PATH=${ROOTSYS}/lib:${QTDIR}/lib:${QTCREATORDIR}/lib:${QTROOTSYSDIR}/lib:${XERCESCLIB}:${CHEWIEDIR}/../Monicelli/plugins/libs/:${LD_LIBRARY_PATH}

#----------------------------------------------------------------------------------------------------
