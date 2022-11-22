  
  ;test3 
    .entry J3
    .extern E5
    MAIN: mov S1.1 , r4
    
    	macro m2
    	
    	get r5
    	
    	dec MAIN
    	
    	endmacro
    	
    LOOP: prn #68
    m2
    S1: .struct 3, "apples"
    J3: cmp #43 , E5
