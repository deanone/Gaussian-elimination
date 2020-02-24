#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>

typedef std::vector<double> DoubleVector;
typedef std::vector<DoubleVector> DoubleVector2D;

int main(int argc, char** argv)
{
	if (argc != 6)
	{
		std::cout << "Argument #1: Number of rows\nArgument #2: Number of columns\n"
       << "Argument #3: Lower bound of the random variables interval\nArgument #4: Upper bound of the random variables interval\n"
       << "Argument #5: 1 for printing initial and final matrices, and 0 for not printing anything\n";
		return 1;	// error code
	}



	std::string arg1(argv[1]);
	std::string arg2(argv[2]);
   std::string arg3(argv[3]);
   std::string arg4(argv[4]);
   std::string arg5(argv[5]);

	int m = stoi(arg1);
	int n = stoi(arg2);
   double lowerBound = stod(arg3);
   double upperBound = stod(arg4);
   int printResults = stoi(arg5);

   if (upperBound < lowerBound)
   {
      std::cout << "Upper bound of the random values interval cannot be lower than the lower bound.\n";
      return 2;   // error code
   }

   if ((printResults != 0) && (printResults != 1))
   {
      std::cout << "Argument #5 should be either 0 or 1\n.";
      return 3;   // error code
   }


	DoubleVector2D A;
   std::uniform_real_distribution<double> unif(lowerBound, upperBound);
   std::default_random_engine re;

   if (printResults) std::cout << "Initial matrix:\nA =\n";
   for (int i = 0; i < m; i++)
   {
      DoubleVector Arow;
   	for (int j = 0; j < n; j++)
   	{
   		double a_random_double = unif(re);
         if (printResults)
         {
            std::cout << std::fixed;
            std::cout << std::setprecision(2);
            std::cout << a_random_double << " ";
         }
   		Arow.push_back(a_random_double);
   	}
      if (printResults) std::cout << std::endl;
   	A.push_back(Arow);
   	Arow.clear();
   }

   /* reduce to row echelon form */
   /* This version of the Gaussian elimination algorithm uses partial pivoting (i.e. it chooses a pivot with the largest absolute value). */
   /* This choice improves the numerical stability of the algorithm. */
   clock_t begin = clock();
   int h = 0;
   int k = 0;
   while ((h < m) && (k < n))
   {
   	/* Find the k-th pivot */
   	int i_max = h;
   	double A_col_max = std::fabs(A[i_max][k]);
   	for (int i = h + 1; i < m; i++)
   	{
   		if (std::fabs(A[i][k]) > A_col_max)
   		{
   			A_col_max = std::fabs(A[i][k]);
   			i_max = i;
   		}
   	}

   	if (A[i_max][k] == 0.0)
   	{
   		/* No pivot in this column, pass to next column */
   		k++;
   	}
   	else
   	{
   		/* Swap rows */
   		for (int j = 0; j < n; j++)
   		{
   		   double temp_val = A[h][j];
   			A[h][j] = A[i_max][j];
   			A[i_max][j] = temp_val;
   		}

   		for (int i = h + 1; i < m; i++)
   		{
   			double f = A[i][k] / A[h][k];
   			/* Fill with zeros the lower part of pivot column: */
   			A[i][k] = 0.0;
   			/* Do for all remaining elements in current row: */
   			for (int j = k + 1; j < n; j++)
   			   A[i][j] -= (A[h][j] * f);
   		}
   		h++;
   		k++;
   	}
   }
   clock_t end = clock();
   double elapsedTime = (double)(end - begin) / CLOCKS_PER_SEC;

   if (printResults)
   {
      std::cout << "\nRow echelon form of matrix:\nAref =\n";
      for (int i = 0; i < m; i++)
      {
         for (int j = 0; j < n; j++)
         {
            std::cout << std::fixed;
            std::cout << std::setprecision(2);
            std::cout << A[i][j] << " ";
         }
         std::cout << std::endl;
      }
   }

   std::cout << std::fixed;
   std::cout << std::setprecision(4);
   if (printResults) std::cout << "\n";
   std::cout << "Elapsed time: " << elapsedTime << " sec.\n";
	return 0;
}