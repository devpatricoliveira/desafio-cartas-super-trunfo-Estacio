#include <stdio.h>
#include <string.h>

#define MAX 100
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

void printCarta(const Carta* c, int i){
    printf("[%02d] %c-%s %s | Pop:%d Area:%.2f PIB:%.2f Pontos:%d | Dens:%.4f PIBpc:%.6f\n",
        i, c->estado, c->codigo, c->nome, c->populacao, c->area, c->pib, c->pontosTuristicos,
        densidade(*c), pib_per_capita(*c));
}

int buscar(Carta v[], int n, const char* cod){
    for(int i=0;i<n;i++) if(strcmp(v[i].codigo,cod)==0) return i;
    return -1;
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
    printf("Novo nome (uma linha): "); scanf(" %59[^\n]", v[i].nome);
    printf("Nova populacao: "); scanf("%d",&v[i].populacao);
    printf("Nova area: "); scanf("%f",&v[i].area);
    printf("Novo PIB: "); scanf("%f",&v[i].pib);
    printf("Novos pontos: "); scanf("%d",&v[i].pontosTuristicos);
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
        printf("\n=== Cartas Super Trunfo (Aventureiro) ===\n");
        printf("1-Listar  2-Cadastrar  3-Editar  4-Remover  0-Sair\n> ");
        if(scanf("%d",&op)!=1) {op=0; break;}
        if(op==1){
            if(n==0) printf("Sem cartas.\n");
            for(int i=0;i<n;i++) printCarta(&v[i],i);
        } else if(op==2) cadastrar(v,&n);
        else if(op==3) { editar(v,n); salvar(v,n); }
        else if(op==4) remover(v,&n);
    }while(op!=0);
    printf("Salvas em '%s'.\n", ARQ);
    return 0;
}
