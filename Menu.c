#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "menu.h"

//Menu Principal
int menu() {
	int cont;
	for(cont = 0; cont < 100; cont++) {
		printf("#");
	}	
	printf("\n\n\t\t\t\t\tLEGAL RENT A CAR\n\n");
	for(cont = 0; cont < 100; cont++) {
		printf("#");
	}
	printf("\n");
	
	//Opções do menu
	printf("\nDigite o numero da opcao desejada\n");
	printf("\n{1} Cadastrar novo veiculo\n{2} Cadastrar Valor da Diaria\n{3} Alugar veiculo\n{4} Recebimento de Veiculo"
	"\n{5} Relatorio de Disponibilidade\n{6} Sair\n\n");
	return 0;
}

//Escreve na tela o Titulo do programa
int titulo() {
	int cont;
	for(cont = 0; cont < 100; cont++) {
		printf("#");
	}	
	printf("\n\n\t\t\t\t\tLEGAL RENT A CAR\n\n");
	for(cont = 0; cont < 100; cont++) {
		printf("#");
	}
	printf("\n");
	return 0;
}
