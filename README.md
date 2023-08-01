# GPIO-Expander (work in progess)

Neste repositório, você encontrará o código para um microcontrolador STM32G030K6Tx utilizado como expansor de GPIO. O projeto tem como objetivo ser um estudo de usabilidade, explorando o microcontrolador como um dispositivo auxiliar para expandir a capacidade de pinos GPIO de um sistema. Por meio dessa integração, o microcontrolador atua como um módulo dedicado, fornecendo portas GPIO adicionais, o que pode aprimorar o controle e a flexibilidade do sistema principal.

## Arquitetura do projeto
A arquitetura do expansor de GPIO é projetada para expandir a capacidade de pinos GPIO de um sistema, permitindo controlar e monitorar os GPIO's do microcntrolador em questão. O expansor de GPIO atua como um dispositivo intermediário entre o microcontrolador principal e os periféricos conectados a ele. A comunicação com o expansor ocorre através de um barramento I2C.

O expansor de GPIO atua formando uma camada de abstração para os próprios registradores de de controle de pinos. Essa camada de abstração fornece registradores virtuais que são acessados por meio de comandos I2C específicos. Esses registradores virtuais representam os pinos de entrada e saída do expansor, e por sua vez, interagem com os registradores físicos do microcontrolador.

<img src="/stm-gpio-expander/exp-docs/arqt_exp_io.png">

## Directory structure
O projeto é feito utilizando o framework STMCubeIDE fornecido pela STMicroelectronics, portanto boa parte dos arquivos e diretórios do projeto são da estrutura construída do framework.
Portanto, o diretório contendo o código fonte do projeto encontra-se no caminho /stm-gpio-expander/stm-exp, onde se encontra o projeto com a seguinte estrutura.


| Diretório    | Descrição                                                           |
|--------------|---------------------------------------------------------------------|
| src/exp_io.c                  | Funções reservadas para manipulação dos registradores virtuais   |
| src/utils.c                   | Funções de uso geral do projeto                                  |
| src/exp_main.c                | Código que descreve o comportamento principal do projeto         |
| inc/                          | Diretório de arquivos de cabeçalho                               |
| stm-gpio-expander/exp-docs/   | Diretório de arquivos de documentacao                            |
