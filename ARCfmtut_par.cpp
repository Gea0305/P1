/* 
Practica 1 Arquitectura de Computadores
Autores:
-Gonzalo Lencina
-Alejandro Blanco
-Alfonso Martinez
-Jorge Navarro
*/
#include <string>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <vector>
#include <algorithm>
#include <cstring>
#include <math.h>
#include <omp.h>
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
int matrix_size; //Variable que almacena el tamaño de la imagen, sin contar con la cabecera

int main(int argc, char ** argv){
	int cont=1;
	int num_funcion=-1;
	int num_histograma=-1;
	double num_angulo=-1;
	double num_radio=-1;
	string in_file="";
	string out_file="";
	string path_mascara="";    
	while ((cont < argc) && (argv[cont][0]=='-')) { //Bucle para almacenar los distintos argumentos, que pueden llegar en cualquier orden
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
            num_angulo = atof(argv[cont]);
        }
        else if (sw=="-r") {
            cont++;
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
        cont++;
    }
    //Comrpobaciones que son comunes para todas las funciones
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
		case(1):MaxMin(in_file, out_file);
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
		#pragma omp parallel shared(t, tram, histogram, imgdata, HEIGHT, WIDTH) private(i, j, found, grey)
		{
			#pragma omp for schedule(guided)
			for (i=0; i<(HEIGHT*WIDTH); ++i){ //Bucle para calcular el gris resultante de cada pixel
				grey=(imgdata[i]*0.3+imgdata[i+HEIGHT*WIDTH]*0.59+imgdata[i+HEIGHT*WIDTH*2]*0.11);
				for (j=0, found=false; j<t && !found; ++j){ //Bucle para encontrar el tramo adecuado en el histograma
					if (grey<((j+1)*tram)){ //Comprobamos desde el primer tramo hasta que entre en uno
						#pragma omp atomic
						histogram[j]+=1;
						found=true; //Cuando encontramos su tramo no volvemos a ejecutar el for
					}
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
	InImagen.open(ImageFile, ios::in | ios::binary); // open fileName and read as binary.
	ifstream InMascara;
	InMascara.open(MaskFile, ios::in | ios::binary); 
	if (InImagen.is_open()) {
        if (InMascara.is_open()) {
			streampos fileSize= matrix_size+8;
			vector<unsigned char> imgdata(fileSize); //Vector para volcar la matriz recibida
			InImagen.read((char*) &imgdata[0], fileSize);
			InImagen.close();
			vector<unsigned char> mskdata(fileSize); //Vector para volcar la mascara recibida
			InMascara.read((char*) &mskdata[0], fileSize);
			InMascara.close();
			#pragma omp parallel  shared(imgdata,mskdata,matrix_size) 
			{
				#pragma omp for schedule (static)
				for(int i=8; i<(fileSize); ++i){
					imgdata[i]*=mskdata[i];
				}
			}
          		ofstream pOutFile; //Creamos el ofstream, para escribir en el fichero de salida
			pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);	
	       		if(pOutFile.is_open()) {
	       		pOutFile.write((char*)& imgdata[0], fileSize);	//Se escribe la matriz resultante en el fichero de salida
			pOutFile.close();
   			}else{
    			cout << "Error al abrir el fichero "<<OutputFile<<" para escribir"<<endl;  
   			}
		}else{
			cerr<<"Error opening "<<MaskFile<<endl;
		}
	}else{
		cerr<<"Error opening "<<ImageFile<<endl;
	}
}

void rotacion(string ImageFile, string OutputFile, double gr){
	ifstream InImagen;
	InImagen.open(ImageFile, ios::in | ios::binary);
	if (InImagen.is_open()) {
		ofstream pOutFile;
		pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);
		if(pOutFile.is_open()){
		//Se lee y escribe la cabecera
			unsigned char cabecera[8];
			InImagen.read((char*)& cabecera[0], 8);
			pOutFile.write((char *)& cabecera[0], 8);
			vector<unsigned char> imgdata(matrix_size); //Vector para volcar la matriz recibida
			InImagen.read((char*) &imgdata[0], matrix_size);
			InImagen.close();
			vector<unsigned char> fin(matrix_size);
			double xc,yc,xi,yi;
			int xf,yf;
			//Calculamos el centro de la imagen
			xc=WIDTH/2;
			yc=HEIGHT/2;
			int contador=0;
			int offset=0;
			for(int k=0; k<3; ++k){
				for (int j=0; j<HEIGHT; j++){
					for(int i=0; i<WIDTH; i++){
						xi=i-xc;
						yi=j-yc;
						xf= ceil( cos((gr*M_PI)/180)*xi - sin((gr*M_PI)/180)*yi +xc);
						yf= ceil( sin((gr*M_PI)/180)*xi + cos((gr*M_PI)/180)*yi +yc);
						if(yf<HEIGHT && yf>=0 && xf<WIDTH && xf>=0){
							fin[(yf*WIDTH + xf)+offset]= imgdata[contador];
						}
						++contador;
					}
				}
				offset+=HEIGHT*WIDTH;
			}
			pOutFile.write((char*)& fin[0], matrix_size);
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
		int colores[]= {0,255,0,255,0,255} ;
		vector<unsigned char> imgdata(matrix_size); //Vector para volcar la matriz recibida
		InFile.seekg(8); //No se almacenan los bytes que indican el tamaño de la matriz
		InFile.read((char*) &imgdata[0], matrix_size); //Se vuelva la imagen en el vector
		InFile.close();
		#pragma omp parallel  shared(imgdata,HEIGHT,WIDTH, colores)
		{
			#pragma omp for schedule (static)
			for (int i=0; i < WIDTH*HEIGHT ; ++i){ //Para el color rojo
				#pragma omp critical (max_red)
				if (colores[0] < imgdata[i]){ //Hay un nuevo maximo rojo
					colores[0]=imgdata[i];
				}
				#pragma omp critical (min_rojo)
				if (colores[1] > imgdata[i]){ //Hay un nuevo minimo rojo	
					colores[1]=imgdata[i];
				}
			}
			#pragma omp for schedule (static)
			for (int i= WIDTH*HEIGHT; i < WIDTH*HEIGHT*2; ++i){ //Para el color verde
				#pragma omp critical (max_green)
				if (colores[2] < imgdata[i]){ //Hay un nuevo maximo verde
					colores[2]=imgdata[i];
				}
				#pragma omp critical (min_green)
				if (colores[3] > imgdata[i]){ //Hay un nuevo minimo verde
					colores[3]=imgdata[i];
				}
			}
			#pragma omp for schedule (static)
			for (int i=WIDTH*HEIGHT*2; i < WIDTH*HEIGHT*3; ++i){ //Para el color azul
				#pragma omp critical (max_blue)
				if (colores[4] < imgdata[i]){ //Hay un nuevo maximo azul
					colores[4]=imgdata[i];
				}
				#pragma omp critical (min_blue)
				if (colores[5] > imgdata[i]){ //Hay un nuevo minimo azul
					colores[5]=imgdata[i];
				}
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
		InImagen.read((char*) &imgdata[0], fileSize);
		InImagen.close();
		struct punto{
			double x;
			double y;
		};
		struct punto centro, p;
		centro.x=WIDTH/2;
		centro.y=HEIGHT/2;
		int i, j;
		#pragma omp parallel shared(imgdata,HEIGHT,WIDTH, centro) private(i, j , p)
		{
			#pragma omp for schedule(guided) collapse(2)
			for(j =0; j<HEIGHT; ++j){
				for(i=0; i<WIDTH; ++i){
					p.x= i-centro.x;
					p.y= j-centro.y;
					if (p.x * p.x + p.y * p.y > r*r){ //Si esta fuera del radio, lo cambio de color
						imgdata[i+j*WIDTH+8] = floor(imgdata[i+j*WIDTH+8]*0.3);	
						imgdata[i+WIDTH*(j+HEIGHT)+8] = floor(imgdata[i+WIDTH*(j+HEIGHT)+8]*0.59);
						imgdata[i+WIDTH*(j+HEIGHT*2)+8] = floor(imgdata[i+WIDTH*(j+HEIGHT*2)+8]*0.11);
					}
				}
			}
		}
		ofstream OutFile;
		OutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);
		if(OutFile.is_open()) {
			OutFile.write((char*)& imgdata[0], fileSize);
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
