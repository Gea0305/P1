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
		unsigned char imgdata;
		double red, green, grey, i, j;
		bool found;
		vector<int> histogram(t, 0); //Vector con los tramos inicializado a 0
		for (i=0; i<(HEIGHT*WIDTH); ++i){ //Bucle para coger los tres colores de cada pixel
			//Pasamos hasta el byte rojo correspondiente al pixel actual
			InFile.seekg(8+i, ios::beg);
			InFile.read((char*)& imgdata,1);
			red=imgdata;
			InFile.seekg(HEIGHT*WIDTH-1, ios::cur); //Avanzamos a su correspondiente valor en los verdes
			InFile.read((char*)& imgdata,1);
			green=imgdata;
			InFile.seekg(HEIGHT*WIDTH-1, ios::cur); //Avanzamos a su correspondiente valor en los azules
			InFile.read((char*)& imgdata,1);
			grey=(red*0.3+green*0.59+imgdata*0.11);
			for (j=0, found=false; j<t && !found; ++j){ //Bucle para encontrar el tramo adecuado en el histograma
				if (grey<((j+1)*(255.0/t))){ //Comprobamos los tramos desde el primero hasta que entre en uno
					histogram[j]+=1;
					found=true; //Cuando encontramos su tramo no volvemos a ejecutar el for
				}
			}
		}
		ofstream OutFile;
		OutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);
		if (OutFile.is_open()){ 
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

void rotacion(string ImageFile, string OutputFile, double gr){
	ifstream InFile;
	InFile.open(ImageFile, ios::in | ios::binary);
	if (InFile.is_open()) {
		//Creamos el ofstream, para escribir en el fichero de salida
       	ofstream pOutFile;
		pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);	
       	if(!pOutFile) { 
    		cout << "Error al abrir el fichero "<<OutputFile<<" para escribir"<<endl;  
   		} 
   		cout<<"Escribo la cabecera en el archivo de rotacion"<<endl;
		unsigned char imgdata; 
		//Escribimos en el rot_out, los primeros 8 bytes de su tamaÃ±o, que sera el tamaÃ±o de la imagen de entrada
	   	unsigned char heightData[4]; 
		unsigned char widthData[4]; 
		InFile.seekg(0, ios::beg); //pos filter at beginning of image file.
		//Leo los 4 primeros bytes
		InFile.read( (char *)& heightData, 4 ); //Lees la altura
		InFile.read( (char *)& widthData, 4); 
		//Escribo en el fichero de salida, los tamaÃ±os de la matriz
		pOutFile.write( (char *)& heightData, 4);
		pOutFile.write( (char *)& widthData, 4);
		cout<< "He escrito la cabecera y voy con lo siguiente"<<endl;
		double xc , yc , xi,yi;
		int xf,yf;
		vector<unsigned char> fin(WIDTH*HEIGHT, 0); //Vector con los tramos inicializado a 0
		//Calculamos el centro de la imagen
		cout<<"Voy a calcular el centro"<<endl;
		xc=WIDTH/2;
		yc=HEIGHT/2;
		cout<<"El centro es xc:"<<xc<<" yc:"<<yc<<endl;
		int num_colores= 0;
		int i,j;
		int contador=0;
		while(num_colores<3){
			for (j=0; j<HEIGHT; j++){
				for(i=0; i<WIDTH; i++){

					for (; contador<WIDTH*HEIGHT; contador++){
				
					xi=i-xc;
					yi=j-yc;
					xf= ceil( cos((gr*M_PI)/180)*xi - sin((gr*M_PI)/180)*yi +xc);
					yf= ceil( sin((gr*M_PI)/180)*xi + cos((gr*M_PI)/180)*yi +yc);
					InFile.read( (char *)& imgdata,1);
					if(yf<HEIGHT && yf>=0 && xf<WIDTH && xf>=0){
						fin[contador]= imgdata;
					}
					
				}
				}
			}
			
			
			for (contador=0; contador<WIDTH*HEIGHT; contador++){
					
					pOutFile.write( (char *)& fin[contador], 1);
				
			}
			num_colores++;
			memset(&fin[0], 0, fin.size() * sizeof fin[0]);
		}
	}else{
		cerr<<"Error al abrir "<<ImageFile<<endl;
		return;	
	} 
}

