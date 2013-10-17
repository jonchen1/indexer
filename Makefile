indexer: library
	gcc main.c -L. -l indexer -o indexer

library: tokenizer.o sorted-list.o strmap.o
	ar rcs libindexer.a tokenizer.o sorted-list.o strmap.o

tokenizer.o:
	gcc -c tokenizer.c -o tokenizer.o

sorted-list.o:
	gcc -c sorted-list.c -o sorted-list.o

strmap.o:
	gcc -c strmap.c -o strmap.o

clean:
	rm *.o
	rm indexer
	rm libindexer.a