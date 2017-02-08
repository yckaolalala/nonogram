#include "nonogram.h"
#include<iostream>
using namespace std;

int Get_min(Nono *data){
    int min_size=65534,min=0;
    for(int i=0;i<data->cnt;i++){
        if(data->finish[i]==false){
            if((int)data->possible[i].size()<min_size){
                min_size=(int)data->possible[i].size();
                min=i;
            }
        }
    }
    return min;
}

void Fill_map(Nono *data,int index,int value,int sign){
    for(int j=data->cnt/2-1;j>=0;j--){
        if(value%2!=0){
            if(index<data->cnt/2)
                data->map[index][j]=sign;
            else
                data->map[j][index-data->cnt/2]=sign;
        }
        value=value>>1;
    }
}

void Show_map(Nono *data){
    for(int i=0;i<data->cnt/2;i++){
        for(int j=0;j<data->cnt/2;j++)
            if(data->map[i][j]==1)
                cout<<"1";
            else
                cout<<"0";
        cout<<endl;
    }

}

int Check_finish(Nono *data){
    for(int i=0;i<data->cnt;i++)
        if(data->finish[i]==false)
            return 0;
    return 1;
}
int Check_row_finish(std::vector<int> rule[60],Nono *data,int index){
    int count,pos=0;
    for(int i=0;i<data->cnt/2;i++){
        count=0;
        while((index<data->cnt/2 && data->map[index][i]==1)||((index>=data->cnt/2)&&data->map[i][index-data->cnt/2]==1)){
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


int Check_match(Nono *data,int value,int index){
    for(int i=data->cnt/2-1;i>=0;i--){
        if(index<data->cnt/2){
            if((data->map[index][i]==1&&value%2!=1)||(data->map[index][i]==-1&&value%2!=0))
                return 0;
        }
        else if((data->map[i][index-data->cnt/2]==1&&value%2!=1)||(data->map[i][index-data->cnt/2]==-1&&value%2!=0)){
            return 0;
        }
        value=value>>1;
    }

    return 1;
}

void Init_possible(std::vector<int> rule[60],Nono *data){
    for(int i=0;i<data->cnt;i++)
        for(int num=0;num<data->cnt/2;num++)
            List_possible(rule,data,0,num,i,0);
}

void List_possible(std::vector<int> rule[60],Nono *data,int value,int pos,int i,int j){
    if(j==(int)rule[i].size()-1){
        for(int k=0;k<rule[i][j];k++)
            value=(value<<1)+1;
        pos=pos+(int)rule[i][j]; 
        if(pos<=data->cnt/2){
            value=value<<(data->cnt/2-pos);
            data->possible[i].push_back(value);
        }
        return;
    } else{
        for(int k=0;k<rule[i][j];k++)
            value=(value<<1)+1;
        value=value<<1;  
        pos=pos+(int)rule[i][j]+1; 
        for(int num=0;num<data->cnt/2;num++){
            if(pos>data->cnt/2)
                return;
            List_possible(rule,data,value<<num,pos+num,i,j+1);
        }

    } 
}
