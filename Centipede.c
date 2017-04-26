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

#define DELAY 30000

int main(void)
{
    int x, y = 0;
    
    initscr();
    noecho();
    curs_set(FALSE);
    
    while(true)
    {
        clear();
        mvprintw(y, x, ")");
        refresh();    
        
        usleep(DELAY);   
        x++;
    }
        
        
        endwin();
}
