#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <iomanip>

// typedefs
typedef std::vector<double> DoubleVector;
typedef std::vector<DoubleVector> DoubleVector2D;

void generateCoefficientMatrix(double lowerBound, double upperBound, int printResults, int m, int n, DoubleVector2D& A)
{
	std::uniform_real_distribution<double> unif(lowerBound, upperBound);
	std::default_random_engine re;

	if (printResults)
	{
		std::cout << "Coefficient matrix:\nA =\n";
	}

	for (int i = 0; i < m; ++i)
	{
		DoubleVector Arow;
		for (int j = 0; j < n; ++j)
		{
			double aVal = unif(re);
			if (printResults)
			{
				std::cout << std::fixed;
				std::cout << std::setprecision(4);
				std::cout << aVal << " ";
			}
			Arow.push_back(aVal);
		}
		if (printResults)
		{
			std::cout << std::endl;
		}

		A.push_back(Arow);
		Arow.clear();
	}

	if (printResults)
	{
		std::cout << std::endl;
	}
}

void generateRightHandSideVector(double lowerBound, double upperBound, int printResults, int m, DoubleVector& b)
{
	std::uniform_real_distribution<double> unif(lowerBound, upperBound);
	std::default_random_engine re;

	if (printResults)
	{
		std::cout << "Right-hand side vector:\nb =\n";
	}

	for (int i = 0; i < m; ++i)
	{
		double bVal = unif(re);
		if (printResults)
		{
			std::cout << std::fixed;
			std::cout << std::setprecision(4);
			std::cout << bVal << " ";
		}
		b.push_back(bVal);
	}

	if (printResults)
	{
		std::cout << std::endl;
	}
}

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

	// generate coefficient matrix
	DoubleVector2D A;
	generateCoefficientMatrix(lowerBound, upperBound, printResults, m, n, A);

	// generate right-hand side vector
	DoubleVector b;
	generateRightHandSideVector(lowerBound, upperBound, printResults, m, b);

	// reduce the coefficient matrix to row echelon form
	// This version of the Gaussian elimination algorithm uses partial pivoting 
	// (i.e., it chooses a pivot with the largest absolute value).
	// This choice improves the numerical stability of the algorithm.

	clock_t begin = clock();
	// forward elimination
	int h = 0;
	int k = 0;
	while ((h < m) && (k < n))
	{
		// find the k-th pivot
		int iMax = h;
		double AcolMax = std::fabs(A[iMax][k]);
		for (int i = (h + 1); i < m; ++i)
		{
			if (std::fabs(A[i][k]) > AcolMax)
			{
				AcolMax = std::fabs(A[i][k]);
				iMax = i;
			}
		}

		if (A[iMax][k] == 0.0)
		{
			// no pivot in this column, pass to the next column
			k++;
		}
		else
		{
			// swap rows
			for (int j = 0; j < n; ++j)
			{
				double temp = A[h][j];
				A[h][j] = A[iMax][j];
				A[iMax][j] = temp;
			}
			double temp = b[h];
			b[h] = b[iMax];
			b[iMax] = temp;


			for (int i = (h + 1); i < m; ++i)
			{
				double f = A[i][k] / A[h][k];
				
				// fill with zeros the lower part of pivot column
				A[i][k] = 0.0;
				
				// do for all remaining elements in current row
				for (int j = (k + 1); j < n; ++j)
				{
					A[i][j] -= (A[h][j] * f);
				}
				b[i] -= (b[h] * f);
			}
			h++;
			k++;
		}
	}

	// backward substitution
	DoubleVector x;
	for (int j = 0; j < n; ++j)
	{
		x.push_back(0.0);
	}
	for (int i = (m - 1); i >= 0; --i)
	{
		double s = 0.0;
		for (int j = (i + 1); j < n; ++j)
		{
			s += A[i][j] * x[j];
		}
		x[i] = (b[i] - s) / A[i][i];
	}
	clock_t end = clock();
	double elapsedTime = (double)(end - begin) / CLOCKS_PER_SEC;

	if (printResults)
	{
		std::cout << "\nRow echelon form of coefficient matrix:\nAref =\n";
		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < n; ++j)
			{
				std::cout << std::fixed;
				std::cout << std::setprecision(4);
				std::cout << A[i][j] << " ";
			}
			std::cout << std::endl;
		}

		std::cout << "\nx =\n";
		for (int j = 0; j < n; ++j)
		{
			std::cout << std::fixed;
			std::cout << std::setprecision(4);
			std::cout << x[j] << " ";
		}
	}

	std::cout << std::fixed;
	std::cout << std::setprecision(4);
	if (printResults)
	{
		std::cout << "\n\n";
	}

	std::cout << "Elapsed time: " << elapsedTime << " sec.\n";

	return 0;
}