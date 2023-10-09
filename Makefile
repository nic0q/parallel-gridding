FLAGS = -Ofast # optimize the code

all: gridding

gridding: gridding.cpp
	u++ $(FLAGS) -o gridding gridding.cpp

clean:
	rm -f *.o *.exe gridding

test_grid:
	./gridding -i hltau_test.csv -o w -d 2 -N 4 -c 3 -t 5

test_gridding:
	./gridding -i hltau_completo_uv.csv -o w -d 2 -N 4 -c 3 -t 2

test_slurm:
	sbatch gridding.slurm