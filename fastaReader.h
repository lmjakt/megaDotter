#ifndef FASTAREADER_H
#define FASTAREADER_H

#include <string>
#include <vector>
#include "sequence.h"

class FastaReader {
    public :
	FastaReader(const char* fname);
    std::vector<Sequence> sequences();
    std::string fName();
    
    private :
	std::vector<Sequence> seqs;
    std::string fileName;
    Sequence parseHeader(std::string& line);
    void parseLine(std::string& line, Sequence& seq); // removes whitespace by using a istringstream
};

#endif
