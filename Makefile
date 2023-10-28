FLAGS = -Ofast# optimize the code

all: shared private

shared: gridding_shared

private: gridding_private

gridding_shared: gridding_shared.cc
	u++ -multi $(FLAGS) -o gridding_shared gridding_shared.cc

gridding_private: gridding_private.cc
	u++ -multi $(FLAGS) -o gridding_private gridding_private.cc

gridding_cluster: processors_cluster.cc
	u++ -multi -debug $(FLAGS) -o gridding_cluster processors_cluster.cpp

test_shared:
	./gridding_shared -i hltau_completo_uv.csv -o datosgrideados_shared -d 0.003 -N 2048 -c 10 -t 10

test_private:
	./gridding_private -i hltau_completo_uv.csv -o datosgrideados_private -d 0.003 -N 2048 -c 10 -t 10

test_slurm:
	sbatch gridding.slurm

clean:
	rm -f *.o gridding_private gridding_shared