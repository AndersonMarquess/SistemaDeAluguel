#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "menu.h"

/*
	Atualização, Adicionar gravação em Binario para manter os dados salvos.
*/
FILE *arquivoVeiculos;
FILE *arquivoTotal;

//Abre o arquivo que contem as informações dos veiculos salvos
void abrirBinario() {
	arquivoVeiculos = fopen("dados.txt", "rb");
	arquivoTotal = fopen("totalCadastrados.txt", "rb");
	if(arquivoVeiculos == NULL || arquivoTotal == NULL ) {
		printf("\nErro na Leitura do arquivo...\n");
		getchar();
		exit(1);
	}
}



//Pega a data e hora do sistema
int dataPedido(int *ano, int * mes, int * dia, int * horas, int * min) {
	struct tm *local;
	time_t tempo;
	tempo = time(NULL);
	local = localtime(&tempo);

	*ano = local->tm_year+1900;
	*mes = local->tm_mon+1;
	*dia = local->tm_mday;
	*horas = local->tm_hour;
	*min = local->tm_min;
	
	return 0;
}

//Cadastras as informações basicas dos Clientes. Logo apos inicializa a struct.
struct pessoa {
	char cpf[15];
	char telef[16];
	char nome[70];
};struct pessoa cliente[99];

//Cadastra os carros com as informações: Modelo, placa e Identificação(tag). Logo apos inicializa a struct.
struct veiculos{
	int ocupado, tag;
	char modelo[50], placa[15];
};struct veiculos carros[99];

//Escreve as informações gerais do veiculo
int  informacoes(int posicao) {
	char resp[10];
	//Operador ternario para verificar se o veiculo esta vago
	strcpy(resp, carros[posicao].ocupado != 1? "Livre":"Ocupado");
	printf("\nModelo: \t\t%sPlaca: \t\t\t%sIdentificacao: \t\t%i\nDisponibilidade: \t%s\n", 
	carros[posicao].modelo, carros[posicao].placa, carros[posicao].tag, resp);
	return posicao;
}

/*Lista de todos os Veiculos, "totalMax" recebe o total de veiculos cadastrados, 
"tipo" recebe o tipo de verificação desejada*/
int listaVeiculos(int totalMax, char tipo, int *diaEn, int *mesEn, int * ano, int *horasEn) {
	int c;
	switch(tipo) {
		case '0': //Total
			for(c = 1; c <= totalMax; c++) {
				informacoes(c);
				printf("\n--------------------\n");
			}
			break;
		case '1'://Total Livre
			printf("\nTotal de Veiculos Disponiveis no Momento:\n");
			for(c = 1; c <= totalMax; c++) {
				if(carros[c].ocupado != 1) {
					informacoes(c);
					printf("\n------------------------------\n");
				}
			}
			break;
		case '2'://Total Ocupado
			printf("\nTotal de Veiculos Ocupado no Momento:\n");
			for(c = 1; c <= totalMax; c++) {
				if(carros[c].ocupado != 0) {
					informacoes(c);
					printf("\nData Prevista para Entrega: %i/%i/%i  as %i:00 H\n", diaEn[c], mesEn[c], ano[c], horasEn[c]);
					printf("\n--------------------\n");
				}
			}
			break;
		default:
			break;
	}
	return 0;
}

//Faz uma busca em todos os veiculos cadastrados para encontrar o veiculo com a identificação requesitada
int encontrarVeiculo(int posicao) {
	int c = 0;
	while(carros[c].tag != posicao) {
		c++;
	}
	return c;
}

//Faz todos os calculos de cobrança do aluguel
float cobranca(int temp, int ident, int valDiaria, int *ano, int *mes, int *dia, int *hora, int *mesEn, int *diaEn, int *horasEn, int *min, int *minEn) {
	float debito, tempDebito;
	int multaMes, multaDia, multaHoras, tempMes, tempDia, tempHoras;
	//Calcula o valor do Aluguel, transformando mes e dias em Horas.
	if(dia[ident] != diaEn[ident] || mes[ident] != mesEn[ident] || hora[ident] != horasEn[ident]) {
		multaMes = mesEn[ident] - mes[ident];
		multaDia = diaEn[ident] - dia[ident];
		//Verifica se o cliente quer um dia de cortesia, caso alugue mais que 5 dias e abate o valor da cortesia
		if(temp == 1) {
			multaDia -= 1;
		}
		multaHoras = horasEn[ident] - hora[ident];
		tempDebito = ((multaMes * 720) + (multaDia * 24)) + multaHoras;
		tempDebito /= 24.0; //transforma o total de horas em dias
		debito = tempDebito * valDiaria;
		//Verifica se o cliente quer um dia de cortesia, caso alugue mais que 5 dias e aumenta em 1 dia o prazo para entrega
		if(temp == 1) {
			multaDia += 1;
		}
		system("clear||cls");
		titulo();
		printf("\nData da Retirada: %i/%i/%i  as %i:%i H\n", dia[ident], mes[ident], ano[ident], hora[ident], min[ident]);
		printf("\nData da Entrega: %i/%i/%i  as %i:%i H\n", diaEn[ident], mesEn[ident], ano[ident], horasEn[ident], minEn[ident]);
	}
	if(debito < 0) {
		debito *= -1;
		return debito;
	}
	return debito;
}

