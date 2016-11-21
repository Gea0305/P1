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
int matrix_size; //TamaÃ±o de la matriz sin contar con los bytes que indican el tamaÃ±o

//argc= numero de argumentos especificados por linea de comandos
//argv[i] contiene el contenido del argumento i
int main(int argc, char ** argv){
	cout<<"Hola"<<endl;
	int cont=1;
	int num_funcion=-1;
	int num_histograma=-1;
	double num_angulo=-1;
	double num_radio=-1;
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
    //COmrpobaciones que son comunes para todas las funciones
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
    		cout<<"Funcion histograma blanco y negro"<<endl;
			histograma(in_file, out_file, num_histograma);
    		break;
		case(1):
			cout<<"Funcion maximos y minimos"<<endl;
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
			cout<<"Funcion mascara"<<endl;
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
			cout<<"Rotacion de la imagen"<<endl;
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
			cout<<"Funcion de filtro blanco y negro selectivo"<<endl;
			aplicar_filtro(in_file, out_file, num_radio);
			break;
		default:
			cerr<<"Falta introducir -u numero_funcion"<<endl;
    }
	return 0;
}

void leer_dimensiones(string fileName){
	ifstream InFile;
	InFile.open(fileName, ios::in | ios::binary); // open fileName and read as binary.
    if (InFile.is_open()) {
		//Donde almacenamos los datos tienen que ser unsigned char, ya que van desde 0 a 255    
		unsigned char heightData[4]; 
		unsigned char widthData[4]; 
		InFile.seekg(0, ios::beg); //pos filter at beginning of image file. 
		//Leo los 4 primeros bytes
		InFile.read( (char *)& heightData, 4 ); //Lees la altura
		//Lee 2000 0000, como esta en LEndian seria 0000 0020, que equivale a 32.En las posiciones el 0 seria el 3, el 1 el 2 y el 3 el 0
		//Lo cambio a BIGENDIAN
		HEIGHT += (int)heightData[0] | ((int)heightData[1]<<8) | ((int)heightData[2]<<16) | ((int)heightData[3]<<24);
		InFile.read( (char *)& widthData, 4); 
		WIDTH += (int)widthData[0] | ((int)widthData[1]<<8) | ((int)widthData[2]<<16) | ((int)widthData[3]<<24);
		matrix_size=  (HEIGHT*WIDTH)*3;
		InFile.close();
	  	cout<<"Width: "<<WIDTH<<endl;
	 	cout<<"Height: "<<HEIGHT<<endl;
		cout<<"Matrix size: "<< matrix_size<<endl;
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
			//int chunk=2;
			int tid, nthreads;
			//dont need to define the number of threads the program will define them based on the number of cores on the machine
			#pragma omp parallel  shared(imgdata,mskdata,matrix_size,nthreads) private(tid){
				tid = omp_get_thread_num();
				if (tid == 0){
   					nthreads = omp_get_num_threads();
    				printf("Starting matrix multiple example with %d threads\n",nthreads);
				}
				#pragma omp for schedule (static)
				for(int i=8; i<(matrix_size+8); ++i){
					imgdata[i]*=mskdata[i];
				}
			}
          	//Creamos el ofstream, para escribir en el fichero de salida
         	ofstream pOutFile;
			pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);	
	       	if(pOutFile.is_open()) {
	       		for(int i=0; i<(matrix_size+8); ++i){
	       			pOutFile.write((char*)& imgdata[i], 1);
	       		}
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
		//leer_dimenciones(img); 
		int colores[]= {0,255,0,255,0,255} ;
		//No leemos los bytes que indican el tamaÃ±o de la matriz
		vector<unsigned char> imgdata(matrix_size); //Vector para volcar la matriz recibida
		InFile.seekg(8);
		InFile.read((char*) &imgdata[0], matrix_size);
		int tid, nthreads;
		#pragma omp parallel  shared(imgdata,HEIGHT,WIDTH, colores)
		{
			tid = omp_get_thread_num();
			if (tid == 0){
			 	nthreads = omp_get_num_threads();
			 	printf("Starting matrix multiple example with %d threads\n",nthreads);
			}
			#pragma omp for schedule (static)
			for (int i=0; i < WIDTH*HEIGHT ; ++i){
				//Hay un nuevo maximo
				#pragma omp critical (max_red)
				if (colores[0] < imgdata[i]){
					//cout<<"im thread "<<tid<<"new  MAX value of colores[0]: "<<imgdata[i]<<endl;
					/*Añadimos una seccion critica para evitar condiciones de carrera en la variable compartida colores.
					Ademas se da un nombre a cada seccion critica para que solo a afecte a las secciones con dicho nombre,
					si no lo nombraramos, un thread se quedaria esperando por cualquier seccion critica*/
					
					colores[0]=imgdata[i];
					}
				//Hay un nuevo minio	
				#pragma omp critical (min_rojo)
				if (colores[1] > imgdata[i]){
					//cout<<"im thread "<<tid<<"new  MIN value of colores[1]: "<<imgdata[i]<<endl;
				
					colores[1]=imgdata[i];
				}
			}
			#pragma omp for schedule (static)
			for (int i= WIDTH*HEIGHT; i < WIDTH*HEIGHT*2; ++i){
				#pragma omp critical (max_green)
				if (colores[2] < imgdata[i]){
					//cout<<"im thread "<<tid<<"new  MAX value of colores[2]: "<<imgdata[i]<<endl;
					
					colores[2]=imgdata[i];
				}
				#pragma omp critical (min_green)
				if (colores[3] > imgdata[i]){
					//cout<<"im thread "<<tid<<"new  MIN value of colores[3]: "<<imgdata[i]<<endl;
					
					colores[3]=imgdata[i];
				}
			}
			#pragma omp for schedule (static)
			for (int i=WIDTH*HEIGHT*2; i < WIDTH*HEIGHT*3; ++i){
				#pragma omp critical (max_blue)
				if (colores[4] < imgdata[i]){
					//cout<<"im thread "<<tid<<"new  MAX value of colores[4]: "<<imgdata[i]<<endl;
					
					colores[4]=imgdata[i];
				}
				#pragma omp critical (min_blue)
				if (colores[5] > imgdata[i]){
					//cout<<"im thread "<<tid<<"new  MIN value of colores[5]: "<<imgdata[i]<<endl;
					
					colores[5]=imgdata[i];
				}
			}
		}
		ofstream pOutFile;
    	pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);
    	if(!pOutFile) { 
    		cerr << "No se puede abrir el fichero "<<OutputFile<<"Para escribir"<<endl; 
   		} 
		for (int i=0; i<6; i++){
   			pOutFile<<colores[i]; 
   			if(i<5){
   				pOutFile<<" ";
			}
		}
		InFile.close();
   		pOutFile.close(); 
	}else{
	 	cerr<<"Error al abrir el fichero "<<ImageFile<<endl;
	}
}

