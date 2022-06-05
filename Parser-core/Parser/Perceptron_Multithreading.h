#ifndef PERCEPTRON_MULTITHREADING_H
#define PERCEPTRON_MULTITHREADING_H

#include<thread>
#include<mutex>
#include<vector>
#include<string>
#include<fstream>
#include<math.h>
#include <execution>



template<typename Type>
class Perceptron_Multithreading
{

public:

	Perceptron_Multithreading(int width_0, int height_0, int width_1, int height_1);

	static Type function_activate(const Type& x);
	static Type diff_function_activate(const Type&);

	std::vector<Type> matrixMultVector(const std::vector<std::vector<Type>>& matrix_coefficient, const std::vector<Type>& input_x);

	void initializaton_data(const std::vector<Type>& data_set, const std::vector<Type>& condition, const Type& coefficient);
	std::vector<Type>function_result(const std::vector<Type>& data);
	Type function_minimisation(const std::vector<Type>& data_set, const std::vector<Type>& condition);

	void create_file(const std::string& nameFile);
	void read_file(const std::string& nameFile);


private:

	std::vector<std::vector<Type>> coef_0;
	std::vector<Type> b0;
	std::vector<std::vector<Type>> coef_1;
	std::vector<Type> b1;
	std::vector<uint32_t> graph;
};



template<typename Type>
Perceptron_Multithreading<Type>::Perceptron_Multithreading(int width_0, int height_0, int width_1, int height_1)
{
	graph.push_back(width_0);
	graph.push_back(height_0);
	graph.push_back(width_1);
	graph.push_back(height_1);
	coef_0.resize(height_0);
	for (int i = 0; i < coef_0.size(); i++)
		coef_0[i].resize(width_0);
	b0.resize(height_0);
	b1.resize(height_1);
	coef_1.resize(height_1);
	for (int i = 0; i < coef_1.size(); i++)
		coef_1[i].resize(width_1);


	for (int i = 0; i < coef_0.size(); i++)
	{
		for (int j = 0; j < coef_0[i].size(); j++)
		{
			coef_0[i][j] = (Type)(rand() % 2001 / 1000.0f) - 1.0f;
			//	std::cout <<"coef_0["<<i<<"]["<<j<<"]=\t"<< coef_0[i][j] << "\t";

		}
		//std::cout << std::endl;
	}
	for (int i = 0; i < b0.size(); i++)
	{
		b0[i] = 0.1f * (Type)(rand() % 2001 / 1000.0f) - 1.0f;
		//std::cout << "b0[" << i << "]=\t" << b0[i] << std::endl;
	}
	for (int i = 0; i < coef_1.size(); i++)
	{
		for (int j = 0; j < coef_1[i].size(); j++)
		{
			coef_1[i][j] = (Type)(rand() % 2001 / 1000.0f) - 1.0f;
			//	std::cout << "coef_1[" << i << "][" << j << "]=\t" << coef_1[i][j] << "\t";
		}
		//std::cout << std::endl;
	}
	for (int i = 0; i < b1.size(); i++)
	{
		b1[i] = (Type)(rand() % 2001 / 1000.0f) - 1.0f;
		//	std::cout << "b1[" << i << "]=\t" << b1[i] << std::endl;
	}

}



template<typename Type>
inline Type Perceptron_Multithreading<Type>::function_activate(const Type& x)
{
	using std::exp;
	return (1.0f - 1.0f / (1.0f + exp(x)));
}
template<typename Type>
inline Type Perceptron_Multithreading<Type>::diff_function_activate(const Type& x)
{
	using std::exp;
	return ((4.0f * exp(2.0f * x)) / ((exp(2.0f * x) + 1) * (exp(2.0f * x) + 1.0f)));
}



template<typename Type>
inline std::vector<Type> Perceptron_Multithreading<Type>::matrixMultVector(const std::vector<std::vector<Type>>& matrix_coefficient, const std::vector<Type>& input_x)
{
	//Нет проверки на совпадение размеров матрицы и вектора

	std::vector<Type> result(matrix_coefficient.size());

	std::transform(std::execution::par_unseq, matrix_coefficient.begin(), matrix_coefficient.end(), result.begin(), [&]
	(const std::vector<Type>& row)
		{
			size_t i = &row - &matrix_coefficient[0];
			Type mult = Type();
			for (int j = 0; j < row.size(); j++)
			{
				mult += row[j] * input_x[j];
			}
			return mult;
		});

	return result;
}

