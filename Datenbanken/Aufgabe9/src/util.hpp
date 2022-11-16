#pragma once

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