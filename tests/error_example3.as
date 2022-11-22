macro m1
LOOP:       jmp  W


macro error_test1 illegalname
prn #-5
sub r1 ,r4
inc   K

mov S1.2, r3
bne   L3
endmacro


MAIN:       mov S1.1, W
            add r2,STR

error_test1 illegalname

END:        hlt
STR:        .string "abcdef"
LENGTH:     .data 6,-9,15

error_test2 macro m3 
prn #-5
sub r2 ,r8
endmacro

m3
