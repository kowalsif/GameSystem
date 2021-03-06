//gcc gpio-utils.h gpio-utils.c etch-sketch.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <signal.h>	// Defines signal-handling functions (i.e. trap Ctrl-C)
#include <termios.h>
#include <time.h>

#define POLL_TIMEOUT (1 * 10) /* 0.5 seconds */
#define MAX_BUF 64

int keepgoing = 1;	// Set to 0 when ctrl-c is pressed

void signal_handler(int sig);
// Callback called when SIGINT is sent to the process (Ctrl-C)
void signal_handler(int sig)
{
	printf( "Ctrl-C pressed, cleaning up and exiting..\n" );
	keepgoing = 0;
}

void draw(int **grid, int width, int height);

void draw(int **grid, int width, int height){
	system("clear");
	int i = 0; 
	while(i < width){
	   int j = 0; 
	   while(j < height){
	      printf("%d ", grid[i][j]);
	      j++;
	   }
	   printf("\n");
	   fflush(stdout);
	   i++;
	}
	fflush(stdout);
	i++;
}

int main(int argc, char **argv, char **envp)
{
	
	int length, m;

	
	printf("m: %d \n", m);
	signal(SIGINT, signal_handler);
	

    length = 1;
    printf("use wasd to control, pressing enter after each letter");

	struct pollfd fdset[length];
	
	int i, j;
	int **grid;
	int width = 20;
	int height = 20;
	char buf[MAX_BUF];
	int timeout, rc;
	int gpio_fd[length];
	
	
	int ox[100];
	int oy[100]; //sufficiently large snake?
	int pt = 0;
	int ph = 1; 
	i = 0;
//	while (i < 100){
//		x[i] = -1;
//		y[i] = -1;
//	}
	
	
	int xh = 10;
	int yh = 10; //start in corner, wasd controls for key, 30 31 48 49 for buttons

	int x = 10;
	int y = 10;
	ox[pt] = x;
	oy[pt] = y;

	
	grid = malloc(width * sizeof(int *));
	i = 0; 
	while (i < width) {
	   grid[i] = malloc(height * sizeof(int)); //create the 2d array
	   i++;
	}
	i = 0; 
	while (i < width) {
	   j = 0; 
	   while (j < height){
	      grid[i][j] = 0; //fill array with zeroes
	      j++;
	   }
	   i++;
	}
    grid[10][10] = 8; //mark starting position
    
    srand(time(NULL)); //prep random
	int px = 15;
	int py = 15; 
	grid[px][py] = 4; //prep food
	int rx, ry;

	int nfds = length;
	int len;
	timeout = POLL_TIMEOUT;
	
	
   	system("clear");
	i = 0; 
	while(i < width){
	   j = 0; 
	   while(j < height){
	      if (grid[i][j] == 0){
		     printf("  ");
		  }
		  else printf("%d ", grid[i][j]);
	      j++;
	   }
	   printf("|\n");
	   fflush(stdout);
	   i++;
	}
	fflush(stdout);


	int input;
	int count = 0;
	int dir = 0;
	int s = 0;
	int reset = 0;
	char hold[1];
	int halt = 0;
	while (keepgoing) {
	
		if (reset == 1){
			reset = 0;
			clear(grid);
			pt = 0;
			ph = 1;
			grid[10][10] = 8;
			x = 10;
			y = 10;
			ox[0] = 10;
			oy[0] = 10;
			grid[15][15] = 4;
			dir = 0;
			scanf("%s", &hold); //wait for any input
				
		}
	    usleep(300000);
		s = 0;
		memset((void*)fdset, 0, sizeof(fdset));
	

		   fdset[0].fd = STDIN_FILENO;
		   fdset[0].events = POLLIN;
		   
		   rc = poll(fdset, nfds, timeout);
		   if (rc < 0) {
	              printf("\npoll() failed!\n");
		      return -1;
		   }
      
		   if (fdset[0].revents & POLLIN) {
			(void)read(fdset[0].fd, buf, 1);
			input = (int) buf[0];
			//printf(" input is: %d\n", input);
			//printf("\npoll() stdin read 0x%2.2X\n", (unsigned int) buf[0]);
			fflush(stdout);
		  }
		  
		  /*
		  w = 119, s = 115, d = 100, a = 97, q = 113;
		  */

		   if (input == 113){
		   		halt = !halt;
		   		printf("halt button pressed, mode toggling");
		   }
		   if (halt == 1){
		   		continue;
		   }
		   if (input == 10){
		      input = 0; //clear input
		   	  continue;
		   }
		   if (input ==  100){
		      dir = 0;
		   }
		   else if (input == 119){
		      dir = 1;
		   }
		   else if (input == 97){
		      dir = 2;
		   }
		   else if (input == 115){
		      dir = 3;
		   }
		   
		   if (count == 1){
		   	  rx = rand()%20;
		   	  count = 0;
		   	  grid[x][y] = 1;
		   	  if (dir == 0){
		   	     if (y == height-1){
		         	printf("you lose\n");
		         	reset = 1;
		         	continue;
		         	//break;
		         }
		   	     y++;
		   	     s = strike(grid, x, y);
		   	     if (s == 1){
		   	  	 	reset = 1;
		         	continue;
		   	  	 }
		   	  	 else if (s == 2) {
		   	  	 	mv(grid, rx, ry);
		   	  	 }
		   	  	 else {
		   	  	 	rm(grid, ox[pt], oy[pt]);
		   	  	 	pt++;
		   	  	 	if (pt == 100){
		   	  	 		pt = 0;
		   	  	 	}
		   	  	 }
		         grid[x][y] = 8;
		   	  }
		   	  if (dir == 1){
		   	     if (x == 0){
		         	printf("you lose\n");
		         	reset = 1;
		         	continue;
		         }
		   	     x--;
		   	     s = strike(grid, x, y);
		   	     if (s == 1){
		   	  	 	reset = 1;
		         	continue;
		   	  	 }
		   	  	 else if (s == 2) {
		   	  	 	mv(grid, rx, ry);
		   	  	 }
		   	  	 else {
		   	  	 	rm(grid, ox[pt], oy[pt]);
		   	  	 	pt++;
		   	  	 	if (pt == 100){
		   	  	 		pt = 0;
		   	  	 	}
		   	  	 }
		         grid[x][y] = 8;
		   	  }
		   	  if (dir == 2){
		   	     if (y == 0){
		         	printf("you lose\n");
		         	reset = 1;
		         	continue;
		         }
		   	     y--;
		   	     s = strike(grid, x, y);
		   	     if (s == 1){
		   	  	 	reset = 1;
		         	continue;
		   	  	 }
		   	  	 else if (s == 2) {
		   	  	 	mv(grid, rx, ry);
		   	  	 }
		   	  	 else {
		   	  	 	rm(grid, ox[pt], oy[pt]);
		   	  	 	pt++;
		   	  	 	if (pt == 100){
		   	  	 		pt = 0;
		   	  	 	}
		   	  	 }
		         grid[x][y] = 8;
		   	  }
		   	  if (dir == 3 && x < width){
		   	     if (x == width-1){
		         	printf("you lose\n");
		         	reset = 1;
		         	continue;
		         }  
		   	     x++;
		   	     s = strike(grid, x, y);
		   	     if (s == 1){
		   	  	 	reset = 1;
		         	continue;
		   	  	 }
		   	  	 else if (s == 2) {
		   	  	 	mv(grid, rx, ry);
		   	  	 }
		   	  	 else {
		   	  	 	rm(grid, ox[pt], oy[pt]);
		   	  	 	pt++;
		   	  	 	if (pt == 100){
		   	  	 		pt = 0;
		   	  	 	}
		   	  	 }
		         grid[x][y] = 8;
		   	  }
		   	  
		   	  ox[ph] = x;
		   	  oy[ph] = y;
		   	  ph++;
		   	  if (ph == 100){
		   	  	ph = 0;
		   	  }

		   	  system("clear");
		      i = 0; 
		      while(i < width){
		        j = 0; 
		        while(j < height){
		         if (grid[i][j] == 0){
		         	printf("  ");
		         }
		         else printf("%d ", grid[i][j]);
		         j++;
		        }
		        printf("|\n");
		        i++;
		      }
		   }
		   else{
		   	  ry = rand()%20;
		   	  count++;
		   }
		   //printf("\033[%d;%d]", width, height);
		   fflush(stdout);
		   
	}
		
    
    return 0;
}

int strike(int **grid, int x, int y);
int strike(int **grid, int x, int y){
	if (grid[x][y] == 1){
		//keepgoing = 0;
		printf("loss condition\n");
		return 1;
	}
	if (grid[x][y] == 4){
		return 2; //pellet
	}
	return 0; //good
}

void mv(int **grid, int x, int y);
void mv(int **grid, int x, int y){
	grid[x][y] = 4;
}

void rm(int **grid, int x, int y);
void rm(int **grid, int x, int y){
	if(grid[x][y] != 4){
		grid[x][y] = 0;
	}
}

void clear(int **grid);
void clear(int **grid){
	int i = 0;
	int j;
	while (i < 20){
		j = 0;
		while (j < 20){
			grid[i][j] = 0;
			j++;
		}
		i++;
	}

}

