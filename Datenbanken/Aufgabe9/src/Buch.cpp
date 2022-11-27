#include "Buch.hpp"

#include "util.hpp"

#include <type_traits>
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

#define COL_DESC(COLUMN) \
	{ \
		toLowerCase(#COLUMN), \
		Column { \
			#COLUMN, \
			std::is_same_v<int, decltype(COLUMN)> \
				? Column::INTEGER \
				: Column::STRING, \
			offsetof(Buch, COLUMN), \
			sizeof(COLUMN) \
		} \
	}

const std::unordered_map<std::string, Buch::Column> Buch::columns {
	COL_DESC(Autor), // char Autor[21];

	COL_DESC(Titel), // char Titel[41];
	COL_DESC(Verlagsname), // char Verlagsname[21];
	COL_DESC(Erscheinungsjahr), // int Erscheinungsjahr;
	COL_DESC(Erscheinungsort), // char Erscheinungsort[41];
	COL_DESC(ISBN) // char ISBN[15];
};

// const std::vector<Buch::Column> Buch::columns {
// 	COL_DESC(Autor), // char Autor[21];

// 	COL_DESC(Titel), // char Titel[41];
// 	COL_DESC(Verlagsname), // char Verlagsname[21];
// 	COL_DESC(Erscheinungsjahr), // int Erscheinungsjahr;
// 	COL_DESC(Erscheinungsort), // char Erscheinungsort[41];
// 	COL_DESC(ISBN) // char ISBN[15];
// };

#undef STR_COL