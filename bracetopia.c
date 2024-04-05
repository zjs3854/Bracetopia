// 
// File: bracetopia.c
// Description: provide a brief description of the source file content.
// 
// @author Zachary Szelc 
//
// // // // // // // // // // // // // // // // // // // // // // // //


#define _DEFAULT_SOURCE
#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>  // processing for "-flag" command line args
#include <unistd.h>

#include <time.h>

#define DIM_MIN 5 // min dimention size
#define DIM_MAX 35 //max dimention size


int returnCode;
int dim=15; //default dimension
int strength=50; //default strength
int vacancy=20; //default vacancy
int endline=60; //default Endline Percentage
unsigned int sleepdelay=900000;
int cycles=-1; // number of cycles intialized at -1 meaning infinite
int moves=0; // number of moves in a cycle

/*
* calculate_happiness() - calculates the happiness level of a cell at a row and column
* @param map[][] - The initialized map
* @param r - cell row
* @param c - cell column
* @return the happiness level
*/
double calculate_happiness(char a[][dim], int r, int c){
    int i, j; 
    double denom = 0.0, nom=0.0;
    for (i = r - 1; i <= r + 1; i++) {
        for (j = c - 1; j <= c + 1; j++) {
            if ((i == r && j == c) || (i < 0 || j < 0) || (i >= dim || j >= dim)) {
                continue;
                }
            if (a[r][c] == 'e') {
                if(a[i][j]== 'e'){
                    nom+=1;
                    denom+=1;
                    } 
                if(a[i][j]== 'n'){
                    denom+=1;
                    }
                }
            if (a[r][c] == 'n') {
                if(a[i][j]== 'n'){
                    nom+=1;
                    denom+=1;
                    } 
                if(a[i][j]== 'e'){
                    denom+=1;
                    }
                }
            }
        }
    double b=nom/denom;
    return b;
}

/*
* swap() - swaps two points on the map
* @param map[][] - The map
* @param i - row
* @param j - col
* @param iNew - row
* @param jNew - new col
*/
void swap(char a[][dim], int i, int j, int iNew, int jNew) 
{

  char temp = a[i][j]; 
  a[i][j] = a[iNew][jNew]; 
  a[iNew][jNew] = temp;
}
/*
* fisher_yates_shuffle() - Makes a initial confirguration of a map.
* @param dim -dimention
* @param a[][] - The map
*/
void fisher_yates_shuffle(int dim, char a[][dim]) {
    int r, c;
    for (int i = dim-1; i >0; i--)
    {
        for (int j = dim-1; j > 0; j--)
        {
        r=random()%(i+1);
        c=random()%(j+1);
            swap(a, i, j, r, c); // swap element with random later element

            
        }
    }
}
/*
* initialize_map() - Makes a initial confirguration of a map.
* @param map[][] - The map
*/
void initialize_map(char map[][dim]){
    long int row, col;
    int  numcell;
    int vac=(int)(dim*dim*(vacancy*.01));
    for(row = 0; row<dim; row++)
        {
            for(col = 0; col<dim; col++)
            {
              map[row][col] = '.';  
            }
        }
        numcell=dim*dim;
    int calcvac=numcell*(vacancy*.01);
    int endL=(numcell-calcvac)*(endline*.01);
    int newL=(numcell-calcvac)-endL;
    
    for(row = 0; row<dim; row++){
            for(col = 0; col<dim; col++){
                if (vac>0){
                    vac--;
                    continue;
                    }
                if(endL>0){
                    map[row][col]='e';
                    endL--;
                    continue;
                    }
                if(newL>0){
                    map[row][col]='n';
                    newL--;
                    continue;
                    }
        }
    }
}  

/*
* movemp() - makes the changes to the current map configuration
* @param map[][] - The initialized map
*/
void movemp(char map[][dim]){
    char mapb[dim][dim];
    initialize_map(mapb);
    int row, col;
    double happy=0;
    for(row=0; row < dim; row++){  //copy 2D array
        for (col=0; col<dim; col++){
            mapb[row][col]=map[row][col];
            }
        }
    for(row=0;row<dim;row++){
        for(col=0; col < dim; col++){
            happy=calculate_happiness(map,row,col);
            if(happy>0.01*((double)strength)||map[row][col]=='.'){
                continue;
                }
            else{
                int r, c;
                for(r=dim-1;r>=0;r--){
                    for (c=dim-1;c>=0;c--){
                        if(map[r][c]=='.'&&mapb[r][c]== '.'&& mapb[row][col]!='.'){
                            mapb[r][c]=map[row][col];
                            mapb[row][col]='.';
                            moves++;
                            }
                        }
                    }
                }
            }
        }
    for(row=0; row < dim; row++){
          for (col=0; col<dim; col++){
             map[row][col]=mapb[row][col];
              }
          }
}

/*
* print_map() - display the current configuration of the map to stdout
* @param map[][] - The initialized map
*/
void print_map(char map[dim][dim]){
    int row, col;
    for(row = 0; row<dim; row++){
        for(col = 0; col<dim; col++){
            printf("%c", map[row][col]); // fix 1: make %s a %c
        }
        puts(" ");
    }
}

/*
* print_usage() - prints the usage statement
*
*/
void print_usage(){
    fputs("usage:\n",stderr);
    fputs("bracetopia [-h] [-t N] [-c N] [-d D] [-s S] [-v V] [-e E]\n",stderr);
    }
