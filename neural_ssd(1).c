#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define EPS 0.01
int no_hid_units = 5,train_size = 2215,c=0;
float *hidden_layer_1,output_layer[10],err = 0,delta2[10];
float **weights_1,**weights_2,**DEL_2,**DEL_1,lr = 0.08;
float y[10];
float input[17]={1,5,12,3,7,2,10,5,5,4,13,3,9,2,8,4,10};

int label[2215];
int trainTotal = 2215;
int testTotal=997;
int count = 17;
int ** readFromCSV(char filename[],int totalLines)
{
	int **ar = (int**)malloc(totalLines*sizeof(int*));
	FILE * F = fopen(filename,"r");
	int i = 0;
	char buf[1000];
	char *str;
	fgets(buf, sizeof(buf), F);
	//printf("%s\n",buf );
	//printf("test");
	while(fgets(buf, sizeof(buf), F))
	{

		
		str = strtok(buf," ");
		ar[i]=(int *)malloc(count*sizeof(int));
			
		int j = 0;
		while(str!=NULL)
		{	

			ar[i][j++]=atoi(str);
			if(j-1==0){
				label[i] = ar[i][j-1]; 
				ar[i][j-1]=1;
			}
			str = strtok(NULL," ");
		}
		i++;
	}
	fclose(F);
	return ar;
}
float sig_act(float x){
	return (float)((1.0)/(1+exp(-1*x)));
}

float sig_der(float x){
	//return (float)(sig_act(x)*(1-sig_act(x)));
	return (float)(x*(1-x));
}

float weig_rand(){
	return ((float)(rand()) / (float)(RAND_MAX))*(1) - 0.5 ;
	/*float k = (rand()%((long long)(EPS*1000000)))/1000000.0;
	if(rand()%2)
		return  k;
	else
		return -1*k;*/

}

void vectorise(float *a){
	float max = a[0];
	int i,max_ind = 0;
	for (i = 1; i < 10; ++i){
		if(a[i] > max){
			////printf("%f\n",max );
			max = a[i];
			max_ind = i;
		}
	}
	for (i = 0; i < 10; ++i){
		a[i] = 0;
		if( i == max_ind){
			a[i] = 1;
		}

	}
	
}

void matrix_mul(float a[],float **b,int ch){
	int i,j,k;
	if(ch == 0){
		for(i = 0;i <= no_hid_units;i++){
			hidden_layer_1[i]=0;
			for(j = 0;j <= 16 ;j++){
				hidden_layer_1[i] += a[j]*b[j][i];
			}
			hidden_layer_1[i] = sig_act(hidden_layer_1[i]);
			////printf("%f ",hidden_layer_1[i]);
		}
		hidden_layer_1[0] = 1;
	}
	else if(ch == 1){
		for(i = 0;i < 10;i++){
			output_layer[i]=0;
			for(j = 0;j < 6;j++){
				output_layer[i] += a[j]*b[j][i];
			}
			output_layer[i] = sig_act(output_layer[i]);
			////printf("%f ",output_layer[i]);
			
		}
		//vectorise(output_layer);
		//printf("\n");
		for (i = 0; i < 10; ++i){
			//printf("%f ",output_layer[i]);
		}//printf("\n");
	}
	else{

	}
	
	
	

	
}

