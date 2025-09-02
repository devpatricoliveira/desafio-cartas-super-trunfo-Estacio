#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 300
#define ARQ "cartas.db"

typedef struct {
    char estado;
    char codigo[6];
    char nome[60];
    int  populacao;
    float area;
    float pib;
    int  pontosTuristicos;
} Carta;

float densidade(Carta c){ return (c.area>0)? c.populacao/c.area : 0.0f; }
float pib_per_capita(Carta c){ return (c.populacao>0)? c.pib/c.populacao : 0.0f; }

int cmp_pop(const void* a,const void* b){ return ((Carta*)b)->populacao - ((Carta*)a)->populacao; }
int cmp_area(const void* a,const void* b){ float d=((Carta*)b)->area - ((Carta*)a)->area; return (d>0)-(d<0); }
int cmp_pib(const void* a,const void* b){ float d=((Carta*)b)->pib - ((Carta*)a)->pib; return (d>0)-(d<0); }
int cmp_dens(const void* a,const void* b){ float d=densidade(*(Carta*)a)-densidade(*(Carta*)b); return (d>0)-(d<0); } // menor vence
int cmp_pibpc(const void* a,const void* b){ float d=pib_per_capita(*(Carta*)b)-pib_per_capita(*(Carta*)a); return (d>0)-(d<0); }

void printCarta(const Carta* c, int i){
    printf("[%03d] %c-%s %s | Pop:%d Area:%.2f PIB:%.2f Pts:%d | Dens:%.4f PIBpc:%.6f\n",
        i, c->estado, c->codigo, c->nome, c->populacao, c->area, c->pib, c->pontosTuristicos,
        densidade(*c), pib_per_capita(*c));
}

int carregar(Carta v[], int max){
    FILE* f=fopen(ARQ,"r"); if(!f) return 0;
    int i=0;
    while(i<max && fscanf(f,"%c;%5[^;];%59[^;];%d;%f;%f;%d\n",
        &v[i].estado, v[i].codigo, v[i].nome, &v[i].populacao, &v[i].area, &v[i].pib, &v[i].pontosTuristicos)==7) i++;
    fclose(f);
    return i;
}

void salvar(Carta v[], int n){
    FILE* f=fopen(ARQ,"w"); if(!f){perror("Salvar"); return;}
    for(int i=0;i<n;i++){
        fprintf(f,"%c;%s;%s;%d;%.2f;%.2f;%d\n",
            v[i].estado,v[i].codigo,v[i].nome,v[i].populacao,v[i].area,v[i].pib,v[i].pontosTuristicos);
    }
    fclose(f);
}

int buscar(Carta v[], int n, const char* cod){
    for(int i=0;i<n;i++) if(strcmp(v[i].codigo,cod)==0) return i;
    return -1;
}

void exportar_txt(Carta v[], int n, const char* nome){
    FILE* f=fopen(nome,"w"); if(!f){perror("Exportar"); return;}
    for(int i=0;i<n;i++) fprintf(f,"%s\t%s\t%d\t%.2f\t%.2f\t%d\n",
        v[i].codigo, v[i].nome, v[i].populacao, v[i].area, v[i].pib, v[i].pontosTuristicos);
    fclose(f);
    printf("Exportado para %s\n", nome);
}

void listar(Carta v[], int n){
    for(int i=0;i<n;i++) printCarta(&v[i], i);
}

void cadastrar(Carta v[], int* n){
    if(*n>=MAX){ printf("Limite atingido.\n"); return; }
    Carta c;
    printf("Estado: "); scanf(" %c",&c.estado);
    printf("Codigo: "); scanf("%5s", c.codigo);
    if(buscar(v,*n,c.codigo)!=-1){ printf("Codigo ja existe.\n"); return; }
    printf("Nome: "); scanf(" %59[^\n]", c.nome);
    printf("Populacao: "); scanf("%d",&c.populacao);
    printf("Area: "); scanf("%f",&c.area);
    printf("PIB: "); scanf("%f",&c.pib);
    printf("Pontos: "); scanf("%d",&c.pontosTuristicos);
    v[(*n)++]=c; salvar(v,*n);
}

void editar(Carta v[], int n){
    char cod[6]; printf("Codigo da carta: "); scanf("%5s",cod);
    int i=buscar(v,n,cod); if(i==-1){ printf("Nao encontrada.\n"); return; }
    printf("Editando %s - %s\n", v[i].codigo, v[i].nome);
    printf("Novo nome (enter p/ manter): ");
    char nome[60]; int ch=getchar();
    if(fgets(nome,60,stdin)){
        if(nome[0]!='\n'){ nome[strcspn(nome,"\n")]=0; strncpy(v[i].nome,nome,59); }
    }
    printf("Nova populacao (0 mantem): "); int p; scanf("%d",&p); if(p>0) v[i].populacao=p;
    printf("Nova area (0 mantem): "); float a; scanf("%f",&a); if(a>0) v[i].area=a;
    printf("Novo PIB (0 mantem): "); float pb; scanf("%f",&pb); if(pb>0) v[i].pib=pb;
    printf("Novos pontos (0 mantem): "); int pts; scanf("%d",&pts); if(pts>0) v[i].pontosTuristicos=pts;
    salvar(v,n);
}

void remover(Carta v[], int* n){
    char cod[6]; printf("Codigo da carta: "); scanf("%5s",cod);
    int i=buscar(v,*n,cod); if(i==-1){ printf("Nao encontrada.\n"); return; }
    for(int j=i;j<(*n)-1;j++) v[j]=v[j+1];
    (*n)--; salvar(v,*n);
}

int main(){
    Carta v[MAX]; int n=carregar(v,MAX);
    int op;
    do{
        printf("\n=== Cartas Super Trunfo (Mestre) ===\n");
        printf("1-Listar 2-Cadastrar 3-Editar 4-Remover 5-Ordenar 6-Exportar 0-Sair\n> ");
        if(scanf("%d",&op)!=1){op=0;break;}
        if(op==1) listar(v,n);
        else if(op==2) cadastrar(v,&n);
        else if(op==3) editar(v,n);
        else if(op==4) remover(v,&n);
        else if(op==5){
            int modo;
            printf("Ordenar por: 1-Pop(desc) 2-Area(desc) 3-PIB(desc) 4-Dens(asc) 5-PIBpc(desc)\n> ");
            scanf("%d",&modo);
            if(modo==1) qsort(v,n,sizeof(Carta),cmp_pop);
            else if(modo==2) qsort(v,n,sizeof(Carta),cmp_area);
            else if(modo==3) qsort(v,n,sizeof(Carta),cmp_pib);
            else if(modo==4) qsort(v,n,sizeof(Carta),cmp_dens);
            else if(modo==5) qsort(v,n,sizeof(Carta),cmp_pibpc);
            listar(v,n);
        } else if(op==6){
            exportar_txt(v,n,"cartas_export.txt");
        }
    }while(op!=0);
    printf("Dados salvos em '%s'.\n", ARQ);
    return 0;
}