template<typename Type>
void Perceptron_Multithreading<Type>::initializaton_data(const std::vector<Type>& data_set, const std::vector<Type>& condition, const Type& coefficient)
{
	std::vector<Type> u0 = matrixMultVector(coef_0, data_set);
	std::vector<Type> y0(u0.size());

	//for (int i = 0; i < u0.size(); i++)
	//	y0[i] = activate_function(u0[i] + b0[i]);
	std::transform(std::execution::par_unseq, u0.begin(), u0.end(), b0.begin(), y0.begin(), []
	(const Type& u, const Type& b) { return function_activate(u + b); });

	std::vector<Type> u1 = matrixMultVector(coef_1, y0);
	std::vector<Type> z(u1.size());
	//for (int i = 0; i < z.size(); i++)
	//	z[i] = activate_function(u1[i] + b1[i]);
	std::transform(std::execution::par_unseq, u1.begin(), u1.end(), b1.begin(), z.begin(), []
	(const Type& u, const Type& b) { return function_activate(u + b); });

	//for (int i = 0; i < b1.size(); i++)
//{
//	b1[i] += coefficient * (condition[i] - z[i]) * (1.0f - z[i] * z[i]);
//}
	std::for_each(std::execution::par_unseq, b1.begin(), b1.end(), [&]
	(Type& b)
		{
			size_t i = &b - &b1[0];
			b += coefficient * (condition[i] - z[i]) * (1.0f - z[i]) * z[i];
		});


	//for (int i = 0; i < coef_1.size(); i++)
	//{
	//	for (int j = 0; j < coef_1[i].size(); j++)
	//	{
	//		coef_1[i][j] += coefficient * (condition[i] - z[i]) * (1.0f - z[i] * z[i]) * y0[j];
	//	}
	//}
	std::for_each(std::execution::par_unseq, coef_1.begin(), coef_1.end(), [&]
	(std::vector<Type>& row)
		{
			size_t i = &row - &coef_1[0];
			for (int j = 0; j < coef_1[i].size(); j++)
			{
				row[j] += coefficient * (condition[i] - z[i]) * (1.0f - z[i]) * z[i] * y0[j];
			}
		});


	//for (int i = 0; i < b0.size(); i++)
	//{
	//	Type support = 0.0;
	//	for (int j = 0; j < z.size(); j++)
	//	{
	//		support += (condition[j] - z[j]) * (1.0f - z[j] * z[j]) * coef_1[j][i];
	//	}
	//	b0[i] += coefficient * (1 - y0[i] * y0[i]) * support;
	//}
	std::for_each(std::execution::par_unseq, b0.begin(), b0.end(), [&]
	(Type& b)
		{
			size_t i = &b - &b0[0];
			Type support = 0.0;

			for (int j = 0; j < z.size(); j++)
			{
				support += (condition[j] - z[j]) * (1.0f - z[j]) * z[j] * coef_1[j][i];
			}
			b += coefficient * (1 - y0[i] * y0[i]) * support;
		}
	);



	//for (int i = 0; i < coef_0.size(); i++)
	//{
	//	for (int j = 0; j < coef_0[i].size(); j++)
	//	{
	//		Type support = 0.0f;
	//		for (int s = 0; s < z.size(); s++)
	//		{
	//			support += (condition[s] - z[s]) * (1.0f - z[s] * z[s]) * coef_1[s][i];
	//		}
	//		coef_0[i][j] += coefficient * (1.0f - y0[i] * y0[i]) * data_set[j] * support;
	//	}
	//}
	std::for_each(std::execution::par_unseq, coef_0.begin(), coef_0.end(), [&]
	(std::vector<Type>& row)
		{
			size_t i = &row - &coef_0[0];
			for (int j = 0; j < row.size(); j++)
			{
				Type support = 0.0f;
				for (int s = 0; s < z.size(); s++)
				{
					support += (condition[s] - z[s]) * (1.0f - z[s]) * z[s] * coef_1[s][i];
				}
				row[j] += coefficient * (1.0f - y0[i]) * y0[i] * data_set[j] * support;
			}
		});


}

template<typename Type>
std::vector<Type> Perceptron_Multithreading<Type>::function_result(const std::vector<Type>& data_set)
{
	std::vector<Type> u0 = matrixMultVector(coef_0, data_set);
	std::vector<Type> y0(u0.size());

	//for (int i = 0; i < u0.size(); i++)
	//	y0[i] = activate_function(u0[i] + b0[i]);
	std::transform(std::execution::par_unseq, u0.begin(), u0.end(), b0.begin(), y0.begin(), []
	(const Type& u, const Type& b) { return function_activate(u + b); });

	std::vector<Type> u1 = matrixMultVector(coef_1, y0);
	std::vector<Type> z(u1.size());
	//for (int i = 0; i < z.size(); i++)
	//	z[i] = activate_function(u1[i] + b1[i]);
	std::transform(std::execution::par_unseq, u1.begin(), u1.end(), b1.begin(), z.begin(), []
	(const Type& u, const Type& b) { return function_activate(u + b); });

	return z;
}


template<typename Type>
Type Perceptron_Multithreading<Type>::function_minimisation(const std::vector<Type>& data_set, const std::vector<Type>& condition)
{
	std::vector<Type> u0 = matrixMultVector(coef_0, data_set);
	std::vector<Type> y0(u0.size());

	for (int i = 0; i < u0.size(); i++)
		y0[i] = function_activate(u0[i] + b0[i]);
	std::vector<Type> u1 = matrixMultVector(coef_1, y0);
	std::vector<Type> z(u1.size());
	for (int i = 0; i < z.size(); i++)
		z[i] = function_activate(u1[i] + b1[i]);

	Type result = 0.0;

	for (int i = 0; i < condition.size(); i++)
	{
		result += (condition[i] - z[i]) * (condition[i] - z[i]);
	}
	return result;
}