//Faz todos os calculos de cobrança da multa
float multaVerificacao(int ident, int valDiaria, int *ano, int *mes, int *dia, int *hora, int *mesEn, int *diaEn, int *horasEn, int *min, int *minEn) {
	float debito, tempDebito;
	int multaMes, multaDia, multaHoras, tempMes, tempDia, tempHoras;
	//Calcula o valor do Aluguel, transformando mes e dias em Horas.
	if(dia[ident] >= diaEn[ident] || mes[ident] >= mesEn[ident] || hora[ident] > horasEn[ident]) {
		multaMes = mes[ident] - mesEn[ident];
		multaDia = dia[ident] - diaEn[ident];
		multaHoras = horasEn[ident] - hora[ident];
		tempDebito = ((multaMes * 720) + (multaDia * 24)) + multaHoras;
		tempDebito /= 24.0; //transforma o total de horas em dias
		debito = tempDebito * valDiaria;
		system("clear||cls");
		titulo();
		printf("\nData da Retirada: %i/%i/%i  as %i:%i H\n", dia[ident], mes[ident], ano[ident], hora[ident], min[ident]);
		printf("\nData Prevista para Entrega: %i/%i/%i  as %i:%i H\n", diaEn[ident], mesEn[ident], ano[ident], horasEn[ident], minEn[ident]);
	}
	if((multaMes < 0 ||multaDia < 0) && debito < 0) {
		debito = 0;
		return debito;
	}else if(debito < 0) {
		debito *= -1;
		return debito;
	}
	return debito;
}

