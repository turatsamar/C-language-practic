
#include <stdio.h>
// struct too store every body
struct record{
    char name[10];
    float height ;
    float weight;
};

//here we use struct array for data hold
struct record records[5] = {
    {"tima",  165.5, 60.0 },
    {"rita",  155.5, 50.0},
    {"sofia", 120.0, 10.0},
    {"sean",  100.0, 80.0},
    {"stephen", 100.0, 80.0}

};

// here function returns standart_weight();
float standart_weight(struct record r);

float obesity_weight(struct record r);

int main(void)
{
    
     float standart_weight(struct record r){
         return(r.height-100.0) * 0.9;
     }
     
     float obesity_weight(struct record r){
        float std_weight=standart_weight(r);
         
         return(r.weight - std_weight) * 100.0;
     }
   
   

    return 0;
}
