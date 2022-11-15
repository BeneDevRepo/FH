#pragma once

#include <iostream>


struct Buch {
	char Autor[21];
	char Titel[41];
	char Verlagsname[21];
	int Erscheinungsjahr;
	char Erscheinungsort[41];
	char ISBN[15];

	Buch();
	Buch(
		const char *const Autor,
		const char *const Titel,
		const char *const Verlagsname,
		const int Erscheinungsjahr,
		const char *const Erscheinungsort,
		const char *const ISBN);
};

std::ostream& operator<<(std::ostream& cout, const Buch& b);