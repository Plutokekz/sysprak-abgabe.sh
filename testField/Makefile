sys: field.c field.o 
	gcc -o sysprak-field field.o

field.o: field.c field.h
	gcc -c field.c

play:
	./sysprak-field

vg:
	valgrind --leak-check=full --trace-children=yes ./sysprak-field
	
clean:
	rm field.o
