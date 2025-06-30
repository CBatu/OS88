global syscall_entry
syscall_entry:
  swapgs
  mov cr2, rax ; use cr2 as an extra register
  mov rax, qword [gs:0] ; thread pointer
  ; mov rax, [rax] ; kernel stack top
  xchg rsp, rax ; switch stack ptrs

  push rax
  mov rax, cr2

  ; mimic: interrupt stuff
  push qword 0
  push qword 0
  push qword 0
  push qword 0
  push qword 0

  push qword 0 ; error
  push qword 0 ; interrupt

  push rax
  push rbx
  push rcx
  push rdx
  push rsi
  push rdi
  push rbp
  push r8
  push r9
  push r10
  push r11
  push r12
  push r13
  push r14
  push r15

  mov rbp, ds
  push rbp

  mov rdi, rsp
  extern syscallHandler
  call syscallHandler
  cli ; important to avoid race conditions
  
  pop rbp
  mov ds, ebp
  mov es, ebp

  pop r15
  pop r14
  pop r13
  pop r12
  pop r11
  pop r10
  pop r9
  pop r8
  pop rbp
  pop rdi
  pop rsi
  pop rdx
  pop rcx
  pop rbx
  pop rax

  add rsp, 16      ; pop error code and interrupt number
  add rsp, 40      ; pop other interrupt stuff

  pop rsp ; reset rsp

  o64 sysret

isr_common:
    push rax
    push rbx
    push rcx
    push rdx
    push rsi
    push rdi
    push rbp
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rbp, ds
    push rbp

    mov bx, 0x30
    mov ds, bx
    mov es, bx
    mov ss, bx
    ; mov fs, bx
    ; mov gs, bx

		mov rdi, rsp
    extern interruptHandler
    call interruptHandler

%macro isr_err_stub 1
global isr_stub_%1
isr_stub_%1:
		; error code is already pushed
    push %1   ; interrupt number
    jmp isr_common
%endmacro

%macro isr_no_err_stub 1
global isr_stub_%1
isr_stub_%1:
    push 0    ; dummy error code to align with TrapFrame
    push %1   ; interrupt number
    jmp isr_common
%endmacro

global isr255
isr255:
    push 0    ; dummy error code to align with TrapFrame
    push 255   ; interrupt number
    jmp isr_common


extern interruptHandler
extern syscallHandler
isr_no_err_stub 0
isr_no_err_stub 1
isr_no_err_stub 2
isr_no_err_stub 3
isr_no_err_stub 4
isr_no_err_stub 5
isr_no_err_stub 6
isr_no_err_stub 7
isr_err_stub    8
isr_no_err_stub 9
isr_err_stub    10
isr_err_stub    11
isr_err_stub    12
isr_err_stub    13
isr_err_stub    14
isr_no_err_stub 15
isr_no_err_stub 16
isr_no_err_stub 17
isr_no_err_stub 18
isr_no_err_stub 19
isr_no_err_stub 20
isr_no_err_stub 21
isr_no_err_stub 22
isr_no_err_stub 23
isr_no_err_stub 24
isr_no_err_stub 25
isr_no_err_stub 26
isr_no_err_stub 27
isr_no_err_stub 28
isr_no_err_stub 29
isr_no_err_stub 30
isr_no_err_stub 31

isr_no_err_stub 32
isr_no_err_stub 33
isr_no_err_stub 34
isr_no_err_stub 35
isr_no_err_stub 36
isr_no_err_stub 37
isr_no_err_stub 38
isr_no_err_stub 39
isr_no_err_stub 40
isr_no_err_stub 41
isr_no_err_stub 42
isr_no_err_stub 43
isr_no_err_stub 44
isr_no_err_stub 45
isr_no_err_stub 46
isr_no_err_stub 47


isr_no_err_stub 128

section .data
global isr_stub_table
isr_stub_table:
%assign i 0 
%rep    48
    dq isr_stub_%+i
%assign i i+1 
%endrep