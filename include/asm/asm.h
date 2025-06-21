#include "locks/spinlock.h"
#include "types.h"

#ifndef ASM_H
#define ASM_H

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

 void cpuid(uint32_t *eax, uint32_t *ebx, uint32_t *ecx, uint32_t *edx);

// Ports
uint8_t inportb(uint16_t _port);
void    outportb(uint16_t _port, uint8_t _data);

uint16_t inportw(uint16_t port);
void     outportw(unsigned short port, unsigned short data);

uint32_t inportl(uint16_t portid);
void     outportl(uint16_t portid, uint32_t value);

// Model Specific Registers (MSRs)
uint64_t rdmsr(uint32_t msrid);
uint64_t wrmsr(uint32_t msrid, uint64_t value);

void panic();


#define FLAT_PTR(PTR) (*((uintptr_t(*)[])(PTR)))

#define BYTE_PTR(PTR) (*((uint8_t *)(PTR)))
#define WORD_PTR(PTR) (*((uint16_t *)(PTR)))
#define DWORD_PTR(PTR) (*((uint32_t *)(PTR)))
#define QWORD_PTR(PTR) (*((uint64_t *)(PTR)))

void halt(void);
void cli(void);
void sti(void);
void pause(void);
void nop(void);

#endif