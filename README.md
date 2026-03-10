# Assemler translator LASM

## Installation
1. `git clone https://github.com/lavosami/lasm.git`
2. `cd lasm`
3. `mkdir build && cd build`
4. `cmake ..`
5. `make`

## Using
`./lasm file_name.asm`

## Test

**ASM-file:**
```
BEGIN 1000                       start

begin:  MOV ax, a                begin
        MOV bx, b
        CALL compr
        MUL ax, bx
        MOV cx, 16

cycle:  ADD ax, 1                cycle
        LOOP cycle

compr:  CMP bx, ax               compr
        JE  exit
        RET
        
exit:   MOV c, ax                exit
        HLT

a:      DB 15
b:      DB 7
c:      DW 1
END begin
```

**Output:**
```
Address of the begin: 1000h
Entry point: 1000h
Object code:
:0210000200D2XX
:06000100104201107201XX
186101140000XX
106016110001XX
176001130000XX
166101190000XX
1062011A0000XX
006015000007XX
000001XX
:00000101XX
Size of the program: D2h
Dump: 10420110720118510114000010001611000117F00113000016E10119000010A2011A0000000015000007000001
```
