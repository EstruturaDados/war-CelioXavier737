// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO (com MISSÕES)
// ============================================================================
//
// OBJETIVOS:
// - Modularizar completamente o código em funções especializadas.
// - Implementar um sistema de missões para um jogador.
// - Criar uma função para verificar se a missão foi cumprida.
// - Utilizar passagem por referência (ponteiros) para modificar dados e
//   passagem por valor/referência constante (const) para apenas ler.
// - Foco em: Design de software, modularização, const correctness, lógica de jogo.
//
// ============================================================================

// COMENTARIO DO ALUNO: Aqui eu começo meu código
// COMENTARIO DO ALUNO: Primeiro eu import a bibliotecas

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // uso essa para manipular os fgets()
#include <time.h> // incluir time pois será necessário para gerar a semente (seed) da função rand para gerar numeros aleatórios

// COMENTARIO DO ALUNO: crio um conjunto de constantes para facilitar a escalabilidade e manuntenção

#define TAM_NOME 30 // tamanho do campo nome dos territorios
#define TAM_COR 10  // tamanho do campo cor dos territorios

// COMENTARIO DO ALUNO: agora crio a Struct
// COMENTARIO DO ALUNO: no nivel aventureiro a Struct continua igual ao nivel anterior
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// ---------------------------------------------------------------------------
//   Protótipos das funções (organização)
//   -------------------------------------------------------------------------

Territorio* alocarMapa(int quantidade);
void liberarMemoria(Territorio* mapa, char* missao1, char* missao2);

void atacar(Territorio* atacante, Territorio* defensor);

void cadastrarTerritorios(Territorio* mapa, int quantidade);
void exibirMapa(const Territorio* mapa, int quantidade);

int escolheqtd();

// COMENTARIO DO ALUNO: essas funcções fazem parte do desafio do nível MESTRE
void atribuirMissao(char** destino, char* missoes[], int totalMissoes);
void exibirMissao(const char* missao, const char* nomeJogador);
int verificarMissao(const char* missao, Territorio* mapa, int tamanho, const char* corJogador);
void EscolheCorJogador(char* corJogador, int numJogador);
int menuprincipal(void);

// COMENTARIO DO ALUNO: essa função já existiar e serve a todo o sistema
void limparBufferEntrada(void);

// -------------------------------------------------------------------------------------
// COMENTARIO DO ALUNO: Implementação das funções em si, as que foram prototipadas acima
// ------------------------------------------------------------------------------------- 

// Função: alocarMapa
// Aloca dinamicamente um vetor de Territorio com 'quantidade' elementos.
// Retorna ponteiro para o vetor ou NULL em caso de erro.

Territorio* alocarMapa(int quantidade) {
    Territorio* mapa = calloc(quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro: falha ao alocar memoria.\n");
        return NULL;
    }
    return mapa;
}

// Função: liberarMemoria
// Libera o mapa e as missões alocadas dinamicamente.
// sempre confere antes se o parametro passado existe ou é nulo antes de tentar liberar

void liberarMemoria(Territorio* mapa, char* missao1, char* missao2) {
    if (mapa != NULL) {
        free(mapa);
    }
    if (missao1 != NULL) {
        free(missao1);
    }
    if (missao2 != NULL) {
        free(missao2);
    }
}

// Função: atacar
// Simula um ataque entre dois territórios usando rand() (1..6)
// Regras:
// - Se dado_atq > dado_def -> atacante vence:
//      defensor recebe a cor do atacante e metade (inteiro) das tropas do atacante;
//      atacante perde essa metade.
// - Caso contrário -> atacante perde 1 tropa (se tiver).

