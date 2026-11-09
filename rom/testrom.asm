; Programa de Teste do Apple II 
;
;	 0. 16-bit Checksum
;	 1. Strings
;	 2. Fazer Checksum a toda a ROM 
;	 4. Testar LEDs nas linhas ANx (joystick action) 
;    5. Avisar que se vao ouvir uns quantos sons	
;	    para indicar OK, ERRO ou CONTADOR
; ------------------------------------------------------
;	 6. Testar a DRAM
;	 7. RAM esta' OK
;	 8. Localiza o BIT e a COLUNA que falhou
;	 9. TOCAR o SPEAKER pelo primeiro o "NUMERO DO BIT falhado"
;   10. Imprimir o "NUMERO DO BIT falhado" e TOCAR "NUMERO DA COLUNA"
;   11. Imprimir "NUMERO DA COLUNA"
; ------------------------------------------------------
;   12. Se o codigo chegar aqui, alguma coisa de errada
;
;
.INCLUDE "TESTROM.INC"

; DO_INTRODUCTION:				; testa leds ANx, mostra instrucoes com som
 DO_MEMTEST:					; faz memoria
; DO_CHARTEST:					; imprime '!' um-a-um com pausa e som

.ORG $D000						; a ROM começa aos 48KB
JMP Start

	;----------------------------------------------------------
	; 0. 16-bit Checksum
	;----------------------------------------------------------
	
	;checksum:       .byte	 "xor"	
	
	;----------------------------------------------------------
	; 1. Strings
	;----------------------------------------------------------
	str_title:   	.asciiz  "TESTE DE APPLE //"
	str_ledtest:   	.asciiz  "..IO: VERIFICAR LEDS"
	str_memtest:   	.asciiz  "..MEMORIA - VAO SER USADOS 4 SONS:"
	str_memdoing: 	.asciiz  "O ECRA IRA AGORA MUDAR"
	str_1:   		.asciiz  "1"
	str_2:   		.asciiz  "2"
	str_3:   		.asciiz  "3"
	str_SP:   		.asciiz  " "
	str_etc:		.asciiz  "ETC... SE FALHAR,"
	str_etc2:       .asciiz  "CONTA CHIP e APOS,"
	str_etc3:       .asciiz  "CONTA A COLUNA DRAM"
	str_okSound: 	.asciiz  ".OK"
	str_errSound: 	.asciiz  ".ERRO"
	str_cntSound: 	.asciiz  ".CONTADOR DE"
	str_memTitle:   .asciiz  "O ECRA VAI MUDAR TEMPORARIAMENTE"
	str_memOk:     	.asciiz  "MEMORIA OK!"
	str_memErr:    	.asciiz  "MEMORIA ERRO!"
	str_memPatt:  	.asciiz  "  PADRAO:"
	str_memChip:  	.asciiz  "CHIP/BIT:"
	str_memCol:   	.asciiz  "  COLUNA:"
	str_romError:  	.asciiz  "ERRO NA ROM"
	str_xsumError:  .asciiz  "ERRO XSUM"
		
Start:
	
	;----------------------------------------------------------
	; 2. Fazer Checksum a toda a ROM
	;----------------------------------------------------------
	;$3000/$100 = $30
	
 	;LDA #0
 	;LDX #0
	;.REPEAT 15, I_
	;	xsumLoop:
	;	
	;	; 
	;	LDY $D000 + I_* 256, X
	;	TYA 	
	;	LDY $D000 + I_* 256 + 1, X
	;
	;	INX					; 
	;	INX					;
	;	BNE xsumLoop  
	;	
	;.ENDREPEAT
	
	
	LDA str_title	; teste ver descompilaçao se STR_TITLE tem o endereço certo

	;----------------------------------------------------------
	; 3. Colocar em modo de texto e por um titulo
	;----------------------------------------------------------

	STA TEXTMODE_ON
	STA MIXMODE_OFF
	STA PAGE2_OFF
	STA HIRES_OFF

	M_ClearText
	M_WriteText LINE_00,    10,     str_title
	M_ShortPause
	
.IF .DEFINED(DO_CHARTEST)
	M_TestVideo (CHARMAP_SP+1)
.ENDIF
	
