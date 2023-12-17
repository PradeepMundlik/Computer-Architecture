.data
#The following line defines the 15 values present in the memory.
# We would use different values in our evaluation and
# hence you should try various combinations of these values in your testing.
.dword  1000, 1001, 1002, 1003, 1004, 1005, 1006, 1007, 1008, 1009, 523, 524, 525, 533, 512


.text
    #The following line initializes register x3 with 0x10000000 
    #so that you can use x3 for referencing various memory locations. 
    lui x3, 0x10000
    add x10, x0, x0
    
    ld x11, 0(x3)
    add x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    add x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    add x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    add x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    add x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    add x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    add x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    add x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    add x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    add x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    sub x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    sub x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    sub x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    sub x10, x10, x11
    addi x3, x3, 8
    
    ld x11, 0(x3)
    sub x10, x10, x11
    addi x3, x3, 8