#include <stdio.h>
#define BOARD_SIZE 8


/* 盤面表示 */
void print_board(int board[][BOARD_SIZE])
{
   int x, y, i, j=0;
   printf(" ");
   for(i=0; i<BOARD_SIZE; i++){
       printf(" %d ", i);
   }
   printf("\n");
   for(y=0; y<BOARD_SIZE; y++){
       printf("%d", j);
       j++;
       for(x=0; x<BOARD_SIZE; x++){
           if(board[x][y]==1){
               printf(" o ");
           }else if(board[x][y]==-1){
               printf(" x ");
           }else{
               printf(" . ");
           }
       }
       printf("\n");
   }
}


/* 盤面の初期設定 */
void init_board(int board[][BOARD_SIZE])
{
   int x, y;
   for(y=0; y<BOARD_SIZE; y++){
       for(x=0; x<BOARD_SIZE; x++){
           board[x][y]=0;
       }
   }
   board[3][3]=-1;
   board[3][4]=1;
   board[4][3]=1;
   board[4][4]=-1;
}


/* 石の数をカウント */
int count_stone(int player_id, int board[][BOARD_SIZE])
{
   int count=0, x, y;
   for(y=0; y<BOARD_SIZE; y++){
       for(x=0; x<BOARD_SIZE; x++){
           if(board[x][y]==player_id){
               count++;
           }
       }
   }
   return count;
}


/* 石を置いて裏返す */
void place_stone(int player_id, int x, int y, int board[][BOARD_SIZE])
{
   int i, j, p, q;
   for(j=-1; j<=1; j++){
       for(i=-1; i<=1; i++){
           if(i!=0 || j!=0){
               if(board[x+i][y+j]==-player_id){
                   for(p=i, q=j; board[x+p][y+q]==-player_id&&((0<x+p&&x+p<BOARD_SIZE-1&&0<y+q&&y+q<BOARD_SIZE-1)||(i==0&&0<=x+p&&x+p<=BOARD_SIZE-1&&0<y+q&&y+q<BOARD_SIZE-1)||(0<x+p&&x+p<BOARD_SIZE-1&&j==0&&0<=y+q&&y+q<=BOARD_SIZE-1)); p+=i, q+=j){
                   }
                   if(board[x+p][y+q] == player_id){
                       do{
                           p -= i;
                           q -= j;
                           board[x+p][y+q] = player_id;
                       }while(p!=0 || q!=0);
                   }
               }
           }
       }
   }
}


/* 獲得できる石をカウント */
int num_obtained_stone(int player_id, int x, int y, int board[][BOARD_SIZE])
{
   int i, j, p, q, n=0, sum;
   for(j=-1; j<=1; j++){
       for(i=-1; i<=1; i++){
           if(i!=0 || j!=0){
               if(board[x+i][y+j]==-player_id){
                   p = 0;
                   q = 0;
                   do{
                       p += i;
                       q += j;
                   }while(board[x+p][y+q]==-player_id && 0<x+p&&x+p<BOARD_SIZE-1&&0<y+q&&y+q<BOARD_SIZE-1);
                   if(board[x+p][y+q] == player_id){
                       while(p!=0 || q!=0){
                           p -= i;
                           q -= j;
                           n++;
                       }
                   }
               }
           }
       }
   }
   sum = n+1;
   return sum;
}


/* charをintへ変換 */
int ascii_to_int(char string[])
{
   int a=0, e=1, i=0, s, n=0;
   while(string[i+1]!='\0'){
       i++;
   }
   if(string[i]=='\n'){
       i--;
   }
   for(; i>=0; i--){
       s=string[i]-'0';
       if(0<=s && s<=9){
           a += s*e;
       }else{
           n--;
       }
       e *= 10;
   }
   if(n>=0){
       return a;
   }else{
       return -1;
   }
}


/* プレイヤーが石を置く場所を入力 */
int input_place(int player_id, int board[][BOARD_SIZE])
{
   char str[1000];
   int x, y, n;
   do{
       do{
           printf("input yoko!:");
           fgets(str, 1000, stdin);
           x = ascii_to_int(str);
       }while((x<0||BOARD_SIZE<=x)&&(str[0]!='P'||str[1]!='A'||str[2]!='S'||str[3]!='S'||str[4]!='\n'||str[5]!='\0'));
       if(str[0]!='P'||str[1]!='A'||str[2]!='S'||str[3]!='S'||str[4]!='\n'||str[5]!='\0'){
           printf("input tate!:");
           fgets(str, 1000, stdin);
           y = ascii_to_int(str);
       }
       n = num_obtained_stone(player_id, x, y, board);
   }while((y<0||y>=BOARD_SIZE||board[x][y]!=0||n<=1)&&(str[0]!='P'||str[1]!='A'||str[2]!='S'||str[3]!='S'||str[4]!='\n'||str[5]!='\0'));
   if(str[0]=='P'&&str[1]=='A'&&str[2]=='S'&&str[3]=='S'&&str[4]=='\n'&&str[5]!='\0'){
       return -1;
   }else{
       return x*BOARD_SIZE+y;
   }
}


