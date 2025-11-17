// ============================================================================
//         PROJETO WAR ESTRUTURADO - DESAFIO DE CÓDIGO
// ============================================================================
//        
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

// Inclusão das bibliotecas padrão necessárias para entrada/saída, alocação de memória, manipulação de strings e tempo.

// --- Constantes Globais ---
// Definem valores fixos para o número de territórios, missões e tamanho máximo de strings, facilitando a manutenção.

// --- Estrutura de Dados ---
// Define a estrutura para um território, contendo seu nome, a cor do exército que o domina e o número de tropas.

// --- Protótipos das Funções ---
// Declarações antecipadas de todas as funções que serão usadas no programa, organizadas por categoria.
// Funções de setup e gerenciamento de memória:
// Funções de interface com o usuário:
// Funções de lógica principal do jogo:
// Função utilitária:

// COMENTARIO DO ALUNO: Aqui eu começo meu código
// COMENTARIO DO ALUNO: Primeiro eu import a bibliotecas

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // uso essa para manipular os fgets()

// COMENTARIO DO ALUNO: Inseri no nivel aventureiro 
#include <time.h> // incluir time pois será necessário para gerar a semente (seed) da função rand para gerar numeros aleatórios


// COMENTARIO DO ALUNO: crio um conjunto de constantes para facilitar a escalabilidade e manuntenção

#define TAM_NOME 30 // tamanho do campo nome dos territorios
#define TAM_COR 10 // tamanho do campo cor dos territorios
// COMENTARIO DO ALUNO: #define QTD_TERRITORIOS 5 // numero de territorios *** removido no nivel aventureiro pois o número de territórios será definido pelo usuário 

// COMENTARIO DO ALUNO: agora crio a Struct
// COMENTARIO DO ALUNO: no nivel aventureiro a Struct continua igual ao nivel anterior
typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

// COMENTARIO DO ALUNO: Nesse ponto eu coloco a função de alocação do mapa de batalha
Territorio* alocarMapa(int quantidade) {
    Territorio* mapa = (Territorio*) calloc(quantidade, sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro: falha ao alocar memoria.\n");
        return NULL;
    }
    /* calloc zera a memoria, mas podemos inicializar campos explicitamente se quiser */
    for (int i = 0; i < quantidade; i++) {
        mapa[i].nome[0] = '\0';
        mapa[i].cor[0]  = '\0';
        mapa[i].tropas  = 0;
    }
    return mapa;
}



