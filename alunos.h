#ifndef DADOS_H
#define DADOS_H

#include <stdio.h>

#define MAX_USUARIOS 10
#define MAX_ALUNOS 100
#define MAX_STRING 100

typedef struct {
    char usuario[50];
    char senha[50];
    int ativo;
} Usuario;

typedef struct {
    char nome[MAX_STRING];
    char rg[20];
    char cpf[15];
    char data_nascimento[12];
    char escola[MAX_STRING];
    char turma[10];
    char telefone[20];
    char nome_pai[MAX_STRING];
    char nome_mae[MAX_STRING];
    char endereco[MAX_STRING];
    float valor_pago;
    float valor_total;
    int embarque_confirmado; 
    int seguro_ativo; 
    int ativo; 
} Aluno;

extern Usuario usuarios[MAX_USUARIOS];
extern Aluno alunos[MAX_ALUNOS];
extern int total_usuarios;
extern int total_alunos;
extern char usuario_logado[50];

void inicializar_sistema();
void carregar_dados();
void salvar_dados();
void menu_principal();
void debug_log_usuarios();

#endif