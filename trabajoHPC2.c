#include <iostream>
#include <time.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <stdlib.h>  
#include <stdio.h>
#include <fstream>
#include "timer.hh"
//Para convertir el texto a numero en excel se usa: =SUSTITUIR(SUSTITUIR(SUSTITUIR(B3;"'";;1);"'";;1);".";",")

int RANGO=20;
int N=1000;
int NProce=1;
int NIntentos=10;
using namespace std;

int main(int argc, char const *argv[])
{
	N=atoi(argv[1]);
	NProce=atoi(argv[2]);
	NIntentos=atoi(argv[3]);
	
	ofstream file;//Guardar info en archivo
	srand((unsigned)time(NULL));
	int **P_Matriz1;
	P_Matriz1=new int*[N];
	int **P_Matriz2;
	P_Matriz2=new int*[N];
	int *P_MatrizR[N];
	int *f;
	for(int i=0;i<N;i++)
	{
		f = (int*)mmap(NULL, N*sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, 0, 0 );
		P_MatrizR[i]=f;
	}
	for(int i=0;i<N;i++)
	{
		P_Matriz1[i]=new int[N];
		P_Matriz2[i]=new int[N];
	}
	file.open("Resultados.txt",ios::app);
	file << "'Resultados para N=" << N << " y P=" << NProce <<"'\n";
	cout << endl;
	for(int Inte=0;Inte<NIntentos;Inte++)
	{
		pid_t Hijos[N];
		int ColInicio, ColFinal, ColxProce,ColActual=0;
		ColxProce=N/NProce;
		
		for(int x=0;x<N;x++)//Llenado de matrices
			for(int y=0;y<N;y++)
			{
				P_Matriz1[x][y]=(rand()%(RANGO+1))-(RANGO/2);
				P_Matriz2[x][y]=(rand()%(RANGO+1))-(RANGO/2);
				P_MatrizR[x][y]=0;
			}
		Timer t;
		for(int ProActual=0;ProActual<NProce;ProActual++)	
		{
			ColInicio=ColActual;
			ColFinal=ColActual+ColxProce;
			Hijos[ProActual]=fork();
			if (Hijos[ProActual] == 0)
				{
				for(int a=ColInicio;a<ColFinal;a++){
	        		for(int i=0;i<N;i++) {
	            		int suma=0;
	            		for (int j=0;j<N;j++) 
	         		       	suma+=P_Matriz1[i][j]*P_Matriz2[j][a];
	            	P_MatrizR[i][a]=suma;}
	            }
				exit(0);
				}
			else
				{ColActual+=ColxProce;}}
	for(int ProActual=0;ProActual<NProce;ProActual++)
		wait(NULL);
	double ms = t.elapsed();
	double seg=ms/1000;
	cout << "Tamano Matriz: " << N << "x" << N << " tardo: " << seg << " segundos con "<< NProce << " procesos" << endl;
	file << "'Intento #" << Inte+1 << "=';'" << seg <<"';\n";
	}
	time_t tiempo = time(0);
    struct tm *tlocal = localtime(&tiempo);
    char output[128];
    strftime(output,128,"%d/%m/%y %H:%M:%S",tlocal);
    printf("%s\n",output);
    file << "Hora :" << output <<endl;
    file.close();
	return 0;
}