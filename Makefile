compute_l: compute_l.c
	gcc -o compute_l compute_l.c -lm

clean:
	rm -f compute_l
