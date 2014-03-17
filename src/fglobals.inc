; ====================================== ;
;           ↓ Print procedure ↓          ;

_print:
                        ; ▶ [bp] - string address
                        ; ▶ [cx] - string length

        mov  bl,07h     ; set color: 03h - teal
                        ;            07h - white

        mov  ax,1301h   ; display string and update cursor
        int  10h        ; shorthand for BIOS interrupt call
        ret             ; return from procedure

;           ↑ Print procedure ↑          ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;     ↓ Print character procedure ↓      ;

_print_char:
			; ▶ [al] - ASCII character to display

        mov  bl,07h	; set color: 03h - teal
                        ;            07h - white

	mov  cx,1       ; number of times to print character
        mov  ah,09h     ; display string and update cursor
        int  10h        ; shorthand for BIOS interrupt call
        ret             ; return from procedure

;     ↑ Print character procedure ↑      ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;           ↓ Cursor procedure ↓         ;

_cursor:
                        ; ▶ [dh] - row
                        ; ▶ [dl] - column

        mov  ah,2h      ; set cursor position
        xor  bh,bh      ; clean [bh] register (bh - page number)

        int  10h        ; shorthand for BIOS interrupt call
        ret             ; return from procedure

;           ↑ Cursor procedure ↑         ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;       ↓ Cursor reset procedure ↓       ;

_cursor_reset:
	mov  dl,0       ; row position = 0
	mov  dh,0       ; col position = 0
        call _cursor    ; local procedure call
        ret             ; return from procedure

;       ↑ Cursor reset procedure ↑       ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;     ↓ Cursor move right procedure ↓    ;

_cursor_move_right:
	add  dl,1       ; row position = [dl] + 1
        call _cursor    ; local procedure call
        ret             ; return from procedure

;     ↑ Cursor move right procedure ↑    ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;     ↓ Cursor move left procedure ↓     ;

_cursor_move_left:
	sub  dl,1       ; row position = [dl] - 1
        call _cursor    ; local procedure call
        ret             ; return from procedure

;     ↑ Cursor move left procedure ↑     ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;      ↓ Cursor newline procedure ↓      ;

_cursor_newline:
	mov  dl,0       ; row position = [dl] - 1
	mov  dh,1       ; col position = 1
        call _cursor    ; local procedure call
        ret             ; return from procedure

;      ↑ Cursor newline procedure ↑      ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;      ↓ Screen cleaner procedure ↓      ;

_screen_clean:
	mov  ax, 0002h
	int  10h        ; clean screen
	ret             ; return from procedure

;      ↑ Screen cleaner procedure ↑      ;
; ====================================== ;
; ====================================== ;

_string_clean:
;	mov cl,0                ; cl is the counter register
;
;_string_clean_loop:		; beginning of loop
;	mov al,bytes[cl]        ; read the next byte from memory

;	cmp al,0                ; compare the byte to null
;	je _string_clean_end    ; if the byte is null, jump out of the loop

;	mov bytes[cl],0

;	add cl,1                ; move to the next byte in the string

;	jmp _string_clean_loop

_string_clean_end:
;	ret

; ====================================== ;
; ====================================== ;
;       ↓ Get character procedure ↓      ;

_get_char:
	mov  ah,10h     ; read extended keyboard input
	int  16h        ; shorthand for BIOS interrupt call

	ret             ; return from procedure

                        ; ◀ [al] - ASCII character
                        ; ◀ [ah] - scancode

;       ↑ Get character procedure ↑      ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;        ↓ Press enter procedure ↓       ;

_get_enter:
	call _get_char          ; wait for keypress

	cmp  al,0Dh             ; is it `Enter` character?
	jne  _get_enter         ; read one more if not
	ret                     ; return from procedure

;        ↑ Press enter procedure ↑       ;
; ====================================== ;
; -------------------------------------- ;
; ====================================== ;
;        ↓ Load kernel procedure ↓       ;

_load_kernel:
	mov  ax,0000h
	mov  es,ax
	mov  bx,500h
	mov  ch,0       ; cylinder number           - 0
	mov  cl,02h     ; starting sector           - 2
	mov  dh,0       ; HDD head number           - 0
	mov  dl,80h     ; HDD itself                - 80h
	mov  al,02h     ; number of sectors to read - 2
	mov  ah,02h     ; read sectors from drive   - 2h
	int  13h        ; shorthand for BIOS interrupt call
	jmp  0000:0500h ; jump to kernel

	ret             ; return from procedure

;        ↑ Load kernel procedure ↑       ;
; ====================================== ;
