#include <vector>
class Nono{
    public:
        int cnt;
        int map[30][30];
        std::vector<int> possible[60];
        bool finish[60];
        Nono(){
            for(int i=0;i<30;i++)
                for(int j=0;j<30;j++)
                    map[i][j]=0;
            for(int i=0;i<60;i++)
                finish[i]=false;
        };
        Nono(int value){
            cnt=value;
            for(int i=0;i<30;i++)
                for(int j=0;j<30;j++)
                    map[i][j]=0;
            for(int i=0;i<60;i++)
                finish[i]=false;
        };

};

void Init_possible(std::vector<int> rule[60],Nono *data);
void List_possible(std::vector<int> rule[60],Nono *data,int value,int pos,int i,int j);
int Check_match(Nono *data,int value,int index);
int Check_row_finish(std::vector<int> rule[60],Nono *data,int index);
int Check_finish(Nono *data);
int Get_min(Nono *data);
void Fill_map(Nono *data,int index,int value,int sign);
void Show_map(Nono *data);
