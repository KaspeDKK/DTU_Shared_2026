.ORIG x3000

;Test eksempel:
        ;AND R0, R0, #0 ;reset
        ;ADD R0, R0, #1 ;prime
        JSR resultS
        HALT

;----
;Funktion: resultS
;Hvis tallet i R0 ≠ 0 : Output prime besked, hvis det er 0: Output ikke prime besked.
;---

resultS ADD R0, R0, #0
        BRnp prime
        BRz noPrime


prime   LEA R0, stringPrime
        BRnzp end

noPrime LEA R0, stringNotPrime
        BRnzp end

end     PUTS
        RET     ;return 

stringPrime     .STRINGZ "The number is prime"
stringNotPrime  .STRINGZ "The number is not prime"

.END