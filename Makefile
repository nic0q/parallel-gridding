all: gridding

gridding: gridding.cpp
	u++ -o gridding gridding.cpp

clean:
	rm -f *.o *.exe gridding datosgrideadosr.raw datosgrideadosi.raw

test_grid:
	./gridding -i hltau_test.csv -o datosgrideados -d 0.003 -N 2048 -c 3 -t 5

test_gridding:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 10 -t 10

test_slurm:
	sbatch gridding.slurm
