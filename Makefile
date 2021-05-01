Teste0: main.c # Teste com 2 localidades válidas
	@printf "\n--Teste para imprimir conexões--\n"
	@printf "Caso 1, IATA escolhido: CWB\n"
	@gcc -lm -o $@ $<
	@echo 1 CWB | ./$@

Teste1: main.c # Teste com 2 localidades válidas
	@printf "\n--Teste com 2 localidades válidas--\n"
	@printf "Caso 2, Rota escolhida: CWB -> BOB\n"
	@gcc -lm -o $@ $<
	@echo 2 CWB BOB | ./$@

Teste2: main.c # Teste com a primeira localidade inválida
	@printf "\n--Teste com a primeira localidade inválida--\n"
	@printf "Caso 2, Rota escolhida: WWW -> LAX\n"
	@gcc -lm -o $@ $<
	@echo 2 WWW LAX | ./$@

Teste3: main.c # Teste com a segunda localidade inválida
	@printf "\n--Teste com a segunda localidade inválida--\n"
	@printf "Caso 2, Rota escolhida: MVD -> XXX\n"
	@gcc -lm -o $@ $<
	@echo 2 MVD XXX | ./$@

Teste4: main.c # Teste com ambas as localidades inválidas
	@printf "\n--Teste com ambas as localidades inválidas--\n"
	@printf "Caso 2, Rota escolhida: XXX -> ZZZ\n"
	@gcc -lm -o $@ $<
	@echo 2 XXX ZZZ | ./$@

Teste5: main.c # Teste com ambas as localidades válidas e idênticas
	@printf "\n--Teste com ambas as localidades válidas e idênticas--\n"
	@printf "Caso 2, Rota escolhida: GRU -> GRU\n"
	@gcc -lm -o $@ $<
	@echo 2 GRU GRU | ./$@

Teste6: main.c # Teste com ambas as localidades inválidas e idênticas
	@printf "\n--Teste com ambas as localidades inválidas e idênticas--\n"
	@printf "Caso 2, Rota escolhida: XXX -> XXX\n"
	@gcc -lm -o $@ $<
	@echo 2 XXX XXX | ./$@

Teste: Teste0 Teste1 Teste2 Teste3 Teste4 Teste5 Teste6
	@rm Teste*