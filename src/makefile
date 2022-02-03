proj1: proj1.o String.o HashTable.o Stack.o FileProcessor.o Utility.o
	gcc -Wall -std=c11 -o proj1 proj1.o String.o HashTable.o Stack.o FileProcessor.o Utility.o -lm

proj1.o: proj1.c proj1.h
	gcc -c -Wall -std=c11 proj1.c

String.o: String.c String.h
	gcc -c -Wall -std=c11 String.c

HashTable.o: HashTable.c HashTable.h
	gcc -c -Wall -std=c11 HashTable.c

Stack.o: Stack.c Stack.h
	gcc -c -Wall -std=c11 Stack.c

FileProcessor.o: FileProcessor.c FileProcessor.h
	gcc -c -Wall -std=c11 FileProcessor.c

Utility.o: Utility.c Utility.h
	gcc -c -Wall -std=c11 Utility.c