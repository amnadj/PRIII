#include<iostream>
#include<vector>
#include<exception>
#include<sstream>
#include<string>
#include<iterator>
#include<algorithm>
#include<thread>
#include<mutex>


using namespace std;

class Datum {
	int* _dan, * _mjesec, * _godina;
public:
	Datum(int dan = 1, int mjesec = 1, int godina = 2000) {
		_dan = new int(dan);
		_mjesec = new int(mjesec);
		_godina = new int(godina);
	}
	~Datum() {
		delete _dan; _dan = nullptr;
		delete _mjesec; _mjesec = nullptr;
		delete _godina; _godina = nullptr;
	}
	friend ostream& operator<< (ostream& COUT,const Datum& obj) {
		COUT << *obj._dan << " " << *obj._mjesec << " " << *obj._godina;
		return COUT;
	}

	Datum(const Datum& d) {
		_dan = new int(*d._dan);
		_mjesec = new int(*d._mjesec);
		_godina = new int(*d._godina);
	}

	Datum& operator=(const Datum& d) {
		if (this != &d) {
			*_dan = *d._dan;
			*_mjesec = *d._mjesec;
			*_godina = *d._godina;
		}
		return *this;
	}

	bool operator==(const Datum& d) {
		return *_dan == *d._dan && *_mjesec==*d._mjesec && *_godina==*d._godina;
	}
	bool operator > (const Datum& d) {
		int days = *_dan + *_mjesec * 30 + *_godina * 365;
		int days_d = *d._dan + *d._mjesec * 30 + *d._godina * 365;
		return days > days_d;
	}
};

class Izuzetak :public exception {
	Datum _datum;//KORISTITE DANASNJI DATUM
	string _funkcija;//FUNKCIJA U KOJOJ JE NASTAO IZUZETAK
	int _linija;//LINIJA CODE-A U KOJOJ JE NASTAO IZUZETAK
public:
	Izuzetak(string fun, int line, const char* poruka) :exception(poruka), _funkcija(fun), _linija(line) {
		time_t current = time(0);
		tm localcurrent;
		localtime_s(&localcurrent, &current);
		Datum d(localcurrent.tm_mday, localcurrent.tm_mon, localcurrent.tm_year);
		_datum = d;
	}
	friend ostream& operator <<(ostream& output, const Izuzetak& i) {
		//output << "Datum -> " << i._datum << endl;
		output << "Funkcija -> " << i._funkcija << endl;
		output << "Linija -> " << i._linija << endl;
		output << "Poruka -> " << i.what() << endl;
		return output;
	}
};
template<class T1, class T2, int max>
class Kolekcija {
	T1 _elementi1[max];
	T2 _elementi2[max];
	int* _trenutno;

public:
	Kolekcija() {
		_trenutno = new int(0);
	}

	~Kolekcija() {
		delete _trenutno;
		_trenutno = nullptr;
	}
	Kolekcija(const Kolekcija& k) {
		_trenutno =new int( *k._trenutno);
		for (int i = 0; i < *k._trenutno; i++)
		{
			_elementi1[i] = k._elementi1[i];
			_elementi2[i] = k._elementi2[i];
		}
	}

	Kolekcija& operator=(const Kolekcija& k) {
		if (this != &k) {
			*_trenutno = *k._trenutno;
			for (int i = 0; i < *k._trenutno; i++)
			{
				_elementi1[i] = k._elementi1[i];
				_elementi2[i] = k._elementi2[i];
			}
		}
		return *this;
	}

	int size()const { return *_trenutno; }
	int GetMax()const { return max; }
	int GetTrenutno()const { return *_trenutno; }

	T1& operator[](int lokacija) {
		time_t current;
		time(&current);
		if (lokacija < 0 || lokacija >= *_trenutno)
			throw Izuzetak(__FUNCTION__, __LINE__, "Nije moguce");
		return _elementi1[lokacija];
	}

	T1 GetElement1(int lok)const {
		time_t current;
		time(&current);
		if (lok < 0 || lok >= *_trenutno)
			throw Izuzetak(__FUNCTION__, __LINE__, "Nije moguce");
		return _elementi1[lok];
	}

	T2 GetElement2(int lok)const {
		time_t current;
		time(&current);
		if(lok<0 || lok>=*_trenutno)
			throw Izuzetak(__FUNCTION__, __LINE__, "Nije moguce");
		return _elementi2[lok];
	}

	void AddElement(const T1& el1, const T2& el2) {
		time_t current;
		time(&current);

		if (*_trenutno == max)
			throw Izuzetak(__FUNCTION__, __LINE__, "Nije moguce");
		_elementi1[*_trenutno] = el1;
		_elementi2[*_trenutno] = el2;
		(*_trenutno)++;
	}

