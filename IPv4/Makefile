main:	clean snd_rcv.o orch.o add.o mul.o sou.o div.o
		gcc -Wall -Wextra -g -o orch obj/orch.o obj/snd_rcv.o -fopenmp
		gcc -Wall -Wextra -g -o add obj/add.o obj/snd_rcv.o -fopenmp
		gcc -Wall -Wextra -g -o sou obj/sou.o obj/snd_rcv.o -fopenmp
		gcc -Wall -Wextra -g -o mul obj/mul.o obj/snd_rcv.o -fopenmp
		gcc -Wall -Wextra -g -o div obj/div.o obj/snd_rcv.o -fopenmp

snd_rcv.o:	src/snd_rcv.c include/snd_rcv.h 
		gcc -Wall -Wextra -g -c src/snd_rcv.c
		mv snd_rcv.o obj

orch.o:	src/orch.c 
		gcc -Wall -Wextra -g -c src/orch.c -fopenmp
		mv orch.o obj

add.o: src/add.c
		gcc -Wall -Wextra -g -c src/add.c -fopenmp
		mv add.o obj
		
div.o: src/div.c
		gcc -Wall -Wextra -g -c src/div.c -fopenmp
		mv div.o obj
		
mul.o: src/mul.c
		gcc -Wall -Wextra -g -c src/mul.c -fopenmp
		mv mul.o obj
		
sou.o: src/sou.c
		gcc -Wall -Wextra -g -c src/sou.c -fopenmp
		mv sou.o obj
		
clean:
		rm -R obj
		mkdir obj

archive:
	tar -zcvf Réseaux.tar.gz src/ include/ Makefile obj
	mv Réseaux.tar.gz Réseaux.tg

exec:
		./sou &
		./add &
		./div &
		./mul &
		./orch
	
