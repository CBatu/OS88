/***
#include <acpi/acpi.h>

#include <asm/asm.h>
#include <uacpi/uacpi.h>
#include <print.h>
#include <linux.h>

#include <uacpi/sleep.h>


void initiateACPI() {
  uacpi_status ret = uacpi_initialize(0);
  if (uacpi_unlikely_error(ret)) {
    printf("uacpi_initialize error: %s", uacpi_status_to_string(ret));
    panic();
  }

  
   * Load the AML namespace. This feeds DSDT and all SSDTs to the interpreter
   * for execution.
   
  ret = uacpi_namespace_load();
  if (uacpi_unlikely_error(ret)) {
    printf("uacpi_namespace_load error: %s", uacpi_status_to_string(ret));
    panic();
  }

  // set the interrupt model
  uacpi_set_interrupt_model(UACPI_INTERRUPT_MODEL_IOAPIC);


   * Initialize the namespace. This calls all necessary _STA/_INI AML methods,
   * as well as _REG for registered operation region handlers.
   
  ret = uacpi_namespace_initialize();
  if (uacpi_unlikely_error(ret)) {
    printf("uacpi_namespace_initialize error: %s", uacpi_status_to_string(ret));
    panic();
  }
}

size_t acpiPoweroff() {
  uacpi_status ret = uacpi_prepare_for_sleep_state(UACPI_SLEEP_STATE_S5);
  if (uacpi_unlikely_error(ret)) {
    printf("[acpi] Couldn't prepare for poweroff: %s\n",
           uacpi_status_to_string(ret));
    return ERR(EIO);
  }

  cli();
  uacpi_status retPoweroff = uacpi_enter_sleep_state(UACPI_SLEEP_STATE_S5);
  if (uacpi_unlikely_error(retPoweroff)) {
    sti();
    printf("[acpi] Couldn't power off the system: %s\n",
           uacpi_status_to_string(retPoweroff));
    return ERR(EIO);
  }

  printf("[acpi] Shouldn't be reached after power off!\n");
  panic();
  return 0;
}

size_t acpiReboot() {
  uacpi_prepare_for_sleep_state(UACPI_SLEEP_STATE_S5);

  uacpi_status ret = uacpi_reboot();
  if (uacpi_unlikely_error(ret)) {
    printf("[acpi] Couldn't restart the system: %s\n",
           uacpi_status_to_string(ret));
    return ERR(EIO);
  }

  printf("[acpi] Shouldn't be reached after reboot!\n");
  panic();
  return 0;
}

****/