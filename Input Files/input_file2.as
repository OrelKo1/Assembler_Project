mcro str
       S1: cmp    @r0    ,    -123
  mov -23, @r2
    mov 123 , La
endmcro
     str
    L213jk: cmp    @r7    ,     @r0
    prn   S1
  prn -28
prn     @r4
S4: .data 4, 7
     sub     LABEL     ,     S4
    LABEL: sub @r7    , @r4
La: cmp           s ,    S4
jmp         LABEL
s: stop
    lea LABEL ,  @r0
    lea s ,  d
    stop
   rts
.extern    d
