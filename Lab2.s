.data 
.dword 0x0340006f

.text

lui x3, 0x10000
ld x4, 0(x3)
 
# Extract the opcode bits from the instruction word
li x1, 0x7F      # Create a mask for the opcode bits (7 bits)
and x1, x4, x1    # Extract the opcode bits from the instruction 

li x11, 0b0110011         # Opcode for R-type
li x12, 0b0010011        # Opcodes for I-type
li x17, 0b0000011
li x18, 0b1100111
li x19, 0b1110011

li x13, 0b1100011         # Opcode for B-type
li x14, 0b0100011         # Opcode for S-type
li x15, 0b1101111         # Opcode for J-type
li x16, 0b0110111         # Opcode for U-type
li x20, 0b0010111

add x10, x0, x0

beq x1, x11, R_TYPE  # Check if it's an R-type instruction
beq x1, x12, I_TYPE  # Check if it's an I-type instruction
beq x1, x17, I_TYPE
beq x1, x18, I_TYPE
beq x1, x19, I_TYPE

beq x1, x13, B_TYPE  # Check if it's a B-type instruction
beq x1, x14, S_TYPE  # Check if it's an S-type instruction
beq x1, x15, J_TYPE  # Check if it's a J-type instruction
beq x1, x16, U_TYPE  # Check if it's a U-type instruction
beq x1, x20, U_TYPE

li x10 -1
j END

R_TYPE:
  li x10, 1  # R-type instruction
  j END

I_TYPE:
  li x10, 2  # I-type instruction
  j END

B_TYPE:
  li x10, 3  # B-type instruction
  j END

S_TYPE:
  li x10, 4  # S-type instruction
  j END

J_TYPE:
  li x10, 5  # J-type instruction
  j END

U_TYPE:
  li x10, 6  # U-type instruction
  j END

END:
 add x10, x10, x0