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
   char body[10];
   int end;
} pede;

typedef struct shot
{
  int y, x;
  bool move;
} shot;

int main()
{
   /* Game defaults */
   
   initscr(); // Initialise ncurses
   noecho(); // Stop typing input
   curs_set(0); // Deactivate cursor
   
   int yMax = 44, xMax = 116;
   // getmaxyx(stdscr, yMax, xMax);
   int score = 0, endScore;
   
   /* Game defaults end */
   
   /* Initialise player, centipede and bullets */ 
   
   player p1 = {
                 yMax - 3, xMax/2 // Set player at middle-bottom of screen
               };
               
   pede c = {
              1, 1, 1, "}@@@@@@@@@", 9 // Set centipede at top left 
            };    
            
   shot s = {
              false
            };  
                         

   bool gameOver = false; // Used to end the game 

   /* Main game loop */
   for(nodelay(stdscr, 1); !gameOver; usleep(30000))
   {      
        score++; // Rolling score over time
        c.x += c.d; // Sets the centipede movement
        erase(); // Deletes character trail of centipede
      
         /* Centipede Direction controls */
         
         if(c.x == xMax-6) // Right edge of screen
         {
          c.d *= -1; // Change direction
          c.y += 1;  // Move down the screen
         }
            else if (c.x == 0) // Left edge of screen
            {
              c.d = 1;
              c.y += 1;
            }
      
         if(c.x == p1.x && c.y == p1.y || c.end == -1)
         {
            gameOver = true; // End game if the centipede reaches the player or is shot to death
            endScore = score; // Save the score 
         } 
         
         if (c.x == s.x && c.y == s.y)
         {
            c.body[c.end] = 0; // Remove a body piece if the centipede's head is shot
            c.end -= 1;
            score += 1000;
         }
             
               else if (c.x+1 == s.x && c.y+1 == s.y || c.x+2 == s.x && c.y == s.y || c.x+2 == s.x && c.y == s.y || c.x+3 == s.x && c.y == s.y || c.x+4 == s.x && c.y == s.y || c.x+5 == s.x && c.y == s.y || c.x+6 == s.x && c.y == s.y || c.x+7 == s.x && c.y == s.y || c.x+8 == s.x && c.y == s.y || c.x+9 == s.x && c.y == s.y) 
               {
                  c.body[c.end] = 0; // Remove a body piece if the bullet reaches the centipede's body 
                  c.end -= 1;
                  score += 100;
               }
         
         if(s.move == true)
         {  
               s.y--;
               mvprintw(s.y, s.x, "*");
         } 
     
         /* Take player input */ 
         switch (getch())
         {                           
                                     /* Move: */
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
                            
                  case ' ':
                            s.move = true;
                            s.y = p1.y;
                            s.x = p1.x+1;
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
        mvaddstr(c.y, c.x, c.body);
        mvprintw(0, 0, "Welcome to Centipede | WASD keys to move | Spacebar to shoot | Score: %d", score);
   
        refresh();
   }
   
   endwin(); // End ncurses
   printf("Game over! Your score was %d \n", endScore);
}
