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
void rotacion();	
	
	int HEIGHT;
	int WIDTH;
	int matrix_size; //TamaÃ±o de la matriz sin contar con los bytes que indican el tamaÃ±o

int main(int argc, char ** argv){
 //argc= numero de argumentos especificados por linea de comandos
//argv[i] contiene el contenido del argumento i
  leer_dimensiones();
  

  
  cout<<"Hola"<<endl;
  

   int cont=1;
   int num_funcion=-1;
   int num_histograma=-1;
   double num_decimal=-1;
   string in_file="";
   string out_file="";
   string path_mascara="";


    
    while ((cont < argc) && (argv[cont][0]=='-')) {
        string sw = argv[cont];
        if (sw=="-u") {
            cont++;
            num_funcion = atoi(argv[cont]);
            	if(num_funcion<0 || num_funcion> 4){
            		cerr<<"Numero de funcion no valido"<<endl;
            		return 1;
            	}
        }
        else if (sw=="-i") {
            cont++;
            in_file = argv[cont];
        }
        else if (sw=="-o") {
            cont++;
            out_file = argv[cont];
        }
        else if (sw=="-t") {
            cont++;
            num_histograma = atoi(argv[cont]);;
            	if(num_histograma<=0){
            		cerr<<"El numero de histrogramas debe ser mayor que 0"<<endl;
            		return 1;

            	}
        }
        else if (sw=="-f") {
            cont++;
            path_mascara = argv[cont];
        }
        else if (sw=="-a") {
            cont++;
            num_decimal = atof(argv[cont]);
        }
        else if (sw=="-r") {
            cont++;
            num_decimal = atof(argv[cont]);
            	if(num_decimal<0){
            		cerr<<"El numero del radio no es valido"<<endl;
            		return 1;
            	}
        }
        else {
            cerr << "Comando no valido: " 
                 << argv[cont] << endl;
                 return 1;
        }
        cont++;
    }

    //COmrpobaciones que son comunes para todas las funciones
    if(in_file == ""){
    				cerr<<"Falta introducir -i input_file"<<endl;
    				return 1;
    				}
	if(out_file == ""){
    				cerr<<"Falta introducir -o output_file"<<endl;
    				return 1;
    				}



    //Seleccion del numero de funcion
    switch(num_funcion){
    	case(0):	
    				if(num_histograma<0){
    				cerr<<"Flata introducir -t numero_histograma"<<endl;
    				return 1;
    				}
    				if(argc!=9){
    				cerr <<"Numero de argumentos no valido"<<endl;
    				return 1;
    				}

    				cout<<"Funcion histograma blanco y negro"<<endl;
    				break;
		case(1):	cout<<"Funcion maximos y minimos"<<endl;
					MaxMin();
					break;
		case(2):	if(path_mascara==""){
					cerr<<"Falta introducir -f path_mascara"<<endl;
					return 1;
					}
					if(argc!=9){
    				cerr <<"Numero de argumentos no valido"<<endl;
    				return 1;
    				}
					cout<<"Funcion mascara"<<endl;
					aplicar_mascara();
					break;
		case(3):	if(num_decimal<0){
					cerr<<"Falta introducir -a angulo_rotar"<<endl;
					return 1;
					}
					if(argc!=9){
    				cerr <<"Numero de argumentos no valido"<<endl;
    				return 1;
    				}
					cout<<"Rotacion de la imagen"<<endl;
					rotacion();
					break;
		case(4):	if(num_decimal<0){
					cerr<<"Falta introducir -r radio_circulo"<<endl;
					return 1;
					}
					if(argc!=9){
    				cerr <<"Numero de argumentos no valido"<<endl;
    				return 1;
    				}
					cout<<"Funcion de filtro blanco y negro selectivo"<<endl;
					break;
		default:	cerr<<"Falta introducir -u numero_funcion"<<endl;
    }



	
	
  
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

void rotacion(/*const char* img, const char* exit, int gr*/){
	ifstream pInFile;
	pInFile.open("imagen.img", ios::in | ios::binary);
		if (pInFile.is_open()) {
			
			
			//Creamos el ofstream, para escribir en el fichero de salida
         	ofstream pOutFile;
			pOutFile.open("rot_out.img", ios::out | ios::trunc | ios::binary);	
			
         	if(!pInFile) { 
    		cout << "Cannot open file"<<endl;  
   			} 
   			
   			
		 unsigned char imgdata; 
		
		//Escribimos en el rot_out, los primeros 8 bytes de su tamaÃ±o, que sera el tamaÃ±o de la imagen de entrada
	   	unsigned char heightData[4]; 
		unsigned char widthData[4]; 
	   pInFile.seekg(0, ios::beg); //pos filter at beginning of image file.
	 
		 //Leo los 4 primeros bytes
			pInFile.read( (char *)& heightData, 4 ); //Lees la altura
		 	pInFile.read( (char *)& widthData, 4); 
		 	
		 	//Escribo en el fichero de salida, los tamaÃ±os de la matriz
		 		pOutFile.write( (char *)& heightData, 4);
		 		pOutFile.write( (char *)& widthData, 4);
		 		
		 		
		 		
		 		
			int xc , yc , xf, yf, xi,yi;
	
			 char fin[WIDTH][HEIGHT];
			
			 //Con el ceil, redondeamos al de arriba
			xc=trunc(WIDTH/2);
			yc=trunc(HEIGHT/2);
		
			
			
			cout<<"El centro es xc:"<<xc<<" yc:"<<yc<<endl;
			
			for (int j=0; j<HEIGHT; j++){
				for (int i=0; i<WIDTH; i++){
					xi=i-xc;
					yi=j-yc;
					
					
					xf= ceil( cos( (90*PI)/180 )*xi   - sin( (90*PI)/180)*yi +xc);
					
					
					yf= ceil( sin( (90*PI)/180 )*xi   + cos( (90*PI)/180)*yi +yc);
					
					
					
					pInFile.read( (char *)& imgdata,1);
					
					if(yf<HEIGHT && yf>=0 && xf<WIDTH && xf>=0){
					
						fin[xf][yf]= imgdata;
					
					}
				}
			}
			
			
		for (int j=0; j<HEIGHT; j++){
				for (int i=0; i<WIDTH; i++){	
				
				pOutFile.write( (char *)& fin[i][j], 1);
			
		}}
		
		}
}	

void MaxMin(/*const char* img, const char* exit*/){
	
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
		ofstream pOutFile;
    	pOutFile.open("maxmin.txt", ios::out | ios::trunc | ios::binary);
    	
    	if(!pOutFile) { 
    		cerr << "Cannot open file "<<endl; 
   			
   			} 
		for (int i=0; i<6; i++){
			
		
			
   			pOutFile<<colores[i]; 
   			if(i<5){
   			pOutFile<<",";
			}
		} 
   		 pOutFile.close(); 
	 }else{
	 	cerr<<"Error opening imagen.img"<<endl;
	 }
}



	
	
	



