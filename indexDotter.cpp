#include "indexDotter.h"
#include <iostream>
#include <string.h>

using namespace std;

positionSet::positionSet(unsigned int dimNo){
    positions.resize(dimNo);
}

positionSet::positionSet(){}


void positionSet::addPosition(unsigned int dim, int pos){
    if(dim >= positions.size()){
	positions.resize(dim + 1);
    }
    positions[dim].push_back(pos);
}

IndexDotter::IndexDotter(){
    windowSize = 10;  // default.. 
    dimensionNo = 0;
}

IndexDotter::~IndexDotter(){
}

    


vector<coord_set> IndexDotter::selfPoints(){
    vector<coord_set> points;
    cout << "Size of wordPositions " << wordPositions.size() << endl;
    long counter = 0;
    if(dimensionNo){
	map<string, positionSet>::iterator it;
	vector<int>::iterator vit1;
	vector<int>::iterator vit2;
	for(it = wordPositions.begin(); it != wordPositions.end(); it++){
//	    cout << "Size of points now : " << points.size() << "  and number of positions here : " << it->second.positions[0].size() << endl;
	    points.push_back(coord_set());
	    points.back().a.reserve(it->second.positions[0].size());
	    points.back().b.reserve(it->second.positions[0].size());
	    counter += (it->second.positions[0].size() * it->second.positions[0].size());
	    for(vit1 = it->second.positions[0].begin(); vit1 != it->second.positions[0].end(); vit1++){
		points.back().a.push_back(*vit1);
		points.back().b.push_back(*vit1);
	    }
//	    for(vit2 = it->second.positions[0].begin(); vit2 != it->second.positions[0].end(); vit2++){
//		points.back().b.push_back(*vit2);
//	    }
//		    //points.push_back(point(*vit1, *vit2));
//		    if(*vit1 == *vit2)
//			counter++;
//		}
//	    }
	}
    }
    cout << "Self Points should result in a total of : " << counter << "  drawing positions" << endl;
    return(points);
}

vector<coord_set> IndexDotter::secondPoints(){
    vector<coord_set> points;
    if(dimensionNo > 1){
	map<string, positionSet>::iterator it;
	vector<int>::iterator vit1;
	vector<int>::iterator vit2;
	for(it = wordPositions.begin(); it != wordPositions.end(); it++){
	    points.push_back(coord_set());
	    points.back().a.reserve(it->second.positions[0].size());
	    for(vit1 = it->second.positions[0].begin(); vit1 != it->second.positions[0].end(); vit1++){
		points.back().a.push_back(*vit1);
	    }
	    points.back().b.reserve(it->second.positions[1].size());
	    for(vit2 = it->second.positions[1].begin(); vit2 != it->second.positions[1].end(); vit2++){
		points.back().b.push_back(*vit2);
	    }
	}
    }
    return(points);
}

void IndexDotter::selfCompare(string s, unsigned char ws){
    windowSize = ws;
    seq1 = s;
    prepareSequence(seq1);
    rseq = revCompSequence(seq1);
    
    wordPositions.clear();
    dimensionNo = 2;   // this to save time and space..
    makeIndex(wordPositions, 0, dimensionNo, seq1, false);  // in a selfcompare, there is no point to do anything more..
    makeIndex(wordPositions, 1, dimensionNo, rseq, true);
// and that is to some extent all we have to do hmm. 
}

void IndexDotter::makeIndex(map<string, positionSet>& pos, unsigned int dim, unsigned int dimNo, string& seq, bool rev){
    map<string, positionSet>::iterator it;
    // ignore long strings of N as these can cause a great deal of problems.
    char* nseq = new char[windowSize + 1];
    memset((void*)nseq, 'N', windowSize);
    nseq[windowSize] = 0;
    int p;
    unsigned int l = seq.size() - windowSize - 1;
    for(uint i=0; i < seq.size() - windowSize; ++i){
	string subSeq = seq.substr(i, windowSize);
	if(subSeq == nseq)
	    continue;
	it = pos.find(subSeq);
	if(it == pos.end()){
	    pos.insert(make_pair(subSeq, positionSet(dimNo)));
	    it = pos.find(subSeq);
	}
	// which I suppose could fail ?
	p = rev ? l - i : i;
	it->second.addPosition(dim, p);
    }
    delete nseq;
}

void IndexDotter::prepareSequence(string& s){
    for(uint i=0; i < s.length(); ++i)
	s[i] = toupper(s[i]);
}

string IndexDotter::revCompSequence(string& s){
    map<char, char> comp;
    comp['A'] = 'T';    
    comp['C'] = 'G';
    comp['G'] = 'C';
    comp['T'] = 'A';
    string ts = s;
    for(uint i=0; i < s.length(); ++i){
	if(comp.count(s[i])){
	    ts[s.length() - (i+1)] = comp[s[i]];
	}else{
	    ts[s.length() - (i+1)] = s[i];
	}
    }
    return(ts);
}