.IF .DEFINED(DO_INTRODUCTION)
	
	;----------------------------------------------------------
	; 4. Testar LEDs nas linhas ANx (joystick action) 
	;----------------------------------------------------------
	; 	e' necessario LIGAR o botao que liga a linha
	; 	PISCA-PISCA (timer 555) a CASSETE_DIGITAL_INPUT
	;----------------------------------------------------------
	
	M_WriteText LINE_01,    0,      str_ledtest

	STA AN0_OFF					; desligar LED 0
	STA AN1_OFF					; desligar LED 1
	STA AN2_OFF					; desligar LED 2
	STA AN3_OFF					; desligar LED 3
	
	STA AN0_ON					; ligar LED 0
	M_ShortPause
	STA AN0_OFF					; desligar LED 0
	M_ShortPause
	STA AN1_ON					; ligar LED 1	
	M_ShortPause
	STA AN1_OFF                 ; desligar LED 1
	M_ShortPause
	STA AN2_ON					; ligar LED 2
	M_ShortPause
	STA AN2_OFF                 ; desligar LED 2
	M_ShortPause
	STA AN3_ON					; ligar LED 3
	M_ShortPause
	STA AN3_OFF                 ; desligar LED 3
	M_ShortPause

	M_ShortPause
	M_ShortPause
	M_ShortPause
	M_ShortPause

	;----------------------------------------------------------
	; 5. Avisar que se vao ouvir uns quantos sons
	;	 ara indicar OK, ERRO ou CONTADOR
	;----------------------------------------------------------

			    ; linha     coluna	  texto
	M_WriteText LINE_01,    0,      str_memtest
	M_ShortPause
	M_ShortPause
	M_ShortPause
	
	; Som de OK
	
	M_WriteText LINE_02,    1,      str_1 
	M_WriteText LINE_02,    2,      str_okSound 
	M_PlaySound_OK
	M_ShortPause
	
	; Som de Erro
	
	M_WriteText LINE_03,    1,      str_2
	M_WriteText LINE_03,    2,      str_errSound 
	M_PlaySound_Error
	M_ShortPause
	
	; Som de Contador
	
	M_WriteText LINE_04,    1,      str_3 
	M_WriteText LINE_04,    2,      str_cntSound
	M_ShortPause
	
	; Contador de 1
	
	M_WriteText LINE_04,   15,      str_1 
	M_PlaySound_Counter 1
	M_ShortPause
	M_ShortPause
	M_WriteText LINE_04,   15,      str_SP 
	M_ShortPause
	
	; Contador de 2
		
	M_ShortPause
	M_WriteText LINE_04,   15,      str_2 
	M_PlaySound_Counter 2
	M_ShortPause
	M_ShortPause
	M_WriteText LINE_04,   15,      str_SP 
	M_ShortPause
	
	; Contador de 3
	
	M_WriteText LINE_04,   15,      str_3
	M_PlaySound_Counter 3
	M_ShortPause
	M_ShortPause
	M_WriteText LINE_04,   15,      str_SP 
	M_ShortPause
	
	; Contador de ETC
	M_WriteText LINE_04,   15,      str_etc
	M_WriteText LINE_05,   15,      str_etc2
	M_WriteText LINE_06,   15,      str_etc3 
	M_ShortPause
	M_ShortPause
	M_ShortPause
	
.ENDIF

	;----------------------------------------------------------
	; 6. Testar a DRAM
	;----------------------------------------------------------

	M_ClearText
	M_PlaySound_OK
	M_WriteText LINE_00,   0,      str_memdoing
	
	; <!> nao muda o ecra
	M_MemTest 0, $AA, MemTestError_1
	;M_MemTest 0, $55, MemTestError_1
	;M_MemTest 1, $AA, MemTestError_1
	;M_MemTest 1, $55, MemTestError_1
	;M_MemTest 2, $AA, MemTestError_1
	;M_MemTest 2, $55, MemTestError_1
	
	

	;----------------------------------------------------------
	; 7. DRAM esta' OK
	;----------------------------------------------------------
	MemTestOK:
		; se chegou aqui a memoria esta' OK!
		M_ClearLine LINE_01
		M_WriteText LINE_01,   0,   str_memOk
		M_PlaySound_OK
		
	
EndLoop:
JMP EndLoop	

