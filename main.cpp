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
	
int main(int argc, char *argv[]){
 //argc= numero de argumentos especificados por linea de comandos
//argv[i] contiene el contenido del argumento i
 // leer_dimensiones();
	if(argc > 9){
 		cerr << "argumentos invalidos";
  		return 0;
  	}else if (argc > 7 && argv[1]== "-u1"){
			cerr << "argumentos invalidos";
  			return 0;
		}
	if(argv[1]!= "-u" || argv[3]!= "-i" || argv[5] != "-o"){
		cerr << "argumentos invalidos";
 		return 0;
	}
	swich (argv[2]){
		case 0: if(argv[7]!= "-t"){
				cerr << "argumentos invalidos";
  				return 0;
			} else {
				histograma(argv[4],argv[6],argv[8]);
		      	}
			break;
		
		case 1: MaxMin(argv[4],argv[6]);
			break;
		
		case 2: if(argv[7]!= "-f"){
				cerr << "argumentos invalidos";
  				return 0;
			} else{
				aplicar_mascara(argv[4],argv[6],argv[8]);
		      	}
			break;
		
		case 3: if(argv[7]!= "-a"){
				cerr << "argumentos invalidos";
  				return 0;
			} else{
				rotacion(argv[4],argv[6],argv[8]);
		     	}
			break;
		
		case 4: if(argv[7]!= "-r"){
				cerr << "argumentos invalidos";
  				return 0;
			} else{
				filtro(argv[4],argv[6],argv[8]);
		    	}
			break;
		default:  cerr << "argumentos invalidos";
	}
	return 0;
}

void leer_dimensiones(/*const char* fileName*/, ){
	
	ifstream pInFile;
	pInFile.open("imagen.img", ios::in | ios::binary); // open fileName and read as binary.
    
   	 if (pInFile.is_open()) {
        
   	       //Donde almacenamos los datos tienen que ser unsigned char, ya que van desde 0 a 255    
		unsigned char heightData[4]; 
		unsigned char widthData[4]; 

	 
	 pInFile.seekg(0, ios::beg); //pos filter at beginning of image file.
	 
	 //Leo los 4 primeros bytes
	pInFile.read( (char *)& heightData, 4 ); //Lees la altura
	
	//Lee 2000 0000, como esta en LEndian seria 0000 0020, que equivale a 32.En las posiciones el 0 seria el 3, el 1 el 2 y el 3 el 0
	//Lo cambio a BIGENDIAN
   	HEIGHT += (int)heightData[0] | ((int)heightData[1]<<8) | ((int)heightData[2]<<16) | ((int)heightData[3]<<24);
	cout<<"HEIGHT:  "<<HEIGHT<<endl;
	
	pInFile.read( (char *)& widthData, 4); 
	WIDTH += (int)widthData[0] | ((int)widthData[1]<<8) | ((int)widthData[2]<<16) | ((int)widthData[3]<<24);
	cout <<"WIDTH: "<<WIDTH<<endl;

	
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

	
void MaxMin(const char* img, const char* exit){
	
	ifstream pInFile;
	pInFile.open(img, ios::in | ios::binary);
 	if (pInFile.is_open()) {
		leer_dimenciones(img);
		unsigned char imgdata; 
		int colores[6];
		colores[0]=255;
		colores[2]=255;
		colores[4]=255;
		int red;
		int blue;
		int green;
		for (int i =0; i<HEIGHT*WIDTH; i++){
		pInFile.read(imgdata,1);
		red=imgdata;
		if (colores[0] > red){
		colores[0]=red;
		}
		if (colores[1] < red){
		colores[1]=red;
		}
		}
		for (int i =0; i<HEIGHT*WIDTH; i++){
		pInFile.read(imgdata,1);
		green=imgdata;
		if (colores[0] > green){
		colores[0]=green;
		}
		if (colores[1] < green){
		colores[1]=green;
		}
		}
		for (int i =0; i<HEIGHT*WIDTH; i++){
		pInFile.read(imgdata,1);
		blue=imgdata;
		if (colores[0] > blue){
		colores[0]=blue;
		}
		if (colores[1] < blue){
		colores[1]=blue;
		}
		}
		ofstream pOutFile ;
    		pOutFile.open(exit, ios::out | ios::trunc | ios::binary);
		for (int i=0; i<6; i++){
			pOutFile.write("< ", 1);
   			pOutFile.write(colores[i], 1); //write header data onto outpu
			pOutFile.write(" >", 1);
		}
   		 pOutFile.close(); //close stream
	 }
return 0;
}

void Conversor(int n){
	int decimal;   
	stringstream ss;    
	ss << hex << n;
	ss >> decimal;
	return decimal;
}

	
	
	