template<typename Type>
inline void Perceptron_Multithreading<Type>::create_file(const std::string& nameFile)
{
	std::ofstream ofstr(nameFile, std::ios::out | std::ios::binary);

	if (!ofstr.is_open())
		throw std::runtime_error("error create file");

	std::string file_extension = "Jnana_Graph_Neural_Network";
	size_t size_head_file = file_extension.size() + 3 * sizeof(int);
	std::vector<unsigned char> headerFile(size_head_file);

	for (int i = 0; i < file_extension.size(); i++)
		headerFile[i] = file_extension[i];


	// width количество входящих каналов
	headerFile[file_extension.size() + 0] = graph[0];
	headerFile[file_extension.size() + 1] = graph[0] >> 8;
	headerFile[file_extension.size() + 2] = graph[0] >> 16;
	headerFile[file_extension.size() + 3] = graph[0] >> 24;
	//height количествой нейронов в скрытом слое
	headerFile[file_extension.size() + 4] = graph[1];
	headerFile[file_extension.size() + 5] = graph[1] >> 8;
	headerFile[file_extension.size() + 6] = graph[1] >> 16;
	headerFile[file_extension.size() + 7] = graph[1] >> 24;
	// количество выходящих каналов
	headerFile[file_extension.size() + 8] = graph[3];
	headerFile[file_extension.size() + 9] = graph[3] >> 8;
	headerFile[file_extension.size() + 10] = graph[3] >> 16;
	headerFile[file_extension.size() + 11] = graph[3] >> 24;


	ofstr.write(reinterpret_cast<char*>(headerFile.data()), size_head_file);

	// первый слой

	ofstr.write(reinterpret_cast<char*>(b0.data()), b0.size() * sizeof(Type));

	for (int i = 0; i < coef_0.size(); i++)
	{
		ofstr.write(reinterpret_cast<char*>(coef_0[i].data()), coef_0[i].size() * sizeof(Type));
	}

	// второй слой
	ofstr.write(reinterpret_cast<char*>(b1.data()), b1.size() * sizeof(Type));


	for (int i = 0; i < coef_1.size(); i++)
	{
		ofstr.write(reinterpret_cast<char*>(coef_1[i].data()), coef_1[i].size() * sizeof(Type));
	}
	ofstr.close();
	std::cout << "file create!" << std::endl;
}

template<typename Type>
inline void Perceptron_Multithreading<Type>::read_file(const std::string& nameFile)
{
	std::ifstream ifstr(nameFile, std::ios::in | std::ios::binary);

	std::string file_extension = "Jnana_Graph_Neural_Network";
	size_t size_head_file = file_extension.size();
	std::vector<unsigned char> headerFile(size_head_file);
	if (!ifstr.is_open())
		throw std::runtime_error("error open file");
	ifstr.read(reinterpret_cast<char*>(headerFile.data()), headerFile.size());
	for (int i = 0; i < headerFile.size(); i++)
	{
		if (file_extension[i] != headerFile[i])
			throw std::runtime_error("error open file");
	}

	for (int i = 0; i < graph.size(); i++)
	{
		graph[i] = 0;
	}

	unsigned char width_0[sizeof(int)];
	ifstr.read(reinterpret_cast<char*>(width_0), sizeof(int));

	graph[0] = width_0[0] + (width_0[1] << 8) + (width_0[2] << 16) + (width_0[3] << 24);

	unsigned char height_0[sizeof(int)];
	ifstr.read(reinterpret_cast<char*>(height_0), sizeof(int));
	graph[1] = height_0[0] + (height_0[1] << 8) + (height_0[2] << 16) + (height_0[3] << 24);

	graph[2] = graph[1];

	unsigned char width_1[sizeof(int)];
	ifstr.read(reinterpret_cast<char*>(width_1), sizeof(int));
	graph[3] = width_1[0] + (width_1[1] << 8) + (width_1[2] << 16) + (width_1[3] << 24);

	b0.resize(graph[1]);
	b1.resize(graph[3]);

	coef_0.resize(graph[1]);
	for (int i = 0; i < coef_0.size(); i++)
		coef_0[i].resize(graph[0]);

	coef_1.resize(graph[3]);
	for (int i = 0; i < coef_1.size(); i++)
		coef_1[i].resize(graph[2]);

	// первый слой

	ifstr.read(reinterpret_cast<char*>(b0.data()), b0.size() * sizeof(Type));

	for (int i = 0; i < coef_0.size(); i++)
	{

		ifstr.read(reinterpret_cast<char*>(coef_0[i].data()), coef_0[i].size() * sizeof(Type));
	}
	// второй слой

	ifstr.read(reinterpret_cast<char*>(b1.data()), b1.size() * sizeof(Type));

	for (int i = 0; i < coef_1.size(); i++)
	{

		ifstr.read(reinterpret_cast<char*>(coef_1[i].data()), coef_1[i].size() * sizeof(Type));
	}
	ifstr.close();
	std::cout << "read file!" << std::endl;
}

#endif //!PERCEPTRON_MULTITHREADING_H