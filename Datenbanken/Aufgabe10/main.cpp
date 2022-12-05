#include "Database.hpp"
#include "DbCLI.hpp"


int main() {
	DbCLI cli;

	cli.run();
	
    return 0;
}

/*

insert into Buch values('Franz Ferdinand', 'Harry Potter 1', 'Cornelsen', 1984, 'Berlin', '987-3-00-065785');
insert into Buch values('Anna liebert', 'Monster', 'Bandai', 2002, 'Muenchen', '657-6-13-678098');
insert into Buch values('Beate Becker', 'Harry Potter 2', 'OReily', 1967, 'Frankfurt', '987-0-13-7569789');
insert into Buch values('Carsten c.', 'Sozialwissenschaften II', 'Cornelsen', 1813, 'Amsterdam', '907-2-76-879546');
insert into Buch values('Dagobert Duck', 'Ducktales', 'OReilly', 2026, 'Entenhausen', '678-0-50-789754');
insert into Buch values('Faust Autor', 'Faust', 'Cornelsen', 2026, 'Entenhausen', '678-0-50-789754');

select * from buch;

create index a on Buch(Erscheinungsjahr);

create index a on Buch(Autor);
create index a on Buch(Titel);
create index a on Buch(Verlagsname);
create index a on Buch(Erscheinungsort);

find Verlagsname = 'Cornelsen';

select * from Buch where Verlagsname = 'Cornelsen';

find erscheinungsjahr = 1984;



*/