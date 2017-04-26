/*
    Centipede
    =========   
    Author: Aidan Hackett
    
    About: Centipede is a fixed shooter arcade game produced by Atari in 1980.
           This is a recreation of the game in C using the ncurses library.
           To play, simply compile and run this file.
 */
 
#include <ncurses.h>
#include <unistd.h>


int main(void)
{
    
    // Variables for x&y movement of Centipede
    int direction = 1; // Initialise movement to the left
    int cXPos, cYPos = 0; // Begin at the top left corner of the screen
    int yMax, xMax = 0; // The edges of the terminal window
    int xNext = 0; // Next xposition for collision detection
    

    initscr();
    noecho();
    curs_set(FALSE);
    
    
    //  Loop to make centipede descend 
        while(true)
        {
            getmaxyx(stdscr, yMax, xMax);

            clear();
            mvprintw(cYPos, cXPos, "}@@@@@@@@{");  
            refresh();   
            usleep(30000);
            
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
        } 
        
    
        endwin();
}
