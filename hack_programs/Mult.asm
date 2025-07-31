// nand2tetris project 4
// multiply numbers already stored in R0 and R1 virtual registers
// store result in R2
// R0 >=0, R1 >= 0, R0*R1 <32768

// initialize variables sum and i
@sum
M=0
@i
M=0


(LOOP)
// while(i<R1) header
@i
D=M
@R1
D=D-M
@AFTERLOOP
D;JGE

@R0
D=M
@sum
M=D+M // sum +=R1
@i
D=M
M=M+1 // i++

@LOOP
0;JMP

(AFTERLOOP)
@sum
D=M
@R2
M=D

(END)
@END
0;JMP