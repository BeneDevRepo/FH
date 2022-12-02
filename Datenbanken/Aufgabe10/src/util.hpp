#pragma once

#include <algorithm>
#include <iostream>
#include <string>


inline std::string readLine() {
    std::string line;

    for(;;) {
    	const int c = getchar();

        if(c == EOF)
            return line;

		if(c == '\n')
			return line;

        line += (char)c;
    }
}

inline std::string toLowerCase(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), ::tolower);
	return s;
}