#!/bin/bash

# 1º uso
# Abrir um terminal -> 3 tracinhos -> Preferencias -> Adicionar um profile e chamar "RCOM" -> tab "Command" -> When command exits: Hold the terminal open -> Done

# Usos normais
# ./run.bash
# escrever palavra-passe de sudo | Tens 5 segundos para escrever - Ou mudas o valor do sleep de baixo
# deixar andar

gnome-terminal --window-with-profile=RCOM -- ./run_read.bash
gnome-terminal --window-with-profile=RCOM -- ./run_write.bash
