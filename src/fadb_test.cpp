#include "dbwrite.h"
#include "dbread.h"
#include <iostream>
#include <fstream>
#include <assert.h>
#include <string>

using namespace std;

int main(){
	fstream theFile;
	string dna;
	unsigned i, j;
	char accu[] = "";
	char line[LINESIZE], endck;
	char name[LINESIZE], desc[LINESIZE];
	string filename = "tests/test.fa";
	long long filepos, dnsiz;
	int nmsiz, desiz;
	dbwrite db(filename);

	if(!db.is_open()){
		cerr << "ERROR: DATABASE FILES ARE NOT OPEN\n";
		exit(1);
	}
	theFile.open(filename.c_str(), fstream::in);
	while(!theFile.eof()){
		nmsiz = 0; // Initialize the sizes of the relevant c-strings to
		desiz = 0; // 0
		dnsiz = 0;

		//Increment the file position one to skip the '>' in front of
		//the name
		filepos = theFile.tellg();
		theFile.seekg(filepos+1);
		theFile >> name >> ws; // Pull in the line and skip whitespace
		for(i=0;name[i]!='\0';i++){}
		i++;
		nmsiz = i;

		theFile.getline(desc, LINESIZE);
		for(i=0;desc[i]!='\0';i++){}
		i++;
		desiz = i;

		dna.clear();
		endck = '0';
		while((endck != '>') && (!theFile.eof())){
				theFile.getline(line, LINESIZE);
				dna.append(line);
				endck = theFile.peek();
		}
		dnsiz = dna.size();
		dnsiz++;

		char chdna[dnsiz];
		for(i=0;i<dnsiz-1;i++){
			chdna[i] = dna[i];
		}
		chdna[i] = '\0';

		db.addrecord(name, desc, accu, chdna, nmsiz, desiz, 1,
				dnsiz);
	}

	//Closes reading and writing files
	theFile.close();
	db.close();

	//Do the testing to make sure everything still works
	filename.append(".seqdb2");
	dbread thedb(filename);
	assert(thedb.query(0, 1) == "ENSMICT00000012722");	
	assert(thedb.query(2, 2) == "cdna:novel scaffold:micMur1:scaffold_184569:3:516:-1 gene:ENSMICG00000004628");
	assert(thedb.query(7, 4) == "CTGGTCATCGTGCTCGTGTGCAAGAAGCTGAGGAGCATCACGGACGTGTACCTCTTGAACCTGGCCCTGTCCGACCTGCTCTTTGTCTTCTCCTTCCCCTTTCAGACCCACTATCAGCTGGACCAGTGGGTGTTTGGGACCGTGATGTGCAAAGTGGTCTCTGGATTTTATTACATTGGCTTCTTCAGCAGCATGTTCTTCATCACCCTCATGAGCGTGGACAGGTACCTGGCTATCGTCCACGCCGTGTACGCCATAAAAGTGAGGACGGCCAGAATGGGCACAGCCCTGAGCCTGGTAGTGTGGCTGACAGCCATCATGGCCACCAGCCCACTGCTAGTATTTTACCAAGTGGCCTCTGAAGACGGCGTCCTGCAGTGTTACTTGTCTTACAACCAGCAGACTTTGAAGTGGAAGATCTTCACCCACTTCGAAATGAACATCTTGGGCCTGTTGATCCCGTTCACCGTTCTTCTGTTCTGCTACCTTAGCATCCTGCACCAGCTGAGGAGGTGCCAGAACCACAACAAGACCAAGGCCATCAAGCTGGTGCTCATCGTGGTCGTTGCATCTTTACTCTTCTGGGTCCCATTCAACGTGGTCCTCTTCCTCACGTCCCTGCATAACATGCACGTCTTGGATGGGTGTGCCCTGAGCCAGCAGCTGATTTATGCCACCCATGTCACAGAAACCATTTCGTTCACTCACTGCTGCGTGAACCCAATTATCTATGCTTTCATG");
	assert(thedb.query(21, 1) == "ENSMICT00000003880"); 
	assert(thedb.query(21, 2) == "cdna:novel scaffold:micMur1:scaffold_175819:130:631:1 gene:ENSMICG00000003884");
	assert(thedb.query(21, 4) == "ATGCTGCCTAAGTTTGACCCCAACGCGATCAAAGTCATGTACCTGAGGTGCACGGGTGGCGAAGTCAGTGCCGTGTCTGCCCTGGCTCCCAAGATCAGCCCCCTGGGTCTGTTGATTGAAGTGGTACCTTCTGCCTCTGCCCTGATCATCAAAGCTCTCAAGGAACCACTAGGAGACAGAAAGAAACAGAAAAACATTAAACACAACAGAAACATCACTTTTCATAAGATTGTCAACACTGCCAGACAGATGTGGCATCCATCTTTAGCCAGAGAACTCTCCAAAACCATTAAATATATCCTGGGGACTGCCTCTGTGGGATGCAACATTGATGGCCACCATCTTCATGCCATTATAGATGACATCAACAGTGGTGCAGTGGCATGCCCAGCTAGTTAA");

	return 0;
}