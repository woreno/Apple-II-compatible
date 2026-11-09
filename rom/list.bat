@set TEXT_EDITOR=start "C:\ProgramData\Microsoft\Windows\Start Menu\Programs\notepad++"
@echo off 
@cls
echo  ...........................................
echo   Este diretorio contem os ficheiros batchs 
echo   necessarios para gerar e gravar as 2 ROMs 
echo   para o Apple 2. LEMBRAR que o layout dos
echo   pinos e enderecamento local nao e' igual 
echo   ao original "Apple 2".
echo  ...........................................
echo 1. make-charrom:
echo   a ROM original de caracteres pode ser alterada 
echo   com o programa charrom_explorer, e vai ser
echo   alterada para mudar as linhas de endereco
echo   e de dados para o layout de pinos decidido
echo  ...........................................
echo 2. 
echo  2.1. make-rom:
echo   a ROM original de sistema situada a 0x5000 
echo  2.2. make-testrom:
echo   o programa de teste situado a 0x1000
echo   este programa de teste pode ser editado 
echo   (ficheiros .asm e .inc).
echo   - o SWITCH ao pe' da ROM de sistema deve-se
echo     comutar para iniciar o programa de teste
echo   - o SWITCH ao pe' do circuito de video
echo     deve-se comutar para que o sinal de "pisca-pisca"
echo     seja redirecionado para a porta DIGITAL_CASSETE_IN
echo   - a miniboard com os LEDs devem ser ligados
echo     'as linhas ANx (originalmente para joystick) e 'a 
echo     alimentacao em 2 conetores que estao perto do CPU
echo   - este programa, pisca os LEDs ao sinal do "pisca-pisca"
echo     para entender se o CPU esta' executando codigo
echo     (sem usar RAM)
echo   - este programa escreve padroes na memoria e verifica
echo     para entender se a memoria esta' utilizavel.
echo     (primeiro testa o 1. byte da RAM necessario a proceder)
echo   o programa vai ser compilado com ca65 e ligado la65, 
echo   e como apenas parte do ficheiro e' gravado na base, 
echo   VAI SER GRAVADO 'A PARTE o ponteiro de RESET 
echo   em 0x3FFC, o equivalente a 0xFFFC visto pelo CPU
echo   quando arrancar com o SWITCH na posicao de teste.
echo  ...........................................


dir *.bat

%TEXT_EDITOR% testrom.asm
%TEXT_EDITOR% testrom.inc
%TEXT_EDITOR% make-testrom.bat
rem %TEXT_EDITOR% testrom.cfg
rem %TEXT_EDITOR% make-charrom.bat
rem %TEXT_EDITOR% charrom-remap.txt
rem %TEXT_EDITOR% make-rom.bat