#!/usr/bin/env python
import os
import string
import subprocess

def doMakefile():
  makefile = open("../Makefile","r")
  makefileContent = makefile.readlines()
  makefile.close()
  i=0
  outFile = open("../MakefileExpress","w")
  for line in makefileContent:
    if (line.find("main.") != -1):
      makefileContent[i] = line.replace("main.","Express/ChewieExpress.");
      makefileContent[i] = makefileContent[i].replace("objFiles/Express/","objFiles/");
      makefileContent[i] = makefileContent[i].replace("src/Express/","Express/");
    if (line.find("QMAKE_TARGET") != -1 or line.find("TARGET") != -1):
      makefileContent[i] = line.replace("Chewie","Express/ChewieExpress");
    outFile.write(makefileContent[i])
    i = i+1
  outFile.close()


def main():
  doMakefile();
#  cmd = "rm -f ChewieExpress.o ChewieExpress; cd ..; qmake; make -f MakefileExpress; cd Express; rm ../MakefileExpress"; 
  cmd = "rm -f ChewieExpress.o ChewieExpress; cd ..; qmake; make -f MakefileExpress; cd Express; "; 
  proc = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE);
  out,err = proc.communicate();
  print out
  if(err != ''):
    print err

if __name__ == '__main__':
    main()
