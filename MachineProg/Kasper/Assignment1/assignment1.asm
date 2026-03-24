.ORIG x3000
LD R3,A        ; load first value (lower bound)
LD R4,B        ; load second value (upper bound)

X    NOT R5,R3 ; prepare to compute R4 - R3
     ADD R5,R5,#1
     ADD R5,R5,R4
     BRz DONE  ; stop when the two values meet

     ADD R4,R4,#-1 ; move upper bound down
     ADD R3,R3,#1  ; move lower bound up
     BRnzp X       ; repeat loop no matter what

DONE ST R4,C   ; store the midpoint value
     TRAP x25  ; halt program

A    .FILL #2
B    .FILL #8
C    .BLKW 1
.END