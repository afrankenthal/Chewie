#include "MessageTools.h"
#include "ANSIColors.h"
#include "XmlParser.h"
#include "XmlConverter.h"
#include "EventManager.h"
#include "AnalysisManager.h"

#include <TApplication.h>
#include <TROOT.h>
#include <TSystem.h>

#include <QDomDocument>
#include <QFile>
#include <QString>
#include <QDomNode>

#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

std::string filesPath;

//==============================================================
class XmlDefaults;
class XmlFiles;
class XmlFile;

//==============================================================
class ExpressXmlParser
{
public:
  ExpressXmlParser (void);
  ~ExpressXmlParser(void);
  
  void parseDocument(std::string fileName);
  
  XmlDefaults*           getDefaults (void) {return theDefaults_ ;}
  std::vector<XmlFiles*> getFilesList(void) {return theFilesList_;}
  
private:
  QDomDocument* document_;
  QDomNode      rootNode_;
  
  XmlDefaults*           theDefaults_;
  std::vector<XmlFiles*> theFilesList_;
  stringstream ss_;
};

//==============================================================
class XmlDefaults
{
public:
  XmlDefaults (QDomNode& node);
  ~XmlDefaults(void) {;}
  
  QDomNode&   getNode(void) {return thisNode_;}
  
  std::string filesPath_;
  bool        convert_;
  bool        runAnalysis_;
  int         numberOfEvents_;     
  
private:
  QDomNode    thisNode_;
};

//==============================================================
class XmlFiles
{
public:
  XmlFiles (QDomNode& node);
  ~XmlFiles(void) {;}
  
  QDomNode& getNode(void) {return thisNode_;}
  
  vector<XmlFile*> fileNames_;
  std::string configurationName_;
  std::string outFileName_;
  
private:
  QDomNode thisNode_;
};

//==============================================================
class XmlFile
{
public:
  XmlFile (QDomNode& node);
  ~XmlFile(void) {;}
  
  QDomNode& getNode(void) {return thisNode_;}
  
  std::string fileName_;
  
private:
  QDomNode thisNode_;
};

//===================================================================================
int main (int argc, char** argv)
{
  stringstream ss;

  gROOT->SetBatch(true);

  std::string color = std::string(ACYellow)+std::string(ACBold)+std::string(ACReverse) ;
  STDLINE("",color);
  STDLINE("+--------------------------------------------------+",color);
  STDLINE("|                                                  |",color);
  STDLINE("|                W e l c o m e  t o                |",color);
  STDLINE("|           C h e w i e   E x p r e s s            |",color);
  STDLINE("|                                                  |",color);
  STDLINE("|      The MTEST pixel-telescope tracks            |",color);
  STDLINE("|              data analysis code                  |",color);
  STDLINE("|    at Fermi National Accelerator Laboratory      |",color);
  STDLINE("|                                                  |",color);
  STDLINE("|        D. Menasce, L. Moroni, S. Terzo           |",color);
  STDLINE("|      J. Ngadiuba, L. Uplegger, L. Vigani         |",color);
  STDLINE("|                                                  |",color);
  STDLINE("+--------------------------------------------------+",color);
  STDLINE("",color);

  ExpressXmlParser theExpressXmlParser;
  
  std::string configFileName = "ExpressConfiguration.xml";

  if (argc == 2) 
  {
    configFileName = std::string("./xml/") + argv[1];
    STDLINE(configFileName,ACCyan) ;
  }
  else if (argc > 2)
  {
    ss.str("");
    ss << "Usage: ./ChewieExpress [cfile] (existing configuration file in ./xml)";
    STDLINE(ss.str(),ACRed);
    exit(EXIT_SUCCESS);
  }
  ss.str("");
  ss << "Using : " << configFileName << " configuration.";
  STDLINE(ss.str(),ACGreen);
  
  TApplication tApp("App",&argc,argv);

  theExpressXmlParser.parseDocument(configFileName.c_str());
  
  XmlParser* theChewieXmlParser = new XmlParser();    
  filesPath 	                = theExpressXmlParser.getDefaults()->filesPath_     ;
  bool   convert         	= theExpressXmlParser.getDefaults()->convert_	    ;
  bool   runAnalysis     	= theExpressXmlParser.getDefaults()->runAnalysis_   ;
  int    numberOfEvents  	= theExpressXmlParser.getDefaults()->numberOfEvents_;
  string chewieXmlDir    	= getenv("CHEWIEXMLDIR"   );
  string chewieDataDir   	= getenv("CHEWIEDATADIR"  );
  string chewieOutputDir 	= getenv("CHEWIEOUTPUTDIR");
  string chewieInputDir  	= getenv("CHEWIEINPUTDIR" );
  //if (chewieInputDir[chewieInputDir.size()-1] != '/')
   // chewieInputDir += '/';
  
  for (unsigned int fs=0; fs<theExpressXmlParser.getFilesList().size(); fs++)
    {
      string configurationName = chewieXmlDir    + "/" + theExpressXmlParser.getFilesList()[fs]->configurationName_;
      string outFileName       = chewieOutputDir + "/" + theExpressXmlParser.getFilesList()[fs]->outFileName_;
      theChewieXmlParser->parseDocument(QString(configurationName.c_str()));
      EventManager*    theEventManager    = new EventManager();
      AnalysisManager* theAnalysisManager = new AnalysisManager;
      theEventManager   ->setConfiguration(theChewieXmlParser);
      theAnalysisManager->setConfiguration(theChewieXmlParser);
      
      // Setting the number of events
      if(numberOfEvents != -1)
	{
	  QDomAttr maxEvents = theChewieXmlParser->getDocument()->createAttribute("maxEvents");
	  std::stringstream ss; ss.str("");
	  ss << numberOfEvents;
	  maxEvents.setValue(ss.str().c_str());
	  theChewieXmlParser->getConverter()->getNode().toElement().setAttributeNode(maxEvents);
	  //theEventManager->setEventsLimit();it is done in startConverter
	}
      
      //////////////////////////////////////////
      // Open Monicelli File
      std::vector<std::string> monicelliFileList;
      std::vector<std::string> convertedFileList;
      QStringList convertedFileNames;
      string tmp;
      for (unsigned int f=0;f<theExpressXmlParser.getFilesList()[fs]->fileNames_.size();f++)
	{
	  std::string fileToAnalyze = theExpressXmlParser.getFilesList()[fs]->fileNames_[f]->fileName_;
	  STDLINE(fileToAnalyze,ACCyan);
	  std::string fileName      = filesPath + fileToAnalyze;
	  STDLINE(fileName,ACRed);
	  monicelliFileList.push_back(fileName);
	  tmp = fileName.substr(fileName.find_last_of("/"), fileName.size());
	  tmp = chewieInputDir + tmp;
	  STDLINE(tmp.substr(0,tmp.length()-5) + "_Converted.root", ACWhite);
	  convertedFileList.push_back(tmp.substr(0,tmp.length()-5) + "_Converted.root");
	}
      theEventManager   ->setInFilesList(monicelliFileList);
      theAnalysisManager->setInFilesList(convertedFileList);
      
      if (convert)
	theEventManager->startConverter();
      
      if (runAnalysis)
	{
	  theAnalysisManager->setOutputFileName(outFileName);
	  if(numberOfEvents != -1) theAnalysisManager->setMaxEvents(numberOfEvents);
	  theAnalysisManager->startSequence();
	}
      
      delete theAnalysisManager;
      delete theEventManager;
    }
  delete theChewieXmlParser;
  
  return EXIT_SUCCESS;
}

