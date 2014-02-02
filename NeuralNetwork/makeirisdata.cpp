//Hillel Weintraub
//12/6/13
//Create training and test data 

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char const *argv[]){
	
	ifstream input("iris.txt");
	if (!input) {
    	cerr << "Error: could not open iris.txt"<<endl;
    	exit(1);
	}
	ofstream output("i.train");
	if (!output) {
    	cerr << "Error: could not open i.train"<<endl;
    	exit(1);
	}

	int Ni=4,No=3,numexamples=75,type;
	double PW,PL,SW,SL;
	string _0str="1 0 0",_1str="0 1 0",_2str="0 0 1";
	output<<fixed<<setprecision(3);
	output<<numexamples<<" "<<Ni<<" "<<No<<endl;
	for(int i=0;i<numexamples;i++){
		input>>type>>PW>>PL>>SW>>SL;
		switch(type){
			case 0: output<<PW/25<<" "<<PL/67<<" "<<SW/41<<" "<<SL/77<<" "<<_0str<<endl; break;
			case 1: output<<PW/25<<" "<<PL/67<<" "<<SW/41<<" "<<SL/77<<" "<<_1str<<endl; break;
			case 2: output<<PW/25<<" "<<PL/67<<" "<<SW/41<<" "<<SL/77<<" "<<_2str<<endl; break;
		}
	}
	output.close();

	//test set
	output.open("i.test");
	if (!output) {
    	cerr << "Error: could not open i.test"<<endl;
    	exit(1);
	}
	output<<fixed<<setprecision(3);
	output<<numexamples<<" "<<Ni<<" "<<No<<endl;
	for(int i=0;i<numexamples;i++){
		input>>type>>PW>>PL>>SW>>SL;
		switch(type){
			case 0: output<<PW/25<<" "<<PL/67<<" "<<SW/41<<" "<<SL/77<<" "<<_0str<<endl; break;
			case 1: output<<PW/25<<" "<<PL/67<<" "<<SW/41<<" "<<SL/77<<" "<<_1str<<endl; break;
			case 2: output<<PW/25<<" "<<PL/67<<" "<<SW/41<<" "<<SL/77<<" "<<_2str<<endl; break;
		}
	}
	input.close();
	output.close();
	return 0;
}