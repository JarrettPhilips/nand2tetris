// nand2tetris project 4
// check for any KBD input
// if yes, make screen black

// while true
(OUTER_LOOP)
@flag
M=0

// check KBD for inputs
@i
M=24756
(KEYBOARD_LOOP)
@i
D=M
@24906
D=D-A
@AFTERLOOP
D;JGE

@i
D=M-1
@END_OF_KBD_LOOP
D;JGT

@i
M=M+1

(END_OF_KBD_LOOP)

// if flag true fill screen else skip
@i
M=16384
(SCREEN_LOOP)
@i
D=M
@24575
D=D-A
@AFTERLOOP
D;JGE

@i
M=1

@i
M=M+1




@OUTER_LOOP
0;JMP

