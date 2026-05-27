#include "Main.h"

void Test()
{
	char c;
	Data data;
	for (int i = 0; i < 7; i++)
	{
		data.InsertEntry(GetTestItem(i));
	}
	initializer_list il{ GetTestItem(7), GetTestItem(8), GetTestItem(9) };
	data.InsertEntries(il);

	cout << "KÕIK KIRJED" << endl << endl;
	data.PrintAll();
	cin.get(c);

	cout << "KÕIK KIRJED LIIGIST ÕÕNETUVI" << endl << endl;
	try
	{
		data.PrintEntries("õõnetuvi");
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}
	cin.get(c);

	cout << "KIRJETE ARV LIIGIST ÕÕNETUVI" << endl << endl;
	cout << data.CountEntries("õõnetuvi") << endl;
	cin.get(c);

	cout << "LIST KÕIKIDEST KIRJETEST LIIGIST ÕÕNETUVI" << endl << endl;
	auto pl = data.GetEntries("õõnetuvi");;
	if (pl)
	{
		ranges::for_each(*pl, [](const Entry& e) { cout << e << endl; });
	}
	else
	{
		cout << "Not found" << endl;
	}
	cin.get(c);

	cout << "KOKKUVÕTLIK KIRJE LIIGIST ÕÕNETUVI" << endl << endl;
	auto pe1 = data.GetSummaryEntry("õõnetuvi");;
	if (pe1)
	{
		cout << *pe1 << endl;
	}
	else
	{
		cout << "Not found" << endl;
	}
	cin.get(c);

	cout << "KIRJETE ARV SUGUKONNAST TUVILASED" << endl;
	cout << data.CountFamilyEntries("tuvilased") << endl;
	cin.get(c);

	cout << "KÕIK KIRJED SUGUKONNAST TUVILASED" << endl << endl;
	try
	{
		data.PrintFamilyEntries("tuvilased");
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}
	cin.get(c);

	cout << "KÕIK KIRJED SELTSIST KURELISED" << endl << endl;
	try
	{
		data.PrintOrderEntries("kurelised");
	}
	catch (exception& e)
	{
		cout << e.what() << endl;
	}
	cin.get(c);

	cout << "KIRJETE ARV SELTSIST KURELISED" << endl << endl;
	cout << data.CountOrderEntries("kurelised") << endl;
	cin.get(c);

	cout << "KÕIK KIRJED PÄRAST LIIGI ÕÕNETUVI EEMALDAMIST" << endl << endl;
	if (data.RemoveEntries("õõnetuvi"))
	{
		data.PrintAll();
	}
	else
	{
		cout << "Not found" << endl;
	}
	cin.get(c);

	cout << "KÕIK KIRJED PÄRAST SUGUKONNA RUIKLASED EEMALDAMIST" << endl << endl;
	if (data.RemoveFamilyEntries("ruiklased"))
	{
		data.PrintAll();
	}
	else
	{
		cout << "Not found" << endl;
	}
	cin.get(c);

	cout << "KÕIK KIRJED PÄRAST SELTSI TUVILISED EEMALDAMIST" << endl << endl;
	if (data.RemoveOrderEntries("tuvilised"))
	{
		data.PrintAll();
	}
	else
	{
		cout << "Not found" << endl;
	}
	cin.get(c);

	cout << "KÕIK KIRJED PÄRAST UUTE TESTKIRJETE SISSELUGEMIST" << endl << endl;
	for (int i = 10; i < 17; i++)
	{
		data.InsertEntry(GetTestItem(i));
	}
	data.PrintAll();
	cin.get(c);

	cout << "KOKKUVÕTLIK KIRJE LIIGIST MUSTTIHANE" << endl << endl;
	auto pe2 = data.GetSummaryEntry("Musttihane");;
	if (pe2)
	{
		cout << *pe2 << endl;
	}
	else
	{
		cout << "Not found" << endl;
	}
	cin.get(c);

	cout << "KÕIK KIRJED PÄRAST SELTSI KURVITSAKISED JA SUGUKONNA TIHASLASED EEMALDAMIST" << endl << endl;
	data.RemoveOrderEntries("kurvitsalised");
	data.RemoveFamilyEntries("tihaslased");
	data.PrintAll();
	cin.get(c);

	cout << "KÕIK KIRJED PÄRAST 10 JUHUSLIKU KIRJE SISSELUGEMIST" << endl << endl;
	for (int i = 0; i < 10; i++)
	{
		data.InsertEntry(GetItem());
	}
	data.PrintAll();
	return;
}