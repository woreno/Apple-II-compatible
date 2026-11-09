;
;
; RECORDA:
;		#a   imediato	
;		#$a  imediato hexadecimal	
;		a    acesso direto
;		$a   acesso direto hexadecimal	
;
; 1. CONSTANTES
;     1.1. Constantes
;     1.2. Constantes de Enderecamento
; 2. MACROS
;     2.1. M_Pause 
;          Pausa esperando por CASSETE_IN ter o valor de VAR_
;		   (nao usa registos)
;
;     2.2. M_ShortPause
;          Espera um pouco por DIGITAL_CASSETE_IN
;		   (nao usa registos)
;
;     2.3. M_ClearLine
;          Apaga uma linha de texto dada por LINE_
;
;     2.4. M_ClearText
;          Apaga toda a area de texto
;
;     2.5. M_WriteText
;          Escreve o texto no endereco TEXT_STRING 
;          no endereco LINE,  na coluna POS   
;		   (usa registos A,Y)
;
;     2.6. M_Write16bit
;          Escreve o NIBBLE_ dum 16-bit usando o
;          registo REGISTER_, passado por VALUE_
;          no endereco de video M_Write16bit
;
;
;     2.7. M_PlayFastSound
;          Emite um som curto (usado no contador)
;		   (usa registo Y)
;
;     2.8. M_PlaySound
;          Emite um som pela duracao de DUR_  
;          com periodo dada por VAR    
;          (usa registos X,Y)
;
;     2.9. M_PlaySound_OK
;          M_PlaySound_Error
;          M_PlaySound_Counter
;          Emite os 3 tipos de sons aqui usados
;          (chama a macro de cima)
;
;    2.10. M_MemTestAddresses
;          Testa endereco DRAM ADDRESS_ contra 
;          PATTERN_ (COL_ e' dado como redudancia
;          em caso de falhar) 
;
;    2.11.1. M_MemTestByte
;            Testa byte em [ADDR_+X]
;         
;    2.11.2. M_MemTest
;            Testa COL_-esima de 16KB na DRAM 
;            contra PATTERN_ 
;
;    2.12. M_TestNextBit
;          Rola para a direita 1 bit mandando o LSB
;          para CARRY; se CARRY salta para JUMP_ADDRESS_
;          guardando A=BIT_+1
;
;    2.13. M_BeepCounter
;          testa se A<VALUE_ e toca se for o caso
;         
;    2.14. M_PutLedsAccordingA
;      	   Ilumina os LEDs ligados a ANx pelo 
;          valor de A=[0..15]


;----------------------------------------------
; 1.1. Constantes varias
;----------------------------------------------
						; usada por algumas macros
	X_REGISTER = 0
	Y_REGISTER = 1
	A_REGISTER = 2
						; charmap A2
	CHARMAP_SP	= $A0	; ' '
	CHARMAP_0	= $B0	; '0'

						; charmap Ascii 
	ASCII_SP = $20		; ' '
	ASCII_0  = $30		; '0'

;----------------------------------------------
; 1.2. Constantes de enderecamento
;----------------------------------------------
	
CASSETE_IN   = $C060
SPEAKER_OUT  = $C030

TEXTMODE_OFF = $C050
TEXTMODE_ON  = $C051
MIXMODE_OFF  = $C052
MIXMODE_ON   = $C053
PAGE2_OFF    = $C054
PAGE2_ON     = $C055
HIRES_OFF    = $C056
HIRES_ON     = $C057
;
AN0_ON       = $C058 ; ON acende a luz, OFF acende a luz
AN0_OFF      = $C059 ; quando a entrada do "chip Open Colector" está 
AN1_ON       = $C05A ; a OFF liga GND aos LEDS, senão deixa-os em ABERTO
AN1_OFF      = $C05B
AN2_ON       = $C05C
AN2_OFF      = $C05D
AN3_ON       = $C05E
AN3_OFF      = $C05F

; ecra modo texto 40-colunas
LINE_00 	 = $400
LINE_01 	 = $480
LINE_02 	 = $500
LINE_03 	 = $580
LINE_04 	 = $600
LINE_05 	 = $680
LINE_06 	 = $700
LINE_07 	 = $780
LINE_08 	 = $428
LINE_09 	 = $4A8
LINE_10 	 = $528
LINE_11 	 = $5A8
LINE_12 	 = $628
LINE_13 	 = $6A8
LINE_14 	 = $728
LINE_15 	 = $7A8
LINE_16 	 = $450
LINE_17 	 = $4D0
LINE_18 	 = $550
LINE_19 	 = $5D0
LINE_20 	 = $650
LINE_21 	 = $6D0
LINE_22 	 = $750
LINE_23 	 = $7D0


