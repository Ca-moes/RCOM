![Exp6](./imgs/exp6.png)

# Steps

1. Compile your download application in tuxy3
2. In tuxy3, restart capturing with Wireshark and run your  application  download ftp://ftp.up.pt/...
3. Verify if file has arrived correctly, stop capturing and save the log
4. Using Wireshark observe packets exchanged including:
    - TCP  control and data connections, and its phases (establishment, data, termination)
    - Data transferred through the FTP control connection
    - TCP ARQ mechanism
    - TCP congestion control mechanism in  action
    - Note: use also Wireshark  Statistics tools (menu) to study TCP phases, ARQ and congestion control mechanism
5. Repeat download in tuxy3 but now, in the middle of the transference, start a new download in tuxy2
    - Use the Wireshark statistics tools to understand how the throughput of  a TCP connection varies along the time

# Step 1

Ir buscar a nossa aplicação `download` e compilar no tux23.

Não esquecer de configurar a rede de acordo com a imagem

# Step 2

Começar a capturar pacotes no tux23 e fazer 
```powershell
download ftp://ftp.up.pt/pub/CPAN/robots.txt
```
```txt
Conteudo do ficheiro robots.txt:


# Hello Robots!
# Welcome to CPAN.

User-agent: *
Disallow: /badrobot/

User-Agent: The Knowledge AI
Disallow: /


```

# Step 3

Parar a captura e guarda os logs como `exp6_step3.pcapng`

# Step 4

Analisar os Logs

# Step 5

No tux23 o download terá de descarregar um ficheiro maior, podemos fazer:
```powershell
download ftp://ftp.up.pt/pub/CPAN/RECENT-J.json
```
este ficheiro tem 78.5MB, se for rápido a fazer download é preciso arranjar um ficheiro maior. Se ainda demorar, então inicia-se a captura no WireShark, inicia-se o download no tux23, vai-se para o tux22 e faz-se
```powershell
download ftp://ftp.up.pt/pub/CPAN/robots.txt
```
Não esquecer de compilar o download no tux22 antes de fazer isto.

No final guardar os logs to tux23 como `exp6_step5.pcapng`