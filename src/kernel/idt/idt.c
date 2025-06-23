#include <gdt/gdt.h>
#include <idt/idt.h>
#include <asm/asm.h>
#include <util.h>

extern void* isr_stub_table[];
extern void  isr128();

static idtr_t idtr;
idt_entry_t idt[256];

char *exceptions[] = {"Division By Zero",
                      "Debug",
                      "Non Maskable Interrupt",
                      "Breakpoint",
                      "Into Detected Overflow",
                      "Out of Bounds",
                      "Invalid Opcode",
                      "No Coprocessor",

                      "Double Fault",
                      "Coprocessor Segment Overrun",
                      "Bad TSS",
                      "Segment Not Present",
                      "Stack Fault",
                      "General Protection Fault",
                      "Page Fault",
                      "Unknown Interrupt",

                      "Coprocessor Fault",
                      "Alignment Check",
                      "Machine Check",
                      "Reserved",
                      "Reserved",
                      "Reserved",
                      "Reserved",
                      "Reserved",

                      "Reserved",
                      "Reserved",
                      "Reserved",
                      "Reserved",
                      "Reserved",
                      "Reserved",
                      "Reserved",
                      "Reserved"};

char *equals = "=======================================";

void registerDump(AsmPassedInterrupt *regs) {
  printf("%s %sR%sE%sG%sD%sU%sM%sP%s %s\n"
         "+ RAX=%016lx RBX=%016lx RCX=%016lx RDX=%016lx +\n"
         "+ RSI=%016lx RDI=%016lx RBP=%016lx RSP=%016lx +\n"
         "+ R8 =%016lx R9 =%016lx R10=%016lx R11=%016lx +\n"
         "+ R12=%016lx R13=%016lx R14=%016lx R15=%016lx +\n"
         "+ RIP=%016lx RFL=%016lx _CS=%016lx _SS=%016lx +\n"
         "+ ERR=%016lx INT=%016lx _DS=%016lx RES=%016x +\n"
         "%s=========%s\n",
         equals, ANSI_RED, ANSI_GREEN, ANSI_BLUE, ANSI_RED, ANSI_GREEN,
         ANSI_BLUE, ANSI_CYAN, ANSI_RESET, equals, regs->rax, regs->rbx,
         regs->rcx, regs->rdx, regs->rsi, regs->rdi, regs->rbp,
         regs->usermode_rsp, regs->r8, regs->r9, regs->r10, regs->r11,
         regs->r12, regs->r13, regs->r14, regs->r15, regs->rip, regs->rflags,
         regs->cs, regs->usermode_ss, regs->error, regs->interrupt, regs->ds, 0,
         equals, equals);
}

void idtSetDescriptor(uint8_t vector, void* isr, uint8_t flags){
  idt_entry_t* descriptor = &idt[vector];

  descriptor->isr_low        = (uint64_t)isr & 0xFFFF;
  descriptor->kernel_cs     = GDT_KERNEL_CODE;
  descriptor->ist            = 0;
  descriptor->attributes     = flags;
  descriptor->isr_mid        = ((uint64_t)isr >> 16) & 0xFFFF;
  descriptor->isr_high       = ((uint64_t)isr >> 32) & 0xFFFFFFFF;
  descriptor->reserved       = 0;
}

void disable_pic() {
  // outportb(0x20, ICW_1);
  // outportb(0xA0, ICW_1);
  // outportb(0x21, ICW_2_M);
  // outportb(0xA1, ICW_2_S);
  // outportb(0x21, ICW_3_M);
  // outportb(0xA1, ICW_3_S);
  // outportb(0x21, ICW_4);
  // outportb(0xA1, ICW_4);
  outportb(0x21, 0xFF);
  outportb(0xA1, 0xFF);
}

void remap_pic() {
  outportb(0x20, 0x11);
  outportb(0xA0, 0x11);
  outportb(0x21, 0x20);
  outportb(0xA1, 0x28);
  outportb(0x21, 0x04);
  outportb(0xA1, 0x02);
  outportb(0x21, 0x01);
  outportb(0xA1, 0x01);
  outportb(0x21, 0x00);
  outportb(0xA1, 0x00);

  disable_pic();
}

void initiateIDT(){
    remap_pic();


    idtr.base = (uint64_t)&idt;
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * 256 - 1;

    for(int i = 0; i < 48; i++)
    {
        idtSetDescriptor(i, isr_stub_table[i], 0x8e);
    }

    idtSetDescriptor(0x60, isr_stub_table[128], 0xEE);

    __asm__ volatile ("lidt %0" :: "m"(idtr));
    printf("IDT Initialized!\n");
    sti();
}



void interruptHandler(uint64_t rsp) {
  AsmPassedInterrupt *cpu = (AsmPassedInterrupt *)rsp;
  if (cpu->interrupt >= 0 && cpu->interrupt < 32) {
    if (cpu->interrupt == 14) {
      uint64_t err_pos;
      asm volatile("movq %%cr2, %0" : "=r"(err_pos));
      printf("[isr] Page fault occured at cr2{%lx} rip{%lx}\n", err_pos,
             cpu->rip);
    }
    registerDump(cpu);
    printf("Panic! Interrupt %s occured\n", exceptions[cpu->interrupt]);
    panic();
  }
}

void syscallHandler() {
    printf("\nNot implemented yet. Syscall\n");
    return;
}