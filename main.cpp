#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;

polla
 	
	int HEIGHT;
	int WIDTH;
	
int main(int argc, char *argv[]){
 //argc= numero de argumentos especificados por linea de comandos
//argv[i] contiene el contenido del argumento i
  
  
  /*

	ifstream inFile;
	size_t size = 0; 

	inFile.open( "imagen.txt", ios::in | ios::binary | ios::ate );
	 if (inFile.is_open()) {
	char* oData = 0;
	
	//inFile.seekg(0, ios::end); // set the pointer to the end
	size = inFile.tellg() ; // get the length of the file
	cout << "Size of file: " << size<<endl;
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
*/
return 0;
}
void leer_dimensiones(const char* fileName){
	
	ifstream pInFile;
	pInFile.open(fileName, ios::in | ios::binary); // open fileName and read as binary.
    
    if (pInFile.is_open()) {
    pInFile.seekg(0, ios::beg); //pos filter at beginning of image file.
	char heightData[4]; 
	char widthData[4]; 
	pInFile.read(heightData, 4); //Lees la altura
	
	/*Calculamos la altura y lo metemos en height*/
	
	pInFile.read(widthData, 4); //Lees la anchura
	
	/*Calculas la anchura y lo metes en width*/
	
}else{
	cerr <<"Error opening file";
}
	
}

void aplicar_mascara(){
		
		/* 	1)Leer 1byte, que son dos posiciones del fichero
		   	2)Leet 1 byte, del fichero de la mascara, que serian dos digitos, y pasarlo a decimal. 01=1, 00=0
			3)Multiplicar el primero numero, por el 0 o el 1 de la mascara segun corresponda
			4)Escribir el resultado en un fichero de salida llamado mask_out.img
	*/
	
}

	
	
	
	
	
	




