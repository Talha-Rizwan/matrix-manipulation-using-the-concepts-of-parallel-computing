#include<iostream>
#include<time.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include<chrono>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
using namespace std::chrono;
using namespace std;


int n;
float ** mat1;
float ** mat2;
float ** res;
float ** res2;
float ** res3;
int no_threads;
int per_thread;
int thread_no=1;


void multiply1(int i){


	for(int j=0;j<n;j++){		
		for(int k=0;k<n;k++){		
		res3[i][j]=res3[i][j]+mat1[i][k]*mat2[k][j];		
		}
	}	
}



void multiply(int i){

	for(int j=0;j<n;j++){
		for(int k=0;k<n;k++){		
		res2[i][j]=res2[i][j]+mat1[i][k]*mat2[k][j];		
		}
	}
}


//for part 2 (block wise)
void* multi1(void* arg)
{


int start = *((int*) arg);
int check=0;

int a=per_thread+n%no_threads;

if(start==0){

for(int i=start;i<a;i++){
multiply1(i);
}

}
else{
	while(check<per_thread){	
	multiply1(start);
	start++;
	check++;
	}
}

pthread_exit(NULL);

}



//for part 2 (cyclic)

void* multi(void* arg)
{

int b = *((int*) arg);



for(int i=b;i<n;i+=2){
multiply(i);
}

pthread_exit(NULL);
}




int main(int argc, char **argv){
// part 1 naive way multiplication

cout<<"enter the number of threads you want to run for the second part of the program"<<endl;
cin>>no_threads;

pthread_t tid[no_threads];


n=1024;
mat1=new float*[n];
mat2=new float*[n];
res=new float*[n];
res2=new float*[n];
res3=new float*[n];


float aa=1;

for(int i=0;i<n;i++){
mat1[i]=new float[n];
mat2[i]=new float[n];
res[i]=new float[n];

	for(int j=0;j<n;j++){
	mat1[i][j]=aa;
	mat2[i][j]=aa;
	res[i][j]=0;
	aa++;
	}

}
cout<<"the matrix 1 is :"<<endl;
for(int i=0;i<n;i++){
	for (int j=0;j<n;j++){
	cout<<mat1[i][j]<<" ";
	}
	cout<<endl;
}


cout<<"the matrix 2 is :"<<endl;
for(int i=0;i<n;i++){
	for (int j=0;j<n;j++){
	cout<<mat2[i][j]<<" ";
	}
	cout<<endl;
}





auto start1 = std::chrono::system_clock::now();

for(int i=0;i<n;i++){
	for(int j=0;j<n;j++){
		
		for(int k=0;k<n;k++){
		
		res[i][j]=res[i][j]+mat1[i][k]*mat2[k][j];
		
		}
	}
}
auto stop1 = high_resolution_clock::now();
auto duration = duration_cast<microseconds>(stop1 - start1);
cout << "Time taken for the naive based serial multiplication: "<< duration.count()/1000000.0 << " seconds" << endl<<endl;




cout<<endl<<endl;
//cout<<"the final result is of part 1 is :"<<endl;
//for(int i=0;i<n;i++){
//	for (int j=0;j<n;j++){
//	cout<<res[i][j]<<" ";
//	}
//	cout<<endl;
//}


//cout<<endl<<endl;




for(int i=0;i<n;i++){
res2[i]=new float[n];

	for(int j=0;j<n;j++){
	res2[i][j]=0;
	}
}





//part 2 with cyclic answer

start1 = std::chrono::system_clock::now();

for(int i=0;i<no_threads;i++){

int * st=new int(i);
pthread_create(&tid[i],NULL, multi, (void*)st);
}


for(int i=0;i<no_threads;i++)
pthread_join(tid[i],NULL);

stop1 = high_resolution_clock::now();
duration = duration_cast<microseconds>(stop1 - start1);
cout << "Time taken for the thread cyclic multiplication: "<< duration.count()/1000000.0 << " seconds" << endl<<endl;


//cout<<"the final result of part 2 (b) is :"<<endl;
//for(int i=0;i<n;i++){
//	for (int j=0;j<n;j++){
//	cout<<res2[i][j]<<" ";
//	}
//	cout<<endl;
//}

//cout<<endl<<endl;



// starting part 2 (a)


for(int i=0;i<n;i++){

res3[i]=new float[n];

	for(int j=0;j<n;j++){
	res3[i][j]=0;
	}
}


per_thread=n/no_threads;

start1 = std::chrono::system_clock::now();

int start=0;
for(int i=0;i<no_threads;i++){

int * st=new int(start);


pthread_create(&tid[i],NULL, multi1, (void*)st);


if(i==0){
start+=per_thread+n%no_threads;

}
else{

start+=per_thread;

}

}


for(int i=0;i<no_threads;i++)
pthread_join(tid[i],NULL);


stop1 = high_resolution_clock::now();
duration = duration_cast<microseconds>(stop1 - start1);
cout << "Time taken for the thread block wise multiplication: "<< duration.count()/1000000.0 << " seconds" << endl<<endl;


//cout<<"the final result is of part 2 (a) is :"<<endl;
//for(int i=0;i<n;i++){
//	for (int j=0;j<n;j++){
//	cout<<res3[i][j]<<" ";
//	}
//	cout<<endl;
//}


// now comparing results of part 1 and part 2(a)

bool flag=0;

for(int i=0;i<n;i++){
	for (int j=0;j<n;j++){
	if(res3[i][j]!=res[i][j]){
	flag=1;
	break;
	}
	}
}

if(flag==0){
cout<<"the values in res and res3 matrices are exactly same"<<endl;

}
else{

cout<<"the values are not same in res and res3 matrices "<<endl;

}



// now comparing results of part 1 and part 2(b)
flag=0;

for(int i=0;i<n;i++){
	for (int j=0;j<n;j++){
	if(res2[i][j]!=res[i][j]){
	flag=1;
	break;
	}
	}
}

if(flag==0){
cout<<"the values in res and res2 matrices are exactly same"<<endl;

}
else{

cout<<"the values are exactly same in res and res2 matrices "<<endl;

}








pthread_exit(NULL);








return 0;
}
