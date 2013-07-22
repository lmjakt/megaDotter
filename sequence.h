#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <string>

struct Sequence {
    Sequence(std::string n, std::string d, std::string s){
	name = n;
	description = d;
	sequence = d;
    }
    Sequence(){}
    std::string name;
    std::string description;
    std::string sequence;
};

#endif
