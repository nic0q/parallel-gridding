FLAGS = -Ofast # optimize the code

shared: gridding_shared_matrix

private: gridding_private_matrix

gridding_shared_matrix: gridding_shared.cpp
	u++ $(FLAGS) -o gridding_shared gridding_shared.cpp

gridding_private_matrix: gridding_private.cpp
	u++ $(FLAGS) -o gridding_private gridding_private.cpp

test_shared:
	./gridding_shared -i hltau_completo_uv.csv -o datosgrideados_shared -d 0.003 -N 2048 -c 10 -t 10

test_private:
	./gridding_private -i hltau_completo_uv.csv -o datosgrideados_private -d 0.003 -N 2048 -c 10 -t 10

test_slurm:
	sbatch gridding.slurm

clean:
	rm -f *.o *.exe gridding_private gridding_shared