/* コンピュータが石を置く場所を決定 */
int computer(int player_id, int board[][BOARD_SIZE])
{
   static int imgboard[BOARD_SIZE][BOARD_SIZE];
   int p=0, cy, cc, x, y, sum=0, ix, iy, in, pp, mn, mp=60, mx, my, i, j;
   for(y=0; y<BOARD_SIZE; y++){
       for(x=0; x<BOARD_SIZE; x++){
           sum = num_obtained_stone(player_id, x, y, board);
           if(sum>1){
               p++;
           }
       }
   }
   cy = count_stone(-player_id,board);
   cc = count_stone(player_id,board);
   for(y=0; y<BOARD_SIZE; y++){
       for(x=0; x<BOARD_SIZE; x++){
           if(imgboard[x][y] != board[x][y]){
               sum=sum-1;
           }
       }
   }
   if(p==0 || (sum<=0 && cy<cc)){
       return -1;
   }else{
       for(y=0; y<BOARD_SIZE; y++){
           for(x=0; x<BOARD_SIZE; x++){
               imgboard[x][y] = board[x][y];
           }
       }
       for(y=0; y<BOARD_SIZE; y++){
           for(x=0; x<BOARD_SIZE; x++){
               sum = num_obtained_stone(player_id, x, y, board);
               if(sum>1 && board[x][y]==0){
                   place_stone(player_id, x, y, imgboard);
                   pp=0;
                   for(iy=0; iy<BOARD_SIZE; iy++){
                       for(ix=0; ix<BOARD_SIZE; ix++){
                           in = num_obtained_stone(-player_id, ix, iy, imgboard);
                           if(in>1){
                               pp++;
                           }
                       }
                   }
                   mn = num_obtained_stone(player_id,x,y,board);
                   if(mp>pp || (mp==pp && mn<sum)){
                       mp = pp;
                       mx = x;
                       my = y;
                   }
                   for(j=0; j<BOARD_SIZE; j++){
                       for(i=0; i<BOARD_SIZE; i++){
                           imgboard[i][j]=board[i][j];
                       }
                   }
               }
           }
       }
       return mx*BOARD_SIZE+my;
   }
}


/* 履歴を記録 */
void update_history(int location, int history[])
{
   int i=0, HISTORY_SIZE;
   while(i<HISTORY_SIZE){
       if(history[i]==-20){
           history[i]=location;
       }
   }
   return;
}


/* 履歴history.txtに保存 */
void save_history(int history[])
{
   int i, HISTORY_SIZE;
   FILE *fp;
   int player, x, y;
   char *fname = "history.txt";

   fp = fopen(fname, "w");

   if(fp==NULL){
       printf("History NOT saved.\n");
       return;
   }

   for(i=0; i<HISTORY_SIZE; i++){
       player = (i%2==0)?1:2;
       if(history[i]==-1){
           x=-1;
           y=-1;
       }else{
           x=history[i]/BOARD_SIZE;
           y=history[i]%BOARD_SIZE;
       }
       fprintf(fp, "player%d %d %d", player, x, y);
   }
   return;
}


/* main関数 */
main()
{
       int player_id=1, board[BOARD_SIZE][BOARD_SIZE], you, cpu, i, c, x, y, pass, cy, cc;
       do{
           printf("Player1:saki\nPlayer2:ato\n");
           printf("choose player, 1or2:");
           scanf("%d", &you);
       }while(you<1||2<you);
       if(you==2){
           you=-1;
       }
       cpu=-you;
       init_board(board);
       pass=0;
       do{
           print_board(board);
           i=0;
           c=0;
           if(player_id==you){
               i = input_place(player_id, board);
               x = i/BOARD_SIZE;
               y = i%BOARD_SIZE;
           }
           if(player_id == cpu){
               c = computer(player_id, board);
               x = c/BOARD_SIZE;
               y = c%BOARD_SIZE;
               printf("input yoko:");
               if(c==-1){
                   printf("PASS\n");
               }else{
                   printf("%d\n",x);
                   printf("input tate:%d\n", y);
               }
           }printf("%d,%d", i, c);
           if(i!=-1 && c!=-1){
               pass=0;
               place_stone(player_id, x, y, board);
           }else{
               pass++;
           }
           cy = count_stone(you, board);
           cc = count_stone(cpu, board);
           player_id = -player_id;
       }while(pass<2 && cy!=0 && cc!=0);
       print_board(board);
       printf("YOU:%d vs CPU:%d\n", cy, cc);
       if(cy>cc){
           printf("Winner:YOU");
       }
       if(cy==cc){
           printf("Draw");
       }
       if(cy<cc){
           printf("Winner:CPU");
       }
}

