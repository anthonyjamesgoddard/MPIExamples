#include <iostream>
#include <mpi.h>
#include <vector>

namespace {
const int problem_size = 100;
}

struct Matrix {
  double *m_data;
  int m_r;
  int m_c;
  Matrix(int r, int c) {
    m_r = r;
    m_c = c;
    m_data = new double[m_r * m_c];
    for (int i = 0; i < m_r * m_c; i++) {
      m_data[i] = i;
    }
  }
};

struct Vector {
  double *m_data;
  int m_size;
  Vector(int size) {
    m_size = size;
    m_data = new double[size];
    for (int i = 0; i < size; i++) {
      m_data[i] = i;
    }
  }
};

int main(int argc, char **argv) {
  // HERE --------------------------

  // Stuff before MPI_Init is not very well
  // defined. So be careful.
  MPI_Init(NULL, NULL);
  int rank;
  int world;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &world);

  // TO HERE -----------------------
  // is pretty much boiler plate code.
  // the rank gives you the local process id
  // the world gives you the number of how many
  // process ids.

  // this is being defined on EVERY process.
  // Not good.
  // Maybe better to distribute the memory across
  // process? Maybe all the memory on one process
  // and broadcast when necessary ?
  Matrix the_matrix(problem_size, problem_size);
  Vector the_vector(problem_size);
  int chunk_size = problem_size / world;

  // the chunk, be carefull with the indices
  std::vector<double> the_result(chunk_size, 0);

  for (int i = rank * chunk_size; i < rank * chunk_size + chunk_size; i++) {
    for (int j = 0; j < problem_size; j++) {
      the_result[i - rank * chunk_size] +=
          the_matrix.m_data[i * problem_size + j] * the_vector.m_data[j];
    }
  }

  std::cout << "we are on process : " << rank << std::endl;
  for (int i = 0; i < chunk_size; i++) {
    std::cout << the_result[i] << " from rank " << rank << " comp " << i
              << std::endl;
  }
  // look at the out put, we can see that rank 0 does not have to preceed 
  // rank 1 output.
  MPI_Finalize();
}
