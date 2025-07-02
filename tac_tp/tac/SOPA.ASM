.8086
.model small
.stack 2048h

dseg    segment para public 'data'
		;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		;											FICHEIROS 
		;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		Fich         		db      'DADOS.TXT', 0
		main_menu			db		'MENU.TXT', 0  								
		gameover			db		'GAMEOVER.TXT', 0							
		winner				db		'WINNER.TXT', 0  						
		top10file			db		'TOP10.TXT', 0  							
		wordsfile			db		'PALAVRAS.TXT', 0  	
		Erro_Open      	 	db      'Erro ao tentar abrir o ficheiro$'
        Erro_Ler_Msg    	db      'Erro ao tentar ler do ficheiro$'
        Erro_Close      	db      'Erro ao tentar fechar o ficheiro$'									
		HandleFich      	dw      0
        car_fich        	db      ?

		;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		;											VARS
		;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		palavras 			db		250 dup(?)
		numwords			db 		8
		wordspos			db		'1 01 07|5 00 00|1 03 09|4 11 00|1 00 11|4 01 02|4 06 00|6 10 01|$',0		
		orientation			db 		0
		contador			db 		0
		contador2			db 		-1
		tempo				db		'Tempo: $',0
		;STRS DE 20 DIGITOS
		STR2CMP				db 		20 dup(?),'$'
		STR2BLD				db 		20 dup(?),'$'
		message1			db 		'Match$',0
		message2			db 		'Palavra Atual: $',0
		message3			db 		'Selecionado: $',0
		tabuleiro			db 		'  ###########################', 13, 10
							db		12 dup('                       # ? ? ? ? ? ? ? ? ? ? ? ? #', 13, 10)
							db 		'                       ###########################', 13, 10
		ultimo_num_aleat 	dw 		0
		str_num 			db 		5 dup(?),'$'
		playername			db		'         $', 0
		;NUMERO DE DIGITOS DO NUMERO LIDO
		NUMDIG				db		0
		;NUMERO MAXIMO DE DIGITOS A SER ACEITE
		MAXDIG				db		4
		;GUARDA UM CHAR NO ECRAN
		Car					db		32				
		;ATRIBUTOS DE COR DO CHAR
		Cor					db		7			
		;1 -> 25
		POSy				db		1				
		;1 -> 80
		POSx				db		0			
		posX_ini			db		25
		posY_ini			db		2

		;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		;											HORAS
		;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
		;STR PARA 5 DIGITOS
		STR5	 			db 		"     "	
		;STR PARA GUARDAR O NUMERO LIDO
		NUMERO				db		"                    $",0 
		;MINUTOS ATUAIS
		Minutos				dw		0
		;SEGUNDOS ATUAIS
		Segundos			dw		0
		;MINUTOS INICIAIS
		Minutos_fin			dw		0
		;SEGUNDOS INICIAIS
		Segundos_fin		dw		0
		;ULTIMOS SEGUNDOS LIDOS
		Old_seg				dw		0
dseg    ends


cseg    segment para public 'code'
		assume  cs:cseg, ds:dseg


;$$$$$$$$$$$$$$$$$
goto_xy	macro		POSx,POSy
		mov		ah,02h
		;nmr da pagina escolhida
		mov		bh,0
		mov		dl,POSx
		mov		dh,POSy

		int		10h	
endm

;$$$$$$$$$$$$$$$$$
; macro para fazer o display de uma palavra terminada em $
; usage: mostra STRING
mostra macro str
	mov ah, 09h
	lea dx, str
	int 21h
endm


;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
Main    Proc

        mov     ax,dseg
        mov     ds,ax
		mov		ax,0B800h
		mov		es,ax 

Menu:
		call		apaga_ecran
		goto_xy		0,0

		;load effective address para dx do ficheiro main menu
		lea			dx, main_menu
		;mostrar o menu
		call		imp_Ficheiro

		;esperar que o utilizador introduza um char
		mov  ah, 07h
  		int  21h
		; case 1 vai para o inicio
  		cmp  al, '1'
  		je   inicio
		;case 2 vai para o top 10
		cmp al, '2'
		je top10
		; case 3 sai
		cmp  al, '3'
		je  sair
		jmp Menu 	

