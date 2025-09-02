    #include <stdio.h>
#include <string.h>

#define MAX 20

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

void printCarta(Carta c, int i){
    printf("[%02d] %c-%s %s | Pop:%d Area:%.2f PIB:%.2f Pontos:%d | Dens:%.4f PIBpc:%.6f\n",
        i, c.estado, c.codigo, c.nome, c.populacao, c.area, c.pib, c.pontosTuristicos,
        densidade(c), pib_per_capita(c));
}

int main(){
    Carta vet[MAX];
    int n=0;
    printf("=== Cartas Super Trunfo - Nivel Novato ===\n");
    printf("Quantas cartas deseja cadastrar (1-%d)? ", MAX);
    scanf("%d",&n); if(n<1) n=1; if(n>MAX) n=MAX;

    for(int i=0;i<n;i++){
        Carta c;
        printf("\n-- Carta %d --\n", i+1);
        printf("Estado (A-H): "); scanf(" %c",&c.estado);
        printf("Codigo (ex A01): "); scanf("%5s", c.codigo);
        printf("Nome (uma linha): "); scanf(" %59[^\n]", c.nome);
        printf("Populacao: "); scanf("%d",&c.populacao);
        printf("Area: "); scanf("%f",&c.area);
        printf("PIB: "); scanf("%f",&c.pib);
        printf("Pontos turisticos: "); scanf("%d",&c.pontosTuristicos);
        vet[i]=c;
    }

    printf("\n=== Listagem de Cartas ===\n");
    for(int i=0;i<n;i++) printCarta(vet[i], i);
    printf("\nTotal de cartas: %d\n", n);
    return 0;
}
