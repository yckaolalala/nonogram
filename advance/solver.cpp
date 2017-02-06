#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<cstring>
using namespace std;

vector<int> possible[60];
vector<int> rule[60];
int cnt=0;

void Readfile(char *filename);
void Init_possible();
void List_possible(int data,int pos,int i,int j);
int Check_match(int *map[30],int value,int index);
int Check_row_finish(int *map[30],int index);
int Check_finish(bool finish[60]);
int Get_min(int *map[30],bool finish[60]);
void Fill_map(int *map[30],int index,int value,int sign);
void Show_map(int *map[30]);
void Solver(int *Map[30],bool Finish[60],int value,int index);

int main(int argc,char **argv){

    if(argc!=2){
        cout<<"usage: filename\n";
        exit(2);
    }
    Readfile(argv[1]);
    
    int *map[30];
    bool finish[60];
    for(int i=0;i<cnt;i++)
        finish[i]=false;

    for(int i=0;i<30;i++)
        map[i]=new int[30];

    for(int i=0;i<30;i++)
        for(int j=0;j<30;j++)
            map[i][j]=0;
    
    Init_possible();
    Solver(map,finish,0,-1);
    cout<<"solve fail"<<endl;
    return 0;
}




void Solver(int *Map[30],bool Finish[60],int value,int index){
    int flag=1; 
    int *map[30];
    bool finish[60];

    for(int i=0;i<30;i++)
        map[i]=new int[30];

    for(int i=0;i<30;i++)
        for(int j=0;j<30;j++)
            map[i][j]=Map[i][j];

    for(int i=0;i<60;i++)
        finish[i]=Finish[i];


    if(index!=-1){
        Fill_map(map,index,value,1);
        Fill_map(map,index,~value,-1);
        finish[index]=true;   
    }

    while(flag){
        int pre_one_set[60];
        int pre_zero_set[60];
        flag=0;
        for(int i=0;i<cnt;i++){
            if(finish[i]==true)
                continue;
            int one_set=0;
            int zero_set=0;
            int first=0;

            for(int j=0;j<(int)possible[i].size();j++){
                if(Check_match(map,possible[i][j],i)){
                    if(first++==0){
                        one_set=possible[i][j];
                        zero_set=~possible[i][j];
                    }else{
                        one_set&=possible[i][j];
                        zero_set&=~possible[i][j];
                    }
                }
            }

            if(one_set!=pre_one_set[i] || zero_set!=pre_zero_set[i])
                flag=1;
            pre_one_set[i]=one_set;
            pre_zero_set[i]=zero_set;

            Fill_map(map,i,one_set,1);
            Fill_map(map,i,zero_set,-1);

            if(Check_row_finish(map,i))
                finish[i]=true;
        }
    }
    
    if(Check_finish(finish)){
        cout<<"solve success"<<endl;
        Show_map(map);
        exit(0);
    }else{
        int min=Get_min(map,finish);
        for(int j=0;j<(int)possible[min].size();j++){          
            if(Check_match(map,possible[min][j],min)){
                Solver(map,finish,possible[min][j],min);
            }
        }
    }

    for(int i=0;i<30;i++)
        delete [] map[i];
}

int Get_min(int *map[30],bool finish[60]){
    int min_size=65534,min=0;
    for(int i=0;i<cnt;i++){
        if(finish[i]==false){
            int count=0;
            for(int j=0;j<(int)possible[i].size();j++)
                if(Check_match(map,possible[i][j],i))
                     count++;
            if(count<min_size){
                min_size=count;
                min=i;
            }
        }
    }
    return min;
}

void Fill_map(int *map[30],int index,int value,int sign){
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

void Show_map(int *map[30]){
    for(int i=0;i<cnt/2;i++){
        for(int j=0;j<cnt/2;j++)
            if(map[i][j]==1)
                cout<<"1";
            else
                cout<<"0";
        cout<<endl;
    }

}

int Check_finish(bool finish[60]){
    for(int i=0;i<cnt;i++)
        if(finish[i]==false)
            return 0;
    return 1;
}
int Check_row_finish(int *map[30],int index){
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


int Check_match(int *map[30],int value,int index){
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
