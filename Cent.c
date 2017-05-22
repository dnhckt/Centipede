#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

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
   bool reachedEnd;
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

typedef struct spider
{
  int y, x;
  int health;
  bool bounceV, bounceH, dead;
} spider;


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
   init_pair(4, COLOR_MAGENTA, COLOR_BLACK);
   init_pair(5, COLOR_YELLOW, COLOR_BLACK);
   init_pair(6, COLOR_BLACK, COLOR_RED);
   
   int yMax = 44, xMax = 86;
   int score = 0, endScore;
   srand(time(NULL));
   
   /* Game defaults end */
   
   /* intro screen */
   wbkgd(stdscr, COLOR_PAIR(6));
   attron(COLOR_PAIR(6));
   mvprintw(10, xMax/3-2, "@@@ @@@ @@@ @@@  @  @@@ @@@ @@@ @@@");
   mvprintw(11, xMax/3-2, "@   @   @ @  @   @  @ @ @   @ @ @  ");
   mvprintw(12, xMax/3-2, "@   @@@ @ @  @   @  @@@ @@@ @ @ @@@");
   mvprintw(13, xMax/3-2, "@   @   @ @  @   @  @   @   @ @ @  ");
   mvprintw(14, xMax/3-2, "@@@ @@@ @ @  @   @  @   @@@ @@@ @@@");
   mvprintw(15, xMax/3-2, "===================================");
   mvprintw(16, xMax/3-2, "              CONTROLS             ");
   mvprintw(17, xMax/3-2, "===================================");
   mvprintw(18, xMax/3-2, " | w | s  | a  |  d  |space|  p  | ");
   mvprintw(19, xMax/3-2, " *===============================* ");
   mvprintw(20, xMax/3-2, " | up|down|left|right|shoot|quit | ");
   mvprintw(21, xMax/3-2, "===================================");
   mvprintw(22, xMax/3-2, "                                   ");
   mvprintw(23, xMax/3-2, "       Press any key to begin      ");
   mvprintw(24, xMax/3-2, "___________________________________");
   for (int i = 15; i < 25; i++)
   {
      mvprintw(i, xMax/3-3, "|");
      mvprintw(i, xMax/3+33, "|");
   }
   attroff(COLOR_PAIR(6));   
   
   getch();
   /*intro screen end*/
   
   /* Initialise player, centipede, mushrooms and bullets */ 
   
   player p1 = {
                 yMax - 3, xMax/2 // Set player at middle-bottom of screen
               };
               
   pede c = {
              2, 2, 1, "@@@@@@@@@@", strlen(c.body), false // Set centipede at top left 
            };    
           
   pede cSub = {
                  -1, -1, -c.d, "@", strlen(cSub.body), false
               };        
           
   shot s = {
              false
            }; 
             
   mushroom m[30] = {
                     10, 10, 1
                };
   spider a = {
                 42, 5, 5, false
              };
                
       
    for(int i = 0; i < 30; i++)
    {
      m[i].y = i+2; // Set mushrooms on each row
      m[i].x = rand() % 60+10; // Set x position as random
      if (m[i].x == m[i-1].x || m[i].x+1 == m[i-1].x || m[i].x-1 == m[i-1].x)
      {
         m[i].x = rand() % 60+10; // Reroll the xpos of the mushroom if too close to the one before 
      }
      if(m[i].x < 2)
      {
        m[i].x = rand() % 60+10;
      }
    
      m[i].health = 5;                   
    }       
    
   bool gameOver = false; // Used to end the game 
   bool split = false;
   int splitPoint;
   
   wbkgd(stdscr, COLOR_PAIR(1)); // Background colour
     
   /* Main game loop */
   
   for(nodelay(stdscr, 1); !gameOver; usleep(30000))
   {      
         //score++; // Rolling score over time
         c.x += c.d; // Sets the centipede movement
         cSub.x += cSub.d;
         erase(); // Deletes character trail of centipede
        
          /* Add mushrooms */
           
          attron(COLOR_PAIR(3));
          for(int i = 0; i < 30; i++)
          {
              mvprintw(m[i].y, m[i].x, "%d", m[i].health); // Mushrooms, blue
          }  
          attroff(COLOR_PAIR(3));
      
          /* Centipede direction controls */
         
            if (!c.reachedEnd)
            {
                  if (c.x == xMax-c.end && c.d == 1) // Right edge of screen
                  {
                     c.d *= -1; // Change direction
                     c.y += 1;  // Move down the screen
                  }
                     else if (c.x == 2) // Left edge of screen
                     {
                         c.d = 1;
                         c.y += 1;
                     }
                     
                 if (c.y == 42)
                 { 
                     c.reachedEnd = true; // If end of screen reached, reverse
                     c.y -= 1;
                 }
            }
         
         else
            {
               if (c.x == 83-c.end)
               {
                  c.d *= -1;
                  c.y -= 1;
               }
               else if (c.x == 2)
               {
                 c.d = 1;
                 c.y -= 1;
               }
               if (c.y == 39)
               {
                 c.reachedEnd = false;
               }
            
           }
          
         /* Centipede collision detection */
                  
         if(c.x-c.end == p1.x && c.y == p1.y || cSub.x == p1.x && cSub.y == p1.y)
         {
            gameOver = true; // End game if the centipede reaches the player
            endScore = score; // Save the score 
         } 
         
         if(c.end == -1 && cSub.end == -1)
         {
           gameOver = true; // End the game if the centipedes are shot to death
           endScore = score;
         }
         
         if (c.x == s.x && c.y == s.y) // Headshot
         {
            // Reset bullet upon hit
            s.move = false;
            s.x = p1.x;
            s.y = p1.y;
            
            c.end -= 1;
            c.body[c.end] = 0;
            score += 1000;
         }         
         
             // Before centipede split
             if(!split)
             {
             
                     for(int i = 0; i < 9; i++)
                     {
                         if (c.x + (c.end-i) == s.x && c.y == s.y) 
                        {
                           // Reset bullet upon hit
                           s.move = false;
                           s.x = p1.x;
                           s.y = p1.y;
                           c.end -= 1; // Remove the body piece if the bullet reaches the centipede's body
                           c.body[c.end] = 0;
                           
                           split = true;
                           for(int j = 0; j < i; j++)
                           {
                              c.end -= 1;
                              c.body[c.end] = 0;
                              cSub.body[j] = 64;
                           }
                           cSub.x = c.x+c.end;
                           cSub.y = c.y;
                           score += 100;
                        }
                      }
            }
            // If centipede has split
            else 
            {
                     for(int i = 0; i < c.end; i++)
                     {
                         if (c.x + (c.end-i) == s.x && c.y == s.y) 
                        {
                           // Reset bullet upon hit
                           s.move = false;
                           s.x = p1.x;
                           s.y = p1.y;
                           
                           score += 100;
                           c.body[c.end] = 0; // Remove a body piece if the bullet reaches the centipede's body
                           c.end -= 1;
                        }
                     }
                     for(int i = 0; i < cSub.end; i++)
                     {
                        if (cSub.x + (cSub.end-i) == s.x && cSub.y == s.y) 
                        {
                           // Reset bullet upon hit
                           s.move = false;
                           s.x = p1.x;
                           s.y = p1.y;
                           score += 50;
                           
                           cSub.body[cSub.end] = 0;
                           cSub.end -= 1;
                           
                        }
                     }
            }
          
          /* Spider movement */  
          
          // Move across 
        
        if(!a.dead)
        {  
          if (!a.bounceH)
          {
           a.x++;
          }
                else 
                {
                 a.x--;
                }
         
                if (a.x == xMax)
                {
                  a.bounceH = true; 
                }
                
                      else if (a.x == 2)
                      {
                        a.bounceH = false;
                      }
               
           // Move up and down
            
            if(!a.bounceV)
            {
              a.y--;
            }
               else
               {
                a.y++;
               }
                   
            if (a.y == 33)
            {
              a.bounceV = true;
            }
               else if (a.y == 42)
               {
                a.bounceV = false;
               }
        }
          
          /* Spider collision detection */
          
          if (a.y == p1.y && a.x == p1.x)
          {
            score -= 100;
            if (score == -500)
            {
             gameOver = true;
             endScore = score;
            }
          }
          
         if (a.y == s.y && a.x == s.x)
         {
              mvprintw(a.y, a.x, "~");
              a.dead = true;
              a.y = -10;
              a.x = -10;
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
                if(c.x+c.end-1 == m[i].x && c.y == m[i].y)
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
              // Second Centipede 
              
              if (cSub.d == 1)
              {
               if(cSub.x == m[i].x && cSub.y == m[i].y)
               {
                  cSub.d *= -1;
                  cSub.y += 1;
               }
              }
              else 
              {
               if (cSub.x == m[i].x && cSub.y == m[i].y)
               {
                  cSub.d *= -1; 
                  c.y += 1;
               
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
           case 43: 
                   p1.y--; // Lower
                   break; 
         } 
         
         /* Horizontal player boundaries */
         switch (p1.x)
         {
            case 1: 
                   p1.x++; // Left
                   break;
            case 83:
                   p1.x--; // Right
                   break;
         }
     
        /* Print and display characters */
        
        attron(COLOR_PAIR(2));
        mvprintw(p1.y, p1.x, "<o>"); // Player, green 
        attroff(COLOR_PAIR(2));
        mvaddstr(c.y, c.x, c.body); // Centipede, red
        attron(COLOR_PAIR(a.health));
        mvprintw(a.y, a.x, "X");
        attroff(COLOR_PAIR(a.health));
        
        /* Print second centipede */
        if (split)
        {            
                     attron(COLOR_PAIR(4));
                     mvaddstr(cSub.y, cSub.x , cSub.body); // Magenta
                     attroff(COLOR_PAIR(4));
                     
      
                            
                            
                       if (!cSub.reachedEnd)
                        {
                        
                        
                               if(cSub.x == xMax-cSub.end && cSub.d == 1) // Right edge of screen
                                 {
                                  cSub.d *= -1; // Change direction
                                  cSub.y += 1;  // Move down the screen
                                 }
                           
                               else if (cSub.x == 2) // Left edge of screen
                              {
                                cSub.d = 1;
                                cSub.y += 1;
                              } 
                     
                                 
                             if (cSub.y == 42)
                             { 
                                 cSub.reachedEnd = true; // If end of screen reached, reverse
                                 cSub.y -= 1;
                             }
                        }
                     
                     else
                        {
                           if (cSub.x == 83)
                           {
                              c.d *= -1;
                              c.y -= 1;
                           }
                           else if (cSub.x == 2+c.end)
                           {
                             cSub.d = 1;
                             c.y -= 1;
                           }
                           if (cSub.y == 39)
                           {
                             cSub.reachedEnd = false;
                           }
                        
                       }  
        }      
        
        mvprintw(0, 2, "Welcome to Centipede | WASD keys to move | Spacebar to shoot | P to exit | Score: %d", score);
        mvprintw(1, 1, " ------------------------------------------------------------------------------------");
        
        for(int i = 2; i < xMax; i++)
        {
            mvprintw(39, i, "_"); // Barrier, red
            mvprintw(i, xMax, "|");
            mvprintw(i, 1, "|");
            mvprintw(43, i, "-");
        }
        
        
        refresh();
   }
   
   endwin(); // End ncurses
   printf("Game over! Your score was %d \n", endScore);
}