MemTestError_1:
 
	;----------------------------------------------------------
	; 8. Localiza o "NUMERO do BIT" e a "COLUNA" que falhou
	;    Entra:
	;      A=BIT falhados(a 1, resultado dum XOR com o 
	;        padrao escrito)
	;      X=COLUNA da DRAM onde houve a falha
	;----------------------------------------------------------

	M_WriteBits LINE_01	; bits falhados		 ; destroi Y
	TXA 				; A = COLUNA
	M_WriteBits LINE_02	; coluna			 ; destroi Y	
	
	JMP MemTestError_1
	
;--------------------------------------------------------------------
.IF .DEFINED(DO_MEMTEST)
	M_TestNextBit 0, MemTestError_2
	M_TestNextBit 1, MemTestError_2
	M_TestNextBit 2, MemTestError_2
	M_TestNextBit 3, MemTestError_2
	M_TestNextBit 4, MemTestError_2
	M_TestNextBit 5, MemTestError_2
	M_TestNextBit 6, MemTestError_2
	M_TestNextBit 7, MemTestError_2
	
	; ** nao pode chegar aqui (tem de saltar algures acima)
	JMP ROMError
 
	;----------------------------------------------------------
	; 9. TOCAR o SPEAKER pelo primeiro o "NUMERO DO BIT falhado"
	;    Entra:
	;      A=BIT falhado (nº de 0 a 7)
	;      X=COLUNA da DRAM onde houve a falha
	;----------------------------------------------------------

MemTestError_2:		
		 
	M_BeepCounter 0 			; testa sucessivamente A
	M_BeepCounter 1             ; se A < VALUE_ continua tocando...
	M_BeepCounter 2 			; Y é destruido
	M_BeepCounter 3 
	M_BeepCounter 4 
	M_BeepCounter 5 
	M_BeepCounter 6 
	M_BeepCounter 7 			; A = BIT (igual)
	
	CMP 8
	BMI MemTestError_3			; não pode acontecer!	
	JMP ROMError
	
	;----------------------------------------------------------
	; 10. Imprimir o "NUMERO DO BIT falhado" e TOCAR "NUMERO DA COLUNA"
	;     Entra:
	;      A=BIT falhado (nº de 0 a 7)
	;      X=COLUNA da DRAM onde houve a falha
	;----------------------------------------------------------

MemTestError_3:
	
	; descontar 1 ((BIT+1)-1) e aumentar caractere ascii '0'
	; para impressao do BIT
	CLC					
	ADC CHARMAP_0			; A = '0'+BIT
	STA LINE_01+10 			;
	
	CLC
	SBC CHARMAP_0			; A = BIT
	M_PutLedsAccordingA		; colocar nos leds
	
	M_ShortPause
	M_ShortPause
	
	TXA  ; A = COLUNA
	
	;<!> nao toca
	M_BeepCounter 0			; testa sucessivamente A
	M_BeepCounter 1			; se A < VALUE_ continua tocando...
	M_BeepCounter 2			; Y é destruido
	M_BeepCounter 3 
	
	BMI MemTestError_4
	JMP ROMError			; não pode acontecer!
	
	
	
	;----------------------------------------------------------
	; 11. Imprimir "NUMERO DA COLUNA"
	;     Entra:
	;      A=COLUNA
	;----------------------------------------------------------

MemTestError_4:
	
	CLC
	ADC CHARMAP_0				; A = '0'+COLUNA
	STA LINE_02+10				; 
	
	TXA							; A = COLUNA
	M_PutLedsAccordingA			; colocar nos leds
	
	
; Imprimir RESTANTES COISAS e TOCAR SOM DE ERRO
	M_ClearLine LINE_00
	M_ClearLine LINE_01
	M_ClearLine LINE_02
	
	M_WriteText LINE_00,     0,   str_memErr
	M_WriteText LINE_01,     0,   str_memChip
	M_WriteText LINE_02,     0,   str_memCol
	M_WriteText LINE_03,     0,   str_memPatt
	M_PlaySound_Error
	
JMP EndLoop

	;----------------------------------------------------------
	; 12. Se o codigo chegar aqui, alguma coisa de errada
	;----------------------------------------------------------

ROMError:
	M_PutLedsAccordingA
	M_ClearLine LINE_00
	M_WriteText LINE_00,    00,     str_romError
	
ROMErrorLoop:
	M_PlaySound_Error
	M_ShortPause
JMP ROMErrorLoop


.ENDIF