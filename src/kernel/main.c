#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <print.h>
#include <asm/asm.h>
#include <gdt/gdt.h>
#include <fb/console.h>
#include <bootloader.h>
#include <fb/vga.h>
#include <idt/idt.h>


static volatile LIMINE_BASE_REVISION(2);
void _start() {
    serial_init();
    initialiseBootloaderParser();
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        printf("NE");
        panic();
    }

    cli();
    initiateVGA();
    initiateConsole();
    initialized = true;
    clearScreen();
    initiateGDT();
    initiateIDT();
    printf("Hello, world!\n");
    __asm__ volatile("int $0x03");

    printf("Testing completed\n");
    
    // We're done, just hang...
    for(;;)
        halt();
}