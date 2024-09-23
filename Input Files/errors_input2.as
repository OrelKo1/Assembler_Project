; Testing miscellaneous operations
.entry MISCSTART
.extern MISCFUNC
.extern MISCSTART /*problem here*/
MISCOP:     red @r1
             jsr STR9
             stop @r2 /* problem here */
MISCSTART:   inc NUM9
             rts
STR9:       .string "misctest"
NUM9:       .data 18, 19, 20