// --- Função Principal (main) ---
// Função principal que orquestra o fluxo do jogo, chamando as outras funções em ordem.
int main() {
    // 1. Configuração Inicial (Setup):
    // - Define o locale para português.
    // - Inicializa a semente para geração de números aleatórios com base no tempo atual.
    // - Aloca a memória para o mapa do mundo e verifica se a alocação foi bem-sucedida.
    // - Preenche os territórios com seus dados iniciais (tropas, donos, etc.).
    // - Define a cor do jogador e sorteia sua missão secreta.

    // 2. Laço Principal do Jogo (Game Loop):
    // - Roda em um loop 'do-while' que continua até o jogador sair (opção 0) ou vencer.
    // - A cada iteração, exibe o mapa, a missão e o menu de ações.
    // - Lê a escolha do jogador e usa um 'switch' para chamar a função apropriada:
    //   - Opção 1: Inicia a fase de ataque.
    //   - Opção 2: Verifica se a condição de vitória foi alcançada e informa o jogador.
    //   - Opção 0: Encerra o jogo.
    // - Pausa a execução para que o jogador possa ler os resultados antes da próxima rodada.

    // 3. Limpeza:
    // - Ao final do jogo, libera a memória alocada para o mapa para evitar vazamentos de memória.


// COMENTARIO DO ALUNO: Aqui começo a codificar o main
// COMENTARIO DO ALUNO:criando o vetor dos territorios e declarando as variáveis

// COMENTARIO DO ALUNO: removido no nivel aventureiro ->   Territorio territorios[QTD_TERRITORIOS]; // usei o mesmo nome mas com capitular e no plural para diferenciar e ficar mais identificavel qdo se trata do nome da estrutura e qdo é o vetor
    // COMENTARIO DO ALUNO: inserido no nivel aventureiro
    srand((unsigned int) time(NULL)); // iniciado o gerador de numero aleatórios

    int quantidade = 0; // variavel para armazenar a escolha de numero de tropoas do usuário
    do {
        printf("------------------------------------------------\n");
        printf("   Informe a quantidade de territorios (min 2): \n");
        printf("------------------------------------------------\n");
        printf("Quantidade: ");
        if (scanf("%d", &quantidade) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {} // isso limpa todos os caracteres que sobrarem. Dica que encontrei na internet
            quantidade = 0;
        }
    } while (quantidade < 2); // garanto que não será aceito valores menores que 2;
    getchar(); // tentar limpar o \n (só por garantia e costume)

    // crio a variavel mapa que substitui o antigo "territorios" como vetor contendo as informações dos territorios
    // uso ponteiro para referencia e uma função para alocar e inicializar 
    Territorio* mapa = alocarMapa(quantidade); 
    if (mapa == NULL) return 1; // se falhar na criação e incialização retorna erro

    ***** parei aqui *** continuo depois

    int i; // variavel usada para controle dos loops

// COMENTARIO DO ALUNO: Banner de informação para o usuário

    printf("=============================================\n");
    printf("       SISTEMA DE CADASTRO DE TERRITORIOS\n");
    printf("=============================================\n\n");

//  COMENTARIO DO ALUNO: Entrada de dados: leitura dos 5 territórios

    for (i = 0; i < QTD_TERRITORIOS; i++){
        printf("Dados do Territorio: %d\n",i+1);

        printf("Nome do Territorio:");
        fgets(territorios[i].nome,TAM_NOME,stdin);                    // solitica a informação do nome do territorio    
        territorios[i].nome[strcspn(territorios[i].nome,"\n")] = '\0';  // a diferenca de uso da aspa dupla e simples deve-se ao fato de um lugar se referir a cadeia de strings e outro de apenas um caractere

        printf("Cor do Territorio:");
        fgets(territorios[i].cor,TAM_COR,stdin);                    // solitica a informação da cor do territorio    
        territorios[i].cor[strcspn(territorios[i].cor,"\n")] = '\0';  // a diferenca de uso da aspa dupla e simples deve-se ao fato de um lugar se referir a cadeia de strings e outro de apenas um caractere
        
        printf("Quantidade de Tropas no Territorio:");
        scanf("%d",&territorios[i].tropas);                         // solitica a informação da qtd de tropas e usa scanf para recdeber um dado numerico   
        
        printf("------------------------------\n\n");               // so par efeito estético e separaçãod e blocos de entrada

        getchar(); // para limpar o buffer do teclado
      
    }   // fim do for de entrada de dados

    // COMENTARIO DO ALUNO: aqui eu apresento os dados entrados depois de cadastrados todos os territorios


    printf("=============================================\n");
    printf("           TERRITORIOS CADASTRADOS\n");
    printf("=============================================\n\n");

    for (i = 0; i < QTD_TERRITORIOS; i++){
        printf("Territorio: %d\n",i+1);
        printf("Nome do Territorio: %s\n",territorios[i].nome);                   // imprime a string com o nome do territorio
        printf("Cor do Territorio: %s\n", territorios[i].cor);                    // imprime a string com a cor do territorio    
        printf("Quantidade de Tropas no Territorio: %d\n",territorios[i].tropas); // imprime a informação da qtd de tropas como formato de inteiro   
        printf("------------------------------\n\n");               // so par efeito estético e separaçãod e blocos de entrada
      
    }   // fim do for de apresentação do dados cadastrados

    printf("\n****  Cadastro concluido com sucesso!  ***\n");

    return 0;
}

// --- Implementação das Funções ---

// alocarMapa():
// Aloca dinamicamente a memória para o vetor de territórios usando calloc.
// Retorna um ponteiro para a memória alocada ou NULL em caso de falha.

// inicializarTerritorios():
// Preenche os dados iniciais de cada território no mapa (nome, cor do exército, número de tropas).
// Esta função modifica o mapa passado por referência (ponteiro).

// liberarMemoria():
// Libera a memória previamente alocada para o mapa usando free.

// exibirMenuPrincipal():
// Imprime na tela o menu de ações disponíveis para o jogador.

// exibirMapa():
// Mostra o estado atual de todos os territórios no mapa, formatado como uma tabela.
// Usa 'const' para garantir que a função apenas leia os dados do mapa, sem modificá-los.

// exibirMissao():
// Exibe a descrição da missão atual do jogador com base no ID da missão sorteada.

// faseDeAtaque():
// Gerencia a interface para a ação de ataque, solicitando ao jogador os territórios de origem e destino.
// Chama a função simularAtaque() para executar a lógica da batalha.

// simularAtaque():
// Executa a lógica de uma batalha entre dois territórios.
// Realiza validações, rola os dados, compara os resultados e atualiza o número de tropas.
// Se um território for conquistado, atualiza seu dono e move uma tropa.

// sortearMissao():
// Sorteia e retorna um ID de missão aleatório para o jogador.

// verificarVitoria():
// Verifica se o jogador cumpriu os requisitos de sua missão atual.
// Implementa a lógica para cada tipo de missão (destruir um exército ou conquistar um número de territórios).
// Retorna 1 (verdadeiro) se a missão foi cumprida, e 0 (falso) caso contrário.

// limparBufferEntrada():
// Função utilitária para limpar o buffer de entrada do teclado (stdin), evitando problemas com leituras consecutivas de scanf e getchar.
