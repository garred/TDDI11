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

SerialPut:
						mov eax, [LSR_PORT]								; <your code here>	; (1) Wait for THRE = 1
						mov ebx, 5
						bt eax, ebx
						jc .if_true
						jmp SerialPut

						;in ax, LSR_PORT
						;bt ax, 5
						;jc .if_true
						;jmp SerialPut
.if_true:
						mov dx, THR_PORT									; <your code here>	; (2) Output character to UART
						mov ax, [esp+4]
						out dx, ax

						ret																; <your code here>	; (3) Return to caller


; ---------------------------------------------------------------------
; void interrupt SerialISR(void)
; ---------------------------------------------------------------------
; This is an Interrupt Service Routine (ISR) for
; serial receive interrupts.  Characters received
; are placed in a queue by calling Enqueue(char).

		GLOBAL	SerialISR
		EXTERN	QueueInsert	; (provided by LIBPC)

SerialISR:	STI             	; Enable (higher-priority) IRQs

		pushad ; <your code here>	; (1) Preserve all registers

		mov dx, [LSR_PORT]				; Check if bit RBF in the LSR is 1
		bt dx, 0
		jnc _Eoi							; If not, we ignore this interrupt

		mov dx, [RBR_PORT]
		in eax, dx								; <your code here>	; (2) Get character from UART

		; <your code here>	; (3) Put character into queue
		mov [data], eax
		push data
		;push eax 						; <your code here>	; Param #2: address of data
		push QueueInsert 		; <your code here>	; Param #1: address of queue

		CALL	QueueInsert
		ADD	ESP,8

_Eoi:		; <your code here>		; (4) Enable lower priority interrupts
		;mov al, 0x20
		;out 0x20, al	 						; <your code here>	;       (Send Non-Specific EOI to PIC)
		popad ; <your code here>	; (5) Restore all registers
		iret 											; <your code here>	; (6) Return to interrupted code