void aplicar_filtro(string ImageFile, string OutputFile, double r){
	ifstream InImagen;
	InImagen.open(ImageFile, ios::in | ios::binary);
	if (InImagen.is_open()){
		vector<unsigned char> imgdata(matrix_size+8); //Vector para volcar la matriz recibida
		streampos fileSize= matrix_size+8;
		InImagen.read((char*) &imgdata[0], fileSize);
		InImagen.close();
		struct punto{
		double x;
		double y;
		};
		struct punto centro, p;
		int k=8;
		centro.x=WIDTH/2;
		centro.y=HEIGHT/2;
		//Para el color rojo
		for(int j =0; j<HEIGHT; ++j){
			for(int i=0; i<WIDTH; ++i){
				p.x= i-centro.x;
				p.y= j-centro.y;
				//Si esta fuera del radio, lo cambio de color
				if (p.x * p.x + p.y * p.y > r*r){
					imgdata[k] = floor(imgdata[k] * 0.3);	
				}
				++k;
			}
		}
		//Para el color verde
		for(int j =0; j<HEIGHT; ++j){
			for(int i=0; i<WIDTH; ++i){
				p.x= i-centro.x;
				p.y= j-centro.y;
				//Si esta fuera del radio, lo cambio de color
				if (p.x * p.x + p.y * p.y > r*r){
					imgdata[k] = floor(imgdata[k] * 0.59);
				}
				++k;
			}
		}
		//Para el azul
		for(int j =0; j<HEIGHT; ++j){
			for(int i=0; i<WIDTH; ++i){
				p.x= i-centro.x;
				p.y= j-centro.y;
				//Si esta fuera del radio, lo cambio de color
				if (p.x * p.x + p.y * p.y > r*r){
					imgdata[k] = floor(imgdata[k] * 0.11);
				}
				++k;
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
