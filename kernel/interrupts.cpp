/**
 * @file interrupts.cpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Definitions from interrupts.hpp
 * @version 0.1
 * @date 2024-02-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <kernel/interrupts.hpp>
#include <kernel/gdt.h>
#include <kernel/isr.h>
#include <klib/cstdlib.hpp>
#include <klib/io.hpp>
#include <stdint.h>
#include <stddef.h>

// Main IDT
__attribute__((aligned(0x10)))
static sys::idt_entry idt[256]; // Aligned for performance

// IDTR
static sys::idtr idtr_table;

static void setIdtEntry(size_t vec, void (*function)(uint64_t errCode), sys::idt_flags flags)
{
    auto& entry = idt[vec];

    // Setup the entry
    entry.flags = flags;
    entry.ist = 0; // For now
    entry.ist_reserved = 0;
    entry.reserved = 0;
    entry.segment_cs = CODE_SEGMENT;

    auto functionAddr = reinterpret_cast<uint64_t>(function);
    entry.isr_low = functionAddr & 0xffff;
    entry.isr_mid = (functionAddr >> 16) & 0xffff;
    entry.isr_high = functionAddr >> 32;
}

static void setupISR()
{
    setIdtEntry(0, _handler0, {0xE, 0, 0, 1});
    setIdtEntry(1, _handler1, {0xE, 0, 0, 1});
    setIdtEntry(2, _handler2, {0xE, 0, 0, 1});
    setIdtEntry(3, _handler3, {0xE, 0, 0, 1});
    setIdtEntry(4, _handler4, {0xE, 0, 0, 1});
    setIdtEntry(5, _handler5, {0xE, 0, 0, 1});
    setIdtEntry(6, _handler6, {0xE, 0, 0, 1});
    setIdtEntry(7, _handler7, {0xE, 0, 0, 1});
    setIdtEntry(8, _handler8, {0xE, 0, 0, 1});
    setIdtEntry(9, _handler9, {0xE, 0, 0, 1});
    setIdtEntry(10, _handler10, {0xE, 0, 0, 1});
    setIdtEntry(11, _handler11, {0xE, 0, 0, 1});
    setIdtEntry(12, _handler12, {0xE, 0, 0, 1});
    setIdtEntry(13, _handler13, {0xE, 0, 0, 1});
    setIdtEntry(14, _handler14, {0xE, 0, 0, 1});
    setIdtEntry(15, _handler15, {0xE, 0, 0, 1});
    setIdtEntry(16, _handler16, {0xE, 0, 0, 1});
    setIdtEntry(17, _handler17, {0xE, 0, 0, 1});
    setIdtEntry(18, _handler18, {0xE, 0, 0, 1});
    setIdtEntry(19, _handler19, {0xE, 0, 0, 1});
    setIdtEntry(20, _handler20, {0xE, 0, 0, 1});
    setIdtEntry(21, _handler21, {0xE, 0, 0, 1});
    setIdtEntry(22, _handler22, {0xE, 0, 0, 1});
    setIdtEntry(23, _handler23, {0xE, 0, 0, 1});
    setIdtEntry(24, _handler24, {0xE, 0, 0, 1});
    setIdtEntry(25, _handler25, {0xE, 0, 0, 1});
    setIdtEntry(26, _handler26, {0xE, 0, 0, 1});
    setIdtEntry(27, _handler27, {0xE, 0, 0, 1});
    setIdtEntry(28, _handler28, {0xE, 0, 0, 1});
    setIdtEntry(29, _handler29, {0xE, 0, 0, 1});
    setIdtEntry(30, _handler30, {0xE, 0, 0, 1});
    setIdtEntry(31, _handler31, {0xE, 0, 0, 1});
}

void sys::setupIDT()
{
    setupISR();

    idtr_table.addr = reinterpret_cast<uint64_t>(idt);
    idtr_table.size = 256-1;

    __asm__ __volatile__ ("lidt %0" : : "m"(idtr_table));
}

extern "C" void interruptHandler(uint64_t intNumber, uint64_t errCode)
{
    out << "Interrupt number " << intNumber << "\n";
    if (intNumber == 13)
    {
        earlyPanic("Double Fault");
    }   
    return;
}