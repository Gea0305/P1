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
void leer_datos(string, unsigned char m[] );
void escribir_datos(string, unsigned char m[] );
void aplicar_mascara(string, string, string);
void MaxMin(unsigned char m[], string);
void rotacion(unsigned char m[], double);	
void aplicar_filtro(unsigned char m[], double);
int HEIGHT;
int WIDTH;
int matrix_size; //TamaÃ±o de la matriz sin contar con los bytes que indican el tamaÃ±o
//argc= numero de argumentos especificados por linea de comandos
//argv[i] contiene el contenido del argumento i

int main(int argc, char ** argv){
	
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
	unsigned char imagen[matrix_size+8];
	leer_datos(in_file, imagen);
	
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
			MaxMin(imagen, out_file);
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
			escribir_datos(out_file, imagen);
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
			rotacion(imagen, num_angulo);
			escribir_datos(out_file, imagen);
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
			aplicar_filtro(imagen, num_radio);
			escribir_datos(out_file, imagen);
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
	 	cout<<"HEIGHT:  "<<HEIGHT<<endl;
		InFile.read( (char *)& widthData, 4); 
		WIDTH += (int)widthData[0] | ((int)widthData[1]<<8) | ((int)widthData[2]<<16) | ((int)widthData[3]<<24);
	  	cout <<"WIDTH: "<<WIDTH<<endl;
		matrix_size=  (HEIGHT*WIDTH)*3;
		cout<<"Matrix size: "<< matrix_size<<endl;
	}else{
		cerr <<"Error opening file";
	} 
}
void  leer_datos(string fileName, unsigned char imagen[]){
	ifstream InFile;
	InFile.open(fileName, ios::in | ios::binary); // open fileName and read as binary.
    if (InFile.is_open()) {
		for(int i=0; i< matrix_size+8; i++){
			InFile.read( (char *)& imagen[i], 1); 
		}
	}else{
		cerr <<"Error opening file";
	} 
}
void  escribir_datos(string OutputFile, unsigned char imagen[]){
		//Creamos el ofstream, para escribir en el fichero de salida
       	ofstream pOutFile;
		pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);	
       	if(!pOutFile) { 
    		cout << "Error al abrir el fichero "<<OutputFile<<" para escribir"<<endl;  
   		} 
   		for(int i=0; i<matrix_size+8; i++){
   			pOutFile.write( (char *) &imagen[i], 1);
		}
}

void histograma(string ImageFile, string OutputFile, int t){
	/*
	ifstream InFile;
	InFile.open(ImageFile, ios::in | ios::binary);
	if (InFile.is_open()){
		unsigned char imgdata;
		int red;
		int green;
		double grey;
		vector<int> histograma(t, 0); //Vector con los tramos inicializado a 0
		for (int i=0; i<(HEIGHT*WIDTH); ++i){ //Bucle para coger los tres colores de cada pixel
			//Pasamos hasta el byte rojo correspondiente al pixel actual
			InFile.seekg(8+i, ios::beg);
			InFile.read((char*)& imgdata,1);
			red=imgdata;
			InFile.seekg(HEIGHT*WIDTH-1); //Avanzamos a su correspondiente valor en los verdes
			InFile.read((char*)& imgdata,1);
			green=imgdata;
			InFile.seekg(HEIGHT*WIDTH-1); //Avanzamos a su correspondiente valor en los azules
			InFile.read((char*)& imgdata,1);
			grey=(red*0.3+green*0.59+imgdata*0.11);
			for (double j=(255/t), bool found=false; j<=255 && !found; j+=j){ //Bucle para encontrar el tramo adecuado en el histograma
				if (grey<=j){ //Si encontramos su tramo no volvemos a ejecutar el for
					histograma[j*t/255-1]+=1;
					found=true;
				}
			}
		}
		ofstream OutFile;
		OutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);
		if (OutFile.is_open()){ 
			for (int i=0; i<t; ++i){
				OutFile<<histograma[i];
			}
			OutFile.close();
		}else{
			cerr<<"No se puede abrir el fichero "<<OutputFile<<" para escribir"<<endl; 
		}
	}else{
		cerr<<"Error al abrir el fichero "<<ImageFile<<endl;
	} */
}

void aplicar_mascara(string ImageFile, string MaskFile, string OutputFile){
	ifstream InImagen;
	InImagen.open(ImageFile, ios::in | ios::binary); // open fileName and read as binary.
	ifstream InMascara;
	InMascara.open(MaskFile, ios::in | ios::binary); 
	if (InImagen.is_open()) {
        if (InMascara.is_open()) {
          	//Creamos el ofstream, para escribir en el fichero de salida
         	ofstream pOutFile;
			pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);	
	       	if(!pOutFile) { 
    			cout << "Error al abrir el fichero "<<OutputFile<<" para escribir"<<endl;  
   			} 
     		//Escribimos en el mask_out, los primeros 8 bytes de su tamaÃ±o, que sera el tamaÃ±o de la imagen de entrada
	   		unsigned char heightData[4]; 
			unsigned char widthData[4]; 
	   		InImagen.seekg(0, ios::beg); //pos filter at beginning of image file.
			//Leo los 4 primeros bytes
			InImagen.read( (char *)& heightData, 4 ); //Lees la altura
		 	InImagen.read( (char *)& widthData, 4); 
		 	//Escribo en el fichero de salida, los tamaÃ±os de la matriz
		 	pOutFile.write( (char *)& heightData, 4);
		 	pOutFile.write( (char *)& widthData, 4);
		    //Donde almacenamos los datos tienen que ser unsigned char, ya que van desde 0 a 255    
			unsigned char ImageData; 
			unsigned char MaskData; 
			int contador=0;
			//Ponemos el puntero para que emIece a leer en el byte 9, ya que los 8 primeros son del tamaÃ±o de la matriz
			InMascara.seekg(8); 
			//Ponemos contador<matrix_size, ya que con !ImageData.eof, debe ser que lee tambien el /0 del final, y escribe una posicion mas de la debida
			while(contador<matrix_size){
				InImagen.read( (char *)& ImageData, 1 ); 
				InMascara.read( (char *)& MaskData, 1 );	
				ImageData= ImageData*MaskData;
				pOutFile.write( (char *) &ImageData, 1);
				contador++;
			}
			InImagen.close();
			pOutFile.close();
			InMascara.close();
		}else{
			cerr<<"Error opening "<<MaskFile<<endl;
		}
	}else{
		cerr<<"Error opening "<<ImageFile<<endl;
	}
}

