#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <unistd.h>

int x, y;
int score;
int fruitx, fruity;

typedef struct{
    int x;
    int y;
} point;


// gets position of fruit and prints to the screen, also refreshes score

void get_fruit(WINDOW *win, int width, int height){
    
    fruitx = 0;
    fruity = 0;

    while ( fruitx == 0 || fruity == 0 || fruitx == width -1 ||  fruity == height - 1 || fruitx == x && fruity  == y){
        fruitx = rand() % width;
        fruity = rand() % height;
        }

    mvwprintw(win, fruity, fruitx, "*");
    mvprintw(height, 0, "Score: %d\nEnter q to quit", score);

}


// moves the snake forwards and updates x and to its head

void move_snake(WINDOW *win, point *snake, int direction, int head){
    
    switch(direction) {
        case 0:
            mvwprintw(win, y - 1, x, "0");
            y--;
            break;

        case 1:
            mvwprintw(win, y, x + 1, "0");
            x++;
            break;

        case 2:
            mvwprintw(win, y + 1, x, "0");
            y++;
            break;

        case 3:
            mvwprintw(win, y, x - 1, "0");
            x--;
            break;
    }
    
    //clears snake at head postion in array

    mvwprintw(win, snake[head].y, snake[head].x, " ");

    //prints box to screen and clears input

    box(win, 0, 0);
    int c;
    while ((c = wgetch(win)) != EOF && c != '\n' ){};
}


// takes input and returns a direction, including q to quit

int input(WINDOW *win, int direction){

    keypad(win, true);
    int c = wgetch(win);
    switch (c){
     
    case KEY_UP:
        if (direction == 2){
            return 2;
        }
        return 0; 
     
    case KEY_RIGHT:
        if (direction == 3){
            return 3;
        }
        return 1; 

    case KEY_DOWN:
    if (direction == 0){
        return 0;
    }
        return 2; 

    case KEY_LEFT:
    if (direction == 1){
        return 1;
    }
        return 3;

    case 'q':
        return 4;

    default:
        return direction;

    }
}

//checks if snake has hit a wall

int check_death(int width, int height){

    if ( y == 0 || x == 0 || y == height - 1 || x == width -1){
        return 0;
    }

    else{
        return 1;
    }
}

//checks if snake has eaten a fruit

int check_fruit(){

    if ( y == fruity && x == fruitx){
            return 0;
        }

    else{
        return 1;
        }

}

//checks if snake has hit itself

int check_snake_death(point *snake){
    for (int i = 0; i < score; i++){
        if (snake[i].x == x && snake[i].y == y){
            return 1;
        }
        else{
            continue;
        }
    }
    return 0;
}

//sets initial variable and loops through the game

void play(WINDOW *win, int width, int height){
    
    //starting values. direction is right, speed is how long it waits

    int direction = 1;
    int speedx = 60000;
    int speedy = 110000;
    nodelay(win, TRUE);
    int head = 0;
 
    //initialises snake as array of points

    point *snake;
    snake = malloc(sizeof(point) * (width * height));

    // gets first fruit

    get_fruit(win, width, height);


    //gameplay loop

    while (1){
        
        //adds current head position to snake array, moves head around circular array

        snake[head].x = x;
        snake[head].y = y; 
        head = (head + 1) % (score + 1);


        //gets direction input, if q then quits game

        direction = input(win, direction);
        if (direction == 4){
            break;
        }

        move_snake(win, snake, direction, head);
        
        // because of character width x and y sleep time has to be different

        if (direction == 1 || direction == 3){
            usleep(speedx);
        }
        else{
            usleep(speedy);
        }

        //checks for deaths

        if (! check_death(width, height)){
            break;
        }

        if (check_snake_death(snake)){
            break;
        }

        //if a fruit is eaten update score, speed up the snake and et another fruit

        if (! check_fruit()){
            score++;
            speedx = speedx * 0.98;
            speedy = speedy *0.98;
            get_fruit(win, width, height);
        }
        
        refresh();
        wrefresh(win);

    }

    //clears screen and displays game over screen
    
    wclear(win);
    clear();
    box(win, 0, 0);
    mvwprintw(win, height / 2 - 1, width / 2 - 5, "Game Over!");
    mvwprintw(win, height / 2, width / 2 - 5, "Score: %d", score);
    refresh();
    wrefresh(win);
}


int main(){

    //sets height and width, plus initial x and y

    int width = 40, height = 15;
    x = width / 2 - (height / 2);
    y = height / 2;

    // sets up ncurses screen and window

    initscr();
    noecho();
    curs_set(0);
    WINDOW *win = newwin(height, width, 0, 0);

    play(win, width, height);
    
    //clears input and waits to close
    
    sleep(1);
    int c;
    while ((c = wgetch(win)) != EOF && c != '\n' ){};
    getch();
    endwin();
    return 0;
}
