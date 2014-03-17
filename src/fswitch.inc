; ====================================== ;
; ▶ Entry code segment                   ;
; ====================================== ;

_switch:
	call _screen_clean      ; clean the screen
	call _cursor_reset      ; reset cursor position

	xor ax, ax
 	mov ds, ax              ; update data segment

	cli                     ; disable interrupts
	lgdt [gdtr]             ; load GDT register with start address of GDT

	or   al,2               ; enable A20
	out  0x60, al

	mov  al,0xAE            ; enable keyboard
	out  0x64,al

	mov  eax,cr0 
	or   al,1               ; set PE (Protection Enable) bit in CR0
	mov  cr0,eax
 
	jmp (CODE_DESC - NULL_DESC) : _protected_mode



; ====================================== ;
; ▶ Global Descriptor Table (GDT)        ;
; ====================================== ;

NULL_DESC:
    dd 0                        ; null descriptor
    dd 0

CODE_DESC:
    dw 0xFFFF                   ; limit low
    dw 0                        ; base low
    db 0                        ; base middle
    db 10011010b                ; access
    db 11001111b                ; granularity
    db 0                        ; base high

DATA_DESC:
    dw 0xFFFF                   ; data descriptor
    dw 0                        ; limit low
    db 0                        ; base low
    db 10010010b                ; access
    db 11001111b                ; granularity
    db 0                        ; base high

gdtr:
    limit dw 24                 ; length of GDT
    base  dd NULL_DESC          ; base of GDT



; ====================================== ;
; ▶ Protected mode segment               ;
; ====================================== ;

use32

_protected_mode:
	mov  edi,0xb8000        ; direct VGA address (text-mode)

	mov  eax,msg_prot       ; save message adress to `eax`
	call __print_message    ; print a message

	call __keyboard_loop    ; wait for a `Enter` key be pressed


; ====================================== ;
; ▶ Real fallback mode segment           ;
; ====================================== ; 

__real_mode:
	cli			; Disable interrupts.
 
	; need 16-bit Protected Mode GDT entries
	mov eax,gdtr            ; 16-bit Protected Mode data selector.
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	; disable paging (we need everything to be 1:1 mapped).
	mov eax, cr0            ; copy `cr0` in `eax`
	mov [savcr0], eax	; save pmode CR0
	and eax, 0x7FFFFFFe	; disable paging bit & enable 16-bit pmode
	mov cr0, eax
 
	jmp 0:__real_mode_jump  ; perform Far jump to set CS
 
__real_mode_jump:
	mov sp, 0x8000		; pick a stack pointer
	mov ax, 0		; reset segment registers to 0
	mov ds, ax              ;
	mov es, ax              ;
	mov fs, ax              ;
	mov gs, ax              ;
	lidt [idtr]             ; define a new interrupt table

	sti			; restore interrupts

__real_mode_loop:
	; loop is here
	jmp __real_mode_loop



; ====================================== ;
;       ↓ Keyboard loop procedure ↓      ;

__keyboard_loop:
;	push bp ax bx cx dx     ; protect registers against changes

	in   al,60h             ; read keyboard scan code


	mov  dl,al              ; make a copy of scancode in `dl` register


	xor  bh,bh              ; clean the `bh` register
	mov  bl,al              ; update keyboard state
	and  bl,7Fh             ; bx = scan code
	shr  al,7               ; al = 0 if pressed, 1 if released
	xor  al,1               ; al = 1 if pressed, 0 if released


	mov  cl,al              ; make a copy of keypress event in `cl`


	in   al,61h             ;
	mov  ah,al              ;
	or   al,80h             ; send EOI to XT keyboard
	out  61h,al             ;
	mov  al,ah              ;
	out  61h,al             ;

	mov  al,20h             ; place `EOI` in the `al` register
	out  20h,al             ; send `EOI` to master PIC

	cmp  cl,1               ; is key pressed?
	jne  __keyboard_loop    ; wait for another keypress if not

	cmp  dl,1Ch             ; is it `Enter` key?
	jne  __keyboard_loop    ; wait for another keypress if not

;	pop  dx cx bx ax bp     ; restore origin values from the stack

	ret

;       ↑ Keyboard loop procedure ↑      ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;      ↓ Print a message procedure ↓     ;

__print_message:
	mov  dl,byte[eax]

	cmp  dl,0
	je   __print_message_end
	call __print_letter

	inc  ax
	jmp __print_message

__print_message_end:
	ret

;      ↑ Print a message procedure ↑     ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;      ↓ Print a letter procedure ↓      ;

__print_letter:
	mov byte[edi],dl
	inc edi

	mov byte[edi],0x07
	inc edi

	ret

;      ↑ Print a letter procedure ↑      ;
; ====================================== ;


; ====================================== ;
; ▶ Data segment                         ;
; ====================================== ;

msg_prot db 'Protected mode enabled (CR0 bit PE=1)',0


idtr:
	dw 0x3ff		; 256 entries, 4b each = 1K
	dd 0			; Real Mode IVT @ 0x0000
 
savcr0:
	dd 0			; Storage location for pmode cr0.
