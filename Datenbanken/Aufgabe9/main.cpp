#include "Database.hpp"


int main() {
	Database db;

	db.drop();
	db.insert(Buch("J.K.Rowling", "Harry Potter and the Philosopher's stone", "Bloomsbury", 2014, "UK", "978-1-4088-5589"));
	db.insert(Buch("J.K.Rowling", "Harry Potter and the Chamber of Secrets", "Bloomsbury", 2014, "UK", "978-1-4088-5566"));
	db.insert(Buch("J.K.Rowling", "Harry Potter and the Prisoner of Askaban", "Bloomsbury", 2014, "UK", "978-1-4088-5567"));
	db.insert(Buch("J.K.Rowling", "Harry Potter and the Goblet of Fire", "Bloomsbury", 2014, "UK", "978-1-4088-5568"));
	db.insert(Buch("J.K.Rowling", "Harry Potter and the Order of the Phoenix", "Bloomsbury", 2014, "UK", "978-1-4088-5569"));
	db.insert(Buch("J.K.Rowling", "Harry Potter and the Half Blood Prince", "Bloomsbury", 2014, "UK", "978-1-4088-5570"));
	db.insert(Buch("J.K.Rowling", "Harry Potter and the Deathly Hallows", "Bloomsbury", 2014, "UK", "978-1-4088-5571"));

	db.createIndex();


	// for(const Buch& b : buchDB)
	// 	std::cout << b << "\n";

	// for(const Index& i : indexDB)
	// 	std::cout << i.Position << ": " << std::string_view(i.Ordnungsbegriff, strnlen(i.Ordnungsbegriff, 41)) << "\n";

	// const auto testSearch = [&](const char *const name) {
	// 	std::cout << "Searching \"" << name << "\":\n";
	// 	std::cout << " >> " << db.findOne(name) << "\n";
	// };

	// testSearch("Harry Potter and the Philosopher's stone");
	// testSearch("Harry Potter and the Chamber of Secrets");
	// testSearch("Harry Potter and the Prisoner of Askaban");
	// testSearch("Harry Potter and the Goblet of Fire");
	// testSearch("Harry Potter and the Order of the Phoenix");
	// testSearch("Harry Potter and the Half Blood Prince");
	// testSearch("Harry Potter and the Deathly Hallows");

	db.printAllDesc();
	
    return 0;
}