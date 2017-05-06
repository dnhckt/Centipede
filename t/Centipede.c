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
typedef struct Player
{
  int userX, userY;
} Player;


int UI(); // Game defaults
 
Player * userSettings(); // player defaults 
int handleInput(int input, Player * player1); // Controls user input
int moveUser(int y, int x, Player * player1); // Moves the player

// Edges of the game window for this screen 
int yMax = 42;
int xMax = 116;

/* Main game */

int main()
{    

    // User + movement
    Player * player1;
    int ch;
    
    // Centipede 
    char Pede[10] = "}@@@@@@@@{";
    // Variables for x&y movement of Centipede
    int direction = 1; // Initialise movement to the left
    int cXPos, cYPos = 1; // Begin at the top left corner of the screen
    int xNext; // Next x position for collision detection
   
    UI(); // Setup ncurses
      
    player1 = userSettings(yMax - 2, xMax/2); //Set up player character

        /*Game loop*/
        while(true)
        {  
        
              // Print centipede 
              mvprintw(cYPos, cXPos, Pede);
              usleep(30000);  
                        
              xNext = cXPos + direction;
                            
                    
              if (xNext >= xMax-9 || xNext < 0)
              {
               direction *= -1;
               cYPos++;
              } else {cXPos += direction;}  
       
        
             // Takes user input
             if(ch != ERR)
             {    
                ch = getch();
                handleInput(ch, player1);
             }
        }  
  
      endwin();
}

/* Functions */

int UI()
{
    initscr(); 
    printw("Centipede | Use the WSAD keys to move | Press Ctrl-Z to Quit");
    noecho();
    curs_set(FALSE);  
    
    refresh(); 
}



Player * userSettings(int yPos, int xPos)
{
   Player * newUser;
   newUser = malloc(sizeof(Player));
   
   newUser->userY = yPos;
   newUser->userX = xPos; 
   
   mvprintw(newUser->userY, newUser->userX, "<0>");
   
   return newUser;
}


int handleInput(int input, Player * player1)
{
    switch(input)
    {

        case 'a':
                        moveUser(player1->userY, player1->userX - 1, player1);  
                        break;
                     
        case 'd': 
                        moveUser(player1->userY, player1->userX + 1, player1);                        
                        break;
        case 'w':
                        moveUser(player1->userY - 1, player1->userX, player1);
                        break;
        case 's':
                        moveUser(player1->userY + 1, player1->userX, player1);
                        break;
        default: 
                break;
        
    }

}

int moveUser(int yPos, int xPos, Player * player1)
{
    if (yPos <= yMax && yPos >= 37 && xPos <= xMax-1 && xPos >= 1)
    {
      mvprintw(player1->userY, player1->userX, "   ");
    
      player1->userY = yPos;
      player1->userX = xPos;
  
      mvprintw(player1->userY, player1->userX, "<0>");
    }
}

