#include <iostream>
#include <vector>
#include <fstream>
#include <string>
using namespace std;

string readFromFile(string filename) {
	ifstream myfile(filename);
	if (myfile.is_open()) {
		string text{ istreambuf_iterator<char>(myfile), istreambuf_iterator<char>() };
		myfile.close();
		return text;
	}
	else
		cout << "Datoteke ni mogoce odpreti" << endl;
	return "error";
}
void outputFile(vector<int> *indeksi) {
	ofstream myfile("out.txt");
	if (myfile) {
		for (int i = 0; i < indeksi->size(); i++) {
			myfile << indeksi->at(i) << " ";
		}
		myfile.close();
	}
	else {
		cout << "V datoteko ni bilo mozno pisati!" << endl;
	}
}
vector<int> *kmpNext(string vzorec) {
	int position = 2;
	int cnd = 0;
	vector<int> *kmp = new vector<int>();
	kmp->push_back(-1);
	kmp->push_back(0);
	while (position < vzorec.length()) {
		if (vzorec[position - 1] == vzorec[cnd]) {
			kmp->push_back(cnd+1);
			cnd++;
			position++;
		}
		else if (cnd > 0) {
			cnd = kmp->at(cnd);
		}
		else {
			kmp->push_back(0);
			position++;
		}
	}
	return kmp;
}
vector<int> *kmpAlgorithm(string vzorec, string text, vector<int> *kmpTable) {
	int m = 0;
	int i = 0;
	vector<int> *indeksi = new vector<int>();
	while ((m + i) < text.length()) {
		if (vzorec[i] == text[m + i]) {
			if (i == vzorec.length() - 1) {
				indeksi->push_back(m);
				i = -1;
				m++;
			}
				i++;
		}
		else {
			m = m + i - kmpTable->at(i);
			if (kmpTable->at(i) > -1) {
				i = kmpTable->at(i);
			}
			else {
				i = 0;
			}
		}
	}
	return indeksi;
}
vector<int> *sundayPreProc(string vzorec) {
	vector<int> *indeksi = new vector<int>(256);
	int dolzinaVzorca = vzorec.length();
	for (int i = 0; i < indeksi->size(); i++) {
		indeksi->at(i) = 0;
	}
	for (int i = 0; i < dolzinaVzorca; i++) {
		indeksi->at(vzorec[i]) = dolzinaVzorca - i;
	}
	for (int i = 0; i < indeksi->size(); i++) {
		if (indeksi->at(i) == 0)
			indeksi->at(i) = dolzinaVzorca + 1;
	}
	return indeksi;
}
vector<int> *sundayAlgorithm(string vzorec, string text, vector<int> *bchTable) {
	int dolzinaVzorca = vzorec.length();
	int dolzinaTeksta = text.length();
	int j = 0;
	int i = 0;
	vector<int> *indeksi = new vector<int>();
	while (j <= dolzinaTeksta - dolzinaVzorca) {
		if (vzorec[dolzinaVzorca -i- 1] == text[j + dolzinaVzorca-i-1]) {
			i++;
			if (i == dolzinaVzorca) {
				indeksi->push_back(j);
				i = 0;
				j = j + bchTable->at(text[j + 1]);
			}
		}
		else {
			j += bchTable->at(text[j + dolzinaVzorca - i]);
			i = 0;
		}
	}
	return indeksi;
}

int main(int argc, char** argv) {
	string text;
	string vzorec;
	vector<int> *tabela;
	vector<int> *indeksi;
	if (argc != 4) {
		cout << "Napacno stevilo argumentov!" << endl;
		cout << "ARAvaja2 <opcija> <iskani niz> <datoteka besedila>" << endl;
		cout << "opcija: 0(kmp),1(Sunday)" << endl;
		cout << "iskani niz: niz, ki ga iscete. Npr. 'Lorem Ipsum'" << endl;
		cout << "datoteka besedila: pot do datoteke, v kateri je besedilo, v katerem iscete niz.";
	}
	else {
			text = readFromFile(argv[3]);
			if (text != "error") {
				vzorec = argv[2];
				if (vzorec != "") {
					if (strcmp(argv[1], "0") == 0) {
						tabela = kmpNext(vzorec);
						indeksi = kmpAlgorithm(vzorec, text, tabela);
						outputFile(indeksi);
						cout << "Uspesno zapisal indekse pojavitve: " << vzorec << " v datoteko out.txt z algoritmom Knuth-Morris-Pratt!" << endl;
					}
					else if (strcmp(argv[1], "1") == 0) {
						tabela = sundayPreProc(vzorec);
						indeksi = sundayAlgorithm(vzorec, text, tabela);
						outputFile(indeksi);
						cout << "Uspesno zapisal indekse pojavitve: "<<vzorec <<" v datoteko out.txt z algoritmom Sunday!" << endl;
					}
					else {
						cout << "Na izbiro imate samo algoritem 0(KMP) ali 1(Sunday)!" << endl;
					}
				}
				else {
					cout << "Ne morete iskati praznega stringa..." << endl;
				}
			}
	}
	return 0;
}