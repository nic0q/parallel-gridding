FLAGS = -Ofast # optimize the code

all: gridding

all_2: gridding_2

gridding: gridding.cpp
	u++ $(FLAGS) -o gridding gridding.cpp

gridding_2: gridding_2.cpp
	u++ $(FLAGS) -o gridding_2 gridding_2.cpp

test_grid:
	./gridding -i hltau_test.csv -o datosgrideados -d 0.003 -N 2048 -c 3 -t 5

test_gridding:
	./gridding -i hltau_completo_uv.csv -o datosgrideados -d 0.003 -N 2048 -c 10 -t 10

test_2:
	./gridding_2 -i hltau_completo_uv.csv -o datosgrideados2 -d 0.003 -N 2048 -c 10 -t 10

test_slurm:
	sbatch gridding.slurm

clean:
	rm -f *.o *.exe gridding gridding_2 datosgrideadosr.raw datosgrideadosi.raw datosgrideados2r.raw datosgrideados2i.raw