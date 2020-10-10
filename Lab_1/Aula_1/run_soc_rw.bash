#!/bin/bash

# 1º uso
# Abrir um terminal -> 3 tracinhos -> Preferencias -> Adicionar um profile e chamar "RCOM" -> tab "Command" -> When command exits: Hold the terminal open -> Done

# Usos normais
# ./run.bash
# escrever palavra-passe de sudo | Tens 5 segundos para escrever - Ou mudas o valor do sleep de baixo
# deixar andar

gnome-terminal --window-with-profile=RCOM -- sudo socat -d  -d  PTY,link=/dev/ttyS10,mode=777   PTY,link=/dev/ttyS11,mode=777
sleep 5
./run_rw.bash