/*
* calc_Overall_happiness
* @param map[][] - The initialized map
* @returns double representing the overall happiness of every cell
*/
double calc_Overall_happiness(char map[dim][dim]){
    double Ohap=0.0;
    double denom=0.0;
    for(int row=0; row < dim; row++){
        for (int col=0; col<dim; col++){
            if(map[row][col]!='.'){
                if (calculate_happiness(map,row,col)>=0){
                    Ohap+=calculate_happiness(map,row,col);
                    denom+=1.0;
                    }
                }
            } 
        }
    return Ohap/denom;
    }
    
/*
* runmap() - runs the map 
* @param map[][] - The initialized map
*/ 
void runmap(char map[][dim]){
    int cnum=0;
    if(cycles>=0){ // with cycles
        while(cycles>=0){
            print_map(map);
            printf("cycle: %d\nmoves this cycle: %d\noverall \"happiness\": %0.4f\n",cnum, moves, calc_Overall_happiness(map));
            printf("dim:  %d, threshold:  %d%%, vacancy:  %d%%, endlines:  %d%%\n", dim, strength,vacancy,endline);
            moves=0;
            movemp(map);
            cycles--;
            cnum++;
            }
        }else{ // infinite cycle
            while(cycles==-1){
                clear();
                refresh();
                for( int row = 0; row<dim; row++ )  {
                    for( int col = 0; col<dim; col++ )  {
                        mvaddch(row, col, map[row][col] );
                        }
                }
                refresh();
                mvprintw(dim,0,"cycle: %d\nmoves this cycle: %d\noverall \"happiness\": %0.4f\
                \ndim:  %d, threshold:  %d%%, vacancy:  %d%%, endlines:  %d%%\
                \nUse Control-C to quit.",cnum, moves, calc_Overall_happiness(map), dim, strength,vacancy,endline);
                moves=0;
                refresh();
                usleep(sleepdelay);
                movemp(map);
                cnum++;
                }          
          } 
    return;
}

/*
* print_h() - Prints the help function
*
*/
void print_h(){
    print_usage();
    fputs("Option      Default   Example   Description\n",stderr);
    fputs("'-h'        n/a       -h        print this usage message.\n\
'-t N'      900000    -t 5000   cycle delay, in microseconds.\n\
'-c N'      n/a       -c4       cycle count maximum value.\n\
'-d D'      15        -d    7   width and height dimension.\n\
'-s S'      50        -s 30     strength of preference.\n\
'-v V'      20        -v30      percent vacancies.\n\
'-e E'      60        -e75      percent of braces that are Endline.\n",stderr);
}


/// Main function for the example program.
///
/// @param argc  integer number of command line arguments includes program name
/// @param argv  strings: program-name commandline-argument
/// @returns EXIT_SUCCESS if no error, or EXIT_FAILURE on error

int main( int argc, char* argv[] ) {
    int opt, tempint=0;
    srand(41);
    while ( (opt = getopt( argc, argv, "hd:s:v:e:c:t:") ) != -1 ) { // reads the options
        switch (opt){
            case 'h':
                print_h();
                exit(0);
            case 'd':
                tempint=(int)strtol( optarg, NULL, 10 );
                
                if(tempint>=DIM_MIN && tempint<=DIM_MAX){ 
                    dim=tempint;
                    }
                else{ 
                    fprintf(stderr,"dimension (%d) must be a value in [5..39]\n",tempint);
                    print_usage();
                    exit(1+EXIT_FAILURE);
                    }
                break;
            case 's':
                tempint=(int)strtol( optarg, NULL, 10 );
                if (tempint>=1 || tempint<=99){
                    strength = tempint;
                    }
                else{ 
                    fprintf(stderr,"preference strength (%d) must be a value in [1..99]\n",tempint); 
                    print_usage();
                    exit(1+EXIT_FAILURE);
                    }
                break;
                    
            case 'v':
                tempint=(int)strtol( optarg, NULL, 10 );
                if (tempint>=1 || tempint<=99){
                    vacancy = tempint;
                    }
                else{ 
                    fprintf(stderr,"vacancy (%d) must be a value in [1..99]\n",tempint); 
                    print_usage();
                    exit(1+EXIT_FAILURE);
                    }
                break;
            case 'e':
                tempint=(int)strtol( optarg, NULL, 10 );
                if (tempint>=1 || tempint<=99){
                    endline = tempint;
                    }
                else{ 
                    fprintf(stderr,"endline proportion (%d) must be a value in [1..99]\n",tempint); 
                    print_usage();
                    exit(1+EXIT_FAILURE);
                    }
                break;
            case 'c':
                cycles=(int)strtol( optarg, NULL, 10 );
                if(cycles<0){ 
                    fprintf(stderr,"count (%d) must be a non-negative integer\n",cycles); 
                    print_usage();
                    exit(1+EXIT_FAILURE);
                    }
                break;
            case 't':
                tempint=(int)strtol( optarg, NULL, 10 );
                if(tempint>0){ sleepdelay=tempint;}
                break;
            default:
                print_usage();
                exit(EXIT_FAILURE);   
        }
    }
    if(cycles==-1){ // initialize screen if infinite cycle
        initscr();
        }
    char map[dim][dim];
    initialize_map(map);
    fisher_yates_shuffle(dim,map);
    runmap(map);
    endwin();
    return 0;
}