	friend ostream& operator<< (ostream& output, const Kolekcija& k) {
		for (size_t i = 0; i < k.size(); i++)
			output << k.GetElement1(i) << " " << k.GetElement2(i) << endl;
		return output;
	}

	friend bool operator==(const Kolekcija& k1, const Kolekcija& k2) {
		return k1.GetTrenutno() == k2.GetTrenutno();
	}
};


enum enumKursevi { HtmlCSSJavaScript, SoftwareEngeneeringFundamentals, MasteringSQL, WindowsSecurity };
class Kurs {
	enumKursevi _kurs;
	Datum _pocetak;
	Datum _kraj;
	char* _imePredavaca;
public:
	Kurs() :_imePredavaca(nullptr) {}
	Kurs(enumKursevi k, const char* ime, Datum pocetak, Datum kraj) {
		_kurs = k;
		int vel = strlen(ime) + 1;
		_imePredavaca = new char[vel];
		strcpy_s(_imePredavaca, vel, ime);
		_pocetak = pocetak;
		_kraj = kraj;
	}
	Datum getKraj()const { return _kraj; }
	Datum getPocetak() const{ return _pocetak; }
	enumKursevi getKurs()const { return _kurs; }
	Kurs(const Kurs& k) {
		_kurs = k._kurs;
		_pocetak = k._pocetak;
		_kraj = k._kraj;
		int vel = strlen(k._imePredavaca) + 1;
		_imePredavaca = new char[vel];
		strcpy_s(_imePredavaca, vel, k._imePredavaca);
	}

	Kurs& operator=(const Kurs& k) {
		if (this != &k) {
			_kurs = k._kurs;
			_pocetak = k._pocetak;
			_kraj = k._kraj;
			int vel = strlen(k._imePredavaca) + 1;
			_imePredavaca = new char[vel];
			strcpy_s(_imePredavaca, vel, k._imePredavaca);
		}
		return *this;
	}

	friend ostream& operator << (ostream& cout, const Kurs& k) {
		cout << "Naziv kursa -> " << k._kurs << endl;
		cout << "Predavac -> " << k._imePredavaca << endl;
		cout << "Pocetak -> " << k._pocetak << endl;
		cout << "Kraj -> " << k._kraj << endl;
		return cout;
	}
	
	bool operator==(const Kurs& k) {
		return _kurs == k._kurs && _pocetak == k._pocetak && _kraj == k._kraj && strcmp(_imePredavaca, k._imePredavaca) == 0;
	}
};
class Polaznik {
	static int ID;
	const int _polaznikID;
	//SVAKOM NOVOM POLAZNIKU AUTOMATSKI DODIJELITI NOVI ID (AUTOINCREMENT) POCEVSI OD BROJA 1
	char* _imePrezime;
	string _kontaktTelefon; //BROJ TELEFONA TREBA BITI U FORMATU (06X)XXX-XXX 
	Kolekcija<Kurs*, int, 10> _uspjesnoOkoncaniKursevi;
	//INT PREDSTAVLJA OSTVARENI PROCENAT NA ISPITU, A JEDAN POLAZNIK MOZE POLOZITI NAJVISE 10 KURSEVA
public:
	Polaznik(const char* imePrezime, string telefon) :_polaznikID(ID++) {
		int size = strlen(imePrezime) + 1;
		_imePrezime = new char[size];
		strcpy_s(_imePrezime, size, imePrezime);
		_kontaktTelefon = telefon;
	}
	~Polaznik() {
		delete[] _imePrezime; _imePrezime = nullptr;
		for (size_t i = 0; i < _uspjesnoOkoncaniKursevi.size(); i++)
		{
			delete _uspjesnoOkoncaniKursevi[i];
			_uspjesnoOkoncaniKursevi[i] = nullptr;
		}
	}

	friend ostream& operator << (ostream& cout, const Polaznik& p) {
		cout << "ID -> " << p._polaznikID << endl << "Ime i prezime -> " << p._imePrezime << endl <<
			"Telefon -> " << p._kontaktTelefon << "Lista polozenih kurseva -> " << p._uspjesnoOkoncaniKursevi << endl;
		return cout;
	}

	Polaznik& operator = (const Polaznik& p) {
		if (this != &p) {
			delete[] _imePrezime;
			int size = strlen(p._imePrezime) + 1;
			_imePrezime = new char[size];
			strcpy_s(_imePrezime, size, p._imePrezime);
			_kontaktTelefon = p._kontaktTelefon;
			for (size_t i = 0; i < p._uspjesnoOkoncaniKursevi.GetTrenutno(); i++)
				_uspjesnoOkoncaniKursevi.AddElement(new Kurs(*p._uspjesnoOkoncaniKursevi.GetElement1(i)), p._uspjesnoOkoncaniKursevi.GetElement2(i));
		}
		return *this;
	}

