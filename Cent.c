#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
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
   int endNum[9];
} pede;

typedef struct shot
{
  int y, x;
  bool move;
} shot;

typedef struct mushroom
{
  int y, x;
  int health;
} mushroom;

int main()
{
   /* Game defaults */
   
   initscr(); // Initialise ncurses
   noecho(); // Stop typing input
   curs_set(0); // Deactivate cursor
   
   start_color();
   init_pair(1, COLOR_RED, COLOR_BLACK);
   init_pair(2, COLOR_GREEN, COLOR_BLACK);
   init_pair(3, COLOR_BLUE, COLOR_BLACK);  
   
   int yMax = 44, xMax = 116;
   // getmaxyx(stdscr, yMax, xMax);
   int score = 0, endScore;
   srand(time(NULL));
   
   /* Game defaults end */
   
   /* Initialise player, centipede, mushrooms and bullets */ 
   
   player p1 = {
                 yMax - 3, xMax/2 // Set player at middle-bottom of screen
               };
               
   pede c = {
              1, 1, 1, "}@@@@@@@@@", 9, 1 // Set centipede at top left 
            };    
           
   shot s = {
              false
            }; 
             
   mushroom m[30] = {
                     10, 10, 1
                };
   
     /* Centipede end */
     for( int i = 1; i < 9; i++)
     {
      c.endNum[i] = i;
     }
     
   
    /* Mushroom positioning */ 
    
    for(int i = 0; i < 30; i++)
    {
      m[i].y = i+4; // Set mushrooms on each row
      m[i].x = rand() % 115; // Set x position as random
      if (m[i].x == m[i-1].x || m[i].x+1 == m[i-1].x || m[i].x-1 == m[i-1].x)
      {
         m[i].x = rand() % 115; // Reroll the xpos of the mushroom if too close to the one before
      }
      m[i].health = 5;                    
    }       
    
   bool gameOver = false; // Used to end the game 
   
   wbkgd(stdscr, COLOR_PAIR(1)); // Background colour
     
   /* Main game loop */
   
   for(nodelay(stdscr, 1); !gameOver; usleep(30000))
   {      
        //score++; // Rolling score over time
        c.x += c.d; // Sets the centipede movement
        erase(); // Deletes character trail of centipede
              
        /* Add mushrooms */
        
        attron(COLOR_PAIR(3));
        for(int i = 0; i < 30; i++)
        {
            mvprintw(m[i].y, m[i].x, "%d", m[i].health); // Mushrooms, blue
        }  
        attroff(COLOR_PAIR(3));
      
         /* Centipede Direction controls */
         
         if(c.x == xMax-9) // Right edge of screen
         {
          c.d *= -1; // Change direction
          c.y += 1;  // Move down the screen
         }
            else if (c.x == 0) // Left edge of screen
            {
              c.d = 1;
              c.y += 1;
            }
      
          
         /* Centipede collision detection */
          
         if(c.x == p1.x && c.y == p1.y || c.end == -1)
         {
            gameOver = true; // End game if the centipede reaches the player or is shot to death
            endScore = score; // Save the score 
         } 
         
         if (c.x == s.x && c.y == s.y)
         {
            // Reset bullet upon hit
            s.move = false;
            s.x = p1.x;
            s.y = p1.y;
            
            c.body[c.end] = 0; // Remove a body piece if the centipede's head is shot
            c.end -= 1;
            score += 1000;
         }
               for(int i = 0; i < 9; i++)
               {
                   if (c.x+(c.end-i) == s.x && c.y == s.y) 
                  {
                      // Reset bullet upon hit
                     s.move = false;
                     s.x = p1.x;
                     s.y = p1.y;
               
                  
                     c.body[c.end] = 0; // Remove a body piece if the bullet reaches the centipede's body 
                     c.end -= 1;
                     score += 100;
                  }
               }
               
         /* Mushroom collision detection */
         
         for(int i = 0; i < 30; i++)
         {
             // Bullet detection
             
             if (s.x == m[i].x && s.y == m[i].y)
             {
                // Reset bullet upon hit
                s.move = false;
                s.x = p1.x;
                s.y = p1.y;
                
                mvprintw(m[i].y, m[i].x, "~");
                m[i].health--;
                score += 10;
                if (m[i].health == 0)
                { 
                  m[i] = m[i+1]; // Destroys the mushroom
                  score += 20;
                }
             }
             
             // Centipede detection
             
             if (c.d == 1)
             { 
                if(c.x+c.end == m[i].x && c.y == m[i].y)
                { 
                  c.d *= -1; // Change centipede direction
                  c.y += 1;  // Move down the screen
                } 
             }
             else 
             {
               if (c.x == m[i].x && c.y == m[i].y)
               {
                  c.d *= -1; // Change centipede direction
                  c.y += 1; // Move down the screen
               }
             }
          
         }
         
         
         /* Shooting */
         
         if(s.move == true)
         {  
               s.y--;
               attron(COLOR_PAIR(2));
               mvprintw(s.y, s.x, "*");
               attroff(COLOR_PAIR(2));
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
                            
                                    /* Shoot: */
                  case ' ':
                            s.move = true;
                            s.y = p1.y;
                            s.x = p1.x+1;
                            break;
                            
                  case 'p': 
                            gameOver = true;
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
        
        attron(COLOR_PAIR(2));
        mvprintw(p1.y, p1.x, "<o>"); // Player, green 
        attroff(COLOR_PAIR(2));
        mvaddstr(c.y, c.x, c.body); // Centipede, red
        
        mvprintw(0, 0, "Welcome to Centipede | WASD keys to move | Spacebar to shoot | P to exit | Score: %d", score);
        
        for(int i = 0; i < 116; i++)
        {
            mvprintw(38, i, "_"); // Barrier, red
            mvprintw(i, 116, "|");
        }
        
        
        refresh();
   }
   
   endwin(); // End ncurses
   printf("Game over! Your score was %d \n", endScore);
   
   
}
