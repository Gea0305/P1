/* 
Practica 1 Arquitectura de Computadores
Autores:
-Jorge Navarro
-Alfonso Martinez
-Alejandro Blanco
-Gonzalo Lencina
*/
#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <math.h>
using namespace std;
//Prototipado de las funciones
void histograma(string, string, int);
void leer_dimensiones(string);
void aplicar_mascara(string, string, string);
void MaxMin(string, string);
void rotacion(string, string, double);	
void aplicar_filtro(string, string, double);
int HEIGHT;
int WIDTH;
int matrix_size; 

int main(int argc, char ** argv){
	int cont=1;
	int num_funcion=-1;
	int num_histograma=-1;
	double num_angulo=-1;
	double num_radio=-1;
	string in_file="";
	string out_file="";
	string path_mascara="";   
	//Bucle donde recorremos el listado de argumentos almacenando la informacion
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
            ++cont;
            in_file = argv[cont];
        }
        else if (sw=="-o") {
            ++cont;
            out_file = argv[cont];
        }
        else if (sw=="-t") {
            ++cont;
            num_histograma = atoi(argv[cont]);;
            if(num_histograma<=0){
            	cerr<<"El numero de histrogramas debe ser mayor que 0"<<endl;
            	return 1;
            }
        }
        else if (sw=="-f") {
            ++cont;
            path_mascara = argv[cont];
        }
        else if (sw=="-a") {
            ++cont;
            num_angulo = atof(argv[cont]);
        }
        else if (sw=="-r") {
            ++cont;
            num_radio = atof(argv[cont]);
           	if(num_radio<0){
           		cerr<<"El numero del radio no es valido"<<endl;
           		return 1;
           	}
        }
        else {
            cerr << "Comando no valido: " 
            << argv[cont] << endl;
            return 1;
        }
        ++cont;
    }
    //Comprobaciones que son comunes para todas las funciones
    if(in_file == ""){
    	cerr<<"Falta introducir -i input_file"<<endl;
    	return 1;
    }
	if(out_file == ""){
    	cerr<<"Falta introducir -o output_file"<<endl;
    	return 1;
    }
	leer_dimensiones(in_file);
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
			histograma(in_file, out_file, num_histograma);
    		break;
		case(1):
			MaxMin(in_file, out_file);
			break;
		case(2):
			if(path_mascara==""){
				cerr<<"Falta introducir -f path_mascara"<<endl;
				return 1;
			}
			if(argc!=9){
    			cerr <<"Numero de argumentos no valido"<<endl;
    			return 1;
    		}
			aplicar_mascara(in_file, out_file, path_mascara);
			break;
		case(3):
			if(num_angulo<0){
				cerr<<"Falta introducir -a angulo_rotar"<<endl;
				return 1;
			}
			if(argc!=9){
    			cerr <<"Numero de argumentos no valido"<<endl;
    			return 1;
    		}
			rotacion(in_file, out_file, num_angulo);
			break;
		case(4):
			if(num_radio<0){
				cerr<<"Falta introducir -r radio_circulo"<<endl;
				return 1;
			}
			if(argc!=9){
    			cerr <<"Numero de argumentos no valido"<<endl;
    			return 1;
    		}
			aplicar_filtro(in_file, out_file, num_radio);
			break;
		default:
			cerr<<"Falta introducir -u numero_funcion"<<endl;
    }
	return 0;
}
//Funcion que lee las dimensiones de la matriz y las almacena en las variables globales correspondientes
void leer_dimensiones(string fileName){
	ifstream InFile;
	InFile.open(fileName, ios::in | ios::binary); 
    if (InFile.is_open()) {   
		unsigned char heightData[4]; 
		unsigned char widthData[4]; 
		InFile.seekg(0, ios::beg);  
		InFile.read( (char *)& heightData, 4 ); //Se leen los bytes de la altura
		HEIGHT += (int)heightData[0] | ((int)heightData[1]<<8) | ((int)heightData[2]<<16) | ((int)heightData[3]<<24); //Se convierten de little endian a big endian
		InFile.read( (char *)& widthData, 4); //Se leen los bytes de la anchura
		WIDTH += (int)widthData[0] | ((int)widthData[1]<<8) | ((int)widthData[2]<<16) | ((int)widthData[3]<<24); //Se convierten de little endian a big endian
		matrix_size=  (HEIGHT*WIDTH)*3; //El tamaño de la matriz sera de 3 matrices de tamaño HEIGHT*WIDHT, RGB
		InFile.close();
	}else{
		cerr <<"Error opening file";
	} 
}
void histograma(string ImageFile, string OutputFile, int t){
	ifstream InFile;
	InFile.open(ImageFile, ios::in | ios::binary);
	if (InFile.is_open()){
		int i, j;
		bool found;
		double grey, tram=255.0/t;
		vector<int> histogram(t, 0); //Vector con los tramos inicializado a 0
		vector<unsigned char> imgdata(matrix_size); //Vector para volcar la matriz reciba
		InFile.seekg(8);
		InFile.read((char*) &imgdata[0], matrix_size);
		InFile.close();
		for (i=0; i<(HEIGHT*WIDTH); ++i){ //Bucle para calcular el gris resultante de cada pixel
			grey=(imgdata[i]*0.3+imgdata[i+HEIGHT*WIDTH]*0.59+imgdata[i+HEIGHT*WIDTH*2]*0.11);
			for (j=0, found=false; j<t && !found; ++j){ //Bucle para encontrar el tramo adecuado en el histograma
				if (grey<((j+1)*tram)){ //Comprobamos desde el primer tramo hasta que entre en uno
					histogram[j]+=1;
					found=true; //Cuando encontramos su tramo no volvemos a ejecutar el for
				}
			}
		}
		ofstream OutFile;
		OutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);
		if (OutFile.is_open()){ //Bucle para escribir el resultado
			for (i=0; i<t; ++i){
				OutFile<<histogram[i];
   				if(i<t-1){
   					OutFile<<" ";
				}
			}
			OutFile.close();
		}else{
			cerr<<"No se puede abrir el fichero "<<OutputFile<<" para escribir"<<endl; 
		}
	}else{
		cerr<<"Error al abrir el fichero "<<ImageFile<<endl;
	}
}
void aplicar_mascara(string ImageFile, string OutputFile, string MaskFile){
	ifstream InImagen;
	InImagen.open(ImageFile, ios::in | ios::binary); 
	ifstream InMascara;
	InMascara.open(MaskFile, ios::in | ios::binary); 
	if (InImagen.is_open()) {
		if (InMascara.is_open()) {
	    		streampos fileSize= matrix_size+8;
			vector<unsigned char> imgdata(fileSize); //Vector para volcar la matriz recibida
			InImagen.read((char*) &imgdata[0], fileSize); //Se leen todos los datos de la imagen
			InImagen.close();
			vector<unsigned char> mskdata(fileSize); //Vector para volcar la mascara recibida
			InMascara.read((char*) &mskdata[0], fileSize); //Se leen todos los datos de la mascara
			InMascara.close();
			for(int i=8; i<(fileSize); ++i){	//Aplicacion de la mascara, sin contar los bytes que especifican las dimensiones
				imgdata[i]*=mskdata[i];
			}
          		//Escritura en el fichero de salida
         		ofstream pOutFile;
			pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);	
	       	if(pOutFile.is_open()) {
			pOutFile.write((char*)& imgdata[0], fileSize);	//Se escribe la matriz resultante en el fichero de salida
	       		pOutFile.close();
   			}else{
    			cerr << "Error al abrir el fichero "<<OutputFile<<" para escribir"<<endl;  
   			}
		}else{
			cerr<<"Error opening "<<MaskFile<<endl;
		}
	}else{
		cerr<<"Error opening "<<ImageFile<<endl;
	}
}
void rotacion(string ImageFile, string OutputFile, double gr){
	ifstream InFile;
	InFile.open(ImageFile, ios::in | ios::binary);
	if (InFile.is_open()) {
	 	ofstream pOutFile;
		pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);
    	if(pOutFile.is_open()){
		unsigned char cabecera[8];
   		InFile.read((char*)& cabecera, 8); //Se lee la cabecera
   		pOutFile.write( (char *)& cabecera, 8);	 //Se escribe la cabecera
   		vector<unsigned char> imgdata(matrix_size); 
		InFile.read((char*)& imgdata[0], matrix_size); //Se vuelca la matriz en el vector
		InFile.close();
		vector<unsigned char> fin(matrix_size);  //Vector donde almacenaremos las matrices RGB rotadas
		double xc,yc,xi,yi;
		int xf,yf;
		xc=WIDTH/2; //Calculamos el centro de la imagen
		yc=HEIGHT/2;
		int contador=0;
		int offset=0;
		for(int k=0; k<3; ++k){ //Se aplica una rotacion por cada color
			for (int j=0; j<HEIGHT; ++j){
				for(int i=0; i<WIDTH; ++i){
					xi=i-xc;
					yi=j-yc;
					xf= ceil( cos((gr*M_PI)/180)*xi - sin((gr*M_PI)/180)*yi +xc); //Formula para calcular las posiciones resultantes de la rotacion
					yf= ceil( sin((gr*M_PI)/180)*xi + cos((gr*M_PI)/180)*yi +yc);
					if(yf<HEIGHT && yf>=0 && xf<WIDTH && xf>=0){ //En el caso de que este dentro de las dimensiones/lienzo de la matriz original se escribe
						fin[(yf*WIDTH + xf)+offset]= imgdata[contador];
						
					}
					contador++;
				}
			}
			offset+=HEIGHT*WIDTH;
		}
      		pOutFile.write((char*)& fin[0], matrix_size); //Se escribe las matrices rotadas en el fichero de salida
	   	pOutFile.close();
	   	}else{
			cerr<<"Error al abrir "<<OutputFile<<endl;
	   	}
	}else{
		cerr<<"Error al abrir "<<ImageFile<<endl;
}  
}
void MaxMin(string ImageFile, string OutputFile){
	ifstream InFile;
	InFile.open(ImageFile, ios::in | ios::binary);
 	if (InFile.is_open()) {
		int colores[]= {0,255,0,255,0,255} ; //Valores iniciales
		vector<unsigned char> imgdata(matrix_size); //Vector para volcar la matriz recibida
		InFile.seekg(8); //No se leen los bytes que indican el tamaño de la matriz
		InFile.read((char*) &imgdata[0], matrix_size); //Se lee la matriz 
		InFile.close();
		int i =0;
		for (; i < WIDTH*HEIGHT; ++i){ //Para la matriz del color rojo
			if (colores[0] < imgdata[i]){ //Hay un nuevo maximo
				colores[0]=imgdata[i];
			}
			if (colores[1] > imgdata[i]){ //Hay un nuevo minimo
				colores[1]=imgdata[i];
			}
		}
		for (; i < WIDTH*HEIGHT*2; ++i){ //Para la matriz del color verde
			if (colores[2] < imgdata[i]){ //Hay un nuevo maximo
				colores[2]=imgdata[i];
			}
			if (colores[3] > imgdata[i]){ //Hay un nuevo minimo
				colores[3]=imgdata[i];
			}
		}
		for (; i < matrix_size; ++i){ //Para la matriz del color azul
			if (colores[4] < imgdata[i]){ //Hay un nuevo maximo
				colores[4]=imgdata[i];
			}
			if (colores[5] > imgdata[i]){ //Hay un nuevo minimo
				colores[5]=imgdata[i];
			}
		}
		ofstream pOutFile;
		pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);
		if(pOutFile) { 
			for (int i=0; i<6; i++){ //Se escriben los resultados en el fichero de salida
				pOutFile<<colores[i];
				if(i<5){
					pOutFile<<" ";
				}
			}
    		}else{
		cerr<< "Error al abrir el fichero: "<<OutputFile<<" para escritura"<<endl;
		}
		pOutFile.close(); 
	}else{
	 	cerr<<"Error al abrir el fichero "<<ImageFile<<endl;
	}
}
void aplicar_filtro(string ImageFile, string OutputFile, double r){
	ifstream InImagen;
	InImagen.open(ImageFile, ios::in | ios::binary);
	if (InImagen.is_open()){
		streampos fileSize= matrix_size+8;
		vector<unsigned char> imgdata(fileSize); //Vector para volcar la matriz recibida
		InImagen.read((char*) &imgdata[0], fileSize); //Se lee toda la imagen
		InImagen.close();
		struct punto{
			double x;
			double y;
		};
		struct punto centro, p;
		centro.x=WIDTH/2; //Se calcula el centro de la imagen
		centro.y=HEIGHT/2;
		int i, j;
		for(j =0; j<HEIGHT; ++j){
			for(i=0; i<WIDTH; ++i){
				p.x= i-centro.x;
				p.y= j-centro.y;
				if (p.x * p.x + p.y * p.y > r*r){ //Si esta fuera del radio, se cambia de color
					imgdata[i+j*WIDTH+8] = floor(imgdata[i+j*WIDTH+8]*0.3);	
					imgdata[i+WIDTH*(j+HEIGHT)+8] = floor(imgdata[i+WIDTH*(j+HEIGHT)+8]*0.59);
					imgdata[i+WIDTH*(j+HEIGHT*2)+8] = floor(imgdata[i+WIDTH*(j+HEIGHT*2)+8]*0.11);
				}
			}
		}
		ofstream OutFile;
		OutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);
		if(OutFile.is_open()) {
			OutFile.write((char*)& imgdata[0], 1); //Se escribe la matriz resultante en el fichero de salida
			OutFile.close();
		}
		else{
			cout << "Error al abrir el fichero "<<OutputFile<<" para escribir"<<endl;
		}
	}
	else{
		cerr<<"Error al abrir el fichero "<<ImageFile<<endl;
	}
}
