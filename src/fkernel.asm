org 500h                        ; kernel sector starts from 500h




; ====================================== ;
; ▶ Entry code segment                   ;
; ====================================== ;

kernel:
	call _screen_clean      ; clean the screen
	call _cursor_reset      ; reset the cursor position

	mov  bp,msg_input       ; message address
	mov  cx,msg_input_size  ; message length
	call _print             ; print message on the screen

	call _cursor_newline    ; print a newline
	call _cursor_newline    ; print a newline

	mov  si,0

kernel_loop: 
	call _get_char          ; wait for keypress

	cmp  ah,0Eh             ; is it a `Backspace` character?
	je   kernel_input_backspace

	mov  [buffer+si],al
	inc  si

	cmp  al,0Dh             ; is it a `Enter` character?
	je   kernel_exec

	call _print_char
	call _cursor_move_right

	jmp  kernel_loop

kernel_exec:
	mov  ax,cs
	mov  ds,ax
	mov  es,ax

kernel_exec_switch:
	mov  di,buffer
	mov  si,msg_switch
	mov  cx,msg_switch_size

	repe cmpsb
	jne  kernel_loop

	call _switch
	jmp  kernel

;kernel_exec_end:
;	pop  si
;	jmp  kernel_loop
	     
kernel_input_backspace:
	cmp  dl,0
	je   kernel_loop

	call _cursor_move_left

	mov  al,20h            ; space instead of curren symbol
	mov  [buffer + si],al  ; erase it

	call _print_char

	dec  si                ; dec it
	jmp  kernel_loop

; ====================================== ;
; ▶ Subcode segment                      ;
; ====================================== ;

include 'fglobals.inc'

include 'fswitch.inc'


; ====================================== ;
; ▶ Data segment                         ;
; ====================================== ;

msg_input db 'Input the command...',0
msg_input_size = $ - msg_input

msg_switch db 'switch',13
msg_switch_size = $ - msg_switch

buffer db 24 dup(?) ; buffer
