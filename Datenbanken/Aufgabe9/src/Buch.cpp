#include "Buch.hpp"

#include <string_view>
#include <cstring>


Buch::Buch():
	Autor{},
	Titel{},
	Verlagsname{},
	Erscheinungsjahr(0),
	Erscheinungsort{},
	ISBN{} { }


Buch::Buch(
		const char *const Autor,
		const char *const Titel,
		const char *const Verlagsname,
		const int Erscheinungsjahr,
		const char *const Erscheinungsort,
		const char *const ISBN
		) {
	strncpy(this->Autor, Autor, 21);
	strncpy(this->Titel, Titel, 41);
	strncpy(this->Verlagsname, Verlagsname, 21);
	this->Erscheinungsjahr = Erscheinungsjahr;
	strncpy(this->Erscheinungsort, Erscheinungsort, 41);
	strncpy(this->ISBN, ISBN, 15);
}

std::ostream& operator<<(std::ostream& cout, const Buch& b) {
	// lambda um char[] in string_view umzuwandeln:
	constexpr auto strv =
		[](const char *const str, const size_t arrSize) -> std::string_view {
			return std::string_view(str, strnlen(str, arrSize));
		};

	cout << "Author: " << strv(b.Autor, 21)
		<< ", Titel: " << strv(b.Titel, 41)
		<< ", Verlagsname: " << strv(b.Verlagsname, 21)
		<< ", Erscheinungsjahr: " << b.Erscheinungsjahr
		<< ", Erscheinungsort: " << strv(b.Erscheinungsort, 41)
		<< ", ISBN: " << strv(b.ISBN, 15);
	return cout;
}