#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <thread>
#include <chrono>
//#include <pthread.h>
//#include <cstdlib>
using namespace std;

int window_height = 20,window_width = 20;
int shooter_x = window_width/2, shooter_y = window_height - 1;
int star_x, star_y = 0;
int gameOver = 1;
char shooter_dir;
bool kbhit()
{
    termios term;
    tcgetattr(0, &term);

    termios term2 = term;
    term2.c_lflag &= ~ICANON;
    tcsetattr(0, TCSANOW, &term2);

    int byteswaiting;
    ioctl(0, FIONREAD, &byteswaiting);

    tcsetattr(0, TCSANOW, &term);

    return byteswaiting > 0;
}

int getch(void)
{
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDIN_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
}

void setup(){
	for(int i = 0; i < window_width; i++)
		cout << "#";
	cout << endl;
	
	for(int i = 0; i < window_height; i++){
		for(int j = 0;  j < window_width; j++)
			if(j == 0 || j == window_width-1)
				cout << "#";
			else if(i == shooter_y && j == shooter_x)
				cout << "^";
			else if(i == star_y && j == star_x)
				cout << "*";
			else
				cout << " ";
		cout<<endl;
	}
	
	for(int i = 0; i < window_width; i++)
		cout << "#";
	cout << endl;		
}

void shooter_navigation(){
	if(kbhit()){
		shooter_dir = getch();
		
		switch(shooter_dir){
			case 'n': if(shooter_x > 1)
				  	shooter_x--;
				   break;
			case 'm': if(shooter_x < window_width - 2)
				  	shooter_x++;
				   break;
		}
	}

}

void star_navigation(){
	star_y ++;	
	this_thread::sleep_for(chrono::milliseconds(200));
}

int main(){
	star_x = (rand() % (window_width - 1)) + 1;
	setup();
	thread thread1, thread2;
	while(gameOver){
		thread1 = thread(&shooter_navigation);
		thread2 = thread(&star_navigation);
		setup();
		thread1.detach();		
		thread2.join();
		//usleep(500000);
		system("clear");
	}			
}
