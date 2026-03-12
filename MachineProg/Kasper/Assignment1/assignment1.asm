.ORIG x3000
LD R3,A
LD R4,B

X    NOT R5,R3
     ADD R5,R5,#1
     ADD R5,R5,R4
     BRz DONE
     ADD R4,R4,#-1
     ADD R3,R3,#1
     BRnzp X

DONE ST R4,C
     TRAP x25

A    .FILL #2
B    .FILL #8
C    .BLKW 1
.END