void initialise_weights(int n){
	no_hid_units = n;
	weights_1 = (float**)malloc(17 * sizeof(float *));
	DEL_1 = (float**)malloc(17 * sizeof(float *));
	weights_2 = (float**)malloc((n+1) * sizeof(float *));
	DEL_2 = (float**)malloc((n+1) * sizeof(float *));
	hidden_layer_1 = (float*)malloc((n+1) * sizeof(float));

	int i,j;

	for(i = 0;i<=16;i++){
		weights_1[i] = (float*)malloc((n+1) * sizeof(float));
		DEL_1[i] = (float*)malloc((n+1) * sizeof(float));
		for (j = 0; j <=n; ++j){
			weights_1[i][j] = weig_rand();
			DEL_1[i][j] = 0;
			//printf("%f ",weights_1[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");
	for (i = 0; i <=n; ++i){
		weights_2[i] = (float*)malloc(10 * sizeof(float));
		DEL_2[i] = (float*)malloc(10 * sizeof(float));
		for (j = 0; j < 10; ++j){
			weights_2[i][j] = weig_rand();
			DEL_2[i][j] = 0;
			//printf("%f ",weights_2[i][j]);
		}
		//printf("\n");
	}

	for (i = 0; i <no_hid_units; ++i)	{
		delta2[i]=0;
	}

}

void one_hot_encoding(int label){
	int i;
	for (i = 0; i <10 ; ++i){
		y[i] = 0;}
	y[label - 1] = 1;
}
void back_prop(float *error){
	int i,j;
	float layer[17];
	for (i = 0; i <=no_hid_units; ++i){
		for (j = 0; j < 10; ++j){
			DEL_2[i][j] += hidden_layer_1[i]*error[j];
			//weights_2[i][j] += lr*DEL_2[i][j];
			//printf("%f ",weights_2[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");
	for(i = 0;i<=16;i++){
		layer[i] = sig_act(input[i]);
		for (j = 0; j <=no_hid_units; ++j){
			DEL_1[i][j] += layer[i]*delta2[j];
			//weights_1[i][j] += lr*DEL_1[i][j];
			//printf("%f ",weights_1[i][j]);
		}
		//printf("\n");
	}
}
void calc_error(float *pred,int out,int train){
	one_hot_encoding(out);
	int i,j;
	float error[10];
	for(i =0;i<10;i++){
		error[i] = pred[i] - y[i];
		//error[i]*= sig_der(pred[i]);
	}
	for(i =0;i<10;i++){
		err += error[i]*error[i];
	}

	err = 0.5*err;
	for(i =0;i<=no_hid_units;i++){
		delta2[i] = 0;
		for(j=0;j<10;j++){
			delta2[i] += weights_2[i][j]*error[j];
		}
		////printf("%f\n",delta2[i] );
		//delta2[i]*=sig_der(hidden_layer_1[i]);
		////printf("%f\n",delta2[i] );
	}
	//activation*deltanext
	if(train == 1){
	back_prop(error);
	if(c == trainTotal){
		//printf("check\n");
	for (i = 0; i <=no_hid_units; ++i){
		for (j = 0; j < 10; ++j){
			//printf("%f ",weights_2[i][j]);
			weights_2[i][j] -= lr*DEL_2[i][j];
			//printf("%f \n",weights_2[i][j]);
		}
		//printf("\n");
	}
	//printf("\n");
	for(i = 0;i<=16;i++){
		//layer[i] = sig_act(input[i]);
		for (j = 0; j <=no_hid_units; ++j){
			weights_1[i][j] -= lr*DEL_1[i][j];
			////printf("%f ",weights_1[i][j]);
		}
		//printf("\n");
	}
	
	}
	}
}

void run_model(){

}
void innit_delta_zero(){
	int i,j,n = no_hid_units ;

	for(i = 0;i<=16;i++){
		for (j = 0; j <=n; ++j){
			DEL_1[i][j] = 0;
		}
	}
	for (i = 0; i <=n; ++i){
		for (j = 0; j < 10; ++j){
			DEL_2[i][j] = 0;
		}
	}
	for (i = 0; i <=no_hid_units; ++i)	{
		delta2[i]=0;
	}
}

int max_index(float *a){
	float max = a[0];
	int i,max_ind = 0;
	for (i = 1; i < 10; ++i){
		if(a[i] > max){
			////printf("%f\n",max );
			max = a[i];
			max_ind = i;
		}
	}
	return max_ind;
}
int stopingCondition(float ep)
{
	int i,j,count1=0;
	for(i=0;i<=16;i++)
	{
		for(j=0;j<=no_hid_units;j++)
		{
			if(DEL_1[i][j]<ep)
				count1++;
		}
	}
	for(i=0;i<=no_hid_units;i++)
	{
		for(j=0;j<=16;j++)
		{
			if(DEL_2[i][j]<ep)
				count1++;
		}
	}
	//printf("\n\n\n%d\n\n\n\n",count1 );
	if(count1>141)
		return 1;
	return 0;
}
int main(){
	srand((unsigned int)time(NULL));
	no_hid_units =6;
	initialise_weights(no_hid_units);
	innit_delta_zero();
// done for just a single data point,will have 2 loops outside the outermost epochs(put err =0 inside this loop) and the next for reading the data
	int k = 100 ,i,j;
	int output;
	int cou =0,y1=0;
	char ch='2';
	printf("USING SUM OF SQUARED DEVEATIONS\nNo Of Hidden layers(Bias Included): %d\nLearning rate %f\n",no_hid_units,lr);
	printf("PRESS 1 FOR FIXED EPOCH\nPRESS 2 FOR deltaW\n");
	scanf("%c",&ch);
	while(k>0){
		c=0;
		err = 0;
		innit_delta_zero();
		//printf("check1\n");
		int **ar = readFromCSV("temp(1).txt",trainTotal);
		//printf("check2\n");
		for (i = 0; i < trainTotal; ++i){
		//	printf("check\n");
			for (j = 0; j < count; ++j){
				////printf("%d ",ar[i][j]);
				input[j] = ar[i][j];
			}
			output = label[i];
			matrix_mul(input,weights_1,0);
			matrix_mul(hidden_layer_1,weights_2,1);
			c++;
			calc_error(output_layer,output,1);
			//printf("%d\n",max_index(output_layer) );
			//printf("\n");
	
			//printf("%d\n",i);
			//c = trainTotal;
			//printf("\n");
		}
		//printf("test\n");
		if(k%10 == 0){
		  cou ++;
		  printf("ERROR1: %f  ITERATION: %d\n",err,cou*10);
		}
//	printf("%d\n",k);		
	k--;
	if(k == 0 && ch == '2'){
		y1++;
		if(y1<50)
			k = 100;
	}
	if(ch=='2'&&stopingCondition(0.01))
			break;
	}
	int acc = 0;
	int **ar2 = readFromCSV("temp(2).txt",testTotal);
	for (i = 0; i < testTotal; ++i){
		//	printf("check\n");
			for (j = 0; j < count; ++j){
				////printf("%d ",ar[i][j]);
				input[j] = ar2[i][j];
			}
			input[0] =1;
			output = label[i];
			matrix_mul(input,weights_1,0);
			matrix_mul(hidden_layer_1,weights_2,1);
			//vectorise(output_layer);
			one_hot_encoding(output);
/*
			for(k =0;k<10;k++)
				printf("%f ",output_layer[k]);
			printf("\n");
			printf("%d\n",max_index(output_layer));*/
			if(max_index(y) == max_index(output_layer)){
				//printf("%d %d\n",max_index(y),max_index(output_layer) );
				acc++;
				//printf("hi\n");
			}

			
			//calc_error(output_layer,output,0);
			//printf("\n");
	
			//printf("%d\n",c);
			//c = trainTotal;
			//  //printf("\n");
		}
		//printf("%d\n",acc );
		printf("Accuracy on Test Data: %f\n",(acc*100.0)/testTotal );
		
	////printf("%f\n",1.0/train_size );
	//back_prop();
	//one_hot_encoding(6);
}