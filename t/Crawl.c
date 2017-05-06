
#include <ncurses.h>
#include <unistd.h>

int main(void)
{
    
    // Variables for x&y movement of Centipede
    int direction = 1; // Initialise movement to the left
    int cXPos, cYPos = 0; // Begin at the top left corner of the screen
    int xNext; // Next x position for collision detection
   
    int xMax, yMax; // Screen boundaries

    initscr();
    noecho();
    cbreak();
    curs_set(FALSE);  

    refresh(); 
    
    // Loop in which the game will play 
    while(true)
    {

            // Get edge boundaries,set user position
            getmaxyx(stdscr, yMax, xMax);

            
          
                    /* Display centipede and make it descend */
                    clear();
                    mvprintw(cYPos, cXPos, "}@@@@@@@@{"); 
                    refresh();
                    usleep(20000);
                    xNext = cXPos + direction;
                            
                    
                    if (xNext >= xMax-9 || xNext < 0)
                    {
                        direction *= -1;
                        cYPos++;
                    } 
                    else 
                    {
                        cXPos += direction;
                    }       
            
            endwin();
        
     }
}


/*

WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW * user;
    
    user = newwin(height, width, starty, startx);
    wprintw(user,"<0>");
    wrefresh(user);
    
    return user;
    
}
    
void destroy_win(WINDOW *user)
{

    wborder(user,' ', ' ', ' ',' ',' ',' ',' ',' ');
    wrefresh(user);
    delwin(user);
    
}
*/