inicio:
		; ler as horas
		MOV AH, 2CH
		INT 21H                 

		XOR AX,AX
		; mete os segundos em AL
		MOV AL, DH
		; guarda os segundos na var segundos
		mov Segundos, AX
		XOR AX,AX
		; mete os minutos em AL
		MOV AL, CL
		; guarda os minutos na var segundos
		mov Minutos, AX
		XOR BL, BL
		mov bl, 60
		MUL bl
		add AX, Segundos
		adc al, 0
		mov segundos, ax
		call	apaga_ecran
		; ler as palavras para a matriz
		lea		dx, wordsfile
		call	LER_FICH
		xor		cx, cx
		mov 	cl, numwords
		xor 	bx, bx


mostra_tabuleiro:
		call Preenche_Tabuleiro
		goto_xy	21,1
		lea		dx, tabuleiro
		call	imp_String
		call Imp_Palavras

		
		mov 	posX, 2
		mov 	posY, 2

coluna_dados:
		goto_xy	posX, POSy
		lea     dx, palavras[bx]
		call	imp_String
		add 	bl, 20
		adc		bh, 0
		inc 	POSy
		loop 	coluna_dados
		mov posx, 57
		mov posy, 2
		goto_xy	posX, posY
		lea 	dx, tempo
		call 	imp_String
		goto_xy	32, 16
		MOSTRA 	message2
		goto_xy	32, 17
		MOSTRA 	message3
		
		mov 	al, posX_ini
		mov 	POSx, al
		mov 	al, posY_ini
		mov 	POSy, al


interage_tabuleiro:

		lea 	si, STR2BLD
		call	assinala_P
		
espera_tecla:
		; epera que alguma tecla seja clicada
		mov  	ah, 07h 				
		int  	21h	   
	   	jmp		menu

top10:
		call	apaga_ecran
		goto_xy 0,0
		; carrega o ficheiro do top 10 para o DX
		lea  	dx, top10file
		; mostrar o ficheiro
    	call 	imp_Ficheiro

		; epera que alguma tecla seja clicada
		mov  	ah, 07h
		int  	21h	   
		jmp		menu

sair:
		call		apaga_ecran
        goto_xy 	0,0
		mov			ah,4CH   			;parte do Interrupt para sair
		INT			21H   				;Interrupt para sair
		RET

Main	endp

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;proc para apagar o que está no ecra
apaga_ecran	proc
		xor		bx,bx
		mov		cx,25*80
		
apaga:
		mov		byte ptr es:[bx],' '
		mov		byte ptr es:[bx+1],7
		inc		bx
		inc 	bx
		loop	apaga
		ret

apaga_ecran	endp


;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;proc para ler o tempo
Ler_TEMPO PROC	
		PUSH AX
		PUSH BX
		PUSH CX
		PUSH DX
		PUSHF
		; ler horas
		MOV AH, 2CH
		INT 21H                 
		XOR AX,AX
		;mete os segundos em AL
		MOV AL, DH
		; guarda os segundos na variavel
		mov Segundos_fin, AX
		XOR AX,AX
		; mete os minutos em AL
		MOV AL, CL
		; guarda os minutos na variavel
		mov Minutos_fin, AX
		XOR BX, BX 
		; mete na variavel bl 60
		mov bl, 60
		; multiplica os segundos por 60
		MUL bl
		add AX, Segundos_fin  
		adc ah, 0
		mov segundos_fin, ax
		XOR BX, BX
		mov bx, segundos
		sub segundos_fin, bx
		POPF
		POP DX
		POP CX
		POP BX
		POP AX
 		RET 
Ler_TEMPO   ENDP 

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;proc para atualizar as horas
Trata_Horas PROC
		PUSHF
		PUSH AX
		PUSH BX
		PUSH CX
		PUSH DX		
		; obter as horas do sistema
		CALL 	Ler_TEMPO
		cmp 	segundos, 0
		jne 	continua

continua:
		MOV		AX, Segundos_fin
		; verifica se os segundos mudaram
		cmp		AX, Old_seg
		; senao mudou desde o ultimo read entao sai
		je		fim_horas
		; se mudou atualiza a informacao das horas
		mov		Old_seg, AX
		mov 	ax,Segundos_fin	
		MOV 	bl, 100     
		div 	bl
		add 	al, 30h		
		MOV 	STR5[0],al	
		mov 	AL,AH	
		MOV 	AH, 0
		MOV 	bl, 10     
		div 	bl
		; char para as dezenas
		add 	al, 30h
		; char para as unidades
		add		ah,	30h
		MOV 	STR5[1],al			
		MOV 	STR5[2],ah
		MOV 	STR5[3],'s'		
		MOV 	STR5[4],'$'	
		GOTO_XY	65,2
		MOSTRA	STR5 		
						
