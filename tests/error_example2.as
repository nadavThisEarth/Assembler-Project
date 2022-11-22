macro m1
.entry same
.extern same
endmacro

macro m2
.entry one
.entry one
endmacro


  mov #-32 , W

  cmp r4 , S.1

  m1 
  
   m2