void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) return; // garante que foram passados dos territorio validos

    printf("\nIniciando ataque: %s (%s, %d tropas) -> %s (%s, %d tropas)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    int dado_atq = rand() % 6 + 1; // usando o rand para gerar os dados de ambos os lados
    int dado_def = rand() % 6 + 1;

    printf("Rolagem: Atacante %d x Defensor %d\n", dado_atq, dado_def);

    if (dado_atq > dado_def) {
        // atacante vence
        strncpy(defensor->cor, atacante->cor, TAM_COR - 1);
        defensor->cor[TAM_COR - 1] = '\0';

        int transferencia = atacante->tropas / 2; // divisão inteira
        if (transferencia > 0) {
            defensor->tropas = transferencia;
            atacante->tropas -= transferencia;
        } // se transferência = 0, nenhuma tropa é movida, apenas a cor é trocada (quando e se for possível fazer)

        printf("Atacante venceu: defensor agora e da cor %s com %d tropas.\n",
               defensor->cor, defensor->tropas);
    } else {
        // atacante perde 1 tropa
        if (atacante->tropas > 0) atacante->tropas -= 1;
        printf("Ataque falhou: atacante perde 1 tropa (restam %d).\n", atacante->tropas);
    }

    // mostra como ficaram as tropas e a condição apos o ataque
    printf("Estado apos ataque:\n");
    printf("  %s (%s) - %d tropas\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("  %s (%s) - %d tropas\n", defensor->nome, defensor->cor, defensor->tropas);
}

// Função: cadastrarTerritorios
// Lê do usuário os dados dos territórios e salva no mapa.
// Usa ponteiro para modificar o vetor passado por referência.

void cadastrarTerritorios(Territorio* mapa, int quantidade) {
    if (mapa == NULL) return;

    int tropas_temp;
    for (int i = 0; i < quantidade; i++) {
        printf("\n--- Cadastro do territorio %d de %d ---\n", i + 1, quantidade);

        printf("Nome do territorio: ");
        if (fgets(mapa[i].nome, TAM_NOME, stdin) == NULL) {
            mapa[i].nome[0] = '\0';
        } else {
            mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';
        }

        printf("Cor do exercito: ");
        if (fgets(mapa[i].cor, TAM_COR, stdin) == NULL) {
            mapa[i].cor[0] = '\0';
        } else {
            mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';
        }

        // valida para garantir que o numero de tropas não seja negativo
        do {
            printf("Quantidade de tropas (>=0): ");
            if (scanf("%d", &tropas_temp) != 1) {
                // limpa o buffer
                limparBufferEntrada();
                tropas_temp = -1;
            } else {
                if (tropas_temp < 0) {
                    printf("Valor invalido. Digite um numero 0 ou maior.\n");
                    limparBufferEntrada();
                } else {
                    limparBufferEntrada();
                }
            }
        } while (tropas_temp < 0);

        mapa[i].tropas = tropas_temp;
    }

    printf("\nCadastro concluido para %d territorios.\n", quantidade);
}

// Função: exibirMapa
// Exibe o estado atual do mapa (territórios).

void exibirMapa(const Territorio* mapa, int quantidade) {
    if (mapa == NULL) return;

    printf("------------------------------------------------\n");
    printf("                  MAPA ATUAL \n");
    printf("------------------------------------------------\n");
    for (int i = 0; i < quantidade; i++) {
        printf("Indice: %d\n", i + 1);
        printf(" Nome : %s\n", mapa[i].nome[0] ? mapa[i].nome : "(sem nome)");
        printf(" Cor  : %s\n", mapa[i].cor[0] ? mapa[i].cor : "(sem cor)");
        printf(" Tropas: %d\n", mapa[i].tropas);
        printf("-------------------------------\n");
    }
}

// Função: atribuirMissao
// Sorteia uma missão do vetor "missoes" e aloca dinamicamente (malloc)
// copiando o texto para *destino.

// Nota: usamos char** destino para que a função possa alterar o ponteiro
// do chamador (alocando memória e atribuindo o endereço).

void atribuirMissao(char** destino, char* missoes[], int totalMissoes) {
    if (destino == NULL || missoes == NULL || totalMissoes <= 0) return;

    int idx = rand() % totalMissoes;
    const char* escolhida = missoes[idx];
    size_t len = strlen(escolhida) + 1;
    *destino = (char*) malloc(len);
    if (*destino == NULL) {
        printf("Erro: nao foi possivel alocar memoria para a missao.\n");
        return;
    }
    strcpy(*destino, escolhida);
}

// Função: exibirMissao
// Mostra a missão do jogador apenas uma vez (na chamada).

void exibirMissao(const char* missao, const char* nomeJogador) {
    if (missao == NULL || nomeJogador == NULL) return;
    printf("\n--- Missao de %s ---\n", nomeJogador);
    printf("%s\n", missao);
    printf("--------------------\n\n");
}

// Função: verificarMissao
// Verifica (verificação simples) se a missão foi cumprida.
// Recebe:
//  - missao: string descrevendo a missão (tipo: "Conquistar 3 territorios seguidos")
//  - mapa: vetor de territorios
//  - tamanho: quantidade de territorios
//  - corJogador: cor que identifica os territorios do jogador
//
// Retorna 1 se cumprida, 0 caso contrário.
//
// Observação: esta implementação faz verificações simples usando 
// buscas por palavras-chave nas strings de missão. Precisa ser melhor elaborada.

int verificarMissao(const char* missao, Territorio* mapa, int tamanho, const char* corJogador) {
    if (missao == NULL || mapa == NULL || tamanho <= 0) return 0;

    // Missão: Conquistar 3 territorios seguidos
    if (strstr(missao, "Conquistar 3 territorios seguidos") != NULL) {
        if (tamanho < 3) return 0;
        for (int i = 0; i <= tamanho - 3; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 &&
                strcmp(mapa[i+1].cor, corJogador) == 0 &&
                strcmp(mapa[i+2].cor, corJogador) == 0) {
                return 1;
            }
        }
        return 0;
    }

    // Missão: Eliminar todas as tropas da cor Vermelha
    if (strstr(missao, "Eliminar todas as tropas da cor Vermelha") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Vermelha") == 0 && mapa[i].tropas > 0) return 0;
        }
        return 1;
    }

    // Missão: Conquistar 4 territorios
    if (strstr(missao, "Conquistar 4 territorios") != NULL) {
        int cont = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) cont++;
        }
        return (cont >= 4) ? 1 : 0;
    }

    // Missão: Ter 10 tropas em um territorio
    if (strstr(missao, "Ter 10 tropas em um territorio") != NULL) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 10) return 1;
        }
        return 0;
    }

    // Missão: Conquistar 2 territorios de cor Azul
    if (strstr(missao, "Conquistar 2 territorios de cor Azul") != NULL) {
        int cont = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "Azul") == 0) cont++;
        }
        return (cont >= 2) ? 1 : 0;
    }

    // Se a missão for desconhecida, por enquanto retorna 0 (não cumprida)
    return 0;
}

