org 7C00h                       ; BIOS reads first 512 bytes from first
                                ; MBR sector in the RAM by it's address
                                ; (0x00007C00)

; ====================================== ;
; ▶ Entry code segment                   ;
; ====================================== ;

start:
	cli                     ; disallow interrupts
	xor  ax,ax              ; clean [ax] register
	mov  ds,ax              ; bind the data segment to zero address
	mov  es,ax              ; bind [es] segment to zero adress
	mov  ss,ax              ; bind stack segment to zero adress
	mov  sp,07C00h          ; sp indicates the beginning of the stack
	sti                     ; allow interrupts again

	call _screen_clean      ; clean the screen
	call _cursor_reset      ; reset cursor position

	mov  bp,msg_load        ; message address
	mov  cx,msg_load_size   ; message length
	call _print             ; print message on the screen

	call _cursor_newline    ; print a newline

	mov  bp,msg_enter       ; message address
	mov  cx,msg_enter_size  ; message length
	call _print             ; print message on the screen

	call _get_enter         ; wait for `Enter` keypress

	jmp  _load_kernel       ; load kernel segment


; ====================================== ;
; ▶ Subcode segment                      ;
; ====================================== ;

include 'fglobals.inc'


; ====================================== ;
; ▶ Data segment                         ;
; ====================================== ;

msg_load db 'OS Loaded.',0
msg_load_size = $ - msg_load

msg_enter db 'Press `Enter` to continue',0
msg_enter_size = $ - msg_enter
  
times(512-2-($-07C00h)) db 0    ; erase rested space
db 055h,0AAh                    ; bootsector end
