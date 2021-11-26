#include <Pokitto.h>
#include "sqrt_table.h"

#define ABS(x) ((x)<0 ? -(x) : (x))

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PB = Pokitto::Buttons;

int width=220;
int height=176;


const int NUM_CIRCLES = 6;
struct circle{
    int x;
    int y;
    int r;
    int vx;
    int vy;
    int cr;
    int cg;
    int cb;
};

static circle c[NUM_CIRCLES];
int RandMinMax(int min, int max){
    return rand() % max + min;
}


void createTable(){
    // =APP.customSetVariables({maxLogLength:100000})
    printf("const uint8_t table[]={");
    int t=0;
    for(int y=0; y<176; y++){
        printf("\n");
        for(int x=0; x<220; x++){
            if(x==0 && y==0){
                printf("15,");
            }else{
    		    t=(9216 / (sqrt(x*x + y*y) * 8));
    		    //t = sqrt(x*x + y*y);
                if (t > 255) t = 255;
                printf("%d,",t);
    		}
        }
    }		   
    printf("\n};\n");
}

inline void myBGFiller(std::uint8_t* line, std::uint32_t y, bool skip){

    if(y==0){
        for(uint32_t x=0; x<220; ++x){
            line[x]=x;
        }        
    }

    int i=0;
    int x1,y1,temp;
    uint32_t red=0,green=0,blue = 0;
    int x=0;

    #define addPixel()\
        y1 = c[i].y-y;\
        x1 = c[i].x-x;\
        temp = table[ABS(x1) + 220 * ABS(y1)];\
          red += temp * c[i].cr;\
        green += temp * c[i].cg;\
         blue += temp * c[i++].cb;

    #define renderPixel()\
        addPixel(); addPixel(); addPixel(); addPixel();

    #define pixel();\
        red = 0, green = 0, blue = 0, i = 0;\
        renderPixel();\
        if(red>255)red=255;\
        if(green>255)green=255;\
        if(blue>255)blue=255;\
        Pokitto::Display::palette[x++] = (red/8)<<11 | (green/4<<5) | (blue/8);

        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); pixel(); 
        

}

void move_circles(){
    for (int i = 0; i < NUM_CIRCLES; i++) {
    
        c[i].x += c[i].vx;
        c[i].y += c[i].vy;
    
        if (c[i].x - c[i].r < 0) {
            c[i].vx = +abs(c[i].vx);
        }
        if (c[i].x + c[i].r > width) {
            c[i].vx = -abs(c[i].vx);
        }
        if (c[i].y - c[i].r < 0) {
            c[i].vy = +abs(c[i].vy);
        }
        if (c[i].y + c[i].r > height) {
            c[i].vy = -abs(c[i].vy);
        }
    }
}

int main(){

	PC::begin();

    Pokitto::Display::lineFillers[0] = myBGFiller;

    uint32_t frameCount=0;

    //srand(game.getTime());
    for (int i = 0; i < NUM_CIRCLES; i++) {
        c[i].x = RandMinMax(0,220);
        c[i].y = RandMinMax(0,175);
        c[i].r = RandMinMax(5, 15);
        c[i].vx = RandMinMax(-2,2);
        c[i].vy = RandMinMax(-2,2);
    }

        c[0].cr = 4; c[0].cg = 1; c[0].cb = 1;
        c[1].cr = 1; c[1].cg = 4; c[1].cb = 1;
        c[2].cr = 1; c[2].cg = 1; c[2].cb = 4;
        c[3].cr = 3; c[3].cg = 3; c[3].cb = 1;

    //createTable(); // used to create the lookup table

    while (1) {
        if(!PC::update())continue;
        move_circles();
    }

    return 0;
}