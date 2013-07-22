#include <QApplication>
#include <stdlib.h>
#include <unistd.h>
#include <map>
#include <vector>
#include <iostream>
#include "dotterWindow.h"

using namespace std;

int main(int argc, char** argv){
  QApplication app(argc, argv);
  // First obtain any options that might have been called..
  int c;
  map<string, string> opt_commands;
  vector<char*> non_options;
  while( (c = getopt(argc, argv, "-w:d:h")) != -1){
    switch(c){
    case 'w' :
      opt_commands["windowSize"] = optarg;
      break;
    case 'd' :
      opt_commands["die"] = optarg;
      break;
    case 'h' :
      opt_commands["help"] = 1;
      break;
    case 1 :
      non_options.push_back(optarg);
      break;
    case '?' :
      cerr << "Unknown option : " << optopt << endl;
      break;
    default :
      abort();
    }
  }
  
  if(opt_commands.count("help")){
    cout << "megaDotter [-w window_size] [-d file_name] [-h]\n\n"
	 << "-w : sets the window size used for the comparison\n"
	 << "-d : specifies the filename and a non-interactive mode where\n"
	 << "     the program will exit after creating the specified plot\n\n"
	 << "megaDotter: a dotplot for megabase regions for visualising repetitive\n"
	 << "            DNA sequences. megaDotter uses an STL map as an index of\n"
	 << "            sequences of specified length, allowing reasonably large\n"
	 << "            regions of DNA to be visualised.\n\n"
	 << "            The maximum size depends on the amount of memory available\n"
	 << "            but around 4 Mbases are doable on fairly standard hardware" << endl;
    exit(0);
  } 
  DotterWindow dotterWindow(opt_commands);
  //   app.setMainWidget(&dotterWindow);
  dotterWindow.show();
  return app.exec();
}
