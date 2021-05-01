Teste1: main.c # Teste com 2 localidades válidas
	echo "\nTeste com 2 localidades válidas"
	gcc -o $@ $<
	echo 2 CWB BOB | ./$@ 

Teste2: main.c # Teste com a primeira localidade inválida
	echo "\nTeste com a primeira localidade inválida"
	gcc -o $@ $<
	echo 2 WWW LAX | ./$@

Teste3: main.c # Teste com a segunda localidade inválida
	echo "\nTeste com a segunda localidade inválida"
	gcc -o $@ $<
	echo 2 MVD XXX | ./$@

Teste4: main.c # Teste com ambas as localidades inválidas
	echo "\nTeste com ambas as localidades inválidas"
	gcc -o $@ $<
	echo 2 XXX ZZZ | ./$@

Teste5: main.c # Teste com ambas as localidades válidas e idênticas
	echo "\nTeste com ambas as localidades válidas e idênticas"
	gcc -o $@ $<
	echo 2 GRU GRU | ./$@

Teste6: main.c # Teste com ambas as localidades inválidas e idênticas
	echo "\nTeste com ambas as localidades inválidas e idênticas"
	gcc -o $@ $<
	echo 2 XXX XXX | ./$@

Teste: Teste1 Teste2 Teste3 Teste4 Teste5 Teste6