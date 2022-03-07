jrem : jrem.c
	gcc -std=c99 -Wall jrem.c -o jrem

clean:
	rm -rf $(HOME)/.local/bin/jrem
