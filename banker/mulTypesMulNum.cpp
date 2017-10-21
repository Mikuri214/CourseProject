#include<iostream>
#include<string>
#include<fstream>
#include<ctime>

#include<vector>
using namespace  std;


#define RESOURCE 5  //资源种类数
#define PROCESS 5  //进程数


fstream f("/home/quopple/桌面/out.text",ios::out);

bool isFinish( bool finish[]){
    for(int i = 0; i < PROCESS; i++) if(!finish[i]) return false;
    return true;
}


bool isSafe(vector<vector<int>> res_hold, vector<vector<int>> res_demand,vector<int> res_avaid,bool finish[] ){
    while( !isFinish(finish)){
        int i ,j;
        for( i = 0; i < PROCESS; i++){
            if(!finish[i]){
                for( j = 0; j < RESOURCE; j++){
                    if(res_demand[i][j] > res_avaid[j]) break;
                }
                if( j == RESOURCE) break;
            }
        }
        if( i == PROCESS) return false;

        for(j = 0; j < RESOURCE; j++) res_avaid[j] += res_hold[i][j];
        finish[i] = true;
    }

    return true;
}


void init(vector<vector<int>> &res_demand,vector<int> &res_avaid){
    for(int j = 0; j < RESOURCE; j++){
        for(int i= 0 ; i < PROCESS;i++)
        {
           res_demand[i][j] =  rand() % 5;
        }
        res_avaid[j] = rand() % 10;
    }
}


void getRequst(int req[],int &process,bool finish[],const vector<vector<int>>&res_demand){
    do{
        process = rand()%PROCESS;
    }while(finish[process]);

    for(int j = 0; j < RESOURCE; j++){
        req[j] = 0;
        if(res_demand[process][j]!=0)
            req[j] = rand() % res_demand[process][j] +1;}
}


void changeData(vector<int> &res_hold,vector<int> &res_demand,vector<int> &res_avaid,int req[] ){
    for(int i = 0; i < RESOURCE ; i++){
        res_avaid[i] -= req[i];
        res_demand[i] -= req[i];
        res_hold[i] += req[i];
    }
}


void restore(vector<int> &res_hold,vector<int> &res_demand,vector<int> &res_avaid,int req[] ){
    for(int i = 0; i < RESOURCE ; i++){
        res_avaid[i] += req[i];
        res_demand[i] += req[i];
        res_hold[i] -= req[i];
    }
}


void log(int process,int request[],string response){
    f<<process<<"\t";
    for(int i = 0; i < RESOURCE; i++) f<<request[i]<<" ";
    f<<response<<"\n";
}

void banker(vector<vector<int>> res_hold,vector<vector<int>> res_demand,vector<int> res_avaid ){
    bool finish[PROCESS] = {false};
    int process ;
    int requst[RESOURCE];

    while(!isFinish (finish)){
        getRequst (requst,process,finish,res_demand);
        //sleep(1);

        int i;
        for( i = 0; i < RESOURCE && res_avaid[i] >= requst[i]; i++);
        if( i < RESOURCE ) continue;



        changeData (res_hold[process],res_demand[process],res_avaid,requst);

                    cout<<" process:"<<process<<"  requst :";
                   for(int j = 0; j < RESOURCE; j++) cout<<requst[j]<<' ';
                   cout<< endl;
                    for(int i= 0 ; i < PROCESS;i++){
                        for(int j = 0; j < RESOURCE; j++)
                            {
                               cout<<res_demand[i][j]<<' ' ;
                            }
                            cout<<endl;
                        }
                    cout<<"available :";
                    for(int j = 0; j < RESOURCE; j++) cout<<res_avaid[j]<<' ';
                    cout<<endl;

        cout<<(isSafe (res_hold,res_demand,res_avaid,finish)?"safe":"unsafe")<<endl;


        string respose = "refuse";

        if(!isSafe (res_hold,res_demand,res_avaid,finish)){
            cout<<"\nroll back\n";
            restore (res_hold[process],res_demand[process],res_avaid,requst);
        }
        else{
            for( i = 0;i < RESOURCE && res_demand[process][i] == 0;i++);
            if( i == RESOURCE){
                for(i = 0;i < RESOURCE;i++)res_avaid[i] += res_hold[process][i];
                finish[process] = true;
            }

            respose = "success";
        }

//        log(process,requst,respose);
     }
}

int main(){
    vector<vector<int>> res_hold(PROCESS,vector<int>(RESOURCE));
    vector<vector<int>> res_demand(PROCESS,vector<int>(RESOURCE));
    vector<int> res_avaid(RESOURCE);

    srand(time(NULL));
    bool finish[PROCESS];
    do{
        init(res_demand,res_avaid);
        for(bool e: finish) e= false;
    }while(!isSafe (res_hold,res_demand,res_avaid,finish));

    for(int i= 0 ; i < PROCESS;i++){
        for(int j = 0; j < RESOURCE; j++)
            {
               cout<<res_demand[i][j]<<' ' ;
            }
            cout<<endl;
        }
    cout<<"available :";
    for(int j = 0; j < RESOURCE; j++) cout<<res_avaid[j]<<' ';
    cout<<endl<<endl;


    banker(res_hold,res_demand,res_avaid);

    return 0;
}
