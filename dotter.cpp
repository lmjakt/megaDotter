#include "dotter.h"
#include <iostream>
#include <map>

using namespace std;

Dotter::Dotter(){
    forward = reverse = 0;
}


Dotter::~Dotter(){
    delete forward;
    delete reverse;
}

unsigned char* Dotter::f_area(){
    return(forward);
}

unsigned char* Dotter::r_area(){
    return(reverse);
}

unsigned int Dotter::compWidth(){
    return(s1l);
}

unsigned int Dotter::compHeight(){
    return(s2l);
}

void Dotter::compareSeqs(string s1, string s2, unsigned char ws, unsigned int step){
    // prepare the to sequneces..
    seq1 = s1;
    seq2 = s2;
    prepareSequence(seq1);
    prepareSequence(seq2);
    rseq2 = seq2;
    revCompSequence(rseq2);

    //cout << "prepared sequences : \n" << seq2 << "\n\n" << rseq2 << endl;

    // The maximal count of a char is 255, so the ws divided by the 
    // step size must be less than 256;
    stepSize = step;
    // but adjust step size so that we don't run out of memory.

    cout << "window size is " << (int)ws << "  and step size is " << (int)step << endl;

    // fc is forward counts, rc is reverse counts.
    // we should do a sanity check for this, as I'm apt to want to use sequences that are much longer
    s1l = seq1.length() / stepSize;
    s2l = seq2.length() / stepSize;
    unsigned int ms =  s1l * s2l;
    cout << "ms is " << ms << "  s1l " << s1l << "  and s2l " << s2l << endl;
    forward = new unsigned char[ms];
    reverse = new unsigned char[ms];

    cout << "made the thingy.. " << endl;
    // and then simply..
    for(uint i=0; i < (seq2.length() - ws); i += stepSize){
//	cout << "new row : " << i << endl;
	for(uint j=0; j < (seq1.length() - ws); j += stepSize){
//	    cout << "  " << j;
	    unsigned char fc = 0;
	    unsigned char rc = 0;
	    for(uint k=0; k < ws; ++k){
		if(seq1[j + k] == seq2[i + k])
		    ++fc;
		if(seq1[j + k] == rseq2[i + k])
		    ++rc;
	    }
	    forward[(i/stepSize) * s1l + j / stepSize] = fc;
	    reverse[(i/stepSize) * s1l + j / stepSize] = rc;
//	    cout << "  " << (int)fc;
	}
    }
    cout << endl;
}

void Dotter::prepareSequence(string& s){
    for(uint i=0; i < s.length(); ++i)
	s[i] = toupper(s[i]);
}

void Dotter::revCompSequence(string& s){
    map<char, char> comp;
    comp['A'] = 'T';    
    comp['C'] = 'G';
    comp['G'] = 'C';
    comp['T'] = 'A';
    string ts = s;
    for(uint i=0; i < ts.length(); ++i){
	if(comp.count(ts[i]))
	    s[s.length() - (i+1)] = comp[ts[i]];
    }
}