fim_horas:		
		; volta a colocar o cursor na posicao antiga apos atualizar as horas
		goto_xy	POSx,POSy
		POPF
		POP DX		
		POP CX
		POP BX
		POP AX
		RET		
			
Trata_Horas ENDP

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; LE UMA TECLA	
LE_TECLA	PROC

sem_tecla:
		call Trata_Horas
		MOV	AH,0BH
		INT 21h
		cmp AL,0
		je	sem_tecla


		mov		ah,08h
		int		21h
		mov		ah,0
		cmp		al,0
		jne		SAI_TECLA
		mov		ah, 08h
		int		21h
		mov		ah,1

SAI_TECLA:	
		RET

LE_TECLA	endp

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; proc para selecionar char no ecra	
assinala_P	PROC

CICLO:			
		; vai para posicao
		goto_xy	POSx,POSy
		mov 	ah, 08h
		; numero da pagina
		mov		bh,0
		int		10h		
		; guarda o char que está na posicao do rato
		mov		Car, al
		; guarda a cor que stá na posicao do rato
		mov		Cor, ah
		
		; mostra o caracter que estava na posicao do avatar
		goto_xy	78,0
		; mostra o caracter atual no canto
		mov		ah, 02h
		mov		dl, Car	
		int		21H			
	
		; goto posicao do rato
		goto_xy	POSx,POSy
		
LER_SETA:	
		call 	LE_TECLA
		cmp		ah, 1
		je		ESTEND
		
		;esc
		CMP 	AL, 27
		JE		fim
		CMP		AL, 13
		je		ASSINALA
		jmp		LER_SETA
		
ESTEND:	cmp 	al,48h
		jne		BAIXO
		
		;cima
		dec		POSy

		goto_xy POSx, POSy
		mov 	ah, 08h
		mov		bh,0
		int		10h	
		cmp     al, 23h		
		je		outofboundcima

		jmp		CICLO

outofboundcima:
	inc 	POSy
	jmp 	ciclo	

BAIXO:	cmp		al,50h
		jne		ESQUERDA
		
		;baixo
		inc 	POSy

		goto_xy POSx, POSy
		mov 	ah, 08h
		mov		bh,0
		int		10h	
		cmp     al, 23h		
		je		outofboundbaixo

		jmp		CICLO

outofboundbaixo:
	dec 	POSy
	jmp 	ciclo	

ESQUERDA:
		cmp		al,4Bh
		jne		DIREITA

		;esquerda
		dec		POSx					
		dec		POSx					

		goto_xy POSx, POSy
		mov 	ah, 08h
		mov		bh,0
		int		10h	
		cmp     al, 23h	
		je		outofboundesquerda
		
		jmp		CICLO

outofboundesquerda:
	inc 	POSx
	inc 	POSx
	jmp 	ciclo	

DIREITA:
		cmp		al,4Dh
		jne		LER_SETA 

		;direita
		inc		POSx
		inc		POSx

		goto_xy POSx, POSy
		mov 	ah, 08h
		mov		bh,0
		int		10h	
		cmp     al, 23h		
		je		outofbounddireita

		jmp		CICLO

outofbounddireita:
		dec 	POSx
		dec 	POSx
		jmp 	ciclo	

				; INT 10,9 - Write Character and Attribute at Cursor Position
				; AH = 09
				; AL = ASCII character to write
				; BH = display page  (or mode 13h, background pixel value)
				; BL = character attribute (text) foreground color (graphics)
				; CX = count of characters to write (CX >= 1)
ASSINALA:

		push ax
		xor di, di
		
		call Copia_Palavra
		
salta:
		goto_xy	46, 16
		MOSTRA 	STR2CMP
		xor 	ax, ax
		mov 	al, car
		mov 	[si], al
		inc 	si
		goto_xy 46, 17			
		MOSTRA STR2BLD
		
		pop 	ax

		call Compara_Palavras

		goto_xy	POSx,POSy

		mov		bl, cor
		not		bl
		mov		cor, bl
		mov 	ah, 09h
		mov		al, car
		mov		bh, 0
		mov		cx, 1
		int		10h
		jmp		CICLO

FIM:	
		ret
assinala_P	ENDP

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; proc para copiar a palavra
Copia_Palavra proc

inicio:
		push ax
		push bx
		push di
		push si

		xor ax, ax
		xor bx, bx
		xor di, di
		xor si, si

		mov al, 20
		mov bl, contador
		mul bl

		mov di, ax

copia:		
		mov al, palavras[di]
		mov STR2CMP[si], al
		inc di
		inc si
		cmp al, ' '
		jne copia

