#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
/*#include<bits/stdc++.h>
using namespace std;
*/
int c_train[10][192];
float probability[192][10][10],out_prob[10];
int giv_count[192][10][10];
int fin_predict[70000];
float fin_prob[70000],fin_lprob[7000];


float ** readFromCSV(char filename[],int totalLines){
	int count=193;

    float **ar = (float**)malloc(totalLines*sizeof(float*));
    FILE * F = fopen(filename,"r");
    int i = 0,m=0;
    char buf[1000];
    char *str;
    fgets(buf, sizeof(buf), F);
    while(fgets(buf, sizeof(buf), F) != NULL){
        //printf("%d\n",m++);
        str = strtok(buf,",");
        ar[i]=(float *)malloc(count * sizeof(float));   
        int j = 0;
        while(str!=NULL){   
            ar[i][j++] = atof(str);
            str = strtok(NULL,",");
        }
        i++;
    }
    fclose(F);
    return ar;
}


void count(int train[7000][200],int output[7000]){
  int j,k,i;
  for(i=0;i<4;i++)
  {
    for(j=0;j<192;j++)
    {
        c_train[i][j]=0;
    }
  }
    for(j=0;j<192;j++)
    {
      for(k=0;k<6668;k++)
      {
        c_train[train[k][j]][j]++;
      }
      //cout<<c_train[0][j]<<" "<<c_train[1][j]<<" "<<c_train[2][j]<<" "<<c_train[3][j]<<" "<<c_train[4][j]<<endl;
    }
    for(i=0;i<192;i++)
    {
        int c[10];

        for(j=0;j<5;j++)
        {
            for(int y=0;y<10;y++)
            {
                c[y]=0;
            }
       
       for(k=0;k<6668;k++)
       {
        if(train[k][i]==j)
        {
          c[output[k]]+=1;
        }
      }      
      for(k=0;k<10;k++){
        giv_count[i][j][k]=c[k];
      }
    }
  }
}
int calc_prob(int train[7000][200],int output[7000])
{
   int i,j,k;
    for(i=0;i<192;i++)
  {
    for(j=0;j<5;j++)
    {
      for(k=0;k<10;k++)
      {
        if(c_train[j][i]!=0){
        probability[i][j][k]=(float)((float)giv_count[i][j][k]*1.0/c_train[j][i]);
      }
      else
      {
        probability[i][j][k]=0;
      }
      }
    }
  }
  int temp_c[10];
  for(int i=0;i<6668;i++){
    temp_c[output[i]]++;
  }
  for(int i=0;i<10;i++){
    out_prob[i]=(float)temp_c[i]*1.0/6668;
  }
}

int prediction(int train[7000][200],int output[7000])
{
    int i,j,k,p;
  int cun=0;
  for(i=0;i<3330;i++)
  {
    for(j=0;j<10;j++)
    {
      double temp=0.0;
      double prob=1.0;
      for(k=0;k<192;k++)
      {
        for(p=0;p<5;p++)
        {
          if(train[i][k]==p)
          {
              prob=prob*probability[k][p][j];
              temp=temp+log(probability[k][p][j]);     
          }
        }
      }
      if(temp>fin_lprob[i])
      {
        fin_lprob[i]=temp;
        fin_predict[i]=j;
        fin_prob[i]=prob;
      }
    }
}
}

int main()
{
    char filename[100]="shuffled.csv";
    float **ar=readFromCSV(filename,6668);
    int dataset[7000][200],output[7000];
    int i,j;
    for(i=0;i<6668;i++)
    {
        for(j=0;j<192;j++)
        {
            dataset[i][j]=(int)ar[i][j];
           
        }
    }
    for(i=0;i<6668;i++)
    {
        output[i]=(int)ar[i][192];
    }
    count(dataset,output);
    calc_prob(dataset,output);

    char t_filename[100]="test.csv";
    
    float **t_arr=readFromCSV(t_filename,3330);
    int test_set[3330][200],test_out[3330];
    for(i=0;i<3330;i++)
    {
        for(j=0;j<192;j++)
        {
            test_set[i][j]=(int)t_arr[i][j];       
        }   
    }
    for(i=0;i<3330;i++)
    {
        test_out[i]=(int)t_arr[i][192];
    }
    
    for(i=0;i<3330;i++)
        fin_lprob[i]=-999999999.0;
    
    prediction(test_set,test_out);
    int count=0;
    for(i=0;i<3330;i++)
    {
       // cout<<test_out[i]<<" "<<fin_predict[i]<<endl;
        printf("%d %d\n",test_out[i],fin_predict[i] );
        if(fin_predict[i]==test_out[i])
            count++;
    }
    //cout<<"Accuracy on test set is "<<(count/3330.0)*100;
    printf("Accuracy on test set is %f",(count/3330.0)*100);
}