;-------------------------------------------------------------
;- 2. MACROS -------------------------------------------------
;-------------------------------------------------------------


;----------------------------------------------
; 2.1. M_Pause
;      Pausa esperando por CASSETE_IN
;      ter o valor de VAR            
;----------------------------------------------

.MACRO M_Pause  VAR_
	.LOCAL @Loop
	@Loop:
		BIT CASSETE_IN
		.IF VAR_
			BMI @Loop
		.ELSE
			BPL @Loop
		.ENDIF
.ENDMACRO

;----------------------------------------------
; 2.2. M_ShortPause
;      Espera um pouco por DIGITAL_CASSETE_IN
;----------------------------------------------
.MACRO M_ShortPause
	M_Pause 0
	M_Pause 1
	M_Pause 0
.ENDMACRO
 
;----------------------------------------------
; 2.3. M_ClearLine 
;      Limpa uma linha
;---------------------------------------------- 
.MACRO M_ClearLine ADDRESS_, CHAR_, USE_SOUND_
.LOCAL @ClearTextLoop
	LDX #0
	.IFBLANK CHAR_ 
		LDA #CHARMAP_SP
	.ELSE
		LDA #CHAR_
	.ENDIF
	@ClearTextLoop:
		.IFNBLANK CHAR_ 
			.IFNBLANK USE_SOUND_  
				M_ShortPause
				M_PlayFastSound
			.ENDIF
		.ENDIF
		STA ADDRESS_, X		; nao muda flags
		INX
		CPX #40
		BNE @ClearTextLoop
.ENDMACRO

;----------------------------------------------
; 2.4. M_ClearText
;      Limpa o ecra
;---------------------------------------------- 
.MACRO M_ClearText CHAR_ 
.LOCAL @ClearTextLoop
	.IFBLANK CHAR_ 
		LDA #CHARMAP_SP
	.ELSE
		LDA CHAR_
	.ENDIF
	LDY #0
	@ClearTextLoop:
		DEY
		STA $400, Y
		STA $500, Y
		STA $600, Y
		STA $700, Y
		BNE @ClearTextLoop
.ENDMACRO

;----------------------------------------------
; 2.5. M_WriteText
;      Escreve o texto no endereco TEXT_STRING
;      no endereco LINE,  na coluna POS   
;---------------------------------------------- 

.MACRO M_WriteText LINE_, POS_, TEXT_STRING_		
.LOCAL @loop, @loopOut 
		LDY #0
	@loop:
		LDA TEXT_STRING_, Y     ; A = [TEXT_STRING_+Y]  -- A = caractere
		ORA #0
		BEQ @loopOut    		; Se for 0, sair
		CLC 
		ADC #(+CHARMAP_SP-ASCII_SP)
		STA LINE_+POS_, Y
		INY             
		BNE @loop   
	@loopOut:		
.ENDMACRO

;----------------------------------------------
; 2.6. M_Write16bitV
;----------------------------------------------
.MACRO M_Write16bitV   REGISTER_, LINEPOS_, VALUE_, NIBBLE_

	.IF ((((VALUE_)>>(NIBBLE_)) & $F)>9)
		LDY #(  CHARMAP_A + (((VALUE_)>>(NIBBLE_)) & $F) )
	.ELSE
		LDY #(  CHARMAP_0 + (((VALUE_)>>(NIBBLE_)) & $F) )
	.ENDIF
	STY LINEPOS_+ (3-NIBBLE_)
	

	;.IF (((VALUE_>>NIBBLE_) & $F)>9)
	;	;.IF (REGISTER_=X_REGISTER)
	;		;LDX #(CHARMAP_A + ((VALUE_>>NIBBLE_) & $F) )
	;	;.ELSEIF (REGISTER_=Y_REGISTER)
	;		;LDY #(CHARMAP_A + ((VALUE_>>NIBBLE_) & $F) )
	;	;.ELSE
	;		;LDA #(CHARMAP_A + ((VALUE_>>NIBBLE_) & $F) )
	;	;.ENDIF
	;.ELSE
	;	;.IF (REGISTER_=X_REGISTER)
	;		;LDX #(CHARMAP_0 + ((VALUE_>>NIBBLE_) & $F) )
	;	;.ELSEIF (REGISTER_=Y_REGISTER)
	;		;LDY #(CHARMAP_0 + ((VALUE_>>NIBBLE_) & $F) )
	;	;.ELSE
	;		;LDA #(CHARMAP_0 + ((VALUE_>>NIBBLE_) & $F) )
	;	;.ENDIF
	;.ENDIF
	;STY LINEPOS_ + (3-NIBBLE_)
