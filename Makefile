Teste1: main.c # Teste com 2 localidades válidas
	gcc -o $@ $<
	echo 2 CWB BOB | ./TravelAir 

Teste2: main.c # Teste com a primeira localidade inválida
	gcc -o $@ $<
	echo 2 WWW LAX | ./TravelAir

Teste3: main.c # Teste com a segunda localidade inválida
	gcc -o $@ $<
	echo 2 MVD XXX | ./TravelAir

Teste4: main.c # Teste com ambas as localidades inválidas
	gcc -o $@ $<
	echo 2 XXX ZZZ | ./TravelAir

Teste5: main.c # Teste com ambas as localidades válidas e idênticas
	gcc -o $@ $<
	echo 2 GRU GRU | ./TravelAir

Teste6: main.c # Teste com ambas as localidades inválidas e idênticas
	gcc -o $@ $<
	echo 2 XXX XXX | ./TravelAir

Teste: Teste1 Teste2 Teste3 Teste4 Teste5 Teste6