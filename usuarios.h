#ifndef USUARIOS_H
#define USUARIOS_H

int fazer_login();
void menu_usuarios();
void cadastrar_usuario();
void listar_usuarios();
void alterar_usuario();
void remover_usuario();


int fazer_login_backend(const char* usuario, const char* senha);
int cadastrar_usuario_backend(const char *novoUsuario, const char *novaSenha);
int cadastrar_usuario_backend(const char *novoUsuario, const char *novaSenha);
int alterar_usuario_backend(const char *nomeUsuario, const char *novaSenha);
int remover_usuario_backend(const char *nomeUsuario);
#endif