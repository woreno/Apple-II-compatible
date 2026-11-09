@cls
@set COM_PORT=6
@set XSUM_ADDR=3
@set CC65_PATH=C:\Development\docs\cpu\6502\cc65\bin
rem notepad++ testrom.asm testrom.inc

@echo ******************************************
@echo a apagar temporarios...
del testrom.bin
del prom_r.temp


@echo ******************************************
@echo a compilar e ligar programa de teste...

%CC65_PATH%\ca65    testrom.asm              -o testrom.out -v  -t none --cpu 6502 
@IF %ERRORLEVEL% NEQ 0 GOTO process_error1
%CC65_PATH%\ld65 -C testrom.cfg  testrom.out -o testrom.bin
@IF %ERRORLEVEL% NEQ 0 GOTO process_error1

@rem echo ******************************************
@rem echo a estampar XOR sum...
@rem stamp xor testrom.bin -tagsz=2 -d=%XSUM_ADDR%


dir testrom.bin

@echo ******************************************
@echo a escrever na PROM...
prom s=0x00,0xD0      -a=0x3FFC -com=%COM_PORT%
@IF %ERRORLEVEL% NEQ 0 GOTO process_error2

prom w -f=testrom.bin -a=0x1000 -com=%COM_PORT%	
@IF %ERRORLEVEL% NEQ 0 GOTO process_error2

GOTO process_end


:process_error1
@echo ******************************************
@echo ******* ERRO COMPILACAO ******************
@echo ******************************************
GOTO process_end

:process_error2
@echo ******************************************
@echo ******* ERRO ESCRITA PROM ****************
@echo ******************************************
GOTO process_end

:process_end