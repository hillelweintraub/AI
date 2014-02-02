//Hillel Weintraub
//12/1/13
//AI Project#2-Neural network
//
//train.cpp: train a neural network and output a file of weights 

#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <fstream>
using namespace std;

void loadWeights(int &Ni,int &Nh,int &No,double ** &W1,double ** &W2){
	string filename;
	cout << "Enter name of initial neural network file: ";
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

void loadTraining(double ** &X,int ** &Y,int &numexamples){
	string filename;
	cout << "Enter name of training set file: ";
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

double calcBackPropSum(double **W2,double *delta_out,int No,int hiddenNode){
	double sum=0;
	for(int i=0;i<No;i++)
		sum+=W2[hiddenNode][i]*delta_out[i];
	return sum;
}

double sigmoid(double x){
	double ret=1/(1+exp(-x));
	return ret;
}

void trainNet(double **X,int **Y,int Ni,int Nh,int No,double **W1,double **W2,int numexamples){
	int epochs;
	double alpha;
	cout<<"Enter the number of epochs: ";
	cin>>epochs;
	cout<<"Enter the learning rate: ";
	cin>>alpha;
	double *a_in=new double[Ni+1];
	double *a_hid=new double[Nh+1];
	double *a_out=new double[No];
	double *delta_hid=new double[Nh+1];
	double *delta_out=new double[No];
	double *in_hid=new double[Nh];
	double *in_out=new double[No];

	for (int epoch=0;epoch<epochs;epoch++){
		for(int example=0;example<numexamples;example++){
			a_in[0]=-1;
			for(int i=1;i<=Ni;i++)
				a_in[i]=X[i-1][example];
			//hidden layer
			a_hid[0]=-1;
			for(int i=0;i<Nh;i++){
				in_hid[i]=calcInput(W1,a_in,Ni+1,i);
				a_hid[i+1]=sigmoid(in_hid[i]);
			}
			//output layer
			for(int i=0;i<No;i++){
				in_out[i]=calcInput(W2,a_hid,Nh+1,i);
				a_out[i]=sigmoid(in_out[i]);
			}
			//calculate error at ouput layer
			for(int i=0;i<No;i++)
				delta_out[i]=sigmoid(in_out[i])*(1-sigmoid(in_out[i]))*(Y[i][example]-a_out[i]);
			//calculate error at hidden layer
			for(int i=0;i<Nh;i++)
				delta_hid[i]=sigmoid(in_hid[i])*(1-sigmoid(in_hid[i]))*calcBackPropSum(W2,delta_out,No,i+1);//i+1 cause we don't need delta of the fixed input
			//recalculate the weights W1
			for(int i=0;i<Ni+1;i++){
				for(int j=0;j<Nh;j++)
					W1[i][j]+=alpha*a_in[i]*delta_hid[j];
			}
			//recalculate the weights W2
			for(int i=0;i<Nh+1;i++){
				for(int j=0;j<No;j++)
					W2[i][j]+=alpha*a_hid[i]*delta_out[j];
			}
		}
	}
	free(a_in); free(a_hid); free(a_out);
	free(delta_hid); free(delta_out);
	free(in_hid); free(in_out);
}

void writeTrainedNettoFile(int Ni,int Nh,int No,double **W1,double **W2){
	string filename;
	cout << "Enter name of output file: ";
	cin >> filename;
	ofstream output(filename.c_str());
	
	if (!output) {
    	cerr << "Error: could not open " << filename << endl;
    	exit(1);
	}
	output<<Ni<<" "<<Nh<<" "<<No<<endl;
	output<<fixed<<setprecision(3);
	int i;
	for(int j=0;j<Nh;j++){
		for(i=0;i<Ni;i++)
			output<<W1[i][j]<<" ";
		output<<W1[i][j]<<endl;////////
	}
	for(int j=0;j<No;j++){
		for(i=0;i<Nh;i++)
			output<<W2[i][j]<<" ";
		output<<W2[i][j]<<endl;
	}
	output.close();
}
int main(int argc, char const *argv[]){
	int Ni,Nh,No,numexamples;
	double **W1, **W2, **X;
	int **Y;
	loadWeights(Ni,Nh,No,W1,W2);
	loadTraining(X,Y,numexamples);
	trainNet(X,Y,Ni,Nh,No,W1,W2,numexamples);
	writeTrainedNettoFile(Ni,Nh,No,W1,W2);
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
	return 0;
}