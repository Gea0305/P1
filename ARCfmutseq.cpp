#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <math.h>

#define PI 3.14159265
using namespace std;
//Prototipado de las funciones
void leer_dimensiones();
void aplicar_mascara();
void MaxMin();
void rotacion(const char* , const char* , int );	
	
	int HEIGHT;
	int WIDTH;
	int matrix_size; //TamaÃ±o de la matriz sin contar con los bytes que indican el tamaÃ±o

int main(int argc, char *argv[]){
 //argc= numero de argumentos especificados por linea de comandos
//argv[i] contiene el contenido del argumento i
  leer_dimensiones();
  
  cout<<argv[1]<<endl;
  //cout<<argv[2]<<endl;
  cout<<argv[3]<<endl;
  //cout<<argv[4]<<endl;
  cout<<argv[5]<<endl;
  //cout<<argv[6]<<endl;
  cout<<argc<<endl;
	if(argc > 9){
 		cerr << "Numero de argumentos demasiado grande";
  		return 1;
  	} 
  		if(argv[1]!= "-u" ){
		cerr << "Arguementos no validos: falta el argumento -u";
 		return 1;
	}	if(argv[3]!= "-i" ){
		cerr << "Arguementos no validos: falta el argumento -u";
 		return 1;
	}
	if(argv[5]!= "-o" ){
		cerr << "Arguementos no validos: falta el argumento -u";
 		return 1;
	} 
	  

	
	if( argv[2]=="0" ){
		 if(argv[7]!= "-t"){
			cerr << "Histrograma: Se necesitan los argumentos -t <numero entero>";
  			return 0;
			} else {
				cout<<"Llamamos a la funcion histograma con los argumentos"<<argv[4]<<" "<<argv[6]<<" "<<argv[8]<<endl;
				//histograma(argv[4],argv[6],argv[8]);
		      	}
		
	}	else
	if( argv[2]=="1") { 
		 if (argc > 7){
			cerr<<"Maximos y minimos: Demasiados argumentos";
			return 1;
			}
			cout<<"Llamamos a la funcion MAXMIN con los argumentos"<<argv[4]<<" "<<argv[6]<<endl;
			//MaxMin(argv[4],argv[6]);
		
			}else
	if(argv[2]=="2"){
				 if(argv[7]!= "-f"){
				cerr << "Aplicar mascara: Se necesitan los argumentos -f <ruta>";
  				return 1;
			} else{
				cout<<"Llamamos a la funcion aplicar_mascara con los argumentos"<<argv[4]<<" "<<argv[6]<<" "<<argv[8]<<endl;
				//aplicar_mascara(argv[4],argv[6],argv[8]);
		      	}
			
			}else
				if(argv[2]=="3"){
				 if(argv[7]!= "-a"){
				cerr << "Rotacion: Se necesita los argumentos -a <numero decimal>";
  				return 0;
			} else{
				cout<<"Llamamos a la funcion rotacion con los argumentos"<<argv[4]<<" "<<argv[6]<<" "<<argv[8]<<endl;
			//	rotacion(argv[4],argv[6],argv[8]);
		     	}
				}else
			if(argv[2]=="4"){
				if(argv[7]!= "-r"){
				cerr << "Filtro blanco y negro: Se necesitan los argumentos -r <numero decimal positivo>";
  				return 1;
			} else{
				
				if(argv[8]<0){
					cerr<<"El radio del circulo a filtrar debe ser positivo";
					return 1;
				}
				cout<<"Llamamos a la funcion filtro con los argumentos"<<argv[4]<<" "<<argv[6]<<" "<<argv[8]<<endl;
				//filtro(argv[4],argv[6],argv[8]);
		    	}
		}else cerr << "Argumentos introducidos no validos";
		
	
	return 0;
}

void leer_dimensiones(/*const char* fileName*/){
	
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

		matrix_size=  (HEIGHT*WIDTH)*3;
		cout<<"Matrix size: "<< matrix_size<<endl;
	
}else{
	cerr <<"Error opening file";
} 
	
}

