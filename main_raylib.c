#include <stdio.h>
#include <string.h>
#include <stdlib.h> 
#include "raylib.h"
#include "dados.h"
#include "usuarios.h"
#include "alunos.h"

typedef enum GameScreen {
    LOGIN,
    MAIN_MENU,
    MANAGE_USERS,
    USER_ADD,
    USER_LIST,
    USER_EDIT,
    USER_REMOVE,
    PAYMENT_MENU,
    ALUNO_ADD,
    ALUNO_LIST_PAYMENTS,
    ALUNO_UPDATE_PAYMENT,
    BOARDING_MENU,
    BOARDING_CONFIRM,
    BOARDING_LIST_VIEW,
    INSURANCE_MENU,
    INSURANCE_ACTIVATE,
    INSURANCE_LIST_VIEW
} GameScreen;

int fazer_login_backend(const char* usuario, const char* senha);
int cadastrar_usuario_backend(const char *novoUsuario, const char *novaSenha);
int alterar_usuario_backend(const char *nomeUsuario, const char *novaSenha);
int remover_usuario_backend(const char *nomeUsuario);
int gerar_relatorio_html();
int cadastrar_aluno_backend(Aluno novoAluno);
int atualizar_pagamento_backend(const char* rgAluno, float valorAdicionar);