void rotacion( unsigned char imagen[] , double gr){
	
		
		int contador=8;
		double xc , yc , xi,yi;
		int xf,yf;
		unsigned char fin[WIDTH][HEIGHT]; //= {0};
		memset(fin, 0, WIDTH*HEIGHT);
		//Calculamos el centro de la imagen
		xc=WIDTH/2;
		yc=HEIGHT/2;
		cout<<"El centro es xc:"<<xc<<" yc:"<<yc<<endl;
		int num_colores= 0;

		//AQUI NO PARALELIZO
		while(num_colores<3){
			//ESTO LO PARALELIZO
			for (int j=0; j<HEIGHT; j++){
				for (int i=0; i<WIDTH; i++){
					xi=i-xc;
					yi=j-yc;
					xf= ceil( cos((gr*M_PI)/180)*xi - sin((gr*M_PI)/180)*yi +xc);
					yf= ceil( sin((gr*M_PI)/180)*xi + cos((gr*M_PI)/180)*yi +yc);
					
					if(yf<HEIGHT && yf>=0 && xf<WIDTH && xf>=0){
						fin[xf][yf]= imagen[contador];
					}
				}
				contador++;
			}
			for (int j=0; j<HEIGHT; j++){
				for (int i=0; i<WIDTH; i++){	
					pOutFile.write( (char *)& fin[i][j], 1);
				}
			}
			num_colores++;
			memset(fin, 0, WIDTH*HEIGHT);
		} 
	}


void MaxMin(unsigned char imagen[], string OutputFile){
	
		
		
		int colores[]= {0,255,0,255,0,255} ;
		int red;
		int blue;
		int green;

		//No leemos los bytes que indican el tamaÃ±o de la matriz
		int contador=8;
		for (; contador < (matrix_size/3) ; contador++){
		
			red=imagen[contador];
			//Hay un nuevo maximo
			if (colores[0] < red){
				colores[0]=red;
			}
			//Hay un nuevo minio
			if (colores[1] > red){
				colores[1]=red;
			}
		}
		cout<<"Empiezo con los verdes"<<endl;
		for (; contador< (matrix_size*2/3); contador++){
			

			green=imagen[contador];
			

			if (colores[2] < green){
				colores[2]=green;
				
			}
			if (colores[3] > green){
				colores[3]=green;
				
			}
		}
		for ( ; contador<(matrix_size); contador++){
			
			blue=imagen[contador];
			if (colores[4] < blue){
				colores[4]=blue;
			}
			if (colores[5] > blue){
				colores[5]=blue;
			}
		}
	
		//Esta region debe dejar de ser paralela
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
   		pOutFile.close(); 

	}


void aplicar_filtro( unsigned char imagen[], double r){ 
 
		//Empiezo a recorrer la imagen sin leer la cabecera
        int contador=8;
        struct punto{
            double x;
            double y;
        };
        struct punto centro, p;
	    centro.x=WIDTH/2;
    	centro.y=HEIGHT/2;
        //Para el color rojo
        for(int j =0; j<HEIGHT; j++){
        	for(int i=0; i<WIDTH; i++){
				p.x= i-centro.x;
				p.y= j-centro.y;
				//Si esta fuera del radio, lo cambio de color
               	if (p.x * p.x + p.y * p.y > r*r){
                	imagen[contador] = floor(imagen[contador] * 0.3);
                   	
	            }
	            contador++;
			}
        }
        //Para el color verde
        for(int j =0; j<HEIGHT; j++){
        	for(int i=0; i<WIDTH; i++){
        		p.x= i-centro.x;
               	p.y= j-centro.y;
        		//Si esta fuera del radio, lo cambio de color
               	if (p.x * p.x + p.y * p.y > r*r){
                	
                   imagen[contador] = floor(imagen[contador] * 0.59);
                   	
	            }
	            contador++;
			}
        }
        //Para el azul
        for(int j =0; j<HEIGHT; j++){
        	for(int i=0; i<WIDTH; i++){
         		p.x= i-centro.x;
           		p.y= j-centro.y;
         		//Si esta fuera del radio, lo cambio de color
               	if (p.x * p.x + p.y * p.y > r*r){
                	
                   imagen[contador] = floor(imagen[contador] * 0.11);
                
	            }
	            contador++;
			}
		}
		
		
	}
	
	

