.section .data
L1: .word 5000000
.section .text
.global main

main:
    lui x6, 0x10012

    addi x2, x0, 0x00000020

    sw x0, 4(x6)
    sw x2, 8(x6)

    sw x2, 12(x6)

la x3, L1
lw x21, 0(x3)
add x5, x0, x21
add x4, x0, x0
addi x20, x0, 1

switch:
    beq x4, x20, on
    beq x4, x0, off

L:
    beq x1, x5, switch
    addi x1, x1, 1
    beq x0, x0, L

on:
    sw x2, 12(x6)
    add x1, x0, x0
    add x4, x0, x0
    beq x0, x0, L

off:
    sw x0, 12(x6)
    add x1, x0, x0
    addi x4, x0, 1
    beq x0, x0, L
