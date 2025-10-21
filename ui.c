#include "ui.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int parse_move(const char *s, int *from, int *to){
    char f1='?', f2='?'; int r1=0, r2=0;
    if(sscanf(s," %c%d - %c%d",&f1,&r1,&f2,&r2)==4 || sscanf(s," %c%d %c%d",&f1,&r1,&f2,&r2)==4){
        int a = sq_of((char)tolower(f1), r1);
        int b = sq_of((char)tolower(f2), r2);
        if(a>=0 && b>=0){ *from=a; *to=b; return 1; }
    }
    return 0;
}

void loop_game(void){
    GS g = start_pos();
    char line[128];
    while(1){
        print_board(&g);
        if(!any_moves(&g)){
            printf("No moves. Winner: P%d\n", g.turn==1?2:1);
            break;
        }
        printf("[P%d] move (e.g., a3-b4), or : w=save x=load q=quit > ", g.turn);
        if(!fgets(line,sizeof(line),stdin)) break;
        if(line[0]=='q') break;
        if(line[0]=='w'){ if(save_game(&g,"save.chk")) printf("saved\n"); else printf("save fail\n"); continue; }
        if(line[0]=='x'){ if(load_game(&g,"save.chk")) printf("loaded\n"); else printf("load fail\n"); continue; }
        int a,b;
        if(parse_move(line,&a,&b)){
            GS tmp=g;
            if(move_do(&tmp,a,b)){
                g=tmp;
                g.turn = (g.turn==1)?2:1;
            }else{
                printf("bad move\n");
            }
        }else{
            printf("bad input\n");
        }
    }
}
