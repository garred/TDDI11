		SECTION	.data
		EXTERN	inbound_queue	; (defined in main.C)
data		DB	0		; put rcvd byte here

		SECTION	.text
		ALIGN	16
		BITS	32

BASE_PORT	EQU	3F8h		; we have this in our lab

LSR_PORT	EQU	BASE_PORT+5
RBR_PORT	EQU	BASE_PORT
THR_PORT	EQU	BASE_PORT

; ---------------------------------------------------------------------
; void SerialPut(char ch)
; ---------------------------------------------------------------------
; This function uses programmed waiting loop I/O
; to output the ASCII character 'ch' to the UART.

		GLOBAL	SerialPut

SerialPut:						; <your code here>	; (1) Wait for THRE = 1
	mov dx, LSR_PORT
	in al, dx
	bt ax, 5
	jc if_true
	jmp SerialPut

if_true:							; <your code here>	; (2) Output character to UART
	mov dx, THR_PORT
	mov al, [esp+4]
	out dx, al

	ret									; <your code here>	; (3) Return to caller


; ---------------------------------------------------------------------
; void interrupt SerialISR(void)
; ---------------------------------------------------------------------
; This is an Interrupt Service Routine (ISR) for
; serial receive interrupts.  Characters received
; are placed in a queue by calling Enqueue(char).

		GLOBAL	SerialISR
		EXTERN	QueueInsert	; (provided by LIBPC)

SerialISR:
	STI             		; Enable (higher-priority) IRQs
	pushad 							; <your code here>	; (1) Preserve all registers

	mov dx, LSR_PORT
	in ax, dx						; Check if bit RBF in the LSR is 1
	bt ax, 0
	jnc _Eoi						; If not, we ignore this interrupt


	mov dx, RBR_PORT
	in al, dx						; <your code here>	; (2) Get character from UART
	mov [data], al			; <your code here>	; (3) Put character into queue

	push dword data						; <your code here>	; Param #2: address of data
	push dword [inbound_queue]; <your code here>	; Param #1: address of queue
	CALL QueueInsert
	ADD	ESP,8

_Eoi:		; <your code here>		; (4) Enable lower priority interrupts
	mov ax, 0x20
	mov dx, 0x20
	out dx, ax	 						; <your code here>	;       (Send Non-Specific EOI to PIC
	popad ; <your code here>	; (5) Restore all registers
	ret 											; <your code here>	; (6) Return to interrupted code
