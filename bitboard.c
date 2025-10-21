#include "bitboard.h"
#include <stdio.h>

static U64 one(int s){ return (s>=0 && s<64)? (1ull<<s):0; }
U64 file_a(void){ return 0x0101010101010101ull; }
U64 file_h(void){ return 0x8080808080808080ull; }
U64 dark_mask(void){ return 0xAA55AA55AA55AA55ull; }

int sq_of(char f, int r){
    if(f<'a'||f>'h'||r<1||r>8) return -1;
    return (r-1)*8 + (f-'a');
}
void sq_to(int s, char *buf){
    buf[0] = 'a'+(s%8);
    buf[1] = '1'+(s/8);
    buf[2] = 0;
}

GS start_pos(void){
    GS g={0,0,0,0,1};
    U64 m = dark_mask();
    for(int s=0;s<64;s++){
        if(!(m & one(s))) continue;
        int r=s/8;
        if(r<=2) g.p2 |= one(s);
        else if(r>=5) g.p1 |= one(s);
    }
    return g;
}

static int occ(const GS *g, int s){
    U64 all = g->p1|g->k1|g->p2|g->k2;
    return (all>>s)&1ull;
}
static int mine(const GS *g, int s){
    U64 me = (g->turn==1)? (g->p1|g->k1):(g->p2|g->k2);
    return (me>>s)&1ull;
}
static int is_king(const GS *g, int s){
    U64 kk = (g->turn==1)? g->k1 : g->k2;
    return (kk>>s)&1ull;
}
static int theirs(const GS *g, int s){
    U64 op = (g->turn==1)? (g->p2|g->k2):(g->p1|g->k1);
    return (op>>s)&1ull;
}
static void setbit(U64 *x, int s){ *x |= one(s); }
static void clrbit(U64 *x, int s){ *x &= ~one(s); }

static int inb(int s){ return s>=0 && s<64; }
static int diag_ok(int a, int b){ int fa=a%8, fb=b%8; int ra=a/8, rb=b/8; return (fa!=fb)&&(ra!=rb)&&((fa-fb==1||fb-fa==1)&&(ra-rb==1||rb-ra==1)); }
static int diag2_ok(int a, int b){ int fa=a%8, fb=b%8; int ra=a/8, rb=b/8; return (fa-fb==2||fb-fa==2)&&(ra-rb==2||rb-ra==2); }

static int forward_ok(const GS *g, int a, int b){
    int ra=a/8, rb=b/8;
    if(is_king(g,a)) return 1;
    if(g->turn==1) return rb>ra;
    return rb<ra;
}

static void promo(GS *g){
    if(g->turn==1){
        U64 last = 0xFF00000000000000ull;
        U64 m = g->p1 & last;
        g->p1 &= ~m; g->k1 |= m;
    }else{
        U64 first = 0x00000000000000FFull;
        U64 m = g->p2 & first;
        g->p2 &= ~m; g->k2 |= m;
    }
}

void print_board(const GS *g){
    for(int r=7;r>=0;r--){
        printf("%d ", r+1);
        for(int f=0;f<8;f++){
            int s=r*8+f; char c='.';
            if((g->p1>>s)&1ull) c='r';
            if((g->p2>>s)&1ull) c='b';
            if((g->k1>>s)&1ull) c='R';
            if((g->k2>>s)&1ull) c='B';
            printf("%c ", c);
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
    printf("Turn: P%d\n", g->turn);
}

static int try_simple(const GS *g, int a, int b){
    if(!inb(a)||!inb(b)) return 0;
    if(!mine(g,a) || occ(g,b)) return 0;
    if(!diag_ok(a,b)) return 0;
    if(!forward_ok(g,a,b)) return 0;
    return 1;
}
static int try_jump(const GS *g, int a, int b, int *mid){
    if(!inb(a)||!inb(b)) return 0;
    if(!mine(g,a) || occ(g,b)) return 0;
    if(!diag2_ok(a,b)) return 0;
    int mf = (a%8 + b%8)/2;
    int mr = (a/8 + b/8)/2;
    int m = mr*8+mf;
    if(!theirs(g,m)) return 0;
    if(!forward_ok(g,a,b)) return 0;
    *mid = m;
    return 1;
}

int move_do(GS *g, int from, int to){
    if(!inb(from)||!inb(to)) return 0;
    if(!mine(g,from)) return 0;

    int m=-1;
    if(try_jump(g,from,to,&m)){
        if(g->turn==1){
            if((g->k1>>from)&1ull){ clrbit(&g->k1,from); setbit(&g->k1,to); }
            else { clrbit(&g->p1,from); setbit(&g->p1,to); }
            if((g->k2>>m)&1ull) clrbit(&g->k2,m); else clrbit(&g->p2,m);
        }else{
            if((g->k2>>from)&1ull){ clrbit(&g->k2,from); setbit(&g->k2,to); }
            else { clrbit(&g->p2,from); setbit(&g->p2,to); }
            if((g->k1>>m)&1ull) clrbit(&g->k1,m); else clrbit(&g->p1,m);
        }
        promo(g);
        return 1;
    }
    if(try_simple(g,from,to)){
        if(g->turn==1){
            if((g->k1>>from)&1ull){ clrbit(&g->k1,from); setbit(&g->k1,to); }
            else { clrbit(&g->p1,from); setbit(&g->p1,to); }
        }else{
            if((g->k2>>from)&1ull){ clrbit(&g->k2,from); setbit(&g->k2,to); }
            else { clrbit(&g->p2,from); setbit(&g->p2,to); }
        }
        promo(g);
        return 1;
    }
    return 0;
}

static int has_move(const GS *g){
    for(int s=0;s<64;s++){
        if(!mine(g,s)) continue;
        int t1=s+7, t2=s+9, t3=s-7, t4=s-9;
        GS tmp=*g;
        if(move_do(&tmp,s,t1)) return 1;
        tmp=*g; if(move_do(&tmp,s,t2)) return 1;
        tmp=*g; if(move_do(&tmp,s,t3)) return 1;
        tmp=*g; if(move_do(&tmp,s,t4)) return 1;
    }
    return 0;
}

int any_moves(const GS *g){
    return has_move(g);
}

int save_game(const GS *g, const char *path){
    FILE *fp=fopen(path,"w"); if(!fp) return 0;
    fprintf(fp,"%llu %llu %llu %llu %d\n",
        (unsigned long long)g->p1,
        (unsigned long long)g->k1,
        (unsigned long long)g->p2,
        (unsigned long long)g->k2,
        g->turn);
    fclose(fp);
    return 1;
}

int load_game(GS *g, const char *path){
    FILE *fp=fopen(path,"r"); if(!fp) return 0;
    unsigned long long a,b,c,d; int t;
    if(fscanf(fp,"%llu %llu %llu %llu %d",&a,&b,&c,&d,&t)!=5){ fclose(fp); return 0; }
    fclose(fp);
    g->p1=a; g->k1=b; g->p2=c; g->k2=d; g->turn=t;
    return 1;
}
