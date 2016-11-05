#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

int main(){


  
  
  

	ifstream inFile;
	size_t size = 0; 

	inFile.open( "hola.txt", ios::in|ios::binary|ios::ate );
	 if (inFile.is_open()) {
	char* oData = 0;
	
	//inFile.seekg(0, ios::end); // set the pointer to the end
	size = inFile.tellg() ; // get the length of the file
	cout << "Size of file: " << size<<endl;
	//Ponemos el puntero en la primera posicion
	inFile.seekg(0); // set the pointer to the beginning

	oData = new char[ size ]; 
	inFile.read( oData, 5 );
	
	
	cout<<oData[0];
	cout<<oData[1];
	cout<<oData[2];
	cout<<oData[3];
	cout<<oData[4];
	
	//getchar();
}else{
	cerr << "Error opening file";
}
	return 0;

return 0;
}


