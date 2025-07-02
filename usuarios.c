#include <stdio.h>
#include <string.h>
#include "dados.h"
#include "utils.h"
#include "usuarios.h"

int fazer_login() {
    char usuario[50], senha[50];
    int tentativas = 3;
    
    while (tentativas > 0) {
        printf("=== LOGIN ===\n");
        printf("Usuário: ");
        scanf("%s", usuario);
        printf("Senha: ");
        scanf("%s", senha);
        
        for (int i = 0; i < total_usuarios; i++) {
            if (strcmp(usuarios[i].usuario, usuario) == 0 && 
                strcmp(usuarios[i].senha, senha) == 0 && 
                usuarios[i].ativo == 1) {
                strcpy(usuario_logado, usuario);
                printf("\nLogin realizado com sucesso!\n");
                printf("Bem-vindo(a), %s!\n", usuario_logado);
                pausar();
                return 1;
            }
        }
        
        tentativas--;
        printf("\nUsuário ou senha incorretos! Tentativas restantes: %d\n", tentativas);
        pausar();
        limpar_tela();
    }
    
    printf("Número máximo de tentativas excedido!\n");
    return 0;
}

void menu_usuarios() {
    int opcao;
    
    do {
        limpar_tela();
        printf("=== GERENCIAR USUÁRIOS ===\n\n");
        printf("1. Cadastrar Usuário\n");
        printf("2. Listar Usuários\n");
        printf("3. Alterar Usuário\n");
        printf("4. Remover Usuário\n");
        printf("0. Voltar\n");
        printf("\nEscolha uma opção: ");
        scanf("%d", &opcao);
        
        switch (opcao) {
            case 1:
                cadastrar_usuario();
                break;
            case 2:
                listar_usuarios();
                break;
            case 3:
                alterar_usuario();
                break;
            case 4:
                remover_usuario();
                break;
            case 0:
                break;
            default:
                printf("Opção inválida!\n");
                pausar();
        }
    } while (opcao != 0);
}

void cadastrar_usuario() {
    if (total_usuarios >= MAX_USUARIOS) {
        printf("Limite máximo de usuários atingido!\n");
        pausar();
        return;
    }
    
    limpar_tela();
    printf("=== CADASTRAR USUÁRIO ===\n\n");
    
    printf("Nome de usuário: ");
    scanf("%s", usuarios[total_usuarios].usuario);
    
    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, usuarios[total_usuarios].usuario) == 0) {
            printf("Usuário já existe!\n");
            pausar();
            return;
        }
    }
    
    printf("Senha: ");
    scanf("%s", usuarios[total_usuarios].senha);
    usuarios[total_usuarios].ativo = 1;
    
    total_usuarios++;
    printf("\nUsuário cadastrado com sucesso!\n");
    pausar();
}

void listar_usuarios() {
    limpar_tela();
    printf("=== LISTA DE USUÁRIOS ===\n\n");
    
    if (total_usuarios == 0) {
        printf("Nenhum usuário cadastrado.\n");
    } else {
        printf("%-20s %-10s\n", "USUÁRIO", "STATUS");
        printf("----------------------------------------\n");
        for (int i = 0; i < total_usuarios; i++) {
            printf("%-20s %-10s\n", usuarios[i].usuario, 
                   usuarios[i].ativo ? "Ativo" : "Inativo");
        }
    }
    pausar();
}

void alterar_usuario() {
    char usuario[50];
    int encontrado = 0;
    
    limpar_tela();
    printf("=== ALTERAR USUÁRIO ===\n\n");
    
    printf("Nome do usuário a alterar: ");
    scanf("%s", usuario);
    
    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0) {
            printf("Nova senha: ");
            scanf("%s", usuarios[i].senha);
            printf("Usuário alterado com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    
    if (!encontrado) {
        printf("Usuário não encontrado!\n");
    }
    pausar();
}

void remover_usuario() {
    char usuario[50];
    int encontrado = 0;
    
    limpar_tela();
    printf("=== REMOVER USUÁRIO ===\n\n");
    
    printf("Nome do usuário a remover: ");
    scanf("%s", usuario);
    
    if (strcmp(usuario, "luciana") == 0) {
        printf("Não é possível remover o usuário principal!\n");
        pausar();
        return;
    }
    
    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0) {
            usuarios[i].ativo = 0;
            printf("Usuário removido com sucesso!\n");
            encontrado = 1;
            break;
        }
    }
    
    if (!encontrado) {
        printf("Usuário não encontrado!\n");
    }
    pausar();
}

int fazer_login_backend(const char* usuario, const char* senha) {
    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, usuario) == 0 &&
            strcmp(usuarios[i].senha, senha) == 0 &&
            usuarios[i].ativo == 1) {
            return 1;
        }
    }
    return 0; 
}
int cadastrar_usuario_backend(const char *novoUsuario, const char *novaSenha) {
    if (total_usuarios >= MAX_USUARIOS) {
        return 2; 
    }

    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, novoUsuario) == 0) {
            return 1; 
        }
    }

    strcpy(usuarios[total_usuarios].usuario, novoUsuario);
    strcpy(usuarios[total_usuarios].senha, novaSenha);
    usuarios[total_usuarios].ativo = 1;
    total_usuarios++;

    return 0; 
}
int alterar_usuario_backend(const char *nomeUsuario, const char *novaSenha) {
    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, nomeUsuario) == 0) {
            strcpy(usuarios[i].senha, novaSenha);
            return 0; 
        }
    }
    return 1; 
}
int remover_usuario_backend(const char *nomeUsuario) {
    if (strcmp(nomeUsuario, "luciana") == 0) {
        return 2; 
    }

    for (int i = 0; i < total_usuarios; i++) {
        if (strcmp(usuarios[i].usuario, nomeUsuario) == 0) {
            usuarios[i].ativo = 0; 
            return 0; 
        }
    }
    return 1;