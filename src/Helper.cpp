#include "Helper.hpp"


void NormalizeString(std::string* string) {
	while(string->length()){
			char c = string->at(0);
			if(!(c == ' ' || c == '\t')){ break; }
			string->erase(string->begin());
		}
	for(int i = string->length() - 1; i > 0; i--){
		char c = string->at(i);
		if(!(c == ' ' || c == '\t')){ break; }
		string->pop_back();
	}
}
