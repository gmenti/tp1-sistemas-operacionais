# tp1-sistemas-operacionais
---

- Nome: Giuseppe Menti;
- Professor: Fabiano Passuelo Hessel;
- Turma: 4647D-04 - Sistemas Operacionais
- Linguagem: C;
- Proposta:  implementação de primitivas para garantir a exclusão mútua
(mutex) inteiramente por software (ou seja, utilizando espera ocupada).
As primitivas a serem implementadas são enter region() (ou lock()) e
leave region() (ou unlock());

Para alterar a quantidade de Threads basta alterar os macros abaixo:

#define CONSUMER_COUNT 5
#define PRODUCER_COUNT 5

Cada consumidor e produtor sao uma Thread;
