# flapControl
Flap control algorithm in C using a Raspberry Pi, IMU BNO055, Servo driver PCA9685

## Introdução
Este documento destina-se a ser um guia para o uso da IMU BNO055 conectada a uma Raspberry Pi Zero no âmbito do projeto de robô terrestre com extensão aérea de um UAV de asa fixa a ser desenvolvido no INESC TEC Porto.

## Hardware
- Raspberry Pi zero 1.3
- IMU BNO055
- PWM driver PCA9685

# Conexões eléctricas

## BNO055

Raspberry Pi Zero |	BNO055
--- | ---
1 (5.0 VDC) |	Vin
6 (GND) |	GND
3 (SDA1) |	SDA
5 (SCL1) |	SCL

![alt text](https://github.com/oarcanjomiguel/flapControl/blob/main/BNO055-Pinout.jpg "BNO055 board")

## PCA9685

Raspberry Pi Zero | PCA9685
--- | ---
1 (5.0 VDC) | VCC
6 (GND) | GND
3 (SDA1) | SDA
5 (SCL1) | SCL

![alt text](https://github.com/oarcanjomiguel/flapControl/blob/main/pca9685.png "PCA9685 board")

# Software
## IMU BNO055
C: https://github.com/fm4dd/pi-bno055 <br>
Python: sudo pip3 install adafruit-circuitpython-bno055 <br>
## Driver PCA9685
C: https://github.com/Reinbert/pca9685 <br>
## Compilação e dependências:
gcc -Wall example.c -o example -lwiringPi -lwiringPiDev -lwiringPiPca9685 -lm <br>

# State machine


## 2DO
- Translate readme.md to english
- mapear unidade no computador como pasta na Raspberry
- transferir arquivos de trabalho do cartão SD para o diretório no computador
- Guardar uma imagem do cartão SD
- Montar uma política de atualização periódica dessa imagem
- Consultar material da UC de manipuladores para revisar os parâmetros de output da IMU, principalmente os ângulos
- Montar máquina de estados básica (timer, input, output, motor drive etc)

## DONE
- portar para C
- Subir os arquivos de trabalho no Git
- procurar biblioteca para o driver (ou desenvolver)
- refresh rate IMU: de 20Hz a 40Hz 
- 	"dtparam=i2c_arm_baudrate=10000"  > 100000 > 400000
- bias Gyroscopio: 10s => 0,0
- Conectar eletricamente o driver dos atuadores com a raspberry e com leds e/ou com os atuadores reais

# Como montar o sistema do zero em uma Raspberry Pi Zero 1.3

Conectar na raspberry através da USB (sem a necessidade de monitor e teclado)
1) Instalar o software Raspberry Pi Imager em um computador com capacidade de leitura de cartão SD
2) Inserir o cartão SD no leitor (talvez seja necessário utilizar um adaptador)
3) Abrir o software Raspberry Pi Imager e selecionar a distribuição (usar a recomendada), o drive de cartão SD e executar a gravação
4) Depois da gravação terminada, abrir a partição "boot" que aparece no explorador de arquivos
5) Criar um arquivo chamado "ssh" na raiz da partição (o arquivo tem que estar vazio e sem extensão)
6) Editar o arquivo "config.txt" que está na raiz da partição "boot":

- descomentar a linha "dtparam=i2c_arm=on"
- adicionar no final do arquivo a linha "dtoverlay=dwc2"
- Salvar e fechar o arquivo

7) Editar o arquivo cmdline.txt:
		- depois da palavra "rootwait", adicionar "modules-load=dwc2,g_ether" separando dos demais parametros por um espaço
		- Salvar e fechar o arquivo
8) Instalar o Bonjour Print Services da Apple (https://support.apple.com/kb/DL999)
9) Inserir o cartão SD na raspberry
10) conectar a porta micro-USB identificada como "USB" a uma porta USB do computador
10.1) Caso a conexão seja identificada como uma porta "serial COM":

- Descarregar o driver RNDIS em https://www.catalog.update.microsoft.com/Search.aspx?q=USB+RNDIS+Gadget
- Atualizar o driver da "serial COM" para que ele seja identificado como uma conexão Ethetnet RNDIS
Pode ser necessário testar diversos drivers dessa lista até encontrar um que funcione

11) Compartilhar a conexão de internet do computador com a conexão ethernet da raspberry (criada no passo anterior)
12) Conectar via SSH (utilizar o software Putty) ao endereço raspberry.local
12.1) Caso a conexão não funcione, verifique o status da placa e da conexão de ethernet criada nos passos anteriores (não pode ser identificada como uma porta COM)
13) após autenticação, digitar "sudo raspi-config"
14) Na opção "Interfacing options", selecionar "VNC" e habilitar o servidor VNC
15) selecionar "Finish" e sair do raspi-config
16) instalar o VNC Viewer (https://www.realvnc.com/en/connect/download/viewer/)
17) Criar uma nova conexão em "File"->"New Connection"
18) No campo VNC Server preencher "raspberrypi.local"
20) Clicar em OK e abrir a conexão, autenticando com as credenciais (user: pi; password: raspberry)
21) Editar o arquivo "/boot/config.txt" (sudo nano /boot/config.txt ou pela interface gráfica do VNC)
22) vá até a linha "dtparam=i2c_arm=on" e adicione logo após esse bloco de parâmetros, o seguinte parâmetro:

- "dtparam=i2c_arm_baudrate=10000"

23) salve, e reinicie a raspberry
24) Instale o driver python da IMU: "sudo pip3 install adafruit-circuitpython-bno055"
25) Baixe os exemplos do Github:
- sudo apt-get update
-	sudo apt-get install python3-flask
-	cd ~
-	git clone https://github.com/adafruit/Adafruit_CircuitPython_BNO055.git

26) Conecte a IMU bno055 a Raspberry Pi de acordo com a seção Conexões eléctricas deste manual
27) rode o exemplo básico de leitura dos parâmetros:
- cd Adafruit_CircuitPython_BNO055
- cd examples
- python3 bno055_simpletest.py

# Problemas comuns

Problema: Ao tentar abrir a conexão através do Putty, abre uma tela preta e dá erro

Possível solução: Faltou habilitar o SSH através da criação de um arquivo chamado "ssh" sem extensão na raiz da partição boot do cartão SD ou através do raspi-config

Problema: a raspberry perdeu conexão com a internet (não consegue baixar nada)

Possível solução: tentar trocar a conexão wi-fi do computador host; tentar desfazer e refazer o compartilhamento

Problema: Perda de sinal entre a IMU e a Raspberry

Possível solução: Verifique o procedimento de clock stretching nos itens 1.21 e 1.22 da seção “Como montar o sistema do zero em uma Raspberry Pi Zero 1.3” deste documento

Problema: Uma biblioteca que vou utilizar depende da WiringPi (descontinuada)

Possível solução:

- wget https://project-downloads.drogon.net/wiringpi-latest.deb
- sudo dpkg -i wiringpi-latest.deb 

https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet
