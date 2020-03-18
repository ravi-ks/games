    //author - ravi
    
    #include <bits/stdc++.h>
    #include <stdio.h>
    //#include </home/ravi_kumar/Downloads/conio.h>
    //#include <ncurses.h>
    #include <unistd.h>
    #include <termios.h>
    #include <sys/select.h>
    #include <stropts.h>
    #include <sys/ioctl.h>
    //#include <thread>
    #include <fstream>
    #include <chrono>
    #include <ctime>  
    using namespace std;
    
    char undefined_move;
    
    string name;
    
    bool gameOver;

    char ch, buff;
    
    int score, speed;

    int snake_x, snake_y, prey_x, prey_y; //snake_x is window_width and snake_y is window_height.
    
   /* namespace dir_namespace{
        enum direction {stop=0,up, down, left, right};
    }
    
    ::dir_namespace::direction dir = ::dir_namespace::direction::stop ; //:: - global namespace
*/
    char dir;
    bool is_head;
    const int window_height=20, window_width=40;
   
   struct snake_structure{
      int x, y;
      //, prev_dir;
      //int change_in_dir;//did this node change its direction.
   }snake[window_width*window_height];
   
   int snake_len = 1;
   
   int _kbhit() {
    static const int STDIN = 0;
    static bool initialized = false;

    if (! initialized) {
        // Use termios to turn off line buffering
        termios term;
        tcgetattr(STDIN, &term);
        term.c_lflag &= ~ICANON;
        tcsetattr(STDIN, TCSANOW, &term);
        setbuf(stdin, NULL);
        initialized = true;
    }

    int bytesWaiting;
    ioctl(STDIN, FIONREAD, &bytesWaiting);
    return bytesWaiting;
}
   
   int _getch(void)
   {
    struct termios oldattr, newattr;
    int ch;
    tcgetattr( STDERR_FILENO, &oldattr );
    newattr = oldattr;
    newattr.c_lflag &= ~( ICANON | ECHO );
    tcsetattr( STDIN_FILENO, TCSANOW, &newattr );
    ch = getchar();
    tcsetattr( STDIN_FILENO, TCSANOW, &oldattr );
    return ch;
   }
   
    void setup(){
        snake[0].x = window_width / 2;
        snake[0].y = window_height / 2; //initial co-ordinates of the snake.
        prey_x = (rand() % (window_width - 1 + 1)) + 1;
        prey_y = (rand() % (window_height - 1)) + 1;
        score = 0;
        speed = 500000;
        //prey_x=prey_y=0;
        gameOver = false;
    }
    
    int is_snake_pos(int i1, int j1, int start){
    for(int i=start;i<snake_len;i++)
    if(snake[i].y == i1 && snake[i].x == j1){
    if(i == 0)
    is_head = true;
    return 1;
    }
    return 0;      
    }
    
    void architecture(){
        //architecture.
        int flag;
        system("clear");
        for(int i = 0; i < window_width + 2; i++)
            cout<<"#";

        cout<<"\n";
        int k = 0;
        for(int i = 0; i < window_height; i++){
            for(int j = 0;j < window_width + 2; j++){
                //cout<< is_snake_pos(i, 'y') << " " <<is_snake_pos(j, 'x')<<endl;
                if(j == 0 || j == window_width + 1)
                    cout<<"#";
                else if(is_snake_pos(i,j,0))
                	if(is_head){
                	cout << "0";
                	is_head = false;
                	}
                	else
                	cout << "o";
               /* for(int l = 0; l < snake_len;l++)
                if(snake[l].y == i && snake[l].x == j){
                //(is_snake_pos(i, 'y') && is_snake_pos(j, 'x')){
                    cout<<"s";
                    k++;
                }*/
                else if(i == prey_y && j == prey_x)
                    cout<<"*";
                else
                    cout<<" ";
                
                
            }
            cout<<"\n";
        }
                    

        for(int i = 0; i < window_width + 2; i++)
            cout<<"#";
        
        cout<<"\n";
        //ending architecture.
        cout<<"score ~"<<score<<endl;
        
        
    }
    
    void set_snake_dir(int i){
        switch(dir){
                        case('u'):snake[i].y = snake[i-1].y + 1;
                                  snake[i].x = snake[i-1].x;
                                  break;
                        case('d'):snake[i].y = snake[i-1].y - 1;
                                  snake[i].x = snake[i-1].x;
                                  break;
                        case('l'):snake[i].y = snake[i-1].y;
                                  snake[i].x = snake[i-1].x + 1;
                                  break;
                        case('r'):snake[i].y = snake[i-1].y;
                                  snake[i].x = snake[i-1].x - 1;
                                  break;
                        }
    }
    
    void input(){
           if(_kbhit()){
            ch=_getch();
            //snake[0].change_in_dir = 1;
            if((ch != 'w' && ch != 'a' && ch != 's' && ch != 'd')||(ch == undefined_move))
            ch = buff;
            }
           else{
            ch = buff;
            //snake[0].prev_dir = snake[0].dir;
            //snake[0].change_in_dir = 0;
           }
           
           for(int i = snake_len - 1; i >= 1; i--){
           snake[i].x = snake[i-1].x;
           snake[i].y = snake[i-1].y;
           }
           
            switch(ch){
                case 'w': snake[0].y--;
                          dir = 'u';//up
                          undefined_move = 's';
                          break;
                case 'a': snake[0].x--;//left
                          dir = 'l';
                          undefined_move = 'd';
                          break;
                case 's': snake[0].y++;//down
                          dir = 'd';
                          undefined_move = 'w';
                          break;
                case 'd': snake[0].x++;//right
                          dir = 'r';
                          undefined_move = 'a';
                          break;
                default : break;
           }
           
           buff = ch;
           
           if(is_snake_pos(snake[0].y, snake[0].x, 1))
           gameOver = true;
           
           /*if(snake[i-1].change_in_dir == 1)
                snake[i].dir = snake[i-1].prev_dir;
           else
                snake[i].dir = snake[i-1].dir;
                switch(snake[i].dir){
                        case('u'):snake[i].y--;
                                  break;
                        case('d'):snake[i].y++;
                                  break;
                        case('l'):snake[i].x--;
                                  break;
                        case('r'):snake[i].x++;
                                  break;
                        }
           }
*/
        if(snake[0].x < 1 || snake[0].y < 0 || snake[0].x > window_width || snake[0].y > window_height - 1)
         gameOver = true;
        if(snake[0].y == prey_y && snake[0].x == prey_x){
         score += 10;
         snake_len++;
         set_snake_dir(snake_len - 1);
         //snake[snake_len-1].dir = snake[snake_len-2].dir;
         prey_x = (rand() % (window_width - 1 + 1)) + 1;
         prey_y = (rand() % (window_height - 1)) + 1;
         //if(score == 60){
         //cout << prey_x << " " << prey_y << endl;
         //usleep(1000000);
         //}
         if(speed != 100000)
         speed -= 50000;
        }
    }
        void music(){
        while(1)
                system("paplay snake_game_background.wav");
        }
        
    void welcome_script(){

    	system("figlet -c -k SNAKE");
        usleep(1000000);
        system("figlet -c -k     GAME");
        usleep(3000000);
        cout << "Name: ";
        cin >> name;
        system("clear");
                system("figlet -c -k     3");
                usleep(1000000);
                system("clear");
                system("figlet -c -k     2");
                usleep(1000000);
                system("clear");
                system("figlet -c -k     1");
                usleep(1000000);
                system("clear");
    }
    int main(){
    	auto start_time = chrono::system_clock::now();
    	ofstream file_out("snake_game_logs.txt", ios::app);
        //thread thread1(music);
        welcome_script();
        setup();
        architecture();
        while(!gameOver){
            input();
            architecture();
            usleep(speed);
            }
            cout<<"Game Over!!!, " << name << endl;
            auto end_time = chrono::system_clock::now();
            time_t _end_time_ = chrono::system_clock::to_time_t(end_time);
            file_out <<name << " " << score << " " << ctime(&_end_time_) << "\n";
            //char ctrl_z = '^Z';
            //system("^Z");
            return 0;
        }
    

    