int main(void) {
    const int screenWidth = 800;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Lumaster Turismo - Raylib");
    ToggleFullscreen();
    SetExitKey(0);
    SetTargetFPS(60);

    const char *charSet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,:;!?%()[]{}<>@#$_-+=*&/\\\'\"| R$áéíóúÁÉÍÓÚâêîôûÂÊÎÔÛàèìòùÀÈÌÒÙãõÃÕçÇ";
    int codepointCount = 0;
    int *codepoints = LoadCodepoints(charSet, &codepointCount);
    Font font = LoadFontEx("C:/Windows/Fonts/verdana.ttf", 96, codepoints, codepointCount);
    UnloadCodepoints(codepoints);

    inicializar_sistema();
    carregar_dados();
    debug_log_usuarios();

    GameScreen currentScreen = LOGIN;
    bool closeApp = false;
    int activeTextBox = 0;
    int framesCounter = 0;

    char username[51] = "\0"; int usernameLetterCount = 0;
    char password[51] = "\0"; int passwordLetterCount = 0;
    int loginAttempts = 3; const char *loginErrorMsg = "";
    Rectangle textBoxUsername = { screenWidth/2.0f-150, 180, 300, 40 };
    Rectangle textBoxPassword = { screenWidth/2.0f-150, 250, 300, 40 };
    Rectangle buttonLogin = { screenWidth/2.0f-150, 320, 300, 50 };

    char newUsername[51] = "\0"; int newUsernameLetterCount = 0;
    char newPassword[51] = "\0"; int newPasswordLetterCount = 0;
    char userAddFeedbackMsg[100] = "";
    Rectangle textBoxNewUsername = { screenWidth/2.0f-150, 180, 300, 40 };
    Rectangle textBoxNewPassword = { screenWidth/2.0f-150, 250, 300, 40 };
    Rectangle buttonConfirmAdd = { screenWidth/2.0f-150, 320, 140, 50 };
    Rectangle buttonCancelAdd = { screenWidth/2.0f+10, 320, 140, 50 };
    char userToEdit[51] = "\0"; int userToEditLetterCount = 0;
    char newPasswordForEdit[51] = "\0"; int newPasswordForEditLetterCount = 0;
    char userEditFeedbackMsg[100] = "";
    Rectangle textBoxUserToEdit = { screenWidth/2.0f-150, 150, 300, 40 };
    Rectangle textBoxNewPasswordForEdit = { screenWidth/2.0f-150, 220, 300, 40 };
    Rectangle buttonConfirmEdit = { screenWidth/2.0f-150, 290, 300, 50 };
    char userToRemove[51] = "\0"; int userToRemoveLetterCount = 0;
    char userRemoveFeedbackMsg[100] = "";
    Rectangle textBoxUserToRemove = { screenWidth/2.0f-150, 150, 300, 40 };
    Rectangle buttonConfirmRemove = { screenWidth/2.0f-150, 220, 300, 50 };
    char alunoNome[MAX_STRING] = "\0"; int alunoNomeLetterCount = 0;
    char alunoRg[20] = "\0"; int alunoRgLetterCount = 0;
    char alunoCpf[15] = "\0"; int alunoCpfLetterCount = 0;
    char alunoNascimento[12] = "\0"; int alunoNascimentoLetterCount = 0;
    char alunoEscola[MAX_STRING] = "\0"; int alunoEscolaLetterCount = 0;
    char alunoTurma[10] = "\0"; int alunoTurmaLetterCount = 0;
    char alunoTelefone[20] = "\0"; int alunoTelefoneLetterCount = 0;
    char alunoPai[MAX_STRING] = "\0"; int alunoPaiLetterCount = 0;
    char alunoMae[MAX_STRING] = "\0"; int alunoMaeLetterCount = 0;
    char alunoEndereco[MAX_STRING] = "\0"; int alunoEnderecoLetterCount = 0;
    char alunoValorTotalStr[20] = "\0"; int alunoValorTotalLetterCount = 0; 
    char alunoAddFeedbackMsg[100] = "";
    const int formLabelX = 50;
    const int formFieldX = 250;
    const int formFieldWidth = 500;
    const int formVerticalSpacing = 40;
    int formStartY = 80;
    Rectangle txtNome = { formFieldX, formStartY, formFieldWidth, 30 };
    Rectangle txtRg = { formFieldX, formStartY + formVerticalSpacing, formFieldWidth, 30 };
    Rectangle txtCpf = { formFieldX, formStartY + formVerticalSpacing*2, formFieldWidth, 30 };
    Rectangle txtNascimento = { formFieldX, formStartY + formVerticalSpacing*3, formFieldWidth, 30 };
    Rectangle txtTelefone = { formFieldX, formStartY + formVerticalSpacing*4, formFieldWidth, 30 };
    Rectangle txtEscola = { formFieldX, formStartY + formVerticalSpacing*5, formFieldWidth, 30 };
    Rectangle txtTurma = { formFieldX, formStartY + formVerticalSpacing*6, formFieldWidth, 30 };
    Rectangle txtNomePai = { formFieldX, formStartY + formVerticalSpacing*7, formFieldWidth, 30 };
    Rectangle txtNomeMae = { formFieldX, formStartY + formVerticalSpacing*8, formFieldWidth, 30 };
    Rectangle txtEndereco = { formFieldX, formStartY + formVerticalSpacing*9, formFieldWidth, 30 };
    Rectangle txtValorTotal = { formFieldX, formStartY + formVerticalSpacing*10, formFieldWidth, 30 };
    Rectangle btnConfirmarAluno = { screenWidth/2.0f - 150, formStartY + formVerticalSpacing*11 + 20, 140, 50 };
    Rectangle btnCancelarAluno = { screenWidth/2.0f + 10, formStartY + formVerticalSpacing*11 + 20, 140, 50 };
    char rgParaAtualizar[20] = "\0"; int rgParaAtualizarLetterCount = 0;
    char valorParaAdicionarStr[20] = "\0"; int valorParaAdicionarLetterCount = 0;
    char pagamentoUpdateFeedbackMsg[100] = "";
    int alunoIndexPagamento = -1;
    Rectangle txtRgBuscaPag = { screenWidth/2.0f - 150, 150, 300, 40 };
    Rectangle btnBuscarRgPag = { screenWidth/2.0f - 150, 200, 300, 40 };
    Rectangle txtValorAdicionar = { screenWidth/2.0f - 150, 350, 300, 40 };
    Rectangle btnConfirmarPagamento = { screenWidth/2.0f - 150, 400, 300, 50 };
    char rgParaEmbarque[20] = "\0"; int rgParaEmbarqueLetterCount = 0;
    char embarqueConfirmFeedbackMsg[100] = "";
    int alunoIndexEmbarque = -1;
    Rectangle txtRgBuscaEmb = { screenWidth/2.0f - 150, 150, 300, 40 };
    Rectangle btnBuscarRgEmb = { screenWidth/2.0f - 150, 200, 300, 40 };
    Rectangle btnEmbarqueConfirmar = { screenWidth/2.0f - 155, 420, 150, 50 };
    Rectangle btnEmbarqueCancelar = { screenWidth/2.0f + 5, 420, 150, 50 };
    char rgParaSeguro[20] = "\0"; int rgParaSeguroLetterCount = 0;
    char seguroActivateFeedbackMsg[100] = "";
    int alunoIndexSeguro = -1;
    Rectangle txtRgBuscaSeg = { screenWidth/2.0f - 150, 150, 300, 40 };
    Rectangle btnBuscarRgSeg = { screenWidth/2.0f - 150, 200, 300, 40 };
    Rectangle btnSeguroAtivar = { screenWidth/2.0f - 155, 420, 150, 50 };
    Rectangle btnSeguroDesativar = { screenWidth/2.0f + 5, 420, 150, 50 };
    char reportFeedbackMsg[100] = "";
    Rectangle btnManageUsers = { screenWidth/2.0f-200, 150, 400, 50 };
    Rectangle btnPaymentControl = { screenWidth/2.0f-200, 210, 400, 50 };
    Rectangle btnBoardingControl = { screenWidth/2.0f-200, 270, 400, 50 };
    Rectangle btnTravelInsurance = { screenWidth/2.0f-200, 330, 400, 50 };
    Rectangle btnGenerateReport = { screenWidth/2.0f-200, 390, 400, 50 };
    Rectangle btnExit = { screenWidth/2.0f-200, 450, 400, 50 };
    Rectangle btnUserAdd = { screenWidth/2.0f-200, 150, 400, 50 };
    Rectangle btnUserList = { screenWidth/2.0f-200, 210, 400, 50 };
    Rectangle btnUserEdit = { screenWidth/2.0f-200, 270, 400, 50 };
    Rectangle btnUserRemove = { screenWidth/2.0f-200, 330, 400, 50 };
    Rectangle btnUserBack = { screenWidth/2.0f-200, 410, 400, 50 };
    Rectangle btnAlunoAdd = { screenWidth/2.0f-200, 150, 400, 50 };
    Rectangle btnAlunoListPayments = { screenWidth/2.0f-200, 210, 400, 50 };
    Rectangle btnAlunoUpdatePayment = { screenWidth/2.0f-200, 270, 400, 50 };
    Rectangle btnPaymentBack = { screenWidth/2.0f-200, 410, 400, 50 };
    Rectangle btnBoardingConfirm = { screenWidth/2.0f-200, 150, 400, 50 };
    Rectangle btnBoardingList = { screenWidth/2.0f-200, 210, 400, 50 };
    Rectangle btnBoardingBack = { screenWidth/2.0f-200, 410, 400, 50 };
    Rectangle btnInsuranceActivate = { screenWidth/2.0f-200, 150, 400, 50 };
    Rectangle btnInsuranceList = { screenWidth/2.0f-200, 210, 400, 50 };
    Rectangle btnInsuranceBack = { screenWidth/2.0f-200, 410, 400, 50 };
    
    #define HANDLE_TEXT_INPUT(textBoxID, textVar, letterCountVar, maxLength) \
        if (activeTextBox == textBoxID) { \
            int key = GetCharPressed(); \
            while (key > 0) { \
                if (((key >= 32 && key <= 126)) && (letterCountVar < maxLength - 1)) { \
                    textVar[letterCountVar] = (char)key; \
                    textVar[letterCountVar+1] = '\0'; \
                    letterCountVar++; \
                } \
                key = GetCharPressed(); \
            } \
            if (IsKeyPressed(KEY_BACKSPACE)) { \
                letterCountVar--; \
                if (letterCountVar < 0) letterCountVar = 0; \
                textVar[letterCountVar] = '\0'; \
            } \
        }

    while (!WindowShouldClose() && !closeApp) {
        framesCounter++;
        switch (currentScreen) {
            case LOGIN: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), textBoxUsername)) activeTextBox = 1; else if (CheckCollisionPointRec(GetMousePosition(), textBoxPassword)) activeTextBox = 2; else activeTextBox = 0; } HANDLE_TEXT_INPUT(1, username, usernameLetterCount, 51); HANDLE_TEXT_INPUT(2, password, passwordLetterCount, 51); if (CheckCollisionPointRec(GetMousePosition(), buttonLogin) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (fazer_login_backend(username, password)) { currentScreen = MAIN_MENU; strcpy(usuario_logado, username); } else { loginAttempts--; loginErrorMsg = "Usuário ou senha inválidos!"; if (loginAttempts <= 0) loginErrorMsg = "Tentativas esgotadas. Feche a aplicação."; } } } break;
            case MAIN_MENU: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { strcpy(reportFeedbackMsg, ""); if (CheckCollisionPointRec(GetMousePosition(), btnManageUsers)) currentScreen = MANAGE_USERS; if (CheckCollisionPointRec(GetMousePosition(), btnPaymentControl)) currentScreen = PAYMENT_MENU; if (CheckCollisionPointRec(GetMousePosition(), btnBoardingControl)) currentScreen = BOARDING_MENU; if (CheckCollisionPointRec(GetMousePosition(), btnTravelInsurance)) currentScreen = INSURANCE_MENU; if (CheckCollisionPointRec(GetMousePosition(), btnGenerateReport)) { if (gerar_relatorio_html() == 0) { strcpy(reportFeedbackMsg, "Relatório 'relatorio_lumaster.html' gerado com sucesso!"); } else { strcpy(reportFeedbackMsg, "Erro ao gerar o relatório."); } } if (CheckCollisionPointRec(GetMousePosition(), btnExit)) closeApp = true; } } break;
            case MANAGE_USERS: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), btnUserAdd)) { strcpy(userAddFeedbackMsg, ""); currentScreen = USER_ADD; } if (CheckCollisionPointRec(GetMousePosition(), btnUserList)) currentScreen = USER_LIST; if (CheckCollisionPointRec(GetMousePosition(), btnUserEdit)) { strcpy(userEditFeedbackMsg, ""); currentScreen = USER_EDIT; } if (CheckCollisionPointRec(GetMousePosition(), btnUserRemove)) { strcpy(userRemoveFeedbackMsg, ""); currentScreen = USER_REMOVE; } if (CheckCollisionPointRec(GetMousePosition(), btnUserBack)) currentScreen = MAIN_MENU; } if (IsKeyPressed(KEY_ESCAPE)) currentScreen = MAIN_MENU; } break;
            case USER_ADD: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), textBoxNewUsername)) activeTextBox = 1; else if (CheckCollisionPointRec(GetMousePosition(), textBoxNewPassword)) activeTextBox = 2; else activeTextBox = 0; } HANDLE_TEXT_INPUT(1, newUsername, newUsernameLetterCount, 51); HANDLE_TEXT_INPUT(2, newPassword, newPasswordLetterCount, 51); if (CheckCollisionPointRec(GetMousePosition(), buttonConfirmAdd) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { int result = cadastrar_usuario_backend(newUsername, newPassword); if (result == 0) { strcpy(userAddFeedbackMsg, "Usuário cadastrado com sucesso!"); newUsername[0] = '\0'; newUsernameLetterCount = 0; newPassword[0] = '\0'; newPasswordLetterCount = 0; } else if (result == 1) { strcpy(userAddFeedbackMsg, "Erro: Este nome de usuário já existe."); } else if (result == 2) { strcpy(userAddFeedbackMsg, "Erro: Limite máximo de usuários atingido."); } } if (CheckCollisionPointRec(GetMousePosition(), buttonCancelAdd) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { newUsername[0] = '\0'; newUsernameLetterCount = 0; newPassword[0] = '\0'; newPasswordLetterCount = 0; strcpy(userAddFeedbackMsg, ""); currentScreen = MANAGE_USERS; } if (IsKeyPressed(KEY_ESCAPE)) { newUsername[0] = '\0'; newUsernameLetterCount = 0; newPassword[0] = '\0'; newPasswordLetterCount = 0; strcpy(userAddFeedbackMsg, ""); currentScreen = MANAGE_USERS; } } break;
            case USER_EDIT: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), textBoxUserToEdit)) activeTextBox = 1; else if (CheckCollisionPointRec(GetMousePosition(), textBoxNewPasswordForEdit)) activeTextBox = 2; else activeTextBox = 0; } HANDLE_TEXT_INPUT(1, userToEdit, userToEditLetterCount, 51); HANDLE_TEXT_INPUT(2, newPasswordForEdit, newPasswordForEditLetterCount, 51); if (CheckCollisionPointRec(GetMousePosition(), buttonConfirmEdit) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { int result = alterar_usuario_backend(userToEdit, newPasswordForEdit); if (result == 0) { strcpy(userEditFeedbackMsg, "Senha alterada com sucesso!"); } else if (result == 1) { strcpy(userEditFeedbackMsg, "Erro: Usuário não encontrado."); } } if (IsKeyPressed(KEY_ESCAPE)) { userToEdit[0] = '\0'; userToEditLetterCount = 0; newPasswordForEdit[0] = '\0'; newPasswordForEditLetterCount = 0; strcpy(userEditFeedbackMsg, ""); currentScreen = MANAGE_USERS; } } break;
            case USER_REMOVE: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), textBoxUserToRemove)) activeTextBox = 1; else activeTextBox = 0; } HANDLE_TEXT_INPUT(1, userToRemove, userToRemoveLetterCount, 51); if (CheckCollisionPointRec(GetMousePosition(), buttonConfirmRemove) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { int result = remover_usuario_backend(userToRemove); if (result == 0) { strcpy(userRemoveFeedbackMsg, "Usuário desativado com sucesso!"); } else if (result == 1) { strcpy(userRemoveFeedbackMsg, "Erro: Usuário não encontrado."); } else if (result == 2) { strcpy(userRemoveFeedbackMsg, "Erro: Não é permitido desativar o admin."); } } if (IsKeyPressed(KEY_ESCAPE)) { userToRemove[0] = '\0'; userToRemoveLetterCount = 0; strcpy(userRemoveFeedbackMsg, ""); currentScreen = MANAGE_USERS; } } break;
            case USER_LIST: { if (IsKeyPressed(KEY_ESCAPE)) currentScreen = MANAGE_USERS; } break;
            case PAYMENT_MENU: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), btnAlunoAdd)) { strcpy(alunoAddFeedbackMsg, ""); currentScreen = ALUNO_ADD; } if (CheckCollisionPointRec(GetMousePosition(), btnAlunoListPayments)) currentScreen = ALUNO_LIST_PAYMENTS; if (CheckCollisionPointRec(GetMousePosition(), btnAlunoUpdatePayment)) { strcpy(pagamentoUpdateFeedbackMsg, ""); alunoIndexPagamento = -1; rgParaAtualizar[0] = '\0'; rgParaAtualizarLetterCount = 0; valorParaAdicionarStr[0] = '\0'; valorParaAdicionarLetterCount = 0; currentScreen = ALUNO_UPDATE_PAYMENT; } if (CheckCollisionPointRec(GetMousePosition(), btnPaymentBack)) currentScreen = MAIN_MENU; } if (IsKeyPressed(KEY_ESCAPE)) currentScreen = MAIN_MENU; } break;
            case ALUNO_ADD: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), txtNome)) activeTextBox = 1; else if (CheckCollisionPointRec(GetMousePosition(), txtRg)) activeTextBox = 2; else if (CheckCollisionPointRec(GetMousePosition(), txtCpf)) activeTextBox = 3; else if (CheckCollisionPointRec(GetMousePosition(), txtNascimento)) activeTextBox = 4; else if (CheckCollisionPointRec(GetMousePosition(), txtTelefone)) activeTextBox = 5; else if (CheckCollisionPointRec(GetMousePosition(), txtEscola)) activeTextBox = 6; else if (CheckCollisionPointRec(GetMousePosition(), txtTurma)) activeTextBox = 7; else if (CheckCollisionPointRec(GetMousePosition(), txtNomePai)) activeTextBox = 8; else if (CheckCollisionPointRec(GetMousePosition(), txtNomeMae)) activeTextBox = 9; else if (CheckCollisionPointRec(GetMousePosition(), txtEndereco)) activeTextBox = 10; else if (CheckCollisionPointRec(GetMousePosition(), txtValorTotal)) activeTextBox = 11; else activeTextBox = 0; } HANDLE_TEXT_INPUT(1, alunoNome, alunoNomeLetterCount, MAX_STRING); HANDLE_TEXT_INPUT(2, alunoRg, alunoRgLetterCount, 20); HANDLE_TEXT_INPUT(3, alunoCpf, alunoCpfLetterCount, 15); HANDLE_TEXT_INPUT(4, alunoNascimento, alunoNascimentoLetterCount, 12); HANDLE_TEXT_INPUT(5, alunoTelefone, alunoTelefoneLetterCount, 20); HANDLE_TEXT_INPUT(6, alunoEscola, alunoEscolaLetterCount, MAX_STRING); HANDLE_TEXT_INPUT(7, alunoTurma, alunoTurmaLetterCount, 10); HANDLE_TEXT_INPUT(8, alunoPai, alunoPaiLetterCount, MAX_STRING); HANDLE_TEXT_INPUT(9, alunoMae, alunoMaeLetterCount, MAX_STRING); HANDLE_TEXT_INPUT(10, alunoEndereco, alunoEnderecoLetterCount, MAX_STRING); HANDLE_TEXT_INPUT(11, alunoValorTotalStr, alunoValorTotalLetterCount, 20); if (CheckCollisionPointRec(GetMousePosition(), btnConfirmarAluno) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { float valorTotal = atof(alunoValorTotalStr); if (valorTotal <= 0) { strcpy(alunoAddFeedbackMsg, "Erro: Valor total deve ser um número positivo."); } else { Aluno novoAluno; strcpy(novoAluno.nome, alunoNome); strcpy(novoAluno.rg, alunoRg); strcpy(novoAluno.cpf, alunoCpf); strcpy(novoAluno.data_nascimento, alunoNascimento); strcpy(novoAluno.escola, alunoEscola); strcpy(novoAluno.turma, alunoTurma); strcpy(novoAluno.telefone, alunoTelefone); strcpy(novoAluno.nome_pai, alunoPai); strcpy(novoAluno.nome_mae, alunoMae); strcpy(novoAluno.endereco, alunoEndereco); novoAluno.valor_total = valorTotal; novoAluno.valor_pago = 0.0f; novoAluno.embarque_confirmado = 0; novoAluno.seguro_ativo = 0; novoAluno.ativo = 1; int result = cadastrar_aluno_backend(novoAluno); if (result == 0) { strcpy(alunoAddFeedbackMsg, "Aluno cadastrado com sucesso!"); alunoNome[0] = '\0'; alunoNomeLetterCount = 0; alunoRg[0] = '\0'; alunoRgLetterCount = 0; alunoCpf[0] = '\0'; alunoCpfLetterCount = 0; alunoNascimento[0] = '\0'; alunoNascimentoLetterCount = 0; alunoEscola[0] = '\0'; alunoEscolaLetterCount = 0; alunoTurma[0] = '\0'; alunoTurmaLetterCount = 0; alunoTelefone[0] = '\0'; alunoTelefoneLetterCount = 0; alunoPai[0] = '\0'; alunoPaiLetterCount = 0; alunoMae[0] = '\0'; alunoMaeLetterCount = 0; alunoEndereco[0] = '\0'; alunoEnderecoLetterCount = 0; alunoValorTotalStr[0] = '\0'; alunoValorTotalLetterCount = 0; } else if (result == 1) { strcpy(alunoAddFeedbackMsg, "Erro: RG já cadastrado."); } else if (result == 2) { strcpy(alunoAddFeedbackMsg, "Erro: Limite de alunos atingido."); } } } if ((CheckCollisionPointRec(GetMousePosition(), btnCancelarAluno) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) || IsKeyPressed(KEY_ESCAPE)) { currentScreen = PAYMENT_MENU; } } break;
            case ALUNO_LIST_PAYMENTS: { if (IsKeyPressed(KEY_ESCAPE)) currentScreen = PAYMENT_MENU; } break;
            case ALUNO_UPDATE_PAYMENT: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), txtRgBuscaPag)) activeTextBox = 1; else if (CheckCollisionPointRec(GetMousePosition(), txtValorAdicionar) && alunoIndexPagamento != -1 && alunos[alunoIndexPagamento].ativo) activeTextBox = 2; else activeTextBox = 0; } if (alunoIndexPagamento == -1) { HANDLE_TEXT_INPUT(1, rgParaAtualizar, rgParaAtualizarLetterCount, 20); } HANDLE_TEXT_INPUT(2, valorParaAdicionarStr, valorParaAdicionarLetterCount, 20); if (CheckCollisionPointRec(GetMousePosition(), btnBuscarRgPag) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && alunoIndexPagamento == -1) { alunoIndexPagamento = -1; for (int i = 0; i < total_alunos; i++) { if (strcmp(alunos[i].rg, rgParaAtualizar) == 0) { alunoIndexPagamento = i; strcpy(pagamentoUpdateFeedbackMsg, "Aluno encontrado. Digite o valor."); break; } } if (alunoIndexPagamento == -1) { strcpy(pagamentoUpdateFeedbackMsg, "Erro: Aluno não encontrado com este RG."); } } if (alunoIndexPagamento != -1 && CheckCollisionPointRec(GetMousePosition(), btnConfirmarPagamento) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (alunos[alunoIndexPagamento].ativo) { float valor = atof(valorParaAdicionarStr); if (valor > 0) { int result = atualizar_pagamento_backend(rgParaAtualizar, valor); if (result == 0) { if (!alunos[alunoIndexPagamento].ativo) { strcpy(pagamentoUpdateFeedbackMsg, "Pagamento concluído! Aluno desativado."); } else { strcpy(pagamentoUpdateFeedbackMsg, "Pagamento atualizado com sucesso!"); } valorParaAdicionarStr[0] = '\0'; valorParaAdicionarLetterCount = 0; } else { strcpy(pagamentoUpdateFeedbackMsg, "Erro inesperado ao atualizar."); } } else { strcpy(pagamentoUpdateFeedbackMsg, "Erro: Valor inválido. Digite um número positivo."); } } } if (IsKeyPressed(KEY_ESCAPE)) { currentScreen = PAYMENT_MENU; } } break;
            case BOARDING_MENU: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), btnBoardingConfirm)) { alunoIndexEmbarque = -1; rgParaEmbarque[0] = '\0'; rgParaEmbarqueLetterCount = 0; strcpy(embarqueConfirmFeedbackMsg, ""); currentScreen = BOARDING_CONFIRM; } if (CheckCollisionPointRec(GetMousePosition(), btnBoardingList)) currentScreen = BOARDING_LIST_VIEW; if (CheckCollisionPointRec(GetMousePosition(), btnBoardingBack)) currentScreen = MAIN_MENU; } if (IsKeyPressed(KEY_ESCAPE)) currentScreen = MAIN_MENU; } break;
            case BOARDING_CONFIRM: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), txtRgBuscaEmb)) activeTextBox = 1; else activeTextBox = 0; } if (alunoIndexEmbarque == -1) { HANDLE_TEXT_INPUT(1, rgParaEmbarque, rgParaEmbarqueLetterCount, 20); } if (CheckCollisionPointRec(GetMousePosition(), btnBuscarRgEmb) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && alunoIndexEmbarque == -1) { alunoIndexEmbarque = -1; for (int i = 0; i < total_alunos; i++) { if (strcmp(alunos[i].rg, rgParaEmbarque) == 0) { alunoIndexEmbarque = i; strcpy(embarqueConfirmFeedbackMsg, "Aluno encontrado."); break; } } if (alunoIndexEmbarque == -1) { strcpy(embarqueConfirmFeedbackMsg, "Erro: Aluno não encontrado com este RG."); } } if (alunoIndexEmbarque != -1) { if (CheckCollisionPointRec(GetMousePosition(), btnEmbarqueConfirmar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { alunos[alunoIndexEmbarque].embarque_confirmado = 1; strcpy(embarqueConfirmFeedbackMsg, "Embarque CONFIRMADO com sucesso!"); alunoIndexEmbarque = -1; rgParaEmbarque[0] = '\0'; rgParaEmbarqueLetterCount = 0; } if (CheckCollisionPointRec(GetMousePosition(), btnEmbarqueCancelar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { alunos[alunoIndexEmbarque].embarque_confirmado = 0; strcpy(embarqueConfirmFeedbackMsg, "Embarque CANCELADO com sucesso!"); alunoIndexEmbarque = -1; rgParaEmbarque[0] = '\0'; rgParaEmbarqueLetterCount = 0; } } if (IsKeyPressed(KEY_ESCAPE)) currentScreen = BOARDING_MENU; } break;
            case BOARDING_LIST_VIEW: { if (IsKeyPressed(KEY_ESCAPE)) currentScreen = BOARDING_MENU; } break;
            case INSURANCE_MENU: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), btnInsuranceActivate)) { alunoIndexSeguro = -1; rgParaSeguro[0] = '\0'; rgParaSeguroLetterCount = 0; strcpy(seguroActivateFeedbackMsg, ""); currentScreen = INSURANCE_ACTIVATE; } if (CheckCollisionPointRec(GetMousePosition(), btnInsuranceList)) currentScreen = INSURANCE_LIST_VIEW; if (CheckCollisionPointRec(GetMousePosition(), btnInsuranceBack)) currentScreen = MAIN_MENU; } if (IsKeyPressed(KEY_ESCAPE)) currentScreen = MAIN_MENU; } break;
            case INSURANCE_ACTIVATE: { if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { if (CheckCollisionPointRec(GetMousePosition(), txtRgBuscaSeg)) activeTextBox = 1; else activeTextBox = 0; } if (alunoIndexSeguro == -1) { HANDLE_TEXT_INPUT(1, rgParaSeguro, rgParaSeguroLetterCount, 20); } if (CheckCollisionPointRec(GetMousePosition(), btnBuscarRgSeg) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && alunoIndexSeguro == -1) { alunoIndexSeguro = -1; for (int i = 0; i < total_alunos; i++) { if (strcmp(alunos[i].rg, rgParaSeguro) == 0) { alunoIndexSeguro = i; strcpy(seguroActivateFeedbackMsg, "Aluno encontrado."); break; } } if (alunoIndexSeguro == -1) { strcpy(seguroActivateFeedbackMsg, "Erro: Aluno não encontrado com este RG."); } } if (alunoIndexSeguro != -1) { if (CheckCollisionPointRec(GetMousePosition(), btnSeguroAtivar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { alunos[alunoIndexSeguro].seguro_ativo = 1; strcpy(seguroActivateFeedbackMsg, "Seguro ATIVADO com sucesso!"); alunoIndexSeguro = -1; rgParaSeguro[0] = '\0'; rgParaSeguroLetterCount = 0; } if (CheckCollisionPointRec(GetMousePosition(), btnSeguroDesativar) && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { alunos[alunoIndexSeguro].seguro_ativo = 0; strcpy(seguroActivateFeedbackMsg, "Seguro DESATIVADO com sucesso!"); alunoIndexSeguro = -1; rgParaSeguro[0] = '\0'; rgParaSeguroLetterCount = 0; } } if (IsKeyPressed(KEY_ESCAPE)) currentScreen = INSURANCE_MENU; } break;
            case INSURANCE_LIST_VIEW: { if (IsKeyPressed(KEY_ESCAPE)) currentScreen = INSURANCE_MENU; } break;
            default: break;
        }
        
        BeginDrawing();
            ClearBackground(RAYWHITE);
            
            #define DRAW_TEXT_FIELD_WITH_CURSOR(rect, text, letterCount, isActive, frames, font) \
                DrawRectangleRec(rect, LIGHTGRAY); \
                if (isActive) DrawRectangleLinesEx(rect, 2, RED); \
                else DrawRectangleLinesEx(rect, 1, DARKGRAY); \
                DrawTextEx(font, text, (Vector2){(int)rect.x + 5, (int)rect.y + 8}, 25, 1, MAROON); \
                if (isActive) { \
                    Vector2 textSize = MeasureTextEx(font, text, 25, 1); \
                    if (((frames/30)%2) == 0) DrawTextEx(font, "_", (Vector2){(int)rect.x + 8 + textSize.x, (int)rect.y + 12}, 25, 1, MAROON); \
                }

            switch (currentScreen) {
                case LOGIN: { Vector2 titleSize = MeasureTextEx(font, "SISTEMA LUMASTER TURISMO", 20, 1); DrawTextEx(font, "SISTEMA LUMASTER TURISMO", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 20, 1, DARKGRAY); Vector2 loginSize = MeasureTextEx(font, "Login", 20, 1); DrawTextEx(font, "Login", (Vector2){screenWidth/2.0f - loginSize.x/2, 130}, 20, 1, GRAY); DRAW_TEXT_FIELD_WITH_CURSOR(textBoxUsername, username, usernameLetterCount, activeTextBox == 1, framesCounter, font); char passwordStars[51] = {0}; for(int i=0; i < passwordLetterCount; i++) passwordStars[i] = '*'; DRAW_TEXT_FIELD_WITH_CURSOR(textBoxPassword, passwordStars, passwordLetterCount, activeTextBox == 2, framesCounter, font); DrawRectangleRec(buttonLogin, DARKGRAY); Vector2 btnLoginSize = MeasureTextEx(font, "ENTRAR", 20, 1); DrawTextEx(font, "ENTRAR", (Vector2){buttonLogin.x + buttonLogin.width/2 - btnLoginSize.x/2, buttonLogin.y + 15}, 20, 1, RAYWHITE); Vector2 errSize = MeasureTextEx(font, loginErrorMsg, 20, 1); DrawTextEx(font, loginErrorMsg, (Vector2){screenWidth/2.0f - errSize.x/2, 400}, 20, 1, RED); } break;
                case MAIN_MENU: { 
                    DrawTextEx(font, TextFormat("Bem-vindo(a), %s!", usuario_logado), (Vector2){20, 20}, 20, 1, DARKGRAY); 
                    Vector2 titleSize = MeasureTextEx(font, "MENU PRINCIPAL", 40, 1); DrawTextEx(font, "MENU PRINCIPAL", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY); 
                    DrawRectangleRec(btnManageUsers, DARKGRAY); DrawTextEx(font, "1. Gerenciar Usuários", (Vector2){btnManageUsers.x + 20, btnManageUsers.y + 15}, 20, 1, RAYWHITE); 
                    DrawRectangleRec(btnPaymentControl, DARKGRAY); DrawTextEx(font, "2. Controle de Pagamento", (Vector2){btnPaymentControl.x + 20, btnPaymentControl.y + 15}, 20, 1, RAYWHITE); 
                    DrawRectangleRec(btnBoardingControl, DARKGRAY); DrawTextEx(font, "3. Controle de Embarque", (Vector2){btnBoardingControl.x + 20, btnBoardingControl.y + 15}, 20, 1, RAYWHITE); 
                    DrawRectangleRec(btnTravelInsurance, DARKGRAY); DrawTextEx(font, "4. Seguro Viagem", (Vector2){btnTravelInsurance.x + 20, btnTravelInsurance.y + 15}, 20, 1, RAYWHITE); 
                    DrawRectangleRec(btnGenerateReport, (Color){0, 102, 204, 255}); DrawTextEx(font, "5. Gerar Relatório para Impressão", (Vector2){btnGenerateReport.x + 20, btnGenerateReport.y + 15}, 20, 1, RAYWHITE);
                    DrawRectangleRec(btnExit, MAROON); DrawTextEx(font, "0. Sair", (Vector2){btnExit.x + 20, btnExit.y + 15}, 20, 1, RAYWHITE);
                    if (strlen(reportFeedbackMsg) > 0) {
                        Vector2 reportSize = MeasureTextEx(font, reportFeedbackMsg, 20, 1); DrawTextEx(font, reportFeedbackMsg, (Vector2){screenWidth/2.0f - reportSize.x/2, 520}, 20, 1, BLUE);
                    }
                } break;
                case MANAGE_USERS: { Vector2 titleSize = MeasureTextEx(font, "Gerenciar Usuários", 40, 1); DrawTextEx(font, "Gerenciar Usuários", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY); DrawRectangleRec(btnUserAdd, DARKGRAY); DrawTextEx(font, "Cadastrar Novo Usuário", (Vector2){btnUserAdd.x + 20, btnUserAdd.y + 15}, 20, 1, RAYWHITE); DrawRectangleRec(btnUserList, DARKGRAY); DrawTextEx(font, "Listar Usuários", (Vector2){btnUserList.x + 20, btnUserList.y + 15}, 20, 1, RAYWHITE); DrawRectangleRec(btnUserEdit, DARKGRAY); DrawTextEx(font, "Alterar Usuário", (Vector2){btnUserEdit.x + 20, btnUserEdit.y + 15}, 20, 1, RAYWHITE); DrawRectangleRec(btnUserRemove, DARKGRAY); DrawTextEx(font, "Remover (Desativar) Usuário", (Vector2){btnUserRemove.x + 20, btnUserRemove.y + 15}, 20, 1, RAYWHITE); DrawRectangleRec(btnUserBack, MAROON); DrawTextEx(font, "Voltar ao Menu Principal", (Vector2){btnUserBack.x + 20, btnUserBack.y + 15}, 20, 1, RAYWHITE); } break;
                case USER_ADD: { Vector2 titleSize = MeasureTextEx(font, "Cadastrar Novo Usuário", 40, 1); DrawTextEx(font, "Cadastrar Novo Usuário", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY); DrawTextEx(font, "Nome de Usuário", (Vector2){(int)textBoxNewUsername.x, (int)textBoxNewUsername.y - 25}, 20, 1, GRAY); DRAW_TEXT_FIELD_WITH_CURSOR(textBoxNewUsername, newUsername, newUsernameLetterCount, activeTextBox == 1, framesCounter, font); DrawTextEx(font, "Senha", (Vector2){(int)textBoxNewPassword.x, (int)textBoxNewPassword.y - 25}, 20, 1, GRAY); char newPasswordStars[51] = {0}; for(int i=0; i < newPasswordLetterCount; i++) newPasswordStars[i] = '*'; DRAW_TEXT_FIELD_WITH_CURSOR(textBoxNewPassword, newPasswordStars, newPasswordLetterCount, activeTextBox == 2, framesCounter, font); DrawRectangleRec(buttonConfirmAdd, DARKGREEN); DrawTextEx(font, "Confirmar", (Vector2){buttonConfirmAdd.x + 30, buttonConfirmAdd.y + 15}, 20, 1, WHITE); DrawRectangleRec(buttonCancelAdd, MAROON); DrawTextEx(font, "Cancelar", (Vector2){buttonCancelAdd.x + 35, buttonCancelAdd.y + 15}, 20, 1, WHITE); Vector2 feedbackSize = MeasureTextEx(font, userAddFeedbackMsg, 20, 1); DrawTextEx(font, userAddFeedbackMsg, (Vector2){screenWidth/2.0f - feedbackSize.x/2, 400}, 20, 1, BLUE); DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY); } break;
                case USER_EDIT: { ClearBackground(RAYWHITE); Vector2 titleSize = MeasureTextEx(font, "Alterar Senha de Usuário", 40, 1); DrawTextEx(font, "Alterar Senha de Usuário", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY); DrawTextEx(font, "Nome do Usuário", (Vector2){(int)textBoxUserToEdit.x, (int)textBoxUserToEdit.y - 25}, 20, 1, GRAY); DRAW_TEXT_FIELD_WITH_CURSOR(textBoxUserToEdit, userToEdit, userToEditLetterCount, activeTextBox == 1, framesCounter, font); DrawTextEx(font, "Nova Senha", (Vector2){(int)textBoxNewPasswordForEdit.x, (int)textBoxNewPasswordForEdit.y - 25}, 20, 1, GRAY); char newPasswordForEditStars[51] = {0}; for(int i=0; i < newPasswordForEditLetterCount; i++) newPasswordForEditStars[i] = '*'; DRAW_TEXT_FIELD_WITH_CURSOR(textBoxNewPasswordForEdit, newPasswordForEditStars, newPasswordForEditLetterCount, activeTextBox == 2, framesCounter, font); DrawRectangleRec(buttonConfirmEdit, DARKGREEN); DrawTextEx(font, "Confirmar Alteração", (Vector2){buttonConfirmEdit.x + 40, buttonConfirmEdit.y + 15}, 20, 1, WHITE); Vector2 feedbackSize = MeasureTextEx(font, userEditFeedbackMsg, 20, 1); DrawTextEx(font, userEditFeedbackMsg, (Vector2){screenWidth/2.0f - feedbackSize.x/2, 400}, 20, 1, BLUE); DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY); } break;
                case USER_LIST: { ClearBackground(RAYWHITE); DrawTextEx(font, "Lista de Usuários Cadastrados", (Vector2){20, 20}, 40, 1, DARKGRAY); DrawTextEx(font, "NOME DE USUÁRIO", (Vector2){50, 80}, 20, 1, GRAY); DrawTextEx(font, "STATUS", (Vector2){450, 80}, 20, 1, GRAY); DrawLine(50, 105, screenWidth - 50, 105, GRAY); if (total_usuarios == 0) { DrawTextEx(font, "Nenhum usuário cadastrado.", (Vector2){50, 120}, 20, 1, GRAY); } else { for (int i = 0; i < total_usuarios; i++) { int posY = 120 + (i * 30); DrawTextEx(font, usuarios[i].usuario, (Vector2){50, (float)posY}, 20, 1, BLACK); if (usuarios[i].ativo) { DrawTextEx(font, "Ativo", (Vector2){450, (float)posY}, 20, 1, DARKGREEN); } else { DrawTextEx(font, "Inativo", (Vector2){450, (float)posY}, 20, 1, MAROON); } } } DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY); } break;
                case USER_REMOVE: { Vector2 titleSize = MeasureTextEx(font, "Remover (Desativar) Usuário", 40, 1); DrawTextEx(font, "Remover (Desativar) Usuário", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY); DrawTextEx(font, "Nome do Usuário para Desativar", (Vector2){(int)textBoxUserToRemove.x, (int)textBoxUserToRemove.y - 25}, 20, 1, GRAY); DRAW_TEXT_FIELD_WITH_CURSOR(textBoxUserToRemove, userToRemove, userToRemoveLetterCount, activeTextBox == 1, framesCounter, font); DrawRectangleRec(buttonConfirmRemove, MAROON); DrawTextEx(font, "Confirmar Desativação", (Vector2){buttonConfirmRemove.x + 40, buttonConfirmRemove.y + 15}, 20, 1, WHITE); Vector2 feedbackSize = MeasureTextEx(font, userRemoveFeedbackMsg, 20, 1); DrawTextEx(font, userRemoveFeedbackMsg, (Vector2){screenWidth/2.0f - feedbackSize.x/2, 400}, 20, 1, BLUE); DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY); } break;
                case PAYMENT_MENU: { Vector2 titleSize = MeasureTextEx(font, "Controle de Pagamento", 40, 1); DrawTextEx(font, "Controle de Pagamento", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY); DrawRectangleRec(btnAlunoAdd, DARKGRAY); DrawTextEx(font, "Cadastrar Aluno", (Vector2){btnAlunoAdd.x + 20, btnAlunoAdd.y + 15}, 20, 1, RAYWHITE); DrawRectangleRec(btnAlunoListPayments, DARKGRAY); DrawTextEx(font, "Listar Pagamentos", (Vector2){btnAlunoListPayments.x + 20, btnAlunoListPayments.y + 15}, 20, 1, RAYWHITE); DrawRectangleRec(btnAlunoUpdatePayment, DARKGRAY); DrawTextEx(font, "Atualizar Pagamento", (Vector2){btnAlunoUpdatePayment.x + 20, btnAlunoUpdatePayment.y + 15}, 20, 1, RAYWHITE); DrawRectangleRec(btnPaymentBack, MAROON); DrawTextEx(font, "Voltar ao Menu Principal", (Vector2){btnPaymentBack.x + 20, btnPaymentBack.y + 15}, 20, 1, RAYWHITE); } break;
                case ALUNO_LIST_PAYMENTS: { 
                    ClearBackground(RAYWHITE); 
                    DrawTextEx(font, "Lista de Pagamentos de Alunos", (Vector2){20, 20}, 40, 1, DARKGRAY); 
                    DrawTextEx(font, "NOME", (Vector2){20, 80}, 20, 1, GRAY); 
                    DrawTextEx(font, "VALOR PAGO", (Vector2){380, 80}, 20, 1, GRAY); 
                    DrawTextEx(font, "VALOR TOTAL", (Vector2){550, 80}, 20, 1, GRAY); 
                    DrawTextEx(font, "STATUS", (Vector2){700, 80}, 20, 1, GRAY);
                    DrawLine(20, 105, screenWidth - 20, 105, GRAY); 
                    int currentY = 120;
                    if (total_alunos == 0) { 
                        DrawTextEx(font, "Nenhum aluno cadastrado.", (Vector2){20, (float)currentY}, 20, 1, GRAY); 
                    } else { 
                        for (int i = 0; i < total_alunos; i++) { 
                            DrawTextEx(font, alunos[i].nome, (Vector2){20, (float)currentY}, 20, 1, BLACK); 
                            DrawTextEx(font, TextFormat("R$ %.2f", alunos[i].valor_pago), (Vector2){380, (float)currentY}, 20, 1, alunos[i].valor_pago < alunos[i].valor_total ? MAROON : DARKGREEN); 
                            DrawTextEx(font, TextFormat("R$ %.2f", alunos[i].valor_total), (Vector2){550, (float)currentY}, 20, 1, BLACK);
                            DrawTextEx(font, alunos[i].ativo ? "Ativo" : "Pago", (Vector2){700, (float)currentY}, 20, 1, alunos[i].ativo ? DARKGREEN : BLUE);
                            currentY += 30;
                        } 
                    } 
                    DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY); 
                } break;
                case ALUNO_UPDATE_PAYMENT: {
                    ClearBackground(RAYWHITE);
                    Vector2 titleSize = MeasureTextEx(font, "Atualizar Pagamento de Aluno", 40, 1);
                    DrawTextEx(font, "Atualizar Pagamento de Aluno", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY);
                    DrawTextEx(font, "RG do Aluno:", (Vector2){txtRgBuscaPag.x, txtRgBuscaPag.y - 25}, 20, 1, GRAY);
                    if (alunoIndexPagamento != -1) {
                         DrawRectangleRec(txtRgBuscaPag, GRAY); DrawTextEx(font, rgParaAtualizar, (Vector2){(int)txtRgBuscaPag.x + 5, (int)txtRgBuscaPag.y + 8}, 25, 1, DARKGRAY);
                    } else {
                        DRAW_TEXT_FIELD_WITH_CURSOR(txtRgBuscaPag, rgParaAtualizar, rgParaAtualizarLetterCount, activeTextBox == 1, framesCounter, font);
                        DrawRectangleRec(btnBuscarRgPag, DARKGRAY); Vector2 btnTxtSize = MeasureTextEx(font, "Buscar Aluno", 20, 1); DrawTextEx(font, "Buscar Aluno", (Vector2){btnBuscarRgPag.x + btnBuscarRgPag.width/2 - btnTxtSize.x/2, btnBuscarRgPag.y + 10}, 20, 1, WHITE);
                    }
                    if (alunoIndexPagamento != -1) {
                        DrawTextEx(font, TextFormat("Aluno: %s", alunos[alunoIndexPagamento].nome), (Vector2){txtRgBuscaPag.x, 260}, 20, 1, DARKGRAY);
                        DrawTextEx(font, TextFormat("Pagamento Atual: R$%.2f / R$%.2f", alunos[alunoIndexPagamento].valor_pago, alunos[alunoIndexPagamento].valor_total), (Vector2){txtRgBuscaPag.x, 290}, 20, 1, DARKGRAY);
                        
                        if (alunos[alunoIndexPagamento].ativo) {
                            DrawTextEx(font, "Valor a Adicionar (ex: 150.50):", (Vector2){txtValorAdicionar.x, txtValorAdicionar.y - 25}, 20, 1, GRAY);
                            DRAW_TEXT_FIELD_WITH_CURSOR(txtValorAdicionar, valorParaAdicionarStr, valorParaAdicionarLetterCount, activeTextBox == 2, framesCounter, font);
                            DrawRectangleRec(btnConfirmarPagamento, DARKGREEN); Vector2 btnTxtSize = MeasureTextEx(font, "Confirmar Pagamento", 20, 1); DrawTextEx(font, "Confirmar Pagamento", (Vector2){btnConfirmarPagamento.x + btnConfirmarPagamento.width/2 - btnTxtSize.x/2, btnConfirmarPagamento.y + 15}, 20, 1, WHITE);
                        } else {
                            DrawTextEx(font, "PAGAMENTO CONCLUÍDO. ALUNO INATIVO.", (Vector2){txtRgBuscaPag.x, 350}, 20, 1, BLUE);
                        }
                    }
                    DrawTextEx(font, pagamentoUpdateFeedbackMsg, (Vector2){40, 500}, 20, 1, BLUE);
                    DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY);
                } break;
                case ALUNO_ADD: {
                    ClearBackground(RAYWHITE);
                    Vector2 titleSize = MeasureTextEx(font, "Cadastrar Novo Aluno", 40, 1);
                    DrawTextEx(font, "Cadastrar Novo Aluno", (Vector2){screenWidth/2.0f - titleSize.x/2, 30}, 40, 1, DARKGRAY);

                    #define DRAW_LABEL(text, y) DrawTextEx(font, text, (Vector2){formLabelX, y}, 20, 1, GRAY)

                    DRAW_LABEL("Nome Completo:", txtNome.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtNome, alunoNome, alunoNomeLetterCount, activeTextBox == 1, framesCounter, font);
                    DRAW_LABEL("RG:", txtRg.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtRg, alunoRg, alunoRgLetterCount, activeTextBox == 2, framesCounter, font);
                    DRAW_LABEL("CPF:", txtCpf.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtCpf, alunoCpf, alunoCpfLetterCount, activeTextBox == 3, framesCounter, font);
                    DRAW_LABEL("Data Nascimento:", txtNascimento.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtNascimento, alunoNascimento, alunoNascimentoLetterCount, activeTextBox == 4, framesCounter, font);
                    DRAW_LABEL("Telefone:", txtTelefone.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtTelefone, alunoTelefone, alunoTelefoneLetterCount, activeTextBox == 5, framesCounter, font);
                    DRAW_LABEL("Escola:", txtEscola.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtEscola, alunoEscola, alunoEscolaLetterCount, activeTextBox == 6, framesCounter, font);
                    DRAW_LABEL("Turma:", txtTurma.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtTurma, alunoTurma, alunoTurmaLetterCount, activeTextBox == 7, framesCounter, font);
                    DRAW_LABEL("Nome do Pai:", txtNomePai.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtNomePai, alunoPai, alunoPaiLetterCount, activeTextBox == 8, framesCounter, font);
                    DRAW_LABEL("Nome da Mãe:", txtNomeMae.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtNomeMae, alunoMae, alunoMaeLetterCount, activeTextBox == 9, framesCounter, font);
                    DRAW_LABEL("Endereço:", txtEndereco.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtEndereco, alunoEndereco, alunoEnderecoLetterCount, activeTextBox == 10, framesCounter, font);
                    DRAW_LABEL("Valor Total (R$):", txtValorTotal.y + 5); DRAW_TEXT_FIELD_WITH_CURSOR(txtValorTotal, alunoValorTotalStr, alunoValorTotalLetterCount, activeTextBox == 11, framesCounter, font);
                    
                    DrawRectangleRec(btnConfirmarAluno, DARKGREEN); 
                    Vector2 btnConfirmTxtSize = MeasureTextEx(font, "Confirmar", 20, 1);
                    DrawTextEx(font, "Confirmar", (Vector2){btnConfirmarAluno.x + (btnConfirmarAluno.width/2) - (btnConfirmTxtSize.x/2), btnConfirmarAluno.y + 15}, 20, 1, WHITE);
                    
                    DrawRectangleRec(btnCancelarAluno, MAROON); 
                    Vector2 btnCancelTxtSize = MeasureTextEx(font, "Voltar", 20, 1);
                    DrawTextEx(font, "Voltar", (Vector2){btnCancelarAluno.x + (btnCancelarAluno.width/2) - (btnCancelTxtSize.x/2), btnCancelarAluno.y + 15}, 20, 1, WHITE);
                    
                    Vector2 feedbackSize = MeasureTextEx(font, alunoAddFeedbackMsg, 20, 1);
                    DrawTextEx(font, alunoAddFeedbackMsg, (Vector2){screenWidth/2.0f - feedbackSize.x/2, btnConfirmarAluno.y + 70}, 20, 1, BLUE);
                    DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY);
                } break;

                case BOARDING_MENU: { 
                    Vector2 titleSize = MeasureTextEx(font, "Controle de Embarque", 40, 1); 
                    DrawTextEx(font, "Controle de Embarque", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY); 
                    DrawRectangleRec(btnBoardingConfirm, DARKGRAY); DrawTextEx(font, "Confirmar Embarque de Aluno", (Vector2){btnBoardingConfirm.x + 20, btnBoardingConfirm.y + 15}, 20, 1, RAYWHITE); 
                    DrawRectangleRec(btnBoardingList, DARKGRAY); DrawTextEx(font, "Listar Status de Embarque", (Vector2){btnBoardingList.x + 20, btnBoardingList.y + 15}, 20, 1, RAYWHITE); 
                    DrawRectangleRec(btnBoardingBack, MAROON); DrawTextEx(font, "Voltar ao Menu Principal", (Vector2){btnBoardingBack.x + 20, btnBoardingBack.y + 15}, 20, 1, RAYWHITE); 
                } break;
                case BOARDING_CONFIRM: {
                    ClearBackground(RAYWHITE);
                    Vector2 titleSize = MeasureTextEx(font, "Confirmar Embarque de Aluno", 40, 1);
                    DrawTextEx(font, "Confirmar Embarque de Aluno", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY);
                    DrawTextEx(font, "RG do Aluno:", (Vector2){txtRgBuscaEmb.x, txtRgBuscaEmb.y - 25}, 20, 1, GRAY);
                    if (alunoIndexEmbarque != -1) {
                         DrawRectangleRec(txtRgBuscaEmb, GRAY); DrawTextEx(font, rgParaEmbarque, (Vector2){(int)txtRgBuscaEmb.x + 5, (int)txtRgBuscaEmb.y + 8}, 25, 1, DARKGRAY);
                    } else {
                        DRAW_TEXT_FIELD_WITH_CURSOR(txtRgBuscaEmb, rgParaEmbarque, rgParaEmbarqueLetterCount, activeTextBox == 1, framesCounter, font);
                        DrawRectangleRec(btnBuscarRgEmb, DARKGRAY); Vector2 btnTxtSize = MeasureTextEx(font, "Buscar Aluno", 20, 1); DrawTextEx(font, "Buscar Aluno", (Vector2){btnBuscarRgEmb.x + btnBuscarRgEmb.width/2 - btnTxtSize.x/2, btnBuscarRgEmb.y + 10}, 20, 1, WHITE);
                    }
                     if (alunoIndexEmbarque != -1) {
                        Aluno aluno = alunos[alunoIndexEmbarque];
                        DrawTextEx(font, TextFormat("Aluno: %s", aluno.nome), (Vector2){txtRgBuscaEmb.x, 260}, 20, 1, DARKGRAY);
                        if (aluno.valor_pago < aluno.valor_total) { DrawTextEx(font, "ATENÇÃO: Pagamento não está em dia!", (Vector2){txtRgBuscaEmb.x, 290}, 20, 1, RED); DrawTextEx(font, TextFormat("Valor pago: R$ %.2f / Total: R$ %.2f", aluno.valor_pago, aluno.valor_total), (Vector2){txtRgBuscaEmb.x, 320}, 20, 1, MAROON);
                        } else { DrawTextEx(font, "Pagamento em dia.", (Vector2){txtRgBuscaEmb.x, 290}, 20, 1, DARKGREEN); }
                        const char* statusStr = aluno.embarque_confirmado ? "CONFIRMADO" : "PENDENTE"; Color statusColor = aluno.embarque_confirmado ? DARKGREEN : ORANGE;
                        DrawTextEx(font, TextFormat("Status Atual: %s", statusStr), (Vector2){txtRgBuscaEmb.x, 360}, 20, 1, statusColor);
                        DrawRectangleRec(btnEmbarqueConfirmar, DARKGREEN); DrawTextEx(font, "Confirmar", (Vector2){btnEmbarqueConfirmar.x + 35, btnEmbarqueConfirmar.y + 15}, 20, 1, WHITE);
                        DrawRectangleRec(btnEmbarqueCancelar, MAROON); DrawTextEx(font, "Cancelar", (Vector2){btnEmbarqueCancelar.x + 40, btnEmbarqueCancelar.y + 15}, 20, 1, WHITE);
                    }
                    Vector2 feedbackSize = MeasureTextEx(font, embarqueConfirmFeedbackMsg, 20, 1);
                    DrawTextEx(font, embarqueConfirmFeedbackMsg, (Vector2){screenWidth/2.0f - feedbackSize.x/2, 500}, 20, 1, BLUE);
                    DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY);
                } break;
                case BOARDING_LIST_VIEW: {
                    ClearBackground(RAYWHITE); DrawTextEx(font, "Lista de Embarque", (Vector2){20, 20}, 40, 1, DARKGRAY); 
                    DrawTextEx(font, "NOME", (Vector2){50, 80}, 20, 1, GRAY); DrawTextEx(font, "RG", (Vector2){450, 80}, 20, 1, GRAY); DrawTextEx(font, "EMBARQUE", (Vector2){600, 80}, 20, 1, GRAY); 
                    DrawLine(50, 105, screenWidth - 50, 105, GRAY);
                    if (total_alunos == 0) { DrawTextEx(font, "Nenhum aluno cadastrado.", (Vector2){50, 120}, 20, 1, GRAY);
                    } else { for (int i = 0; i < total_alunos; i++) { int posY = 120 + (i * 30); DrawTextEx(font, alunos[i].nome, (Vector2){50, (float)posY}, 20, 1, BLACK); DrawTextEx(font, alunos[i].rg, (Vector2){450, (float)posY}, 20, 1, BLACK); if (alunos[i].embarque_confirmado) { DrawTextEx(font, "CONFIRMADO", (Vector2){600, (float)posY}, 20, 1, DARKGREEN); } else { DrawTextEx(font, "PENDENTE", (Vector2){600, (float)posY}, 20, 1, MAROON); } } }
                    DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY);
                } break;
                case INSURANCE_MENU: {
                    Vector2 titleSize = MeasureTextEx(font, "Seguro Viagem", 40, 1);
                    DrawTextEx(font, "Seguro Viagem", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY);
                    DrawRectangleRec(btnInsuranceActivate, DARKGRAY); DrawTextEx(font, "Ativar/Desativar Seguro", (Vector2){btnInsuranceActivate.x + 20, btnInsuranceActivate.y + 15}, 20, 1, RAYWHITE);
                    DrawRectangleRec(btnInsuranceList, DARKGRAY); DrawTextEx(font, "Listar Seguros Ativos", (Vector2){btnInsuranceList.x + 20, btnInsuranceList.y + 15}, 20, 1, RAYWHITE);
                    DrawRectangleRec(btnInsuranceBack, MAROON); DrawTextEx(font, "Voltar ao Menu Principal", (Vector2){btnInsuranceBack.x + 20, btnInsuranceBack.y + 15}, 20, 1, RAYWHITE);
                } break;
                case INSURANCE_ACTIVATE: {
                    ClearBackground(RAYWHITE); 
                    Vector2 titleSize = MeasureTextEx(font, "Ativar/Desativar Seguro Viagem", 40, 1);
                    DrawTextEx(font, "Ativar/Desativar Seguro Viagem", (Vector2){screenWidth/2.0f - titleSize.x/2, 80}, 40, 1, DARKGRAY);
                    DrawTextEx(font, "RG do Aluno:", (Vector2){txtRgBuscaSeg.x, txtRgBuscaSeg.y - 25}, 20, 1, GRAY);
                    if (alunoIndexSeguro != -1) {
                         DrawRectangleRec(txtRgBuscaSeg, GRAY); DrawTextEx(font, rgParaSeguro, (Vector2){(int)txtRgBuscaSeg.x + 5, (int)txtRgBuscaSeg.y + 8}, 25, 1, DARKGRAY);
                    } else {
                        DRAW_TEXT_FIELD_WITH_CURSOR(txtRgBuscaSeg, rgParaSeguro, rgParaSeguroLetterCount, activeTextBox == 1, framesCounter, font);
                        DrawRectangleRec(btnBuscarRgSeg, DARKGRAY); Vector2 btnTxtSize = MeasureTextEx(font, "Buscar Aluno", 20, 1); DrawTextEx(font, "Buscar Aluno", (Vector2){btnBuscarRgSeg.x + btnBuscarRgSeg.width/2 - btnTxtSize.x/2, btnBuscarRgSeg.y + 10}, 20, 1, WHITE);
                    }
                     if (alunoIndexSeguro != -1) {
                        Aluno aluno = alunos[alunoIndexSeguro];
                        DrawTextEx(font, TextFormat("Aluno: %s", aluno.nome), (Vector2){txtRgBuscaSeg.x, 260}, 20, 1, DARKGRAY);
                        DrawTextEx(font, TextFormat("CPF: %s, Nasc: %s", aluno.cpf, aluno.data_nascimento), (Vector2){txtRgBuscaSeg.x, 290}, 20, 1, DARKGRAY);
                        const char* statusStr = aluno.seguro_ativo ? "ATIVO" : "INATIVO"; Color statusColor = aluno.seguro_ativo ? DARKGREEN : MAROON;
                        DrawTextEx(font, TextFormat("Status Atual do Seguro: %s", statusStr), (Vector2){txtRgBuscaSeg.x, 340}, 20, 1, statusColor);
                        DrawRectangleRec(btnSeguroAtivar, DARKGREEN); DrawTextEx(font, "Ativar", (Vector2){btnSeguroAtivar.x + 50, btnSeguroAtivar.y + 15}, 20, 1, WHITE);
                        DrawRectangleRec(btnSeguroDesativar, MAROON); DrawTextEx(font, "Desativar", (Vector2){btnSeguroDesativar.x + 35, btnSeguroDesativar.y + 15}, 20, 1, WHITE);
                    }
                    Vector2 feedbackSize = MeasureTextEx(font, seguroActivateFeedbackMsg, 20, 1);
                    DrawTextEx(font, seguroActivateFeedbackMsg, (Vector2){screenWidth/2.0f - feedbackSize.x/2, 500}, 20, 1, BLUE);
                    DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY);
                } break;
                case INSURANCE_LIST_VIEW: {
                    ClearBackground(RAYWHITE); DrawTextEx(font, "Lista de Seguros Viagem", (Vector2){20, 20}, 40, 1, DARKGRAY);
                    DrawTextEx(font, "NOME", (Vector2){50, 80}, 18, 1, GRAY); DrawTextEx(font, "RG", (Vector2){330, 80}, 18, 1, GRAY); DrawTextEx(font, "CPF", (Vector2){460, 80}, 18, 1, GRAY); DrawTextEx(font, "SEGURO", (Vector2){650, 80}, 18, 1, GRAY);
                    DrawLine(50, 105, screenWidth - 50, 105, GRAY);
                    if (total_alunos == 0) { DrawTextEx(font, "Nenhum aluno cadastrado.", (Vector2){50, 120}, 20, 1, GRAY);
                    } else { for (int i = 0; i < total_alunos; i++) { int posY = 120 + (i * 30); DrawTextEx(font, alunos[i].nome, (Vector2){50, (float)posY}, 20, 1, BLACK); DrawTextEx(font, alunos[i].rg, (Vector2){330, (float)posY}, 20, 1, BLACK); DrawTextEx(font, alunos[i].cpf, (Vector2){460, (float)posY}, 20, 1, BLACK); if (alunos[i].seguro_ativo) { DrawTextEx(font, "ATIVO", (Vector2){650, (float)posY}, 20, 1, DARKGREEN); } else { DrawTextEx(font, "INATIVO", (Vector2){650, (float)posY}, 20, 1, MAROON); } } }
                    DrawTextEx(font, "Pressione ESC para voltar", (Vector2){20, screenHeight - 40}, 20, 1, LIGHTGRAY);
                } break;
                default: break;
            }
        EndDrawing();
    }
    
    UnloadFont(font); 
    salvar_dados();
    CloseWindow();
    return 0;
}