.entry 64 
.mymodifier ET

DuplicateLabel: mov #-43 , r5

macro m1
r4: mov #3.2 r5
NOCOLON cmp r4 ,r3
endmacro

MISSINGCOMMA: add #32 r1

ADR: lea #45, r4

   : mov r3 r5  

ILLEGALCOMMAS: .data 3 , , 7,6
ILLEGALCOMMAS2: .struct , , , 6 ,"ag"

ANOTHER: .struct unknown 6 , "af"

ThisLabelIswayTooLongForThisProject: prn r3

HebrewLetters: .string "אבגד"

m1

.struct 66.32 , "abd"

.extern DuplicateLabel