fim:	pop si
		pop di
		pop bx
		pop ax
		ret
Copia_Palavra endp

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; proc para mostrar string
imp_String proc
	;output da string que está loaded em DX
	MOV AH, 09H
	INT 21h

sai:
	ret

imp_String	endp

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
;mostrar ficheiro no ecra
imp_Ficheiro	Proc
		;abre ficheiro
        mov     ah,3dh
        mov     al,0				
		;antes desta funcao ser chamada indica-se o ficheiro a ser chamado	 
        int     21h
        jc      erro_abrir
        mov     HandleFich, ax
        jmp     ler_ciclo

erro_abrir:
        mov     ah,09h
        lea     dx,Erro_Open
        int     21h
        jmp     sai

ler_ciclo:
        mov     ah,3fh
        mov     bx,HandleFich
        mov     cx,1
        lea     dx,car_fich
        int     21h
		jc		erro_ler
		cmp		ax,0		;EOF?
		je		fecha_ficheiro
        mov     ah,02h
		mov		dl,car_fich
		int		21h
		jmp		ler_ciclo

erro_ler:
        mov     ah,09h
        lea     dx,Erro_Ler_Msg
        int     21h

fecha_ficheiro:
        mov     ah,3eh
        mov     bx,HandleFich
        int     21h
        jnc     sai

        mov     ah,09h
        lea     dx,Erro_Close
        Int     21h
sai:
		ret
imp_Ficheiro	endp

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; proc para ler o ficheiro
LER_FICH	PROC
		;abre ficheiro
        mov     ah,3dh
        mov     al,0
		mov		di,0
        int     21h
        jc      erro_abrir
        mov     HandleFich,ax
        jmp     ler_ciclo

erro_abrir:
        mov     ah,09h
        lea     dx,Erro_Open
        int     21h
        jmp     sai

ler_ciclo:
        mov     ah,3fh
        mov     bx,HandleFich
        mov     cx,1
        lea     dx,car_fich
        int     21h
		jc		erro_ler
		cmp		ax,0		
		je		fecha_ficheiro
		mov		dl, car_fich
		mov		palavras[di],dl
		inc		di
		jmp		ler_ciclo

erro_ler:
        mov     ah,09h
        lea     dx,Erro_Ler_Msg
        int     21h

fecha_ficheiro:
        mov     ah,3eh
        mov     bx,HandleFich
        int     21h
        jnc     sai

        mov     ah,09h
        lea     dx,Erro_Close
        Int     21h

sai:	
		RET
		
LER_FICH	endp


;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; proc para preencher o tabuleiro
Preenche_Tabuleiro proc

; primeira posição: x - 4 || y - 2
; Tam X - 
; Tam Y - 

		push si

		xor si, si


ciclo:
		inc si
		; até encontrar o $
		cmp tabuleiro[si], 36
		je 	sai

		; compara com ?
		cmp tabuleiro[si], 63
		jne ciclo

calcula_aleatorio:

		call CalcAleat
		pop ax

		cmp al, 41h
		jb calcula_aleatorio
		cmp al, 5ah
		ja calcula_aleatorio
		
		mov tabuleiro[si], al 

		jmp ciclo	

SAI:
		pop si
		ret

Preenche_Tabuleiro endp 

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; proc para mostrar as palavras	
Imp_Palavras proc
		;meter palavras no tabyuleiro
		push ax
		push bx
		push cx
		push si
		push di
		

		xor ax, ax
		xor bx, bx
		xor cx, cx
		xor si, si
		xor di, di
		
ciclo:
		mov posx, 0
		mov posy, 0
		mov al, wordspos[si]
		mov orientation, al
		inc si
		inc si
		mov al, wordspos[si]
		inc si
		sub al,30h
		mov bl,10
		mul bl
		mov ah, wordspos[si]
		sub ah,30h
		add al,ah
		mov bl, 2
		mul bl
		mov POSx,al
		add POSx, 25
		inc si
		inc si
		mov al, wordspos[si]
		inc si
		sub al,30h
		mov bl,10
		mul bl
		mov ah, wordspos[si]
		sub ah,30h
		add al,ah
		mov POSy,al
		add POSy, 2
		inc si
		inc si
drt:
		cmp orientation, '1'
		jne esq

escreve_drt:
		mov al,palavras[di]
		cmp al,' '
		je fimPal	
		inc di
		goto_xy posX, posY
	
		;atualiza o caracter do canto
		mov		ah, 02h
		mov		dl, al	
		int		21H	

		inc POSx
		inc posX
		
		jmp escreve_drt

