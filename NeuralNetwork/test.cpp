//Hillel Weintraub
//12/1/13
//AI Project#2-Neural network
//
//test.cpp: test a trained neural network on a test set and output various metrics 

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <fstream>
#define A 0
#define B 1
#define C 2
#define D 3
using namespace std;

void loadWeights(int &Ni,int &Nh,int &No,double ** &W1,double ** &W2){
	string filename;
	cout << "Enter name of trained neural network file: ";
	cin >> filename;
	ifstream input(filename.c_str());
	
	if (!input) {
    	cerr << "Error: could not open " << filename << endl;
    	exit(1);
	}
	
	input>>Ni>>Nh>>No;
	//allocate 2d arrays
	W1=new double*[Ni+1]; 
	for(int i=0; i<Ni+1; ++i)
		W1[i]=new double[Nh];

	W2=new double*[Nh+1];
	for(int i=0;i<Nh+1;i++)
		W2[i]=new double[No];

	//load data
	for(int j=0;j<Nh;j++){
		for(int i=0;i<Ni+1;i++)
			input>>W1[i][j];
	}
	for(int j=0;j<No;j++){
		for(int i=0;i<Nh+1;i++)
			input>>W2[i][j];
	}
	input.close();
}

void loadTestSet(double ** &X,int ** &Y,int &numexamples){
	string filename;
	cout << "Enter name of test set file: ";
	cin >> filename;
	ifstream input(filename.c_str());
	
	if (!input) {
    	cerr << "Error: could not open " << filename << endl;
    	exit(1);
	}
	int Ni,No;
	input>>numexamples>>Ni>>No;

	//allocate 2d arrays
	X=new double*[Ni];
	for(int i=0;i<Ni;i++)
		X[i]=new double[numexamples];

	Y=new int*[No];
	for(int i=0;i<No;i++)
		Y[i]=new int[numexamples];

	//load data
	for(int j=0;j<numexamples;j++){
		for(int i=0;i<Ni;i++)
			input>>X[i][j];
		for(int i=0;i<No;i++)
			input>>Y[i][j];
	}
	input.close();
}

double calcInput(double **W1,double *a_in,int numweights,int weightCol){
	double input=0;
	for(int i=0;i<numweights;i++)
		input+=W1[i][weightCol]*a_in[i];
	return input;
}

double sigmoid(double x){
	double ret=1/(1+exp(-x));
	return ret;
}

void computeConfusionMatrix(double **W1,double **W2,double **X,int **Y,int Ni,int Nh, int No,int numexamples,double ** &CM){
	double *a_in=new double[Ni+1];
	double *a_hid=new double[Nh+1];
	double a_out;
	double *in_hid=new double[Nh];
	double in_out;
	CM=new double*[No+1]; 
	for(int i=0; i<=No; ++i)
		CM[i]=new double[4];
	for(int i=0;i<=No;i++){
		for(int j=0;j<4;j++)
			CM[i][j]=0;
	}
	for(int example=0;example<numexamples;example++){
			a_in[0]=-1;
			for(int i=1;i<=Ni;i++){
				a_in[i]=X[i-1][example];
			}
			//hidden layer
			a_hid[0]=-1;
			for(int i=0;i<Nh;i++){
				in_hid[i]=calcInput(W1,a_in,Ni+1,i);
				a_hid[i+1]=sigmoid(in_hid[i]);
			}
			//output layer
			for(int i=0;i<No;i++){
				in_out=calcInput(W2,a_hid,Nh+1,i);
				a_out=round(sigmoid(in_out));
				switch(Y[i][example]){
					case 0:
						if(a_out==0) CM[i][D]+=1;
						else CM[i][B]+=1;
						break;
					case 1:
						if(a_out==0) CM[i][C]+=1;
						else CM[i][A]+=1;
						break;
				}
			}
	}
	free(a_in);
	free(a_hid);
	free(in_hid);
	for(int i=0;i<No;i++){
		CM[No][A]+=CM[i][A];
		CM[No][B]+=CM[i][B];
		CM[No][C]+=CM[i][C];
		CM[No][D]+=CM[i][D];
	}
}

double accuracy(double *CM){
	double ac=(CM[A]+CM[D])/(CM[A]+CM[B]+CM[C]+CM[D]);
	return ac;
}

double precision(double *CM){
	double p=CM[A]/(CM[A]+CM[B]);
	return p;
}

double recall(double *CM){
	double rec=CM[A]/(CM[A]+CM[C]);
	return rec;
}

double f1(double precision,double recall){
	double f1=(2*precision*recall)/(precision+recall);
	return f1;
}

void writeOutput(double **CM,int No){
	string filename;
	cout << "Enter name of output file: ";
	cin >> filename;
	ofstream output(filename.c_str());
	
	if (!output) {
    	cerr << "Error: could not open " << filename << endl;
    	exit(1);
	}
	double macac=0,macprec=0,macrec=0,ac,prec,rec;
	output<<fixed<<setprecision(3);
	int i;
	for(i=0;i<No;i++){
		ac=accuracy(CM[i]); prec=precision(CM[i]); rec=recall(CM[i]);
		output<<(int)CM[i][A]<<" "<<(int)CM[i][B]<<" "<<(int)CM[i][C]<<" "<<(int)CM[i][D]<<" "<<ac<<" "<<prec<<" "<<rec<<" "<<f1(prec,rec)<<endl;
		macac+=ac; macprec+=prec; macrec+=rec;
	}
	//micro avg
	ac=accuracy(CM[i]); prec=precision(CM[i]); rec=recall(CM[i]);
	output<<ac<<" "<<prec<<" "<<rec<<" "<<f1(prec,rec)<<endl;
	//macro avg
	macac/=No; macprec/=No; macrec/=No;
	output<<macac<<" "<<macprec<<" "<<macrec<<" "<<f1(macprec,macrec)<<endl;
	output.close();
}

int main(int argc, char const *argv[]){
	int Ni,Nh,No,numexamples;
	double **W1, **W2, **X,**CM;
	int  **Y;
	loadWeights(Ni,Nh,No,W1,W2);
	loadTestSet(X,Y,numexamples);
	computeConfusionMatrix(W1,W2,X,Y,Ni,Nh,No,numexamples,CM);
	writeOutput(CM,No);

	for(int i=0; i<Ni+1; ++i)
		free(W1[i]);
	for(int i=0;i<Nh+1;i++)
		free(W2[i]);
	free(W1); free(W2);
	for(int i=0;i<Ni;i++)
		free(X[i]);
	for(int i=0;i<No;i++)
		free(Y[i]);
	free(X); free(Y);
	for(int i=0;i<=No;i++)
		free(CM[i]);
	free(CM);
	return 0;
}