int escolheqtd(void) { 
    int quantidade = 0;
    do {
        printf("------------------------------------------------\n");
        printf("   Informe a quantidade de territorios (min 2): \n");
        printf("------------------------------------------------\n");
        printf("Quantidade: ");
        if (scanf("%d", &quantidade) != 1) {
            limparBufferEntrada();
            quantidade = 0;
        } else {
            limparBufferEntrada();
        }

        if (quantidade < 2) {
           printf("Precisa ser ao menos 2 territorios.\n");
        }


    } while (quantidade < 2);
    return quantidade;
}

// Função para escolher a cor de cada jogador
// recebe o ponteiro para a variavel que vai armazenar e o numero do jogador
// o numero é só para gerar a informação na tela

void EscolheCorJogador(char* corJogador, int numJogador) {



    printf("\nInforme a cor do Jogador %d (ex: Vermelha, Azul, Verde): ", numJogador);
    if (fgets(corJogador, TAM_COR, stdin) == NULL) corJogador[0] = '\0';
    corJogador[strcspn(corJogador, "\n")] = '\0';
    limparBufferEntrada(); // impede consumo indevido de \n
}

int menuprincipal(void) {
    
        int opcao = 0;
        
        printf("=============================================\n");
        printf("                      MENU\n");
        printf("=============================================\n\n");
        printf("1) Exibir mapa\n");
        printf("2) Atacar\n");
        printf("3) Verificar missões (silencioso no fluxo normal)\n");
        printf("4) Sair\n");
        printf("=============================================\n\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) {
            limparBufferEntrada();
            opcao = 0;
        } else {
            limparBufferEntrada();
        }
        return opcao;
}

// Função de uso geral do sistema: limpa buffer de entrada
void limparBufferEntrada(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}


// ---------------------------------------------------------------------------
//   Função Principal 
// ---------------------------------------------------------------------------

