.entry CONTROLSTART
.extern CONTROLEXTFUNC
CONTROLMAIN: mov @r2, CONTROLDATA
              lea STRCONTROL1, @r5
CONTROLSTART: cmp @r1, @r6
              bne CONTROLPROC
              jmp CONTROLPROC2
              clr @r7
              prn @r5
              stop
CONTROLPROC: add @r4, @r5
              sub @r3, @r6
              rts
CONTROLPROC2: inc CONTROLDATA
              dec CONTROLDATA
              stop
STRCONTROL1:  .string "controlflowtest1"
CONTROLDATA:  .data 5, 15, 25, 35, 45
ARRCONTROL:   .data -10, -9, -8, -7, -6
.entry DATAOPSTART
.extern DATAOPEXTFUNC
DATAOPSTART:    mov @r3, DATAOPDATA
DATAOPLOOP:    add @r2, @r5
                sub @r4, @r6
                cmp @r5, @r7
                bne DATAOPPROC
                clr @r6
                lea STRDATAOP1, @r1
DATAOPPROC:    inc DATAOPDATA
                dec DATAOPDATA
                rts
                stop
STRDATAOP1:    .string "dataoptest1"
DATAOPDATA:    .data 5, 15, 25, 35, 45
ARRDATAOP:     .data -5, -4, -3, -2, -1