	bool operator==(const Polaznik& p) {
		if (strcmp(_imePrezime, p._imePrezime) != 0)
			return false;

		for (int i = 0; i < this->_uspjesnoOkoncaniKursevi.size(); i++)
		{
			if (_uspjesnoOkoncaniKursevi[i] != p._uspjesnoOkoncaniKursevi.GetElement1(i))
				return false;
		}

		return true;
	}
};
int Polaznik::ID = 1;
class SkillsCentar {
	string _nazivCentra;
	vector<Kurs> _kursevi;//KURSEVI KOJE NUDI ODREDJENI CENTAR
	Kolekcija<Kurs, Polaznik*, 150> _aplikanti;
public:
	SkillsCentar(string naziv) { _nazivCentra = naziv; }
	SkillsCentar(const SkillsCentar& obj) :_kursevi(obj._kursevi), _aplikanti(obj._aplikanti) {
		_nazivCentra = obj._nazivCentra;
	}

	bool ProvjeriKoliziju(const Kurs& k) {
		time_t current;
		time(&current);
		for (int i = 0; i < _kursevi.size(); i++)
		{
			if (_kursevi[i].getKraj() > k.getPocetak())
				return false;
		}
		return true;
	}
	void AddKurs(const Kurs& k) {
		_kursevi.push_back(k);
	}
	void AddAplikaciju(const Kurs& k, const Polaznik& p) {
		bool var = false;
		for (int i = 0; i < _kursevi.size(); i++)
		{
			if (_kursevi[i].getKurs() == k.getKurs()) {
				var = true;
			}
		}
		if(!var)
			throw Izuzetak(__FUNCTION__, __LINE__, "Nije moguce");
		
		for (int j = 0; j < _aplikanti.GetTrenutno(); j++)
		{
			if(_aplikanti.GetElement1(j)==k && _aplikanti.GetElement2(j)==&p)
				throw Izuzetak(__FUNCTION__, __LINE__, "Nije moguce");

		}
		_aplikanti.AddElement(k,new Polaznik(p));
	}
};
const char* crt = "\n---------------------------------------\n";
void main()
{
	/****************************************************************************
	1. SVE KLASE TREBAJU POSJEDOVATI ADEKVATAN DESTRUKTOR
	2. NAMJERNO IZOSTAVLJANJE KOMPLETNIH I/ILI POJEDINIH DIJELOVA DESTRUKTORA KOJI UZROKUJU RUNTIME ERROR ĆE BITI OZNACENO KAO "RE"
	3. SPAŠAVAJTE PROJEKAT KAKO BI SE SPRIJEČILO GUBLJENJE URAĐENOG ZADATKA
	4. PROGRAMSKI CODE SE TAKOĐER NALAZI U FAJLU CODE.TXT
	5. NAZIVI FUNKCIJA, TE BROJ I TIP PARAMETARA MORAJU BITI IDENTIČNI ONIMA KOJI SU KORIŠTENI U TESTNOM CODE-U, OSIM U SLUCAJU DA POSTOJI ADEKVATAN RAZLOG ZA NJIHOVU MODIFIKACIJU. OSTALE, POMOĆNE FUNKCIJE MOŽETE IMENOVATI I DODAVATI PO ŽELJI.
	6. IZUZETAK BACITE U FUNKCIJAMA U KOJIMA JE TO NAZNAČENO.
	****************************************************************************/
//
//#pragma region TestiranjeDatuma
	Datum danas(26, 11, 2015);
	Datum sutra(danas);
	Datum prekosutra;
	prekosutra = danas;
	cout << danas << endl << sutra << endl << prekosutra << crt;
//#pragma endregion
#pragma region TestiranjeKolekcije
	Kolekcija<int, int, 10> kolekcija1;
	try
	{
		for (size_t i = 0; i < kolekcija1.GetMax(); i++)
			kolekcija1.AddElement(i, i + 2);
		//FUNKCIJA TREBA BACITI IZUZETAK U SLUCAJ DA NEMA VISE PROSTORA ZA DODAVANJE NOVIH ELEMENATA
		cout << kolekcija1.GetElement1(0) << " " << kolekcija1.GetElement2(0) << endl;
		//cout << kolekcija1.GetElement1(20) << " " << kolekcija1.GetElement2(20) << endl;
		//FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO ELEMENT NA ZAHTIJEVANOJ LOKACIJI NE POSTOJI
	}
	catch (Izuzetak & err)
	{
		cout << "Greska -> " << err << endl;
	}
//
	cout << kolekcija1 << endl;
	Kolekcija<int, int, 10> kolekcija2;
	kolekcija2 = kolekcija1;
	cout << kolekcija2 << crt;
	if (kolekcija1.GetTrenutno() == kolekcija2.GetTrenutno())
		cout << "ISTI BROJ ELEMENATA" << endl;
	Kolekcija<int, int, 10> kolekcija3(kolekcija2);
	cout << kolekcija3 << crt;
//#pragma endregion
//#pragma region TestiranjeKursa
	Kurs sef(SoftwareEngeneeringFundamentals, "Jasmin Azemovic", Datum(28, 1, 2016), Datum(15, 2, 2016));
	cout << sef << endl;
	Kurs msql(MasteringSQL, "Adel Handzic", Datum(28, 2, 2016), Datum(15, 3, 2016));
	sef = msql;
	if (sef == msql)//KURSEVI SU ISTI AKO POSJEDUJU IDENTICNE VRIJEDNOSTI SVIH ATRIBUTA
		cout << "ISTI SU KURSEVI" << endl;
	Kurs msql2(msql);
	cout << msql2 << endl;
//#pragma endregion
//#pragma region TestiranjePolaznika
	string telefon1 = "(061)111-111";
	string telefon2 = "(061)111222";
	string telefon3 = "(061)111-333";
//
//	if (!ProvjeriFormatTelefona(telefon1))
//		cout << "Broj " << telefon1 << " nije validan" << endl;
//	if (!ProvjeriFormatTelefona(telefon2))
//		cout << "Broj " << telefon2 << " nije validan" << endl;
//	if (!ProvjeriFormatTelefona(telefon3))
//		cout << "Broj " << telefon3 << " nije validan" << endl;
//
//
	Polaznik denis("Denis Music", telefon1);
	Polaznik denis2("Denis2 Music2", telefon3);
//
//#pragma endregion
//
//#pragma region TestiranjeCentra
	try {
		SkillsCentar mostar("Skills Center Mostar");
		if (mostar.ProvjeriKoliziju(sef))
//			//JEDAN KURS NE SMIJE POCETI DOK DRUGI TRAJE TJ. VRIJEME ODRZAVANJA KURSA SE NE SMIJE POKLAPATI
			mostar.AddKurs(sef);
		if (mostar.ProvjeriKoliziju(msql))
			mostar.AddKurs(msql);
		if (mostar.ProvjeriKoliziju(msql2))
			mostar.AddKurs(msql2);
//		//ONEMOGUCITI DODAVANJE IDENTICNIH KURSEVA. FUNKCIJA TREBA DA BACI IZUZETAK UKOLIKO SE POKUSA DODATI IDENTICAN KURS
		mostar.AddAplikaciju(sef, denis);
//		//ONEMOGUCITI APLICIRANJE ZA KURSEVE KOJI NISU REGISTROVANI U CENTRU
		mostar.AddAplikaciju(msql, denis);
		mostar.AddAplikaciju(msql, denis);
//		//ONEMOGUCITI APLICIRANJE ZA ISTI KURS. FUNKCIJA BAZA IZUZETAK
		cout << denis2 << endl;
		denis2 = denis;
		if (denis == denis2)
			cout << "ISTI SU" << endl;
//			//POLAZNICI SU ISTI AKO POSJEDUJU ISTO IME I AKO SU POHADJALI ISTE KURSEVE
//
		//mostar.DodajUspjesnoOkoncanKurs(1, sef, 60);//BROJ 1 OZNACAVA ID POLAZNIKA. 
//		FUNKCIJA JE ZADUZENA DA POLAZNIKU DODA INFORMACIJU O USPJESNO POLOZENOM KURSU KOJI JE POSLAN KAO PARAMETAR. PREDUSLOV ZA DODAVANJE JE DA JE 
//			POLAZNIK PRETHODNO APLICIRAO ZA TAJ KURS, TE DA JE NA ISPITU OSTVARIO VISE OD 55%
//		mostar.DodajUspjesnoOkoncanKurs(1, msql, 83);
//		vector<Polaznik> listaPolaznika = mostar.GetPolazniciByPredavac("Jasmin Azemovic", SoftwareEngeneeringFundamentals);//VRACA INFORMACIJE O POLAZNICIMA 
		//KOJI SU KOD ODREDJENOG PREDAVACA USPJESNO OKONCALI ODREDJENI KURS
//
//		if (mostar.RemoveKurs(sef))//ONEMOGUCITI UKLANJANJE KURSA KOJI SU VEC PRIJAVILI NEKI OD POLAZNIKA
//			cout << "Kurs " << sef << " uspjesno uklonjen iz centra " << mostar << endl;
//
		/*SkillsCentar sarajevo = mostar;
		cout << sarajevo << endl;*/
	}
	catch (Izuzetak & err) {
		cout << err << endl;
	}
#pragma endregion
	system("pause");
}