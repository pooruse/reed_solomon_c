

reed_solomon_test: main.o reed_solomon.o galois.o
	gcc $^ -o $@

%.o: %.c
	gcc -c $< -o $@

.PHONY: clean
clean:
	-rm *.o rm reed_solomon_test
