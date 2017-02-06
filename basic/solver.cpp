#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<cstring>
#include<bitset>
using namespace std;

#define debug 0

vector<int> possible[60];
vector<int> rule[60];
bool finish[60];
int map[30][30];
int cnt=0;
int pre_one_set[60];
int pre_zero_set[60];

void Readfile(char *filename);
void Init_possible();
void List_possible(int data,int pos,int i,int j);
int Check_match(int value,int index);
int Check_row_finish(int index);
int Check_finish();
void Fill_map(int index,int value,int sign);
void Show_map();
void Debug_map(int index);
void Show_rule();
void Solver();

int main(int argc,char **argv){

    if(argc!=2){
        cout<<"usage: filename\n";
        exit(2);
    }
    Readfile(argv[1]);
    for(int i=0;i<cnt;i++)
        finish[i]=false;

    Init_possible();
    Solver();
    if(Check_finish())
        cout<<"solve success"<<endl<<endl;
    else
        cout<<"solve fail"<<endl<<endl;
    Show_map();
    return 0;
}




void Solver(){
    int flag=1;
    while(flag){
        flag=0;
        for(int i=0;i<cnt;i++){
            if(finish[i]==true)
                continue;
            int one_set=0;
            int zero_set=0;
            int index=0;

            while(index<(int)possible[i].size()){
                while((index<(int)possible[i].size())&&!Check_match(possible[i][index],i)){     
                    possible[i][index]=possible[i][(int)possible[i].size()-1];
                    possible[i].pop_back();
                }
                index++;
            }


            for(int j=0;j<(int)possible[i].size();j++){
                if(j==0){
                    one_set=possible[i][j];
                    zero_set=~possible[i][j];
                }else{
                    one_set&=possible[i][j];
                    zero_set&=~possible[i][j];
                }
            }

            if(one_set!=pre_one_set[i] || zero_set!=pre_zero_set[i])
                flag=1;
            pre_one_set[i]=one_set;
            pre_zero_set[i]=zero_set;

            Fill_map(i,one_set,1);
            Fill_map(i,zero_set,-1);

            if(Check_row_finish(i))
                finish[i]=true;

            if(debug){
                if(!Check_row_finish(i))
                    cout<<i<<" not finish"<<endl;
                Debug_map(i);
                char t;
                scanf("%c",&t);
            }
        }
    }
}

void Fill_map(int index,int value,int sign){
    for(int j=cnt/2-1;j>=0;j--){
        if(value%2!=0){
            if(index<cnt/2)
                map[index][j]=sign;
            else
                map[j][index-cnt/2]=sign;
        }
        value=value>>1;
    }
}

void Show_map(){
    for(int i=0;i<cnt/2;i++){
        for(int j=0;j<cnt/2;j++)
            if(map[i][j]==1)
                cout<<"1";
            else
                cout<<"0";
        cout<<endl;
    }

}

int Check_finish(){
    for(int i=0;i<cnt;i++)
        if(finish[i]==false)
            return 0;
    return 1;
}
int Check_row_finish(int index){
    int count,pos=0;
    for(int i=0;i<cnt/2;i++){
        count=0;
        while((index<cnt/2 && map[index][i]==1)||((index>=cnt/2)&&map[i][index-cnt/2]==1)){
            count++;
            i++;
        }
        if(count!=0){
            if(pos>=(int)rule[index].size()||count!=rule[index][pos++])
                return 0;
        }

    }
    if(pos==(int)rule[index].size()||rule[index][0]==0) 
        return 1; 
    else
        return 0;
}


int Check_match(int value,int index){
    for(int i=cnt/2-1;i>=0;i--){
        if(index<cnt/2){
            if((map[index][i]==1&&value%2!=1)||(map[index][i]==-1&&value%2!=0))
                return 0;
        }
        else if((map[i][index-cnt/2]==1&&value%2!=1)||(map[i][index-cnt/2]==-1&&value%2!=0)){
            return 0;
        }
        value=value>>1;
    }

    return 1;
}

void Init_possible(){
    for(int i=0;i<cnt;i++)
        for(int num=0;num<cnt/2;num++)
            List_possible(0,num,i,0);
}

void List_possible(int data,int pos,int i,int j){
    if(j==(int)rule[i].size()-1){
        for(int k=0;k<rule[i][j];k++)
            data=(data<<1)+1;
        pos=pos+(int)rule[i][j]; 
        if(pos<=cnt/2){
            data=data<<(cnt/2-pos);
            possible[i].push_back(data);
        }
        return;
    } else{
        for(int k=0;k<rule[i][j];k++)
            data=(data<<1)+1;
        data=data<<1;  
        pos=pos+(int)rule[i][j]+1; 
        for(int num=0;num<cnt/2;num++){
            if(pos>cnt/2)
                return;
            List_possible(data<<num,pos+num,i,j+1);
        }

    } 
}


void Show_rule(){
    for(int i=0;i<cnt;i++){
        for(int j=0;j<(int)rule[i].size();j++)
            cout<<rule[i][j];
        cout<<endl;
    }

}

void Debug_map(int index){
    for(int i=0;i<cnt/2;i++){
        for(int j=0;j<cnt/2;j++)
            if(map[i][j]==-1)
                cout<<" ";
            else
                cout<<map[i][j];
        if(i==index)
            cout<<"-";
        cout<<endl;
    }

    for(int j=0;j<cnt/2;j++)
        if(j==index-cnt/2)
            cout<<"1";
        else
            cout<<" ";
    cout<<endl;
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
