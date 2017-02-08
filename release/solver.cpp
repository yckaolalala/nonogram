#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<cstring>
#include "nonogram.h"
using namespace std;

vector<int> rule[60];
int cnt=0;

void Readfile(char *filename);
void Solver(Nono data,int value,int index);

int main(int argc,char **argv){

    if(argc!=2){
        cout<<"usage: filename\n";
        exit(2);
    }
    Readfile(argv[1]);
    Nono data(cnt);
    Init_possible(rule,&data);
    
    Solver(data,0,-1);
    cout<<"solve fail"<<endl;
    
    return 0;
}

void Solver(Nono data,int value,int index){
    int flag=1; 

    if(index!=-1){
        Fill_map(&data,index,value,1);
        Fill_map(&data,index,~value,-1);
        data.finish[index]=true;   
    }

    while(flag){
        int pre_one_set[60];
        int pre_zero_set[60];
        flag=0;
        for(int i=0;i<cnt;i++){
            if(data.finish[i]==true)
                continue;
            int one_set=0;
            int zero_set=0;
            int index=0;
            while(index<(int)data.possible[i].size()){
                while((index<(int)data.possible[i].size())&&!Check_match(&data,data.possible[i][index],i)){     
                    data.possible[i][index]=data.possible[i][(int)data.possible[i].size()-1];
                    data.possible[i].pop_back();

                }
                index++;
            }

            for(int j=0;j<(int)data.possible[i].size();j++){
                if(j==0){
                    one_set=data.possible[i][j];
                    zero_set=~data.possible[i][j];
                }else{
                    one_set&=data.possible[i][j];
                    zero_set&=~data.possible[i][j];
                }
            }
            if(one_set!=pre_one_set[i] || zero_set!=pre_zero_set[i])
                flag=1;
            pre_one_set[i]=one_set;
            pre_zero_set[i]=zero_set;
            
            Fill_map(&data,i,one_set,1);
            Fill_map(&data,i,zero_set,-1);

            if(Check_row_finish(rule,&data,i))
                data.finish[i]=true;

        }
    }
    
    if(Check_finish(&data)){
        cout<<"solve success"<<endl;
        Show_map(&data);
        exit(0);
    }else{
        int min=Get_min(&data);
        for(int j=0;j<(int)data.possible[min].size();j++)          
                Solver(data,data.possible[min][j],min);
        
    }

}



void Readfile(char *filename){
    FILE* fp=fopen(filename,"r");
    //int argc;
    char buff[100];
    if(fp){
        while(fgets(buff,100,fp)!=NULL){
            char *arg;
            int argc=0;
            char *argv[60];
            arg=strtok(buff," \n\t");
            while(arg!=NULL){
                argv[argc]=new char[strlen(arg)+1];
                strcpy(argv[argc],arg);
                argc++;
                arg=strtok(NULL," \n\t");
            }

            for(int i=0;i<argc;i++)
                rule[cnt].push_back(atoi(argv[i]));
            cnt++;
            for(int i=0;i<argc;i++)
                delete [] argv[i];
        }

        fclose(fp);
    }
    else{
        cout<<"file not exit"<<endl;
        exit(1);
    }
}
