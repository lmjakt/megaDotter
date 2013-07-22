#ifndef DOTTER_H
#define DOTTER_H

#include <string>

typedef unsigned int uint;

// not sure why I bother to make a class rather than a function. But might have its use somewhere.

// ONLY create objects of this class using new, -- and then delete them. You CANNOT use copy or assignment
// operations as I have not written the appropriate function.. 

class Dotter {
    public :
	Dotter();
    ~Dotter();
    void compareSeqs(std::string s1, std::string s2, unsigned char ws, unsigned int step);  // void cos I don't know what to return yet.
    unsigned char* f_area();
    unsigned char* r_area();
    unsigned int compWidth();
    unsigned int compHeight();
    unsigned int step();

    private :
	unsigned int stepSize;
    std::string seq1, seq2, rseq2;
    unsigned char* forward;
    unsigned char* reverse;  // actually reverse complement..
    unsigned int s1l, s2l;   // the length / step size
    
    void prepareSequence(std::string& s);  // make it upper case..
    void revCompSequence(std::string& s);  // reverse complement the sequence.. 
};

#endif
