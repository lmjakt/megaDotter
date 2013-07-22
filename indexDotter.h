#ifndef INDEXDOTTER_H
#define INDEXDOTTER_H

#include <map>
#include <string>
#include <vector>
#include <set>

typedef unsigned int uint;

struct positionSet {
    positionSet(unsigned int dimNo);
    positionSet();
    
    void addPosition(unsigned int dim, int pos);

    std::vector<std::vector<int> > positions;
};

struct point {
    point(){
	x = y = 0;
    }
    point(int xp, int yp){
	x = xp; y = yp;
    }
    int x, y;
};

struct coord_set {
    std::vector<int> a;
    std::vector<int> b;
    coord_set(){}
};

class IndexDotter {
    public :
	IndexDotter();
    ~IndexDotter();
    void selfCompare(std::string, unsigned char ws);

    std::vector<coord_set> selfPoints();
    std::vector<coord_set> secondPoints();  // because the full set of points can be too large for the memory
    
    private :
	
	void prepareSequence(std::string& s);
    std::string revCompSequence(std::string& s);
    void makeIndex(std::map<std::string, positionSet>& pos, unsigned int dim, unsigned int dimNo, std::string& seq, bool rev=false);

    unsigned char windowSize;
    std::map<std::string, positionSet> wordPositions;
    unsigned int dimensionNo;  // the number of dimensions.. 
    std::string seq1, seq2, rseq;
};

#endif