int main() {
    srand((unsigned int) time(NULL)); // iniciado o gerador de numero aleatórios

    // 1) Criação do vetor de missões 
    // para isso criei uma lista contendo as missões (no caso 5)
    // usei um ponteiro para char (char*) ao inves de uma lista char comum pois é o tema do desafio e eu 
    // não terei que mexer nos valores, só usar.
    char* missoesDisponiveis[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor Vermelha",
        "Conquistar 4 territorios",
        "Ter 10 tropas em um territorio",
        "Conquistar 2 territorios de cor Azul"
    };

    // eu poderia já colocar 5 no numero de missões, mas preferi assim caso eu aumente as missões
    // ele pega o numero total de bytes gastos na lista e divide pelo tamanho de bytes de um item
    // obtendo o numero de itens. Se eu aumentar o nome de itens não terei que mudar essa parte

    int totalMissoes = sizeof(missoesDisponiveis) / sizeof(missoesDisponiveis[0]);

    // 2) Solicita quantidade de territorios e aloca mapa
    // a quantidade é o retorno da função escolheqtd onde o usuário é solicitado a dizer
    // quantos territórios vai querer criar
    
    
    int quantidade = 0;
    quantidade = escolheqtd();

    Territorio* mapa = alocarMapa(quantidade);
    if (mapa == NULL) return 1;

    // 3) Cadastro dos territorios
    cadastrarTerritorios(mapa, quantidade);

    // 4) Definir cores dos dois jogadores (essas cores identificam propriedade dos territorios)
    char corJogador1[TAM_COR];
    char corJogador2[TAM_COR];

    // escolha das Cores de cada Jogador 
    // uso uma função para não repetir a mesmas linhas de codigo, passo a variavel a ser preenchida
    // o numero do jogador, e a constante do TAM_COR;
    EscolheCorJogador(corJogador1, 1);
    EscolheCorJogador(corJogador2, 2);

    // 5) Atribuir missões dinamicamente para cada jogador
    char* missaoJogador1 = NULL;
    char* missaoJogador2 = NULL;
    atribuirMissao(&missaoJogador1, missoesDisponiveis, totalMissoes);
    atribuirMissao(&missaoJogador2, missoesDisponiveis, totalMissoes);

    // Mostrar missão ao jogador apenas uma vez (no início)
    exibirMissao(missaoJogador1, "Jogador 1");
    exibirMissao(missaoJogador2, "Jogador 2");

    // 6) Loop principal do jogo (menu)
    int opcao = 0;
    int jogoEncerrado = 0;
    while (!jogoEncerrado) {


        opcao = menuprincipal();

        if (opcao == 1) {
            exibirMapa(mapa, quantidade);

        } else if (opcao == 2) {
            exibirMapa(mapa, quantidade);
            int a_idx = -1, d_idx = -1;

            // Seleção do atacante
            do {
                printf("Indice atacante (1..%d): ", quantidade);
                if (scanf("%d", &a_idx) != 1) { limparBufferEntrada(); a_idx = -1; }
                else limparBufferEntrada();
            } while (a_idx < 1 || a_idx > quantidade);

            // Seleção do defensor
            do {
                printf("Indice defensor (1..%d, diferente do atacante): ", quantidade);
                if (scanf("%d", &d_idx) != 1) { limparBufferEntrada(); d_idx = -1; }
                else limparBufferEntrada();
            } while (d_idx < 1 || d_idx > quantidade || d_idx == a_idx);

            // validações adicionais (não atacar mesmo dono)
            if (mapa[a_idx-1].tropas <= 0) {
                printf("Atacante nao possui tropas.\n");
            } else if (mapa[a_idx-1].cor[0] != '\0' && strcmp(mapa[a_idx-1].cor, mapa[d_idx-1].cor) == 0) {
                printf("Nao pode atacar mesmo dono.\n");
            } else {
                // realiza ataque
                atacar(&mapa[a_idx-1], &mapa[d_idx-1]);

                // após cada ataque, verificar silenciosamente se alguém cumpriu a missão
                if (verificarMissao(missaoJogador1, mapa, quantidade, corJogador1)) {
                    printf("\n************************************************\n");
                    printf("Jogador 1 CUMPRIU sua missao e GANHOU O JOGO!\n");
                    printf("Missao: %s\n", missaoJogador1);
                    printf("************************************************\n");
                    jogoEncerrado = 1;
                } else if (verificarMissao(missaoJogador2, mapa, quantidade, corJogador2)) {
                    printf("\n************************************************\n");
                    printf("Jogador 2 CUMPRIU sua missao e GANHOU O JOGO!\n");
                    printf("Missao: %s\n", missaoJogador2);
                    printf("************************************************\n");
                    jogoEncerrado = 1;
                }
            }

        } else if (opcao == 3) {
            // opção para o jogador checar missões manualmente (útil para debug)
            int cumpriu1 = verificarMissao(missaoJogador1, mapa, quantidade, corJogador1);
            int cumpriu2 = verificarMissao(missaoJogador2, mapa, quantidade, corJogador2);
            printf("\nStatus das missoes:\n");
            printf("Jogador 1 -> %s : %s\n", missaoJogador1, cumpriu1 ? "CUMPRIDA" : "NAO CUMPRIDA");
            printf("Jogador 2 -> %s : %s\n", missaoJogador2, cumpriu2 ? "CUMPRIDA" : "NAO CUMPRIDA");
            if (cumpriu1 || cumpriu2) {
                jogoEncerrado = 1;
            }

        } else if (opcao == 4) {
            printf("Saindo...\n");
            jogoEncerrado = 1;

        } else {
            printf("Opcao invalida.\n");
        }
    } // fim loop jogo

    // 7) Limpeza: liberar todas as memorias alocadas dinamicamente
    liberarMemoria(mapa, missaoJogador1, missaoJogador2);

    return 0;
}
