#include <ncurses.h>
#include <stdlib.h>
#include <unistd.h> 

typedef struct Player
{
  int userX, userY;
} Player;

typedef struct Centipede
{
   int centX, centY;
   char segments[10];
} Centipede;

int UI();

Player * userSettings();
Centipede * centiSetup();

int pedeCrawl(Centipede * pede);
int handleInput(int input, Player * player1);
int moveUser(int y, int x, Player * player1);

int yMax = 42;

/* Main game */

int main()
{    

    // User + movement
    Player * player1;
    int ch;
    
    // Centipede 
    Centipede * pede; 
    
    // Setup ncurses
    UI();
      
    player1 = userSettings(yMax - 2, xMax/2);
    pede = centiSetup(1, 1, 1);
    
  while(true)
  {     
        /*User input loop */
        while(true)
        {
         ch = getch();
         handleInput(ch, player1);
         pedeCrawl(pede);
        }  
   }   
      endwin();
//    return 0;
}

/* Functions */

int UI()
{
    initscr();
    printw("Centipede | Use the WSAD keys to move | Press Ctrl-Z to Quit");
    noecho();
    curs_set(FALSE);  
    
    refresh(); 
    return 0;
}

Centipede * centiSetup(int xPos, int yPos, int direction)
{
   Centipede * newPede;
   newPede = malloc(sizeof(Centipede));
      
   newPede->centX = xPos;
   newPede->centY = yPos;
   mvprintw(1, 1, "}@{");
    
   return newPede; 
}

int pedeCrawl(Centipede * pede)
{
  int next;
  int direction = 1;
  
    if(pede->centX < 116)
    {
    usleep(30);
    mvprintw(pede->centY, pede->centX, "");
    mvprintw(pede->centY, pede->centX+1, "}@{");
    }
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