//Programa com as funcionalidades geral
int main() {
	//Faz a chamada para o arquivo que lê os dados iniciais do arquivo binario
	abrirBinario();

//	struct veiculos;
//	struct pessoa;
	int c, ident, aux, temp;
	char opcaoMenu, opcaoAluguel, opcaoParceiro;
	float valorDiaria, multa;
	static int posicao = 1, totalCadastrados;
	//Variaveis para data de Retirada do Veiculo
	int ano[99], mes[99], dia[99], horas[99], min[99];
	//Variaveis para data de Entrega do Veiculo
	int mesEn[99], diaEn[99], horasEn[99], minEn[99];
	//Variaveis usada para auxiliar no calculo da multa
	int multaMes, multaDia, multaHoras;
	
	menu();
	
	do{
		opcaoMenu = getchar();
		switch(opcaoMenu) {
			//Adicionar Novo Carro
			case '1':
				getchar();
				system("clear||cls");
				titulo();
				printf("\nDigite o modelo do carro:\n");
				fgets(carros[posicao].modelo, 50, stdin);
				printf("\nDigite a placa do carro:\n");
				fgets(carros[posicao].placa, 15, stdin);
				printf("\nDigite o numero de Identificacao do carro:\n");
				printf("Dica: Use um codigo simples entre 1 e 99 \" Ex: 01 \"\n");
				scanf("%i",&carros[posicao].tag);
				carros[posicao].ocupado = 0;
				totalCadastrados++;
				posicao++;
				system("clear||cls");
				printf("\nIdentificacao, Modelo e Placa adicionado com sucesso\n");
				informacoes(posicao - 1);

				//Gravar arquivo com struct de veiculos adicionados				
				arquivoVeiculos = fopen("dados.txt", "wb");
				fwrite(&carros, sizeof(struct veiculos), 99, arquivoVeiculos);
				fclose(arquivoVeiculos);
				//Gravar arquivo com TOTAL de veiculos adicionados				
				arquivoVeiculos = fopen("totalCadastrados.txt", "wb");
				fwrite(&totalCadastrados, sizeof(int), 1, arquivoTotal);
				fclose(arquivoTotal);
				getchar();
				menu();
				break;

			//Valor que será cobrado pela Diaria
			case '2':
				printf("\nInforme o valor cobrado pela diaria:\n");
				printf("**Clientes de Ongs Parceira terao 5%% de desconto e Idosos terao 10%% de desconto** [Apenas numeros e ponto]\nR$: ");
				scanf("%f",&valorDiaria);
				printf("\nDiaria de R$%.2f Cadastrada com sucesso\n", valorDiaria);
				printf("\nA Hora do Alguel sera de R$%.2f\n\n", valorDiaria / 24);
				getchar();
				menu();
				break;

			//Alugar Veiculo Agora ou agendar aluguel
			case '3':
				system("clear||cls");
				titulo();		
				printf("\nInforme o Tipo de Aluguel: \n{1} Alugar Agora\n{2} Agendar Aluguel\n");
				scanf("%i",&aux);
				if(aux == 1) {				
					listaVeiculos(totalCadastrados, '1', diaEn, mesEn, ano, horasEn);
					printf("\nDigite o numero de Identificacao do veiculo que deseja alugar:\n");
					scanf("%i",&ident);
					if(carros[ident].ocupado != 1) {
						system("clear||cls");
						titulo();
						aux = encontrarVeiculo(ident);
						informacoes(aux);
						printf("\nDeseja Alugar o Veiculo? [S / N]\n");
						scanf(" %c", &opcaoAluguel);
						if(opcaoAluguel == 's'|| opcaoAluguel == 'S') {
							do{
								printf("\nDigite o nome completo do cliente: [Apenas Letras]\n"); //Informações cliente nome
								getchar();//NOVA LINHA Resolve o bug de pular a coleta do Nome do cliente;
								fgets(cliente[ident].nome,65,stdin);
								printf("\nDigite o CPF do cliente: [Apenas Numeros]\n"); //Informações cliente CPF
								fgets(cliente[ident].cpf,15,stdin);
								printf("\nDigite o telefone de contato do cliente: [Apenas Numeros]\n"); //Informações cliente telefone
								fgets(cliente[ident].telef,15,stdin);
								printf("\nDeseja confirmar as informacoes: [S / N]\n");
								scanf("%c", &opcaoAluguel);
							}while(opcaoAluguel != 's' && opcaoAluguel != 'S');
							carros[aux].ocupado = 1;
							dataPedido(&ano[ident],&mes[ident],&dia[ident],&horas[ident],&min[ident]);
							minEn[ident] = 00;
							system("clear||cls");
							titulo();
							printf("\nInforme a data para Entrega do Veiculo [Apenas Numeros]\n");
							printf("\nDigite o Dia da Entrega:\n");
							if(getchar() >= 1|| getchar() <= 31) {
								scanf("%i",&diaEn[ident]);
							}
							printf("\nDigite o Mes da Entrega:\n");
							if(getchar() >= 1|| getchar() <= 12) {
								scanf("%i", &mesEn[ident]);
							}
							printf("\n**Dica: Calcule a hora da entrega levando em consideracao Condicoes Adversas**\n");
							printf("\nDigite a Hora da Entrega:\n");
							if(getchar() >= 0|| getchar() <= 23) {
								scanf("%i", &horasEn[ident]);
							}
							getchar();
							//Verifica se o aluguel foi maior que 5 dias ou foi mais de 1 mes
							if(((diaEn[ident] - dia[ident]) >= 5)||(mesEn[ident] > mes[ident])) {
								printf("\nVoce ganhou um dia de cortesia, deseja adicionar esse dia a mais a data de entrega? [S / N]\n");
								opcaoAluguel = getchar();
								if(opcaoAluguel == 's'|| opcaoAluguel == 'S') {
									diaEn[ident] += 1;
									temp = 1;
									printf("\nCortesia adicionada com sucesso\n");
								}
							}
							else
							{
								temp = 0;
								printf("\nNao recebe dia de cortesia\n");
							}
							multa = cobranca(temp, ident, valorDiaria, ano, mes, dia, horas, mesEn, diaEn, horasEn, min, minEn);
							printf("\nInformacoes do Aluguel:\n");
							if(temp == 1) {
								printf("\n**Foi adicionada a cortesia de 1 dia**\n");
							}
							printf("\n*Valor a receber sem descontos R$%.2f\n", multa);
							getchar();
							printf("\nPlaca do veiculo: %s\nNome: %s\nCPF: %s\nTelefone: %s\n", carros[ident].placa,cliente[ident].nome, cliente[ident].cpf, cliente[ident].telef);
							printf("\nO Cliente tem convenio com alguma ong parceira? [S / N]\n");
							opcaoParceiro = getchar();
							if(opcaoParceiro == 's'|| opcaoParceiro == 'S') {
								multa -= (multa * 5)/100;
								printf("\n*Valor a receber para parceiros R$%.2f\n", multa);
							}
							getchar();
							printf("\nO Cliente e um idoso? [S / N]\n");
							opcaoParceiro = getchar();
							if(opcaoParceiro == 's'|| opcaoParceiro == 'S') {
								multa -= (multa * 10)/100;
								printf("\n*Valor a receber para Idosos R$%.2f\n", multa);
							}
							printf("\n***Entregar Folheto com infomacoes da regiao***\n");
							printf("\nVeiculo Alugado com sucesso\n");
							menu();
							break;
						}
						else
						{	//Mostra a mensagem se a resposta para o aluguel for diferente de sim
							printf("\nErro no ao alugar veiculo, Tente novamente\n");
							menu();
							break;
						}
					}
					else
					{
						//Mostra a mensagem se o veiculo já estiver sido ocupado
						printf("\nErro no ao alugar veiculo, Tente novamente\n");
						menu();
						break;
					}
				} else if (aux == 2) {
					listaVeiculos(totalCadastrados, '1', diaEn, mesEn, ano, horasEn);
					printf("\nDigite o numero de Identificacao do veiculo que deseja alugar:\n");
					scanf("%i",&ident);
					if(carros[ident].ocupado != 1) {
						system("clear||cls");
						titulo();
						aux = encontrarVeiculo(ident);
						informacoes(aux);
						printf("\nDeseja Alugar o Veiculo? [S / N]\n");
						scanf(" %c", &opcaoAluguel);
						if(opcaoAluguel == 's'|| opcaoAluguel == 'S') {
							do{
								printf("\nDigite o nome completo do cliente: [Apenas Letras]\n"); //Informações cliente nome
								getchar();//NOVA LINHA Resolve o bug de pular a coleta do Nome do cliente;
								fgets(cliente[ident].nome,65,stdin);
								printf("\nDigite o CPF do cliente: [Apenas Numeros]\n"); //Informações cliente CPF
								fgets(cliente[ident].cpf,15,stdin);
								printf("\nDigite o telefone de contato do cliente: [Apenas Numeros]\n"); //Informações cliente telefone
								fgets(cliente[ident].telef,15,stdin);
								printf("\nDeseja confirmar as informacoes: [S / N]\n");
								scanf("%c", &opcaoAluguel);
							}while(opcaoAluguel != 's' && opcaoAluguel != 'S');
							carros[aux].ocupado = 1;
							dataPedido(&ano[ident],&mes[ident],&dia[ident],&horas[ident],&min[ident]);
							minEn[ident] = 00;
							min[ident] = 00;
							system("clear||cls");
							titulo();
							printf("\nInforme a data para Retirada e Entrega do Veiculo [Apenas Numeros]\n");
							printf("\nDigite o Dia para Retirar:\n");
							if(getchar() >= 1|| getchar() <= 31) {
								scanf("%i", &dia[ident]);
							}
							printf("\nDigite o Mes para Retirar:\n");
							if(getchar() >= 1|| getchar() <= 12) {
								scanf("%i", &mes[ident]);
							}
							printf("\n**Dica: Calcule a hora da entrega levando em consideracao Condicoes Adversas**\n");
							printf("\nDigite a Hora para Retirar:\n");
							if(getchar() >= 0|| getchar() <= 23) {
								scanf("%i", &horas[ident]);
							}
							system("clear||cls");
							printf("\nData da Retirada: %i/%i/%i  as %i:%i H\n", dia[ident], mes[ident], ano[ident], horas[ident], min[ident]);
							printf("\nDigite o Dia da Entrega:\n");
							if(getchar() >= 1|| getchar() <= 31) {
								scanf("%i", &diaEn[ident]);
							}
							printf("\nDigite o Mes da Entrega:\n");
							if(getchar() >= 1|| getchar() <= 12) {
								scanf("%i", &mesEn[ident]);
							}
							printf("\n**Dica: Calcule a hora da entrega levando em consideracao Condicoes Adversas**\n");
							printf("\nDigite a Hora da Entrega:\n");
							if(getchar() >= 0|| getchar() <= 23) {
								scanf("%i", &horasEn[ident]);
							}
							getchar();
							//Verifica se o aluguel foi maior que 5 dias ou foi mais de 1 mes
							if(((diaEn[ident] - dia[ident]) >= 5)||(mesEn[ident] > mes[ident])) {
								printf("\nVoce ganhou um dia de cortesia, deseja adicionar esse dia a mais a data de entrega? [S / N]\n");
								opcaoAluguel = getchar();
								if(opcaoAluguel == 's'|| opcaoAluguel == 'S') {
									diaEn[ident] += 1;
									temp = 1;
									printf("\nCortesia adicionada com sucesso\n");
								}
							}
							else
							{
								temp = 0;
								printf("\nNao recebe dia de cortesia\n");
							}
							multa = cobranca(temp, ident, valorDiaria, ano, mes, dia, horas, mesEn, diaEn, horasEn, min, minEn);
							printf("\nInformacoes do Aluguel:\n");
							if(temp == 1) {
								printf("\n**Foi adicionada a cortesia de 1 dia**\n");
							}
							printf("\nValor a receber R$%.2f\n", multa);
							getchar();
							printf("\nPlaca do veiculo: %s\nNome: %s\nCPF: %s\nTelefone: %s\n", carros[ident].placa,cliente[ident].nome, cliente[ident].cpf, cliente[ident].telef);
							printf("\nO Cliente tem convenio com alguma ong parceira? [S / N]\n");
							opcaoParceiro = getchar();
							if(opcaoParceiro == 's'|| opcaoParceiro == 'S') {
								multa -= (multa * 5)/100;
								printf("\n*Valor a receber para parceiros R$%.2f\n", multa);
							}
							getchar();
							printf("\nO Cliente e um idoso? [S / N]\n");
							opcaoParceiro = getchar();
							if(opcaoParceiro == 's'|| opcaoParceiro == 'S') {
								multa -= (multa * 10)/100;
								printf("\n*Valor a receber para Idosos R$%.2f\n", multa);
							}
							printf("\n***Entregar Folheto com infomacoes da regiao***\n");
							printf("\nVeiculo Alugado com sucesso\n");
							menu();
							break;
						}					
					}
					else
					{
						printf("\nOpcao Invalida\n");
					}
				}
				else
				{
					printf("\nOpcao Invalida\n");
				}
				getchar();
				menu();
				break;

			/*Disponibiliza o carro manualmente, caso o cliente faça a devolução antecipada;
			 caso contrario calcula o valor da multa que será cobrado*/
			case '4':
				system("clear||cls");
				titulo();
				dataPedido(&ano[ident],&mes[ident],&dia[ident],&horas[ident],&min[ident]);
				listaVeiculos(totalCadastrados, '2', diaEn, mesEn, ano, horasEn);
				printf("\nDigite o codigo de identificacao do veiculo que deseja reativar a disponibilidade:\n");
				scanf("%i",&ident);
				aux = encontrarVeiculo(ident);
				informacoes(aux);
				dataPedido(&ano[ident],&mes[ident],&dia[ident],&horas[ident],&min[ident]);
				temp = 0;
				//COBRANÇA
				multa = multaVerificacao(ident, valorDiaria, ano, mes, dia, horas, mesEn, diaEn, horasEn, min, minEn);
				printf("\nInformacoes do Aluguel:\n");
				printf("\nPlaca do veiculo: %s\nNome: %s\nCPF: %s\nTelefone: %s\n", carros[ident].placa, cliente[ident].nome, cliente[ident].cpf, cliente[ident].telef);
				printf("\nValor a receber R$%.2f\n", multa);
				printf("\nDeseja Reativar o Veiculo? [S / N]\n");
				scanf(" %c", &opcaoAluguel);
				if(opcaoAluguel == 's'|| opcaoAluguel == 'S') {
					carros[ident].ocupado = 0;
					printf("\nVeiculo Reativado com sucesso\n");
				}
				menu();
				break;

			//Imprimir na tela os carros disponiveis(1) e Indisponiveis(2)
			case '5':

				//Lê arquivo com struct de veiculos adicionados				
				arquivoVeiculos = fopen("dados.txt", "rb");
				fread(&carros, sizeof(struct veiculos), 99, arquivoVeiculos);
				fclose(arquivoVeiculos);

				//Lê arquivo com TOTAL de veiculos adicionados				
				arquivoVeiculos = fopen("totalCadastrados.txt", "rb");
				fread(&totalCadastrados, sizeof(int), 1, arquivoTotal);
				fclose(arquivoTotal);


				system("clear||cls");
				dataPedido(&ano[0],&mes[0],&dia[0],&horas[0],&min[0]);
				listaVeiculos(totalCadastrados,'1', diaEn, mesEn, ano, horasEn);
				listaVeiculos(totalCadastrados,'2', diaEn, mesEn, ano, horasEn);
				printf("\nPressione alguma tecla para continuar...\n");
				getchar();
				getchar();
				system("clear||cls");
				menu();
				break;

			//Sair do sistema
			case '6':
				printf("\nSaindo do sistema...\n");
				getchar();
				break;

			default:
				break;
		}
	}while(opcaoMenu != '6');

	getchar();
	return 0;
}
