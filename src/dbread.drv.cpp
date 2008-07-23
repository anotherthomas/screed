#include "dbread.h"
#include <iostream>


using namespace std;

int main(){
	dbread db;
	long long index, type;
	string result;

	while(1){
		cout << "Enter a query: ";
		cin >> index >> type;
		if(cin.eof()){
			cout << endl;
			break;
		}
		cout << db.query(index, type) << endl;
	}

	return 0;
}
