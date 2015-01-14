;;   # nasm -f bin -o handcrafted_aout.asm
;;   # chmod +x handcrafted_aout
;;   # ./handcrafted_aout

%define pagesize 4096

	bits 32
	org 0x1000			; origin address in memory

	dw 0xCC				; QMAGIC
	db 0x64				; machine_type (M_386)
	db 0x00				; flags

	dd pagesize			; a_text (section in bytes)
	dd pagesize			; a_data (section in bytes)
	dd 0				; a_bss
	dd 0				; a_syms
	dd _start			; a_entry
	dd 0				; a_trsize
	dd 0				; a_drsize

section .text
_start:
        mov eax, 4
        mov ebx, 1
        mov ecx, str
        mov edx, strsize
        int 0x80

        mov eax, 1
        mov ebx, 0
        int 0x80

	txtsize equ $ - _start
	times pagesize - txtsize db 0

section .data
_data:
        str: db "Hi! I'm an oldish a.out binary!", 0xA
	strsize equ $ - str

	datasize equ $ - _data
	times pagesize - datasize db 0
