#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<vector>
#include<cstring>
using namespace std;

class Nono{
    public:
        int map[30][30];
        vector<int> possible[60];
        bool finish[60];
        Nono(){
            for(int i=0;i<30;i++)
                for(int j=0;j<30;j++)
                    map[i][j]=0;
            for(int i=0;i<60;i++)
                finish[i]=false;
            
        };

};
vector<int> rule[60];
int cnt=0;

void Readfile(char *filename);
void Init_possible(Nono *data);
void List_possible(Nono *data,int value,int pos,int i,int j);
int Check_match(Nono *data,int value,int index);
int Check_row_finish(Nono *data,int index);
int Check_finish(Nono *data);
int Get_min(Nono *data);
void Fill_map(Nono *data,int index,int value,int sign);
void Show_map(Nono *data);
void Solver(Nono data,int value,int index);

int main(int argc,char **argv){

    if(argc!=2){
        cout<<"usage: filename\n";
        exit(2);
    }
    Readfile(argv[1]);
    Nono data;
    Init_possible(&data);
    
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

            if(Check_row_finish(&data,i))
                data.finish[i]=true;

            if(0){
                char t;
                Show_map(&data);
                scanf("%c",&t);
            }
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

int Get_min(Nono *data){
    int min_size=65534,min=0;
    for(int i=0;i<cnt;i++){
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
    for(int j=cnt/2-1;j>=0;j--){
        if(value%2!=0){
            if(index<cnt/2)
                data->map[index][j]=sign;
            else
                data->map[j][index-cnt/2]=sign;
        }
        value=value>>1;
    }
}

void Show_map(Nono *data){
    for(int i=0;i<cnt/2;i++){
        for(int j=0;j<cnt/2;j++)
            if(data->map[i][j]==1)
                cout<<"1";
            else
                cout<<"0";
        cout<<endl;
    }

}

int Check_finish(Nono *data){
    for(int i=0;i<cnt;i++)
        if(data->finish[i]==false)
            return 0;
    return 1;
}
int Check_row_finish(Nono *data,int index){
    int count,pos=0;
    for(int i=0;i<cnt/2;i++){
        count=0;
        while((index<cnt/2 && data->map[index][i]==1)||((index>=cnt/2)&&data->map[i][index-cnt/2]==1)){
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
    for(int i=cnt/2-1;i>=0;i--){
        if(index<cnt/2){
            if((data->map[index][i]==1&&value%2!=1)||(data->map[index][i]==-1&&value%2!=0))
                return 0;
        }
        else if((data->map[i][index-cnt/2]==1&&value%2!=1)||(data->map[i][index-cnt/2]==-1&&value%2!=0)){
            return 0;
        }
        value=value>>1;
    }

    return 1;
}

void Init_possible(Nono *data){
    for(int i=0;i<cnt;i++)
        for(int num=0;num<cnt/2;num++)
            List_possible(data,0,num,i,0);
}

void List_possible(Nono *data,int value,int pos,int i,int j){
    if(j==(int)rule[i].size()-1){
        for(int k=0;k<rule[i][j];k++)
            value=(value<<1)+1;
        pos=pos+(int)rule[i][j]; 
        if(pos<=cnt/2){
            value=value<<(cnt/2-pos);
            data->possible[i].push_back(value);
        }
        return;
    } else{
        for(int k=0;k<rule[i][j];k++)
            value=(value<<1)+1;
        value=value<<1;  
        pos=pos+(int)rule[i][j]+1; 
        for(int num=0;num<cnt/2;num++){
            if(pos>cnt/2)
                return;
            List_possible(data,value<<num,pos+num,i,j+1);
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