esq:
		cmp orientation, '2'
		jne cima

escreve_esq:

		mov al,palavras[di]
		cmp al,' '
		je fimPal	
		inc di
		goto_xy posX, posY
	
		;atualiza o caracter do canto
		mov		ah, 02h
		mov		dl, al	
		int		21H	

		dec posX
		dec posX
		
		jmp escreve_esq


cima:
		cmp orientation, '3'
		jne baixo

escreve_cima:

		mov al,palavras[di]
		cmp al,' '
		je fimPal	
		inc di
		goto_xy posX, posY
	
		;atualiza o caracter do canto
		mov		ah, 02h
		mov		dl, al	
		int		21H	

		dec POSy
		
		jmp escreve_cima

baixo:
		cmp orientation, '4'
		jne baixo_drt

escreve_baixo:

		mov al,palavras[di]
		cmp al, ' '
		je fimPal	
		inc di
		goto_xy posX, posY
	
		;atualiza o caracter do canto
		mov		ah, 02h
		mov		dl, al	
		int		21H	

		inc POSy
		
		jmp escreve_baixo

baixo_drt:
		cmp orientation, '5'
		jne baixo_esq

escreve_baixo_drt:

		mov al,palavras[di]
		cmp al,' '
		je fimPal	
		inc di

		goto_xy posX, posY
	
		;atualiza o caracter do canto
		mov		ah, 02h
		mov		dl, al	
		int		21H	

		inc POSy
		inc posX
		inc posX
		
		jmp escreve_baixo_drt	

baixo_esq:
		cmp orientation, '6'
		jne cima_esq

escreve_baixo_esq:

		mov al,palavras[di]
		cmp al,' '
		je fimPal	
		inc di
		goto_xy posX, posY
	
		;atualiza o caracter do canto
		mov		ah, 02h
		mov		dl, al	
		int		21H	

		inc POSy
		dec posX
		dec posX
		
		jmp escreve_baixo_esq		

cima_esq:
		cmp orientation, '7'
		jne cima_drt

escreve_cima_esq:

		mov al,palavras[di]
		cmp al,' '
		je fimPal	
		inc di
		goto_xy posX, posY
	
		;atualiza o caracter do canto
		mov		ah, 02h
		mov		dl, al	
		int		21H	

		dec POSy
		dec posX
		dec posX
		
		jmp escreve_cima_esq	

cima_drt:
		cmp orientation, '8'
		jne sai

escreve_cima_drt:

		mov al,palavras[di]
		cmp al,' '
		je fimPal	
		inc di
		goto_xy posX, posY
	
		;atualiza o caracter do canto
		mov	ah, 02h
		mov	dl, al	
		int	21H	

		dec POSy
		inc posX
		inc posX
		
		jmp escreve_cima_drt	

fimPal:
		inc di
		cmp palavras[di],'$'
		jne fimPal
		
		inc di
		inc di
		inc di

		inc cl
		cmp cl, 8
		jne ciclo

SAI:	
		
		pop di
		pop si
		pop cx
		pop bx
		pop ax

		ret

Imp_Palavras endp

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; proc para calculo aleatorio
CalcAleat proc near

		sub		sp,2
		push	bp
		mov		bp,sp
		push	ax
		push	cx
		push	dx	
		mov		ax,[bp+4]
		mov		[bp+2],ax

		mov		ah,00h
		int		1ah

		add		dx,ultimo_num_aleat
		add		cx,dx	
		mov		ax,65521
		push	dx
		mul		cx
		pop		dx
		xchg	dl,dh
		add		dx,32749
		add		dx,ax

		mov		ultimo_num_aleat,dx

		mov		[BP+4],dx

		pop		dx
		pop		cx
		pop		ax
		pop		bp
		ret
CalcAleat endp

;%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
; proc para comparar palavras
Compara_Palavras proc
inicio:
		push di
		push bx
		push cx

		xor di, di
		xor bx, bx
		
		xor cx, cx
		mov cl, 15

compara:
		mov bl, STR2BLD[di]
		mov bh,	STR2CMP[di]
		
		cmp bh, ' '
		je msg

		inc di
		cmp bl, bh
		je compara
		
		jmp fim

msg: 
		goto_xy 40,19
		MOSTRA message1
		xor di, di
reset:
		mov STR2BLD[di],  0
		inc di
		loop reset
		inc contador

fim:	
		pop cx
		pop bx
		pop di

		ret
Compara_Palavras endp

cseg	ends
end     Main           