void aplicar_mascara(){
		
	
			ifstream pInImagen;
			pInImagen.open("imagen.img", ios::in | ios::binary); // open fileName and read as binary.
    		ifstream pInMascara;
    		pInMascara.open("mask.img", ios::in | ios::binary); 
    		
   		 if (pInImagen.is_open()) {
         if (pInMascara.is_open()) {
         	
         	//Creamos el ofstream, para escribir en el fichero de salida
         	ofstream pOutFile;
			pOutFile.open("mask_out.img", ios::out | ios::trunc | ios::binary);	
			
         	if(!pOutFile) { 
    		cout << "Cannot open file"<<endl;  
   			} 
     //Escribimos en el mask_out, los primeros 8 bytes de su tamaÃ±o, que sera el tamaÃ±o de la imagen de entrada
	   	unsigned char heightData[4]; 
		unsigned char widthData[4]; 
	   pInImagen.seekg(0, ios::beg); //pos filter at beginning of image file.
	 
		 //Leo los 4 primeros bytes
			pInImagen.read( (char *)& heightData, 4 ); //Lees la altura
		 	pInImagen.read( (char *)& widthData, 4); 
		 	
		 	//Escribo en el fichero de salida, los tamaÃ±os de la matriz
		 		pOutFile.write( (char *)& heightData, 4);
		 		pOutFile.write( (char *)& widthData, 4);
		 	
    //Donde almacenamos los datos tienen que ser unsigned char, ya que van desde 0 a 255    
	unsigned char ImageData; 
	unsigned char MaskData; 
	int dato_imagen;
	int dato_mascara;
	int contador=0;
	
	
	 //Ponemos el puntero para que empiece a leer en el byte 9, ya que los 8 primeros son del tamaÃ±o de la matriz
	 pInMascara.seekg(8); 
	
	//Ponemos contador<matrix_size, ya que con !ImageData.eof, debe ser que lee tambien el /0 del final, y escribe una posicion mas de la debida
	while(contador<matrix_size){
	//	while(!pInMascara.eof()){
	//	}
	
	pInImagen.read( (char *)& ImageData, 1 ); 
	pInMascara.read( (char *)& MaskData, 1 );	
	
	
	ImageData= ImageData*MaskData;
	
	pOutFile.write( (char *) &ImageData, 1);


	contador++;
	
	
	}
	
		pInImagen.close();
		pOutFile.close();
		pInMascara.close();
		
		
			
	}else{
		cerr<<"Error opening mask.img"<<endl;
	}}else{
		cerr<<"Error opening image.img"<<endl;
	}
	
}

void rotacion(const char* img, const char* exit, int gr){
	ifstream pInFile;
	pInFile.open("imagen.img", ios::in | ios::binary);
		if (pInFile.is_open()) {
			unsigned char imgdata; 
			pInFile.seekg(8); 
			int xc , yc , xf, yf, xi,yi;
	
			int fin[HEIGHT][WIDTH];
			 //Con el ceil, redondeamos al de arriba
			xc=ceil(WIDTH/2);
			yc=ceil(HEIGHT/2);
			
			for (int i=0; i<HEIGHT; i++){
				for (int j=0; j<WIDTH; j++){
					xi=j-xc;
					yi=i-yc;
					xf=(cos(gr/180*PI)*xi-sin(gr/180*PI) * yi);
					yf=(sin(gr/180*PI)*xi-cos(gr/180*PI) * yi);
					pInFile.read( (char *)& imgdata,1);
					if(yf<=HEIGHT && yf>=0 && xf<=HEIGHT && xf>=0){
						fin[yf][xf]= imgdata;
						if (fin[i][j] == NULL){
							fin[i][j]= 0;
						}
					}
				}
			}
		}

}	

void MaxMin(const char* img, const char* exit){
	
		ifstream pInFile;
	pInFile.open("imagen.img", ios::in | ios::binary);
 	if (pInFile.is_open()) {
		//leer_dimenciones(img);
		unsigned char imgdata; 
		int colores[]= {0,255,0,255,0,255} ;
		int red;
		int blue;
		int green;
		
		//No leemos los bytes que indican el tamaÃ±o de la matriz
		pInFile.seekg(8); 
		
		for (int i =0; i < (HEIGHT*WIDTH) ; i++){
			
		pInFile.read( (char *)& imgdata,1);
		red=imgdata;
		if (colores[0] < red){
			//Hay un nuevo maximo
		colores[0]=red;
		}
		if (colores[1] > red){
			//Hay un nuevo minio
		colores[1]=red;
		}
		}
		for (int i =0; i< (HEIGHT*WIDTH); i++){
			
		pInFile.read( (char *)& imgdata,1);
		green=imgdata;
		
		if (colores[2] < green){
		colores[2]=green;
		}
		if (colores[3] > green){
		colores[3]=green;
		}
		}
		for (int i =0; i<(HEIGHT*WIDTH); i++){
			 
		pInFile.read( (char *)& imgdata,1);
		blue=imgdata;
		if (colores[4] < blue){
		colores[4]=blue;
		}
		if (colores[5] > blue){
		colores[5]=blue;
		}
		}
		ofstream pOutFile ;
    	pOutFile.open("maxmin.txt", ios::out | ios::trunc | ios::binary);
    	
    	if(!pOutFile) { 
    		cerr << "Cannot open file "<<endl; 
   			
   			} 
    	cout<<"Hasta aqui todo bien, voy a escribir en el archivo"<<endl;
    	cout<< colores[0]<<endl;
    	cout<< colores[1]<<endl;
    	cout<< colores[2]<<endl;
    	cout<< colores[3]<<endl;
    	cout<< colores[4]<<endl;
    	cout<< colores[5]<<endl;
    	
    	
    	
    	
		for (int i=0; i<6; i++){
			//pOutFile.write("<", 1);
		
			
   			pOutFile.write( (char *)&colores[i], sizeof colores[i]); //write header data onto outpu
			//pOutFile.write(">", 1);
		} 
   		 pOutFile.close(); //close stream
	 }else{
	 	cerr<<"Error opening imagen.img"<<endl;
	 }
}



	
	
	


