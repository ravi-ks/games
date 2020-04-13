//author - ravi.
//no bitching.
#include <bits/stdc++.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <thread>
#include <chrono>
#include <time.h>
#include <stdlib.h>
#include <fstream>

using namespace std;

string name;
string trash;
int ith_star, ith_bullet;
int lifes = 3;
int window_height, window_width;
int shooter_x, shooter_y;
bool gameOver = false;
char shooter_dir;
int score = 0, star_speed_count = 3, star_deployment_speed = 64, star_speed_count1 = star_deployment_speed;
int isBullet_verdict, isStar_verdict;

typedef struct bullets_struct{
	int x, y;
	bool isDead;
}bullets;

bullets bullet[1000];
int bullet_count = 0;

typedef struct stars_struct{
	int x, y;
	bool isDead;
}stars;

stars star[500];
int star_count = 0;


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

int isBullet(int j1, int i1){
	for(int i = 0; i < bullet_count; i++){
		if((!bullet[i].isDead) && bullet[i].x == j1 && bullet[i].y == i1){
			bullet[i].y--;
			ith_bullet = i;
			if(bullet[i].y < 0)
			bullet[i].isDead = true;
			return 1;
		}
	}
	return 0;
}

int isStar(int j1, int i1){
	for(int i = 0; i < star_count; i++){
		if((!star[i].isDead) && star[i].x == j1 && star[i].y == i1){
			ith_star = i;
			return 1;
		}
	}
	return 0;
}


void setup(){
	system("clear");
	for(int i = 0; i < window_width + 2; i++)
		cout << "#";
	cout << endl;
	
	for(int i = 0; i < window_height; i++){
		for(int j = 0;  j < window_width + 2; j++){
			isBullet_verdict = isBullet(j, i);
			isStar_verdict = isStar(j, i);
			if(j == 0 || j == window_width + 1)
				cout << "#";
			else if(i == shooter_y && j == shooter_x)
				cout << "^";
			else if((isStar(j, i - 1) && isBullet_verdict) || (isStar(j, i + 1) && isBullet_verdict) || (isStar_verdict && isBullet_verdict)){ 
				star[ith_star].isDead = true;
				bullet[ith_bullet].isDead = true;
				cout << " ";
				score += 10;
				if(star_deployment_speed > 32 && score % 50 == 0)
				star_deployment_speed -= 8;
				if(star_count > 450 || bullet_count > 800)
				star_deployment_speed = 25;
			}
			else if(isBullet_verdict)
				cout << "!";
			else if(isStar_verdict)
				cout << "*";
			else
				cout << " ";
			}
		cout<<endl;
	}
	
	for(int i = 0; i < window_width + 2; i++)
		cout << "#";
	cout << endl;
	
	cout<<"score ~"<<score<<endl;	
	cout << "Total life left: " << lifes << endl;	
}

void shooter_navigation(){
	if(kbhit()){
		shooter_dir = getch();
		
		switch(shooter_dir){
			case 'n': if(shooter_x > 1)
				  	shooter_x--;
				   break;
			case 'm': if(shooter_x < window_width)
				  	shooter_x++;
				   break;
			case 'a':  bullet[bullet_count].x = shooter_x;
				   bullet[bullet_count].y = shooter_y - 1;
				   bullet[bullet_count].isDead = false;
				   bullet_count++;
				   break;
		}
	}

}

void star_navigation(){
	if(star_speed_count == 4){
	star_speed_count = 0;
	for(int i = 0; i < star_count; i++)
		if(!star[i].isDead){
		star[i].y++;
		if(star[i].y > window_height - 1){
		star[i].isDead = true;
		gameOver = true;
		}
		}
	}
	
	if(star_speed_count1 == star_deployment_speed){
	star_speed_count1 = 0;
	star[star_count].x = (rand() % (window_width - 1 + 1)) + 1;
	star[star_count].y = 0;
	star[star_count].isDead = false;
	star_count++;
	}	
}

void play_music(){
	while(1)
	system("paplay game-background-halo.wav");
}

void timer(){
		system("clear");
		system("figlet -c -k 3");
		sleep(1);
		system("clear");
		system("figlet -c -k 2");
		sleep(1);
		system("clear");
		system("figlet -c -k 1");
		sleep(1);
		system("clear");
}

void welcome_script(){
		system("clear");
		system("figlet -c -k you are playing . . .");
		sleep(6);
		cout << "\n";
		system("figlet -c -k S u r v i v a l  1 . 0");
		sleep(10);
		cout << " Instructions: press 'n' to move left, 'm' to move right and 'a' to shoot.\n Enter name: ";
		cin >> name;
		sleep(3);
		timer();
}

int main(int argc, char* argv[]){
	int  flag = 1;
	if(argc != 3){
		cout << "Usage: ./shooter window_height window_width\nRecommended height and width is 20, 20.\n" ;
		return 0;
	}
	window_height = atoi(argv[1]);
	window_width = atoi(argv[2]);
	shooter_x = window_width/2, shooter_y = window_height - 1;
	
	thread thread1(play_music);//comment_if_windows
    	ofstream file_out("shooter_game_logs.txt", ios::app);
        welcome_script();
        file_out << name << " ";
	setup();
	while(flag){
		shooter_navigation();
		star_navigation();	
		setup();
		star_speed_count++;
		star_speed_count1++;
		usleep(40000);
		if(gameOver && lifes > 0){
		lifes--;
		gameOver = false;
		if(lifes < 1){
		flag = 0;
		break;
		}
		sleep(1);
		timer();
		}
	}
	system("clear");
	cout << "Game Over, " << name << "\nScore: " << score << endl;
	file_out << score << endl;
	file_out.close();
	while(1){
	cout << "ctrl+z to exit." << endl;
	cin >> trash;
	}
	thread1.detach();	
	return 0;	
}

//6,26,31
