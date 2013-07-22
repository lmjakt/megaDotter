#include "fastaReader.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

FastaReader::FastaReader(const char* fname){
    fileName = fname;
    ifstream in(fname);
    if(!in){
	cerr << "Unable to open sequence file : " << fname << endl;
	return;
    }
    string line;
    Sequence seq;
    seqs.resize(0);
    while(getline(in, line)){
	if(!line.length())
	    continue;
	if(line[0] == '>'){
	    if(seq.name.length())
		seqs.push_back(seq);
	    seq = parseHeader(line);
	    continue;
	}
	parseLine(line, seq);

    }
    if(seq.name.length())
	seqs.push_back(seq);
    cout << "And now have " << seqs.size() << " sequences and stuff.." << endl;
}

vector<Sequence> FastaReader::sequences(){
    return(seqs);
}

Sequence FastaReader::parseHeader(string& line){
    line = line.substr(1);
    istringstream is(line);
    Sequence seq;
    string word;
    if(!(is >> word)){
	cerr << "Unable to get identifier from " << line << endl;
	return(seq);
    }
    seq.name = word;
    // This is ugly. But I'm using it since I don't know of any way of
    // representing whitespace in a find function..
    while(is >> word){
	seq.description += word;
	seq.description += " ";
    }
    if(seq.description.length())
	seq.description = seq.description.substr(0, seq.description.length()-1);
    return(seq);
}

// This next function is only here to get rid of white space in sequences.
// I don't care about anything else.. 
void FastaReader::parseLine(string& line, Sequence& seq){
    istringstream is(line);
    string word;
    while(is >> word)
	seq.sequence += word;
}
    
	
    
    
