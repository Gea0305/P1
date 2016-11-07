#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
using namespace std;
void leer_dimensiones();

 	
	int HEIGHT;
	int WIDTH;
	
int main(/*int argc, char *argv[]*/){
 //argc= numero de argumentos especificados por linea de comandos
//argv[i] contiene el contenido del argumento i
  leer_dimensiones();
  
  
return 0;
}

void leer_dimensiones(/*const char* fileName*/){
	
	ifstream pInFile;
	pInFile.open("imagen.img", ios::in | ios::binary); // open fileName and read as binary.
    
    if (pInFile.is_open()) {
        pInFile.seekg(0, ios::beg); //pos filter at beginning of image file.
	char heightData[4]; 
	char widthData[4]; 
	pInFile.read(heightData, 4); //Lees la altura
	    cout << heightData[0];
	    cout << heightData[1];
	    cout << heightData[2];
	    cout << heightData[3];
	/*Calculamos la altura y lo metemos en height*/
	
	pInFile.read(widthData, 4); //Lees la anchura
	
	   cout<<  widthData[0]; 
	   cout <<  widthData[1]; 
	    cout <<  widthData[2]; 
	    cout << widthData[3]; 
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

	
void MaxMin(){
	
	ifstream pInFile;
	pInFile.open("imagen.img", ios::in | ios::binary);
 	if (pInFile.is_open()) {
		int colores[6];
		int red[HEIGHT*WIDTH];
		int blue[HEIGHT*WIDTH];
		int green[HEIGHT*WIDTH];
		pInFile.read(red, HEIGHT*WIDTH);
		pInFile.read(blue, HEIGHT*WIDTH);
		pInFile.read(green, HEIGHT*WIDTH);
 }
return 0;
}
	
	
	
	
	




