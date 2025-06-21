#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <limine.h>
#include <print.h>
#include <asm/asm.h>
#include <gdt/gdt.h>
#include <acpi/acpi.h>
#include <fb/console.h>


static volatile LIMINE_BASE_REVISION(2);


void _start() {
    serial_init();
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        printf("NE");
        panic();
    }

    cli();

    serial_init(); // Senin kernelinde zaten var
    initiateConsole();
    printf("System started!");
    initiateGDT();
    printfch('A');
    printf("Okay!");


    // We're done, just hang...
    for(;;)
        halt();
}