#include <stdio.h>
#include <stdlib.h>

FILE *ponteiro;

typedef struct _TNo {
	char ch;
	int freq;
	struct _TNo *prox;
	int X;
	char binario[9];
	struct _TNo *esquerda;
	struct _TNo *direita;
}TNo;

// insere no inicio da lista
TNo * adicionaInicio(TNo *inicio, char ch, int freq) {

    TNo *anterior = 0;
    TNo *atual = inicio;

    while(atual){
        if(atual->freq < freq){
            anterior = atual;
        }
        atual = atual->prox;
    }

	TNo *novo = (TNo *) calloc(1,sizeof(TNo));
	novo->ch = ch;
	novo->freq = freq;

    if(anterior == 0){
        novo->prox = inicio;
        inicio = novo;
    }else{
        TNo *tempAtual = anterior->prox;
        anterior->prox = novo;
        novo->prox =  tempAtual;
    }


	return inicio;
}

//printa lista encadeada
void mostraLista(TNo * inicio ){
	while( inicio ){
		printf("%c", inicio->ch);
		printf(" = %d\n", inicio->freq);
		inicio = inicio->prox;
	}
}

//add na árvore
void addArvoCres(TNo *no, TNo *ini) {

	TNo *anterior, *atual;
	atual = ini;

	while(atual->freq < no->X && atual->prox !=0) {
		anterior = atual;
		atual = atual->prox;
	}

	if(atual->prox == 0) {
		if(no->X > atual->freq)
			atual->prox = no;
		else {
			anterior->prox = no;
			no->prox = atual;
		}
	}else {
		anterior->prox = no;
		no->prox = atual;
	}
}

//leitura da Árvore e prefixos
void Prefixo(TNo *raiz, int i, char bit, char P[]) {
	int k;
	
	
	if(raiz != 0){
		P[i] = bit;
	}
	if(raiz->esquerda != 0 && raiz->direita != 0){
	Prefixo(raiz->esquerda, i+1, '0', P);
	Prefixo(raiz->direita, i+1, '1', P);
}
	if(raiz->ch != 0) {
		for(k=1; k <=i; k++){
			raiz->binario[k]=P[k];
			
		}
		
	}
}

void BinarioDaPalavra(TNo *raiz, char aux, int i){
	int k;
	if(raiz !=0){
	BinarioDaPalavra(raiz->esquerda,aux,i+1);
	BinarioDaPalavra(raiz->direita,aux,i+1);	
	if(raiz->ch==aux &&raiz->ch!=0){
		for(k=1; k < i+1; k++){
			printf("%c", raiz->binario[k]);
			fprintf(ponteiro, "%c",raiz->binario[k]);
		}		
		}
	}
}


int main(){

	FILE *fp;
	int i, auxint, count=0;
	char aux;
	int VFreq[255];
    char P[9];
	memset(VFreq,(int)0,sizeof(int)*255);

	TNo *ini;
	ini = 0;
	TNo *no, *tempIni;
	no=0;


    fp = fopen("input.txt","r"); // abre o arquivo de entrada
	ponteiro = fopen ("output.txt","w"); // abre o arquivo de saida

    if( !fp ){
    	printf("\nfalha na abertura do arquivo\n");
    	return 1;
	}

	//le o arquivo e insere no vetor de frequencia
	while (fscanf(fp,"%c",&aux) != EOF) {
		auxint = (int) aux;
		for(i=0; i<=255; i++) {
			if(i==auxint) {
				VFreq[i]++;
				if(VFreq[i]==1)
					count++;
			}
		}
	}

	// Grava quantidade de caracteres diferente e lista de frequencia no arquivo .txt
	fprintf(ponteiro,"%d\n", count);
	for(i=0; i<=255; i++) {
		if(VFreq[i]>0) {
			fprintf(ponteiro,"%d ", VFreq[i]);
			fprintf(ponteiro,"%c ", (char)i);
		}
	}
	
	fprintf(ponteiro,"\n");

	//Add na lista encadeada
	for(i=0; i<=255; i++)
		if(VFreq[i]>0)
			ini = adicionaInicio(ini, (char)i, VFreq[i]);

	mostraLista(ini);

	tempIni = ini;
	

	//Árvore
	while(tempIni) {
		no = (TNo*) calloc(1,sizeof(TNo));
		no->esquerda = tempIni;
		tempIni = tempIni->prox;
		no->direita = tempIni;
		no->X = no->esquerda->freq + no->direita->freq;
		no->freq = no->X;

		if(tempIni->prox == 0)
			break;

		addArvoCres(no, tempIni);
		tempIni = tempIni->prox;

	}
	Prefixo(no ,0 ,'0',P);
	fp = fopen("entrada.txt","r");	
		while (fscanf(fp,"%c",&aux) != EOF) {
		BinarioDaPalavra(no,aux, 0);
	}

	return 0;
}
