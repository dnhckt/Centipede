#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h>

/*
    Centipede
    =========   
    Author: Aidan Hackett
    
    About: Centipede is a fixed shooter arcade game produced by Atari in 1980.
           This is a recreation of the game in C using the ncurses library.
           
           To play, compile and run this file.
           
           
        
              
*/

// Struct for the player character
typedef struct player 
{
   int y, x;
} player;

// Struct for the centipede
typedef struct pede
{
   int y, x;
   int d; // Direction of the centipede 
} pede;

int main()
{
   /* Game defaults */
   
   initscr(); // Initialise ncurses
   noecho(); // Stop typing input
   curs_set(0); // Deactivate cursor
   
   int yMax = 44, xMax = 116;
  // getmaxyx(stdscr, yMax, xMax); // Get screen boundaries
   
   /* Game defaults end */
   
   /* Initialise player and centipede co-ordinates */ 
   player p1 = {
                yMax - 3, xMax/2 // Set player at middle-bottom of screen
               };
   pede c = {
             0, 0, 1 // Set centipede at top left 
            };

   bool fin = false; // Used to end the game 

   /* Main game loop */
   
   for(nodelay(stdscr, 1); !fin; usleep(7000))
   {
      c.x += c.d; // Sets the centipede movement
      erase(); // Deletes character trail of centipede
      
      /* Direction controls */
      
      if(c.x == xMax) // Right edge of screen
      {
       c.d *= -1; // Change direction
       c.y += 1;  // Move down the screen
      }
      else if (c.x == 0) // Left edge of screen
      {
        c.d = 1; 
        c.y += 1;
      }
      
         if(c.x == p1.x && c.y == p1.y)
         {
            fin = true; // End the game if the centipede reaches the player
         }
      
     
         /* Take player input */ 
         switch (getch())
         {                           // Move:
                  case 'w':
                            p1.y--;  // Up
                            break;
                  case 's': 
                            p1.y++;  // Down
                            break;
                  case 'a': 
                            p1.x--;  // Left
                            break;
                  case 'd':  
                            p1.x++; // Right
                            break;
         }
          
         /* Vertical player boundaries */
         switch (p1.y)
         {
           case 39: 
                   p1.y++; // Upper
                   break; 
           case 45: 
                   p1.y--; // Lower
                   break; 
         } 
         
         /* Horizontal player boundaries */
         switch (p1.x)
         {
            case 1: 
                   p1.x++; // Left
                   break;
            case 116:
                   p1.x--; // Right
                   break;
         }
     
        /* Print and display characters */
        mvprintw(p1.y, p1.x, "<o>");
        mvprintw(c.y, c.x, "}@@@@@{");
        refresh();
   }
   
   endwin(); // End ncurses
}
