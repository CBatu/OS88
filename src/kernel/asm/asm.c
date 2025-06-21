/*
 * Copyright 2021 - 2023 NSG650
 * Copyright 2021 - 2023 Neptune
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

 #include <asm/asm.h>
 #include <print.h>

 void halt(void) {
     asm("hlt");
 }
 
 void cli(void) {
     asm("cli");
 }
 
 void sti(void) {
     asm("sti");
 }
 
 void pause(void) {
     asm("pause");
 }
 
 void nop(void) {
     asm("nop");
 }
 

 void cpuid(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx) {
    asm volatile("cpuid \n"
                 : "=a"(*eax), "=b"(*ebx), "=c"(*ecx), "=d"(*edx)
                 : "a"(*eax), "c"(*ecx)
                 : "memory");
  }
  
  uint8_t inportb(uint16_t _port) {
    uint8_t rv;
    __asm__ __volatile__("inb %1, %0" : "=a"(rv) : "dN"(_port));
    return rv;
  }
  
  void outportb(uint16_t _port, uint8_t _data) {
    __asm__ __volatile__("outb %1, %0" : : "dN"(_port), "a"(_data));
  }
  
  uint16_t inportw(uint16_t port) {
    uint16_t result;
    __asm__("in %%dx, %%ax" : "=a"(result) : "d"(port));
    return result;
  }
  
  void outportw(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : : "a"(data), "d"(port));
  }
  
  uint32_t inportl(uint16_t portid) {
    uint32_t ret;
    __asm__ __volatile__("inl %%dx, %%eax" : "=a"(ret) : "d"(portid));
    return ret;
  }
  
  void outportl(uint16_t portid, uint32_t value) {
    __asm__ __volatile__("outl %%eax, %%dx" : : "d"(portid), "a"(value));
  }
  
  uint64_t rdmsr(uint32_t msrid) {
    uint32_t low;
    uint32_t high;
    __asm__ __volatile__("rdmsr" : "=a"(low), "=d"(high) : "c"(msrid));
    return (uint64_t)low << 0 | (uint64_t)high << 32;
  }
  
  uint64_t wrmsr(uint32_t msrid, uint64_t value) {
    uint32_t low = value >> 0 & 0xFFFFFFFF;
    uint32_t high = value >> 32 & 0xFFFFFFFF;
    __asm__ __volatile__("wrmsr" : : "a"(low), "d"(high), "c"(msrid) : "memory");
    return value;
  }

  void panic() {
    printf("[kernel] Kernel panic triggered!\n");
    cli();
    while (true)
      halt();
  }