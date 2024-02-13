# labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm
  #
  # Please add your own code below this line
  #
  
	.text
	
	.global time2string
time2string:
    	# save registers, push stack
    	PUSH $ra
    	
    
    	# create "write pointer" by copying a0 address
    	move $t5, $a0
    
    	# extract minutes and seconds
    	move $t0, $a1         # copy time into $t0
    	andi $t1, $t0, 0xF000 # extract first min digit
    	srl $t1, $t1, 12      # shift right to get the digit -> 0x000F
    	andi $t2, $t0, 0x0F00 # extract second min digit
    	srl $t2, $t2, 8      
    	andi $t3, $t0, 0x00F0 # extract first sec digit
    	srl $t3, $t3, 4     
    	andi $t4, $t0, 0x000F # extract second sec digit.
    
    	# convert and store first min digit
    	move $a0, $t1
    	jal hexasc
    	nop
    	sb $v0, 0($t5)       # store byte
    	addi $t5, $t5, 1     # increment pointer
    
    	# convert and store second min digit
    	move $a0, $t2
    	jal hexasc
    	nop
    	sb $v0, 0($t5)       # store byte
    	addi $t5, $t5, 1
    
    	# store colon ":"
    	li $t1, 0x3A         # 0x3A is hex for ":", reusing t1
    	sb $t1, 0($t5)       # store byte
    	addi $t5, $t5, 1
    
    	# convert and store first sec digit
    	move $a0, $t3
    	jal hexasc
    	nop
    	sb $v0, 0($t5)       # store byte
    	addi $t5, $t5, 1
    
    	# convert and store second sec digit
    	blt $t4, 9, skip
    	
    	# store "NINE"
	li $t6, 0x4e    	# load ASCII value of 'N' into $t6
	sb $t6, 0($t5) 		# store 'N'
	addi $t5, $t5, 1 	# increment memory address

	li $t6, 0x49    	
	sb $t6, 0($t5) 		
	addi $t5, $t5, 1 	

	li $t6, 0x4e    	
	sb $t6, 0($t5) 		
	addi $t5, $t5, 1 	

	li $t6, 0x45    	
	sb $t6, 0($t5) 		
	addi $t5, $t5, 1 	
    	
    	#addi $t5, $t5, 1	# för att printa hela NINE, 
    	jal skip2
    	nop
skip:	    
	move $a0, $t4
    	jal hexasc
    	nop
    	sb $v0, 0($t5)       # store byte
    	addi $t5, $t5, 1	
skip2:
    	# store null byte to mark end of string
    	sb $0, 0($t5)
    
    	# restore stack
    	POP $ra

    	jr $ra
    	nop

	
hexasc:
  	move $t0, $a0		# create temp
  	blt $t0, 10, dig	# blt for digits lower than 10
  	addi $t0, $t0, 0x37	# else convert digits higher than 9 to hex, 0x37 is offset for ASCII
  	j end
  	nop
dig:				# convert digits lower than 10 to hex, 0x30 is offset for ASCII
  	addi $t0, $t0, 0x30	
  	nop
end:				# return v0 and jump back to main
  	move $v0, $t0
  	jr $ra
  	nop
  	
  	.global delay	
delay:
li $t7,0                      #ny
    li $a1,0
    ble $a0, 0, slut
    nop
    add $a0, $a0, -1
    jvi:
        bne $a1,$a0,delay
        nop
        add $t7,$t7,1          #ändrad
        ble $t7, 3360000, jvi 
        nop           
    j delay
nop

slut:
	jr $ra
	nop
  	
