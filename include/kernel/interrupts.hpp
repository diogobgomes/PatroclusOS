/**
 * @file interrupts.hpp
 * @author Diogo Gomes (dbarrosgomes@gmail.com)
 * @brief Everything about interrupts (structs for dealing with the IDT. all the
 * ISRs, etc)
 * @version 0.1
 * @date 2024-02-25
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#pragma once

#include <stdint.h>
#include <klib/cstdlib.hpp>


namespace sys
{

struct idt_flags {
    /* 0xE for 64-bit Interrupt Gate, and 0xF for 64-bit Trap Gate */
    uint8_t         type: 4;

    /* Reserved, no touching */
    uint8_t         zero: 1;

    /* Defines the CPU privilege levels that are allowed to access this interrupt */
    uint8_t         dpl: 2;

    /* Set if present */
    uint8_t         present: 1;
} __attribute__((packed));

static_assert(sizeof(idt_flags) == 1);

/**
 * @brief IDT entry
 * 
 */
struct idt_entry
{
    /* Lower 16 bits of the ISR's address */
    uint16_t        isr_low;

    /* The GDT segment selector that the CPU will load into %cs */
    uint16_t        segment_cs;

    /* The IST in the TSS that the CPU will load into %rsp */
    uint8_t         ist:3;

    /* Reserved, no touching */
    uint8_t         ist_reserved:5;

    /* 0xE for 64-bit Interrupt Gate, and 0xF for 64-bit Trap Gate */
    idt_flags       flags;

    /* The higher 16 bits of the lower 32 bits of the ISR's address */
    uint16_t        isr_mid;

    /* The higher 32 bits of the ISR's address */
    uint32_t        isr_high;

    /* Set to 0 */
    uint32_t        reserved;
} __attribute__((packed));

static_assert(sizeof(idt_entry) == 16);

/**
 * @brief IDT descriptor
 * 
 */
struct idtr
{
    /* One less than the size of the IDT in bytes */
    uint16_t        size;

    /* The linear address of the IDT */
    uint64_t        addr;
} __attribute__((packed));

static_assert(sizeof(idtr) == 10);

//void setIdtEntry(size_t vec, void (*function)(uint64_t errCode), idt_flags flags );

void setupIDT(void);

static inline void enableInterrupts(void) { __asm__ __volatile__ ("sti"); }

static inline void disableInterrupts(void) { __asm__ __volatile__ ("cli"); }

/*
struct interrupt_frame
{
    uint64_t ip;
    uint64_t cs;
    uint64_t flags;
    uint64_t sp;
    uint64_t ss;
};

__attribute__((interrupt)) void HandlerNoErr(interrupt_frame* frame);

__attribute__((interrupt)) void HandlerErr(interrupt_frame* frame);

template<typename handler>
class IDTEntryClass
{
private:
    idt_entry _idtEntry;
public:

};

using HandlerErrPtr = void(*)(interrupt_frame*);
using HandlerNoErrPtr = void(*)(interrupt_frame*);

static_assert(sizeof(IDTEntryClass<HandlerErrPtr>) == sizeof(idt_entry));
static_assert(sizeof(IDTEntryClass<HandlerNoErrPtr>) == sizeof(idt_entry));

struct exceptionTable
{
    IDTEntryClass<HandlerNoErrPtr> divisionError;
    IDTEntryClass<HandlerNoErrPtr> debug;
    IDTEntryClass<HandlerNoErrPtr> nonMaskableInterrupt;
    IDTEntryClass<HandlerNoErrPtr> breakpoint;
    IDTEntryClass<HandlerNoErrPtr> overflow;
    IDTEntryClass<HandlerNoErrPtr> boundRangeExceeded;
    IDTEntryClass<HandlerNoErrPtr> invalidOpcode;
    IDTEntryClass<HandlerNoErrPtr> deviceNotAvailable;
    IDTEntryClass<HandlerErrPtr>   doubleFault;
    IDTEntryClass<HandlerNoErrPtr> coprocessorSegmentOverrun;
    IDTEntryClass<HandlerErrPtr>   invalidTSS;
    IDTEntryClass<HandlerErrPtr>   segmentNotPresent;
    IDTEntryClass<HandlerErrPtr>   stackSegmentFault;
    IDTEntryClass<HandlerErrPtr>   generalProtectionFault;
    IDTEntryClass<HandlerErrPtr>   pageFault;
    IDTEntryClass<HandlerNoErrPtr> reserved;
    IDTEntryClass<HandlerNoErrPtr> x87FloatingPointException;
    IDTEntryClass<HandlerErrPtr>   alignmentCheck;
    IDTEntryClass<HandlerNoErrPtr> machineCheck;
    IDTEntryClass<HandlerNoErrPtr> simdFloatingPointException;
    IDTEntryClass<HandlerNoErrPtr> virtualizationException;
    IDTEntryClass<HandlerErrPtr>   controlProtectionException;
    // TODO End this
};*/



} // namespace sys

extern "C" void interruptHandler( uint64_t intNumber, uint64_t errCode );