.ENDMACRO


;----------------------------------------------
; 2.7. M_PlayFastSound
;      Emite um som curto (usado no contador)
;---------------------------------------------- 

.MACRO M_PlayFastSound
	.LOCAL @PJmp1
		LDY #20
	@PJmp1:
	    STY SPEAKER_OUT
		.REPEAT 80, I
			NOP 
		.ENDREP
		DEY
		BNE @PJmp1
.ENDMACRO

;----------------------------------------------
; 2.8. M_PlaySound
;      Emite um som pela duracao de DUR_  
;      com periodo de VAR    
;----------------------------------------------  

.MACRO M_PlaySound PERIOD_, DUR_ 
	.LOCAL @ToogleSpeaker, @NextToogle, @Wait, @Finish
		LDY DUR_						; Y=DUR_ (duracao)
	@ToogleSpeaker:
	    STA SPEAKER_OUT					; oscilar Speaker
		LDX PERIOD_						; X=PERIOD_ (periodo)
			@Wait: 
				.REPEAT (PERIOD_), I
					NOP 				; repetir NOPs
				.ENDREP
				DEX
				BNE @Wait
			@NextToogle:
		DEY
		BEQ @Finish
			JMP @ToogleSpeaker
		@Finish:
.ENDMACRO

;----------------------------------------------
; 2.9. M_PlaySound_OK
;      M_PlaySound_Error
;      M_PlaySound_Counter
;      Emite os 3 tipos de sons aqui usados
;----------------------------------------------  

.MACRO M_PlaySound_OK
	M_PlaySound 0, $FF
.ENDMACRO

.MACRO M_PlaySound_Error
	M_PlaySound 10, $FF
.ENDMACRO

.MACRO M_PlaySound_Counter N_
	.REPEAT N_, I
		M_ShortPause
		M_PlayFastSound
	.ENDREPEAT
.ENDMACRO

;----------------------------------------------
;----------------------------------------------  

.MACRO M_WriteBit  LINE_POS_, BIT_
.LOCAL @Skip, @Skip2
	ROR A 
	BCC @Skip
		LDY #'1'
		JMP @Skip2
	@Skip:	
		LDY #'0'
	@Skip2:
		STY LINE_POS_+BIT_
.ENDMACRO

.MACRO M_WriteBits LINE_POS_
	M_WriteBit LINE_POS_, 7
	M_WriteBit LINE_POS_, 6
	M_WriteBit LINE_POS_, 5
	M_WriteBit LINE_POS_, 4
	M_WriteBit LINE_POS_, 3
	M_WriteBit LINE_POS_, 2
	M_WriteBit LINE_POS_, 1
	M_WriteBit LINE_POS_, 0
	ROR A 						
		; sao precisos N+1 RORs para 
		; A voltar a ser de entrada
.ENDMACRO


.MACRO M_Write16bitValue SCRATCH_REGISTER_, VALUE_
	M_Write16bitV SCRATCH_REGISTER_, LINE_00, VALUE_, 3
	M_Write16bitV SCRATCH_REGISTER_, LINE_00, VALUE_, 2
	M_Write16bitV SCRATCH_REGISTER_, LINE_00, VALUE_, 1
	M_Write16bitV SCRATCH_REGISTER_, LINE_00, VALUE_, 0
.ENDMACRO
 

;----------------------------------------------
; 2.11.1. M_MemTestByte
;         Testa byte em [ADDR_+X]
;    
;---------------------------------------------- 
.MACRO M_MemTestByte COL_, PATTERN_, ERROR_JMP_ADDRESS_, ADDR_
.LOCAL @Skip	
		STA AN0_OFF
		M_Write16bitValue Y_REGISTER_, ADDR_
		STA AN0_ON
		M_ShortPause
		;LDA #PATTERN_		
		;STA  ADDR_, X			; guardar padrao em A
		;LDA  ADDR_, X			; ler de novo o padrao
		;CMP #PATTERN_			; compara se A=PATTERN?
		;BEQ @Skip				; se padrao igual, passar a frente	 
		;	JMP ERROR_JMP_ADDRESS_
		@Skip:
.ENDMACRO


