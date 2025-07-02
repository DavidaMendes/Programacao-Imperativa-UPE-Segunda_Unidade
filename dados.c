#include <stdio.h>
#include <string.h>
#include "dados.h"

Usuario usuarios[MAX_USUARIOS];
Aluno alunos[MAX_ALUNOS];
int total_usuarios = 0;
int total_alunos = 0;
char usuario_logado[50];

void inicializar_sistema() {

    if (total_usuarios == 0) {
        strcpy(usuarios[0].usuario, "luciana");
        strcpy(usuarios[0].senha, "123456");
        usuarios[0].ativo = 1;
        total_usuarios = 1;
    }
}

void carregar_dados() {
    FILE *f_usuarios, *f_alunos;

    f_usuarios = fopen("usuarios.txt", "r");
    if (f_usuarios != NULL) {
        fscanf(f_usuarios, "%d", &total_usuarios);
        for (int i = 0; i < total_usuarios; i++) {
            fscanf(f_usuarios, "%s %s %d", usuarios[i].usuario, usuarios[i].senha, &usuarios[i].ativo);
        }
        fclose(f_usuarios);
    }

    f_alunos = fopen("alunos.txt", "r");
    if (f_alunos != NULL) {
        fscanf(f_alunos, "%d", &total_alunos);
        for (int i = 0; i < total_alunos; i++) {
            fscanf(f_alunos, "%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%f|%f|%d|%d|%d\n",
                   alunos[i].nome, alunos[i].rg, alunos[i].cpf, alunos[i].data_nascimento,
                   alunos[i].escola, alunos[i].turma, alunos[i].telefone,
                   alunos[i].nome_pai, alunos[i].nome_mae, alunos[i].endereco,
                   &alunos[i].valor_pago, &alunos[i].valor_total,
                   &alunos[i].embarque_confirmado, &alunos[i].seguro_ativo,
                   &alunos[i].ativo);
        }
        fclose(f_alunos);
    }
}

void salvar_dados() {
    FILE *f_usuarios, *f_alunos;

    f_usuarios = fopen("usuarios.txt", "w");
    if (f_usuarios != NULL) {
        fprintf(f_usuarios, "%d\n", total_usuarios);
        for (int i = 0; i < total_usuarios; i++) {
            fprintf(f_usuarios, "%s %s %d\n", usuarios[i].usuario, usuarios[i].senha, usuarios[i].ativo);
        }
        fclose(f_usuarios);
    }

    f_alunos = fopen("alunos.txt", "w");
    if (f_alunos != NULL) {
        fprintf(f_alunos, "%d\n", total_alunos);
        for (int i = 0; i < total_alunos; i++) {
            fprintf(f_alunos, "%s|%s|%s|%s|%s|%s|%s|%s|%s|%s|%.2f|%.2f|%d|%d|%d\n",
                    alunos[i].nome, alunos[i].rg, alunos[i].cpf, alunos[i].data_nascimento,
                    alunos[i].escola, alunos[i].turma, alunos[i].telefone,
                    alunos[i].nome_pai, alunos[i].nome_mae, alunos[i].endereco,
                    alunos[i].valor_pago, alunos[i].valor_total,
                    alunos[i].embarque_confirmado, alunos[i].seguro_ativo,
                    alunos[i].ativo); 
        }
        fclose(f_alunos);
    }
}

void debug_log_usuarios() {
    FILE *log_file = fopen("debug_log.txt", "w");
    if (log_file == NULL) {
        return; 
    }

    fprintf(log_file, "--- LOG DE USUÁRIOS NA MEMÓRIA ---\n");
    fprintf(log_file, "Total de usuários na memória: %d\n\n", total_usuarios);

    for (int i = 0; i < total_usuarios; i++) {
        fprintf(log_file, "Usuário[%d]:\n", i);
        fprintf(log_file, "  Nome: '%s'\n", usuarios[i].usuario);
        fprintf(log_file, "  Senha: '%s'\n", usuarios[i].senha);
        fprintf(log_file, "  Ativo: %d\n", usuarios[i].ativo);
        fprintf(log_file, "--------------------------------\n");
    }

    fclose(log_file);
}