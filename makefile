shell : shell.c
	gcc shell.c -o shell
.PHONY:clean
clean:
	rm shell
