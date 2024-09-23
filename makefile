
assembler:  assemblerMain.o AnalyzeLine.o Assembler.o FirstPass.o PreAssembler.o SecondPass.o SourceFileLines.o ValidateData.o ValidateEntryAndExtern.o
	gcc -g  -Wall -ansi -pedantic assemblerMain.o AnalyzeLine.o Assembler.o FirstPass.o PreAssembler.o SecondPass.o SourceFileLines.o ValidateData.o ValidateEntryAndExtern.o -lm -o assembler
assemblerMain.o: assemblerMain.c assemblerMain.h
	gcc -c -g   -Wall -ansi -pedantic assemblerMain.c -o assemblerMain.o
AnalyzeLine.o: AnalyzeLine.c AnalyzeLine.h
	gcc -c -g  -Wall -ansi -pedantic AnalyzeLine.c -o AnalyzeLine.o
Assembler.o: Assembler.c Assembler.h
	gcc -c -g   -Wall  -ansi -pedantic Assembler.c -o Assembler.o
FirstPass.o: FirstPass.c FirstPass.h
	gcc -c -g   -Wall  -ansi -pedantic FirstPass.c -o FirstPass.o
PreAssembler.o: PreAssembler.c PreAssembler.h
	gcc -c -g   -Wall  -ansi -pedantic PreAssembler.c -o PreAssembler.o
SecondPass.o: SecondPass.c SecondPass.h
	gcc -c -g   -Wall  -ansi -pedantic SecondPass.c -o SecondPass.o
SourceFileLines.o: SourceFileLines.c SourceFileLines.h
	gcc -c -g   -Wall -ansi -pedantic SourceFileLines.c -o SourceFileLines.o
ValidateData.o: ValidateData.c ValidateData.h
	gcc -c -g   -Wall  -ansi -pedantic ValidateData.c -o ValidateData.o
ValidateEntryAndExtern.o: ValidateEntryAndExtern.c ValidateEntryAndExtern.h
	gcc -c -g   -Wall -ansi -pedantic ValidateEntryAndExtern.c -o ValidateEntryAndExtern.o
clean:
	rm -f *.o *~ s
