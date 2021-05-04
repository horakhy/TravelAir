Run: TravelAir.c
	@gcc -lm -o $@ $<

Teste0: TravelAir.c # Teste com a localidade válidas
	@printf "\n--Teste para imprimir conexões com localidade válida--\n"
	@printf "Caso 1, IATA escolhido: BOB\n"
	@gcc -lm -o $@ $<
	@echo 1 BOB | ./$@

Teste1: TravelAir.c # Teste com a localidade inválidas
	@printf "\n--Teste para imprimir conexões com localidade inválida--\n"
	@printf "Caso 1, IATA escolhido: ASA\n"
	@gcc -lm -o $@ $<
	@echo 1 ASA | ./$@

Teste2: TravelAir.c # Teste com 2 localidades válidas
	@printf "\n--Teste com 2 localidades válidas--\n"
	@printf "Caso 2, Rota escolhida: CWB -> BOB\n"
	@gcc -lm -o $@ $<
	@echo 2 CWB BOB | ./$@

Teste3: TravelAir.c # Teste com a primeira localidade inválida
	@printf "\n--Teste com a primeira localidade inválida--\n"
	@printf "Caso 2, Rota escolhida: WWW -> LAX\n"
	@gcc -lm -o $@ $<
	@echo 2 WWW LAX | ./$@

Teste4: TravelAir.c # Teste com a segunda localidade inválida
	@printf "\n--Teste com a segunda localidade inválida--\n"
	@printf "Caso 2, Rota escolhida: MVD -> XXX\n"
	@gcc -lm -o $@ $<
	@echo 2 MVD XXX | ./$@

Teste5: TravelAir.c # Teste com ambas as localidades inválidas
	@printf "\n--Teste com ambas as localidades inválidas--\n"
	@printf "Caso 2, Rota escolhida: XXX -> ZZZ\n"
	@gcc -lm -o $@ $<
	@echo 2 XXX ZZZ | ./$@

Teste6: TravelAir.c # Teste com ambas as localidades válidas e idênticas
	@printf "\n--Teste com ambas as localidades válidas e idênticas--\n"
	@printf "Caso 2, Rota escolhida: GRU -> GRU\n"
	@gcc -lm -o $@ $<
	@echo 2 GRU GRU | ./$@

Teste7: TravelAir.c # Teste com ambas as localidades inválidas e idênticas
	@printf "\n--Teste com ambas as localidades inválidas e idênticas--\n"
	@printf "Caso 2, Rota escolhida: XXX -> XXX\n"
	@gcc -lm -o $@ $<
	@echo 2 XXX XXX | ./$@

Teste: Teste0 Teste1 Teste2 Teste3 Teste4 Teste5 Teste6 Teste7