#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <time.h>

#define ADVANCED 0 //発展課題（絞り込み検索）に対応する場合は1に変更

#define DATAFILE "KEN_ALL.CSV"
#define CLEN 9 //郵便番号の最大バイト長
#define ALEN 200 //住所欄の最大バイト長
#define MAX_SIZE 200000//住所録中の住所数の最大数

int mode; //検索モード 0:なし，1:郵便番号検索，2:文字列検索
int refine_flag; //絞り込み検索の有無 0:なし，1:あり
char query[ALEN]; //検索クエリ（郵便番号or文字列）


//構造体配列
typedef struct {   
    int code;
    char pref[ALEN+1]; 
    char city[ALEN+1];
    char town[ALEN+1]; 
} post_t;
post_t p[MAX_SIZE];


//住所データファイルを読み取り，配列に保存
void scan(){
  FILE *fp;
  char code[CLEN+1],pref[ALEN+1],city[ALEN+1],town[ALEN+1],tmp[ALEN+1],all[ALEN+1];
  long line=0;
  int i;

  //datasizeの計算
  if ((fp = fopen(DATAFILE, "r")) == NULL) {
    fprintf(stderr,"error:cannot read %s\n",DATAFILE);
    exit(-1);
  }
  while(fscanf(fp,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%[^,],%s",tmp,tmp,code,tmp,tmp,tmp,pref,city,town,tmp) != EOF ){    
    /*
      上のfscanfにより，code,pref,city,townにそれぞれ郵便番号，都道府県，市町村，町域を表す
      文字列が記憶される．これらの情報を用いて構造体の配列に住所データを記憶させる．
    */
	/* 以下は""を外す作業 */
	for(i=0; i<=ALEN+1; i++){
		code[i]=code[i+1];
		if(code[i]=='\"'){
			code[i]='\0';
			break;
		}
	}
	for(i=0; i<=ALEN+1; i++){
	  pref[i]=pref[i+1];
	  if(pref[i]=='\"'){
		  pref[i]='\0';
		  break;
	  }
	}
	for(i=0; i<=ALEN+1; i++){
	  city[i]=city[i+1];
	  if(city[i]=='\"'){
		  city[i]='\0';
		  break;
	  }
	}
	for(i=0; i<=ALEN+1; i++){
	  town[i]=town[i+1];
	  if(town[i]=='\"'){
		  town[i]='\0';
		  break;
	  }
	}
	p[line].code=atoi(code); //文字列を数値に変換
	strcpy(p[line].pref,pref); //prefを構造体のprefに変換
	strcpy(p[line].city,city);
	strcpy(p[line].town,town);
    line++;
  }
  printf("%ld行の住所があります\n",line);
  fclose(fp); 
}


void preprocess(){
  return;
}


double diff_time(clock_t t1, clock_t t2){
  return (double) (t2-t1)/CLOCKS_PER_SEC;
}


//初期化処理
void init(){
  clock_t t1,t2;

  t1 = clock();
  scan();
  preprocess();
  printf("Done initilization\n");
  t2 = clock();
  printf("\n### %f sec for initialization. ###\n",diff_time(t1,t2));  
}


//郵便番号による住所検索．検索結果を出力．
void code_search(){
	int i;
	for(i=0; i<=MAX_SIZE; i++){
		if(p[i].code==atoi(query)){
			printf("%d:%s%s%s\n", p[i].code,p[i].pref,p[i].city,p[i].town);
		}
	}
  return;
}


//文字列による住所検索．検索結果を出力．
void address_search(){
  	int i=0,j=0,k=0;
	char all[ALEN+1];
	
	while(j<=MAX_SIZE){
	
		for(i=0; all[i]!='\0'; i++){
		all[i]='f';
	    }
		i=0;
		k=0;
		while(p[j].pref[k]!='\0'){
			all[i]=p[j].pref[k];
			i++;
			k++;
		}
		k=0;
		while(p[j].city[k]!='\0'){
			all[i]=p[j].city[k];
			i++;
			k++;
		}
		k=0;
		while(p[j].town[k]!='\0'){
			all[i]=p[j].town[k];
			k++;
			i++;
		}
	
		if(strstr(all,query)!=NULL){
			printf("%d:%s%s%s\n",p[j].code,p[j].pref,p[j].city,p[j].town);
		}
		j++;
	}
  return;
}


//絞り込み検索の実施
void refinement(){
  return;
}


void input(){
  printf("\n"
	 "#########Top Menu#########\n"
	 "# Search by postal code: 1\n"
	 "# Search by address    : 2\n"
	 "# Exit                 : 0\n"
	 "> ");
  scanf("%d", &mode);
  if(mode == 1){
    printf("Postal code > ");
    scanf("%s", query);
  }else if(mode == 2){
    printf("Search String > ");
    scanf("%s", query);
  }
}


//絞り込み検索の有無を確認
void re_input(){
  printf("\n"	 
	 "# Continue Searching: 1\n"
	 "# Return to Top Menu: 0\n"
	 "> ");
  scanf("%d", &refine_flag);
  if(refine_flag == 1){
    printf("String for Refinement> ");
    scanf("%s", query);
  }
  return;
}


//クエリへの応答
void respond(){
  clock_t t1,t2;
  mode = 1;
  while(1){
    input();
    if(mode == 1){
      t1 = clock();
      code_search();
      t2 = clock();
      printf("\n### %f sec for search. ###\n", diff_time(t1,t2));
    }
    else if(mode == 2){
      t1 = clock();
      address_search();
      t2 = clock();
      printf("\n### %f sec for search. ###\n", diff_time(t1,t2));
      if(!ADVANCED) continue;
      while(1){
	re_input();
	if(refine_flag == 0) break;
	t1 = clock();
	refinement();
	t2 = clock();
	printf("\n### %f sec for search. ###\n", diff_time(t1,t2));
      }
    }
    else break;
  }  
}


int main()
{
  init();
  respond();
  return 0;
}
