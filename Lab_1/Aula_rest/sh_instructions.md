# Intruções para usar scripts
## Intro

Os scipts foram criados para não ter que estar a gastar tempo a compilar e abrir consolas separadas para cada processo

Há 2 sequências de execução, a de **Teste** e a de **Demonstração**. 

A primeira é para usar enquanto se está a desenvolver o programa, tem de ser alterada em muitos sitios caso se adicionem ficheiros para compilação mas é a mais rápida para recompilar e abrir os programas de *read* e *write* para testar, precisando apenas de 2 botões.

A segunda tem o auxilio do makefile, sendo mais fácil gerir quando se adiciona ficheiros mas não permite recompilar tão rápidamente.

## Set-Up

Usando Linux é preciso criar **Profiles** diferentes de consola para cada programa. Apenas 1 é necessário mas com 3 é mais objetivo ver que programa está a correr em cada consola.

- Abrir uma consola (Ctrl + Alt + T)
- 3 tracinhos
- Preferences
- Ao lado de **Profiles** carregar em **+**
- Adicionar nome *ver em baixo*
- Modificar detalhes...

1ª consola será **RCOM_soc** com os seguintes detalhes modificados:
- *Text* 
  - Initial Terminal size: 88 colums 5 rows
- *Colors* 
  - Uncheck Use colors from system theme
  - Built-in schemes -> White on Black
- *Command* 
  - When command exits -> Hold the terminal open
  
2ª consola será **RCOM_write** com os seguintes detalhes modificados:
- *Colors* 
  - Uncheck Use colors from system theme
  - Built-in schemes -> Tango dark
- *Command* 
  - When command exits -> Hold the terminal open

3ª consola será **RCOM_read** com os seguintes detalhes modificados:
- *Colors* 
  - Uncheck Use colors from system theme
  - Built-in schemes -> Solarized dark
- *Command* 
  - When command exits -> Hold the terminal open

## Sequências

### Demonstração

Executar `./run_1time.sh` numa consola, escrever password de administrador para executar `socat` e esperar até abrirem as outras 2 consolas. No final é só fechar todas as janelas.

### Teste

Executar `./run_test.sh`, escrever password e esperar. No final da execução de `writenoncanonical` e `noncanonical` é só premir **Relauch** para recompilar e iniciar o programa na mesma janela


![yes](https://i.imgur.com/OBj6Kvc.png)