;----------------------------------------------
; 2.11.2. M_MemTest
;         Testa COL_-esima de 16KB na DRAM 
;         contra PATTERN_ 
;---------------------------------------------- 
.MACRO M_MemTest COL_, PATTERN_, ERROR_JMP_ADDRESS_
.LOCAL @MemLoopX, @MemLoopOut, @JmpError

		; dividir o intervalo $4000 (16K) em 256 para 
		; ser iterado pelo registo X:
		;
		; 16K/256  = 2^14/2^8 = 2^6 = 64 repetições
		; 64*256 = 2^6*2^8= 2^14 = 16K
		
		; ADDRESS = 0x4000*COL_ + 
		;        [0x100, 0x200, 0x300... ate' 0x4000[ + X
		;
		
		LDX #0			
		LDA #PATTERN_		
		@MemLoopX:			; repetir 64 vezes a seguinte macro		
			.REPEAT 64, IT
				M_MemTestByte COL_, PATTERN_, @JmpError, COL_*$4000 + IT * 256
			.ENDREPEAT
			INX
			BEQ @MemLoopOut
			JMP @MemLoopX
			@JmpError:
				;EOR #PATTERN_		; A=BIT falhados (A XOR PATTERN_)
				;LDX #COL_			; X=coluna 
				LDX #PATTERN_
				JMP ERROR_JMP_ADDRESS_
			@MemLoopOut:
.ENDMACRO
	
	
;----------------------------------------------
; 2.12. M_TestNextBit
;       Rola para a direita 1 bit mandando o LSB
;       para CARRY; se CARRY guarda A=BITNUMBER_
;       e salta para JUMP_ADDRESS_
;----------------------------------------------   
.MACRO M_TestNextBit BITNUMBER_, JUMP_ADDRESS_
.LOCAL @Skip
	ROR A					; shiftar currente LSB para Carry
	BCC @Skip				; se carry clear, passar 'a frente
		LDA BITNUMBER_		; se falhar substituir A=BITNUMBER_
		JMP JUMP_ADDRESS_	; e saltar para o endereco
	@Skip:
.ENDMACRO
	
;----------------------------------------------
; 2.13. M_BeepCounter
;       testa se A<VALUE_ e toca se for o caso
;----------------------------------------------  
.MACRO M_BeepCounter VALUE_
.LOCAL @Out
	CMP VALUE_			
	BPL @Out					; A > VALUE_ sair		
		M_PlaySound_Counter 1	; A <= VALUE_ tocar
	@Out:
.ENDMACRO



;----------------------------------------------
; 2.14. M_PutLedsAccordingA
;       Ilumina os LEDs ligados a ANx pelo 
;       valor de A=[0..15]
;----------------------------------------------  
.MACRO M_PutLedsAccordingA
.LOCAL @Jmp1, @Jmp2
	STA AN0_OFF
	STA AN1_OFF
	STA AN2_OFF
	STA AN3_OFF

	ROR A
	BCC @Jmp1
		STA AN0_ON
	@Jmp1:
	ROR A
	BCC @Jmp2
		STA AN1_ON
	@Jmp2:
	ROR A
	BCC @Jmp3
		STA AN2_ON
	@Jmp3:
	ROR A
	BCC @Jmp4
		STA AN3_ON
	@Jmp4:
.ENDMACRO



;----------------------------------------------  
; 2.14. M_TestVideo
;----------------------------------------------  
 .MACRO M_TestVideo CHAR_
	M_ClearLine LINE_00, CHAR_, 1
	M_ClearLine LINE_01, CHAR_, 1
	M_ClearLine LINE_02, CHAR_, 1
	M_ClearLine LINE_03, CHAR_, 1
	M_ClearLine LINE_04, CHAR_, 1
	M_ClearLine LINE_05, CHAR_, 1
	M_ClearLine LINE_06, CHAR_, 1
	M_ClearLine LINE_07, CHAR_, 1
	M_ClearLine LINE_08, CHAR_, 1
	M_ClearLine LINE_09, CHAR_, 1
	M_ClearLine LINE_10, CHAR_, 1
	M_ClearLine LINE_11, CHAR_, 1
	M_ClearLine LINE_12, CHAR_, 1
	M_ClearLine LINE_13, CHAR_, 1
	M_ClearLine LINE_14, CHAR_, 1
	M_ClearLine LINE_15, CHAR_, 1
	M_ClearLine LINE_16, CHAR_, 1
	M_ClearLine LINE_17, CHAR_, 1
	M_ClearLine LINE_18, CHAR_, 1
	M_ClearLine LINE_19, CHAR_, 1
	M_ClearLine LINE_20, CHAR_, 1
	M_ClearLine LINE_21, CHAR_, 1
	M_ClearLine LINE_22, CHAR_, 1
	M_ClearLine LINE_23, CHAR_, 1
	M_ClearLine LINE_23, CHAR_, 1
.ENDMACRO