void MaxMin(string ImageFile, string OutputFile){
	ifstream InFile;
	InFile.open(ImageFile, ios::in | ios::binary);
 	if (InFile.is_open()) {
		//leer_dimenciones(img);
		unsigned char imgdata; 
		int colores[]= {0,255,0,255,0,255} ;
		int red;
		int blue;
		int green;
		//No leemos los bytes que indican el tamaÃ±o de la matriz
		InFile.seekg(8); 
		for (int i =0; i < (HEIGHT*WIDTH) ; i++){
			InFile.read( (char *)& imgdata,1);
			red=imgdata;
			//Hay un nuevo maximo
			if (colores[0] < red){
				colores[0]=red;
			}
			//Hay un nuevo minio
			if (colores[1] > red){
				colores[1]=red;
			}
		}
		for (int i =0; i< (HEIGHT*WIDTH); i++){
			InFile.read( (char *)& imgdata,1);
			green=imgdata;
			if (colores[2] < green){
				colores[2]=green;
			}
			if (colores[3] > green){
				colores[3]=green;
			}
		}
		for (int i =0; i<(HEIGHT*WIDTH); i++){
			InFile.read( (char *)& imgdata,1);
			blue=imgdata;
			if (colores[4] < blue){
				colores[4]=blue;
			}
			if (colores[5] > blue){
				colores[5]=blue;
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
   		pOutFile.close(); 
	}else{
	 	cerr<<"Error al abrir el fichero "<<ImageFile<<endl;
	}
}

void aplicar_filtro(string ImageFile, string OutputFile, double r){ 
   ifstream pInImagen;
    pInImagen.open(ImageFile, ios::in | ios::binary);
    if (pInImagen.is_open()){
		ofstream pOutFile;
		pOutFile.open(OutputFile, ios::out | ios::trunc | ios::binary);
		if(!pOutFile) {
			cout << "Cannot open file"<<endl;
		}
		//Escribimos en el circle_out, los primeros 8 bytes de su tamaÃ±o, que sera el tamaÃ±o de la imagen de entrada
		unsigned char heightData[4];
		unsigned char widthData[4];
		pInImagen.seekg(0, ios::beg); 
		//Leo los 4 primeros bytes
		pInImagen.read( (char *)& heightData, 4 ); //Lees la altura
		pInImagen.read( (char *)& widthData, 4);
		//Escribo en el fichero de salida, los tamaÃ±os de la matriz
		pOutFile.write( (char *)& heightData, 4);
		pOutFile.write( (char *)& widthData, 4);
        pInImagen.seekg(8);
        struct punto{
            double x;
            double y;
        };
        struct punto centro, p;
		unsigned char ImageData;
	    centro.x=WIDTH/2;
    	centro.y=HEIGHT/2;
        //Para el color rojo
        for(int j =0; j<HEIGHT; j++){
        	for(int i=0; i<WIDTH; i++){
				p.x= i-centro.x;
				p.y= j-centro.y;
				//Si esta fuera del radio, lo cambio de color
               	if (p.x * p.x + p.y * p.y > r*r){
                	pInImagen.read((char *)& ImageData,1);
                	ImageData = floor(ImageData * 0.3);
                   	pOutFile.write((char *) &ImageData, 1); 
	            }else{ //Si esta dentro del radio, escribo el mismo dato sin modificar
           			pInImagen.read((char *)& ImageData,1);
					pOutFile.write((char *) &ImageData, 1);
				}
			}
        }
        //Para el color verde
        for(int j =0; j<HEIGHT; j++){
        	for(int i=0; i<WIDTH; i++){
        		p.x= i-centro.x;
               	p.y= j-centro.y;
        		//Si esta fuera del radio, lo cambio de color
               	if (p.x * p.x + p.y * p.y > r*r){
                	pInImagen.read((char *)& ImageData,1);
                   	ImageData = floor(ImageData * 0.59);
                   	pOutFile.write((char *) &ImageData, 1); 
	            }else{ //Si esta dentro del radio, escribo el mismo dato sin modificar
					pInImagen.read((char *)& ImageData,1);
					pOutFile.write((char *) &ImageData, 1);
				}
			}
        }
        //Para el azul
        for(int j =0; j<HEIGHT; j++){
        	for(int i=0; i<WIDTH; i++){
         		p.x= i-centro.x;
           		p.y= j-centro.y;
         		//Si esta fuera del radio, lo cambio de color
               	if (p.x * p.x + p.y * p.y > r*r){
                	pInImagen.read((char *)& ImageData,1);
                   	ImageData = floor(ImageData * 0.11);
                   	pOutFile.write((char *) &ImageData, 1); 
	            }else{ //Si esta dentro del radio, esbrio el mismo dato sin modificar
            		pInImagen.read((char *)& ImageData,1);
					pOutFile.write((char *) &ImageData, 1);
				}
			}
		}
		pInImagen.close();
		pOutFile.close();
	}
	else{
		cerr<<"Error al abrir el fichero "<<ImageFile<<endl;  
	}
}
