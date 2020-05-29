#include<stdio.h>
#include<stdlib.h>
#define N 64

int i=0,check=0,x;
char string[N];

double calculation();
double multiplication();
double parenthesis();
double constant();
double digit(double data7);


/* 加算・減算 */
double calculation()
{    
  double cal;
  if(string[0]=='-'){
	   i++;
	   cal=multiplication()*(-1);
 }else{
      cal=multiplication();
 }
  while(string[i]=='+' || string[i]=='-' && check==0){
	       if(string[i]=='+'){
		           i++;
		           cal=cal+multiplication();
	        }else if(string[i]=='-'){
		            i++;
		            cal=cal-multiplication();
           }
	}
  return cal;
}


/* 乗算・除算 */
double multiplication()
{
  double mul,m,n;
  mul=parenthesis();
  while(string[i]=='*' || string[i]=='/'){
      if(string[i]=='*'){
	             i++;
	             mul=mul*multiplication();
      }else if(string[i]=='/'){
	             i++;
				 m=multiplication();
	             mul=mul/m;
      }
  }
  return mul;
}


/* 括弧 */
double parenthesis()
{
	
   double par,data6;
   if(string[i]=='('){
       i++;
       par=calculation();
		i++;
   }else{
       par=constant();
   }
   return par;
}


/* 実数 */
double constant()
{
    double con,data7;
    data7=string[i]-'0';
    con=data7;
    i++;
    if(string[i]<=57 && string[i]>=48 ){
        x=1;
        con=digit(data7);
    }else if(string[i]=='.'){
        i++;
        x=0;
        con=digit(data7);
    }
    return con;
}


/* 複数桁 */
double digit(double data7)
{
	double dig,g;
	int it=0,ig,k;
	dig=data7;
	while(string[i]>=48 && string[i]<=57){
		    if(x==1){				
	             dig=dig*10+string[i]-'0';
			}else if(x==0){
				  g=string[i]-'0';
				  g=g/10;
				  for(k=0; k<it; k++){
					     g=g/10;
				  }
				  dig=dig+g;
				  it=it+1;	
			}
           i++;			
	}
	return dig;
}


/* メイン関数 */
int main()
{
    int t;
    double answer;
    
    fgets(string,N,stdin);
    
    for(t=0; string[t]!='\0'; t=t+1){
        if(string[t]=='\n'){
            string[t]='\0';
            t=t-1;
            break;
        }
    }
    answer=calculation();
    printf("%f",answer);
}
