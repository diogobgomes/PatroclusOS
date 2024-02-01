# @file longMode.S
# @author Diogo Gomes
# @brief Functions to set up long mode, so that the processor can jump
# @version 0.1
# @date 2024-02-01

_multiboot_magic = 0x2BADB002
_CPUID_eax_long_mode = 0x80000000
_CPUID_long_mode_expected = 0x80000001
_EFER_MSR_location = 0xC0000080

.code32
.section .text
.global checkCPUID
.type checkCPUID, @function
checkCPUID:
    # Use CPUID to check whether long mode is supported
    # Returns 0 for success, 1 for no CPUID support, 2 for no long mode

    # First, check if CPUID is supported
    call check_CPUID_available
    test %eax, %eax
    jne .no_CPUID # Hack to return to original caller

    # Now, check long mode
    call check_long_mode
    test %eax, %eax
    jne .no_long_mode

    # Successful return
    mov $0, %eax
    ret

# ------------------------------------------------------------------------------
# Helper functions
# ------------------------------------------------------------------------------
check_CPUID_available:
    # Check if CPUID is supported by trying to flip bit 21 in 
    # the FLAGS register. If we can, CPUID is available

    # Copy FLAGS to %eax
    pushfl
    pop %eax
    mov %eax, %ecx

    # Flip bit 21
    xor $1 << 21, %eax

    # Copy %eax to FLAGS
    push %eax
    popfl

    # Copy back to %eax
    pushfl
    pop %eax

    # Restore FLAGS with old version in %ecx
    push %ecx
    popfl

    # Compare
    cmp %eax, %ecx
    je .no_CPUID
    
    # Return true
    mov $0, %eax
    ret

.no_CPUID: # Err
    mov $1, %eax
    ret


check_long_mode:
    # Test if CPUID has the function that tests long mode
    mov $_CPUID_eax_long_mode, %eax
    cpuid
    cmpl $_CPUID_long_mode_expected, %eax
    jb .no_long_mode

    # Test if long mode is available using CPUID
    mov $_CPUID_long_mode_expected, %eax
    cpuid
    test $1 << 29, %edx
    jz .no_long_mode
    
    mov $0, %eax
    ret
.no_long_mode: # Err
    mov $2, %eax
    ret

.global enablePaging
.type enablePaging, @function
enablePaging:
    # Get p4_table address
    pop %ebx # Return address
    pop %eax

    # Load P4 to CR3 register
    #mov %ebx, %eax
    mov %eax, %cr3

    # Enable PAE-flag in CR4
    mov %cr4, %eax
    or $1 << 5, %eax
    mov %eax, %cr4

    # Set long mode bit in the EFER MSR (model specific register)
    mov $_EFER_MSR_location, %ecx
    rdmsr
    or $1 << 8, %eax
    wrmsr

    # Enable paging in the CR0 register
    mov %cr0, %eax
    or $1 << 31, %eax
    mov %eax, %cr0

    # Put return address on the stack again
    push %ebx

    ret