//================================================================================
ExpressXmlParser::ExpressXmlParser(void) : document_(0)
{
}

//================================================================================
ExpressXmlParser::~ExpressXmlParser()
{
  if (document_) delete document_ ;
}

//================================================================================
void ExpressXmlParser::parseDocument(std::string xmlFileName)
{
  if (document_) delete document_;

  document_ = new QDomDocument( "ConfigurationFile" );
  QFile xmlFile(xmlFileName.c_str());
  if (!xmlFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
      STDLINE(std::string("Could not open ") + xmlFile.fileName().toStdString(),ACRed);
      return;
    }
  
  QString errMsg = "";
  int line;
  int col;
  if (!document_->setContent( &xmlFile, true , &errMsg, &line, &col))
    {
      STDLINE(std::string("Could not access ") + xmlFile.fileName().toStdString(),ACRed);
      ss_ << "Error: " << errMsg.toStdString() << " line: " << line << " col: " << col;
      STDLINE(ss_.str(),ACGreen);
      xmlFile.close();
      return;
    }
  
  STDLINE(std::string("Parsing ") + xmlFile.fileName().toStdString(),ACGreen);
  
  rootNode_ = document_->elementsByTagName("ChewieExpressConfiguration").at(0);
  
  QDomNode defaults = document_->elementsByTagName("Defaults").at(0);
  theDefaults_ = new XmlDefaults(defaults);

  QDomNodeList filesList = document_->elementsByTagName("Files");

  filesPath = getDefaults()->filesPath_;

  for (int fs = 0; fs < filesList.size(); ++fs)
    {
      QDomNode filesNode = filesList.at(fs);
      theFilesList_.push_back(new XmlFiles(filesNode));
    }
  
  xmlFile.close();
}

//================================================================================
XmlDefaults::XmlDefaults(QDomNode& node)
{
  thisNode_ = node;
  filesPath_ = node.toElement().attribute("FilesPath").toStdString();
  convert_ = true;
  
  if (node.toElement().attribute("Convert") == "no" || node.toElement().attribute("Convert") == "No" || node.toElement().attribute("Convert") == "NO")
    convert_ = false;
  runAnalysis_ = true;

  if (node.toElement().attribute("RunAnalysis") == "no" || node.toElement().attribute("RunAnalysis") == "No" || node.toElement().attribute("RunAnalysis") == "NO")
    runAnalysis_ = false;
  numberOfEvents_ = node.toElement().attribute("NumberOfEvents").toInt();
}

//================================================================================
XmlFiles::XmlFiles(QDomNode& node)
{
  thisNode_          = node;
  configurationName_ = node.toElement().attribute("Configuration").toStdString();
  outFileName_       = node.toElement().attribute("OutFileName").toStdString();

  QDomNodeList fileList = node.childNodes();

  Long_t *id    = NULL;
  Long_t *size  = NULL;
  Long_t *flags = NULL;
  Long_t *mt    = NULL;

  STDLINE("List of files to analyze",ACCyan) ;
  for (int f = 0; f < fileList.size(); ++f)
    {
      QDomNode fileNode = fileList.at(f);

      if (!fileNode.isComment())
	{
	  string tmpStr = filesPath + fileNode.toElement().attribute("Name").toStdString();

	  if (gSystem->GetPathInfo(tmpStr.c_str(),id,size,flags,mt) == 0)
	    fileNames_.push_back(new XmlFile(fileNode));
	  else
	    STDLINE(string("WARNING: file ") + tmpStr + string(" not found"),ACRed);
	}
    }
}

//================================================================================
XmlFile::XmlFile(QDomNode& node)
{
  thisNode_ = node;
  fileName_ = node.toElement().attribute("Name").toStdString();
  STDLINE(fileName_,ACGreen);
}
