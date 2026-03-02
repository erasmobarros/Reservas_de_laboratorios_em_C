O Gemini disse
Aqui está um modelo completo e profissional de README.md para o seu projeto. Ele explica o que o sistema faz, como compilar, as regras de negócio implementadas e a estrutura dos arquivos.

Você pode copiar e colar este conteúdo em um arquivo chamado README.md na pasta do seu projeto.

🧪 Sistema de Gerenciamento de Reservas de Laboratórios
Este é um sistema desenvolvido em Linguagem C para gerenciar o agendamento de horários em laboratórios. O software permite criar, listar, atualizar e remover reservas, garantindo a integridade dos dados através de validações rigorosas e persistência em arquivo.

📋 Funcionalidades
O sistema possui as seguintes funcionalidades principais (CRUD):

Inserir Reserva: Cadastra uma nova reserva validando laboratório, solicitante, data e horário.

Listar Reservas: Exibe todas as reservas cadastradas em formato tabular.

Atualizar Reserva: Permite alterar dados específicos (nome, laboratório, data ou horários) de uma reserva existente.

Remover Reserva: Exclui uma reserva baseada no ID.

Persistência de Dados: Salva e carrega os dados automaticamente em um arquivo de texto (dados_reservas.txt).

Gerenciamento de Memória: Utiliza alocação dinâmica (malloc/realloc) para expandir a capacidade de armazenamento conforme necessário.

🛡️ Regras de Negócio e Validações
O sistema implementa diversas travas de segurança para garantir a consistência dos dados:

Anti-Colisão de Horários: O sistema impede o cadastro de duas reservas no mesmo laboratório, na mesma data e em horários que se sobreponham (interseção de intervalos).

Validação de Data:

Verifica formato DD/MM/AAAA.

Verifica dias (1-31) e meses (1-12).

Restrição de Ano: Aceita apenas agendamentos para o ano de 2026 em diante.

Validação de Horário:

Verifica formato HH:MM.

Garante que o Horário Final seja maior que o Horário Inicial.

Sanitização de Entradas:

Laboratório: Aceita apenas letras e números.

Solicitante: Aceita apenas letras e espaços.

📂 Estrutura de Arquivos
main.c: (Arquivo esperado) Contém o menu principal e o loop do programa.

reservas.c: Implementação das funções de lógica e regras de negócio.

reservas.h: Cabeçalho com as definições das estruturas (struct) e protótipos das funções.

dados_reservas.txt: Arquivo gerado automaticamente para armazenar os dados.

🚀 Como Compilar e Executar
Para rodar este projeto, você precisa de um compilador C (como GCC) instalado.

1. Compilação
Abra o terminal na pasta do projeto e execute:

Bash
gcc main.c reservas.c -o sistema_reservas
Nota: Certifique-se de que você possui um arquivo main.c que chama as funções criadas em reservas.c.

2. Execução
No Windows:

Bash
sistema_reservas.exe
No Linux/Mac:

Bash
./sistema_reservas
💾 Formato do Arquivo de Dados
Os dados são salvos em dados_reservas.txt no seguinte formato (CSV separado por ponto e vírgula):

Plaintext
ID;Laboratorio;Solicitante;Data;HoraInicio;HoraFim
Exemplo:

Plaintext
1;Lab01;Carlos Silva;10/05/2026;08:00;10:00
2;LabQuimica;Ana Souza;12/06/2026;14:00;16:00
🛠️ Tecnologias Utilizadas
Linguagem C (Padrão C99 ou superior).

Bibliotecas Padrão: <stdio.h>, <stdlib.h>, <string.h>, <ctype.h>.

📌 Melhorias Futuras (To-Do)
[ ] Implementar busca de reservas por data ou nome.

[ ] Adicionar sistema de login para administradores.

[ ] Criar relatórios de ocupação por laboratório.

Desenvolvido para fins acadêmicos.