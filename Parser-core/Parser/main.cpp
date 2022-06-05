#include"Parser.h"
#include"Perceptron_Multithreading.h"
#include <vector>

using namespace std::chrono;



class countdata
{
public:
	int value;
	int countPovtor;

	countdata(int value = -1)
	{
		this->value = value;
	}
};


int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	
	Pars pars;


	//std::fstream file("dataset.csv", std::ios::in);
	std::string line = "dataset.csv";
	//std::vector<std::string> left_vec(4000000);
	//std::vector<std::string> right_vec(4000000);

	pars.size_string(line);
	pars.pars(line);
	//pars.size_input_layer();
	pars.SortToGroup();
	//std::cout << pars.countclass() << std::endl;
	std::vector<int> element = pars.get_vec_element();
	for (int i = 0; i < element.size(); i++)
	{
		std::cout << element[i] << std::endl;
	}
	Perceptron_Multithreading<float> nn(pars.size_input_layer(), 10, 10, pars.countclass());
	int era = 20;
	float coef = 0;
	int index;
	for (int i = 0; i < era; i++)
	{
		for (int j = 0; j < element.size(); j++)
		{

			std::vector<float> condition(pars.get_hren());

			condition[j] = 1.0f;


			for (int k = 0; k < element[j]; k++)
			{
				nn.initializaton_data(pars.get_data()[j * element[j] + k].right_vec, condition, coef);
				std::cout << j * element[j] + k << std::endl;
			}
		}
		std::cout << "era:\t" << i + 1 << std::endl;
	}


	//std::cout << pars.maksklass() << std::endl;
	//pars.seporation_string();

	/*if (file.is_open())
	{
		int i = 0;
		while (getline(file, line))
		{
			pars.seporation_string( line, "\";\"", i);
			//std::cout << left_vec[i] << std::endl;
			//std::cout << right_vec[i] << std::endl;
			i++;
			if (i >= 100000)
				break;
		}
	}
	else
		std::cout << "Could not open the file\n";
	file.close();
	/*t2 = clock_f();
	std::cout << duration_cast<seconds>(t2 - t1);*/
	system("pause");
	return 0;
	

}










//struct Data
//{
//	char* name;
//	size_t size_name;
//	int number;
//};
//
//const int dataCount = 4;
//Data data[dataCount];
//
//bool identity_string(std::string левая_строка, std::string правая_строка)
//{
//	bool флаг = true;
//	for (int i = 0; i < правая_строка.size(); i++)
//	{
//		if (левая_строка[i] != правая_строка[i])
//		{
//			флаг = false;
//			break;
//		}
//	}
//
//	return флаг;
//}
//
//void seporation_string(std::string& left_str, std::string& right_str, std::string input, std::string condition)
//{
//	int iterator = 0;
//	bool flag = false;
//	for (int i = 0; i < input.size() - condition.size(); i++)
//	{
//		std::string помогающая_строка;
//		for (int j = 1; j < condition.size(); j++)
//			помогающая_строка.push_back(input[i + j]);
//
//		flag = identity_string(помогающая_строка, condition);
//
//		помогающая_строка.clear();
//		if (flag == true)
//			break;
//		iterator++;
//	}
//	if (flag == false)
//		return;
//	for (int i = 1; i < iterator; i++)
//	{
//		left_str.push_back(input[i]);
//	}
//
//	for (int i = iterator + condition.size(); i < input.size() - 14; i++)
//	{
//		right_str.push_back(input[i]);
//	}
//
//}
//
//int main()
//{
//	SetConsoleCP(1251);
//	SetConsoleOutputCP(1251);
//	std::string fname = "dataset.csv";
//
//
//	std::string line;
//
//	using namespace std::chrono;
//	auto clock_f = std::chrono::steady_clock::now;
//	auto t1 = clock_f(), t2 = t1;
//	std::fstream file(fname, std::ios::in);
//	std::vector<std::string> left_vec(4000000);
//	std::vector<std::string> right_vec(4000000);
//
//	if (file.is_open())
//	{
//		int i = 0;
//		while (getline(file, line))
//		{
//			seporation_string(left_vec[i], right_vec[i], line, "\";\"");
//			//std::cout << left_vec[i] << std::endl;
//			//std::cout << right_vec[i] << std::endl;
//			i++;
//			if (i >= 4000000)
//				break;
//
//		}
//	}
//	else
//		std::cout << "Could not open the file\n";
//	file.close();
//	t2 = clock_f();
//	std::cout << duration_cast<seconds>(t2 - t1);
//
//	system("pause");
//
//	return 0;
//}