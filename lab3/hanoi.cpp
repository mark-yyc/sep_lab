#include "termio.h"
#include "stack.h"
#include <iostream>
#include <sstream>

using namespace std;

// Do not delete the following line
char Termio::buffer[Termio::CANVAS_HEIGHT][Termio::CANVAS_WIDTH + 1];

class Hanoi{
private:
    struct Log{
        int from;
        int to;
        Log(const int &f,const int &t){
            from=f;
            to=t;
        }
        Log(){}
    };
    int num;
    Stack<int> rods[3];
    Stack<Log> commands;
public:
    Hanoi(const int &n){
        num=n;
        for (int i=n;i>0;i--){
            rods[0].push(i);
        }
    }

    ~Hanoi(){}

    void move(const int &f,const int &t,const int &type){
        if(rods[f-1].isEmpty()){
            draw();
            return;
        }
        if(rods[t-1].isEmpty()||rods[f-1].top()<rods[t-1].top()){
            rods[t-1].push(rods[f-1].pop());
            if(type==1){
                store_log(f,t);
//                Termio::Clear();
                draw();
            }
            else{
                cout << "Auto moving:"<<f<<"->"<<t << endl;
                draw();
            }
        }
        else{
//            Termio::Clear();
            draw();
        }
    }

    void store_log(const int &f,const int &t){
        Log command(f,t);
        commands.push(command);
    }

    void restore(){
        while(!commands.isEmpty()){
            Log tmp=commands.pop();
            move(tmp.to,tmp.from,0);
        }
    }

    void hanoi(int n,int A,int B,int C){
        if(n>0){
            hanoi(n-1,A,C,B);
            move(A,B,0);
            hanoi(n-1,C,B,A);
        }
    }

    void automove(){
        restore();
        hanoi(num,1,2,3);
        check();
    }

    void draw(){
//        Termio::Clear();
        Termio::ResetBuffer();
        Stack<int>tmp;
        int n=0,num,middle;
        for (int i=0;i<=Termio::CANVAS_WIDTH;i++){
            Termio::buffer[Termio::CANVAS_HEIGHT-1][i]='-';
        }
        for (int j=0;j<Termio::CANVAS_HEIGHT;j++){
            Termio::buffer[j][5]='|';
            Termio::buffer[j][20]='|';
            Termio::buffer[j][35]='|';
        }
        for(int k=0;k<3;k++){
            middle=5+k*15;
            while(!rods[k].isEmpty()){
                tmp.push(rods[k].pop());
            }
            while(!tmp.isEmpty()){
                num=tmp.pop();
                rods[k].push(num);
                int i=Termio::CANVAS_HEIGHT-2-2*n;
                Termio::buffer[i][middle]='*';
                for(int j=1;j<=num;j++){
                   Termio::buffer[i][middle+j]='*';
                   Termio::buffer[i][middle-j]='*';
                }
                n++;
            }
            n=0;
        }
        Termio::Draw();
    }

    bool check(){
        if(rods[0].isEmpty()&&rods[2].isEmpty()){
                std::cout << "Congratulations! You win!" << std::endl;
                return true;
        }
        return false;
    }
};

void write_hi() {
    for (int i = 0; i < 5; i++) {
        Termio::buffer[i][0] = '|';
        Termio::buffer[i][5] = '|';
        Termio::buffer[i][9] = '|';
        Termio::buffer[i][13] = '*';
    }
    Termio::buffer[3][13] = ' ';
    Termio::buffer[0][9] = ' ';
    for (int i = 1; i < 5; i++) {
        Termio::buffer[2][i] = '-';
    }
}

int main() {
    string cmd;
    while(true){
        std::cout << "How many disks do you want? (1 ~ 5)" << std::endl;
        getline(cin, cmd);
        if (cmd == "Q")
            break;
        stringstream ss(cmd);
        int num;
        ss>>num;
        if(num>=1&&num<=5){
        Hanoi h(num);
        h.draw();
        while(true){
            std::cout << "Move a disk. Format: x y" << std::endl;
            getline(cin, cmd);
            int from,to;
            stringstream ss(cmd);
            ss>>from>>to;
            if(from==0&&to==0){
                h.automove();
                break;
            }
            if((from>=1&&from<=3)&&(to>=1&&to<=3)){
                h.move(from,to,1);
                if(h.check()){
                    break;
            }
        }
            else{
                h.draw();
            }
        }
        }
    }
    return 0;
}
