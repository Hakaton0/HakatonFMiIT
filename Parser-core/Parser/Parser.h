#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <chrono>
#include <algorithm>

struct data
{
	int left;
	std::vector<float> right_vec;
	std::vector<int> element_size;
	bool operator < (data s)
	{
		return left < s.left;
	}
};



class Pars
{
	std::vector<data> data_core;

	//data* data0;
	std::vector<int> element_;
	int size_data;
	double fullSize;
	std::vector<int> hren;
public:

	std::vector<data> get_data()
	{
		return data_core;
	}
	int maksklass() {
		int max = 0;
		for (int i = 0; i < data_core.size(); i++) {
			if (data_core[i].left > max)
				max = data_core[i].left;
			
		}
		return max;

	}

	void SortToGroup()
	{
		std::make_heap(data_core.begin(), data_core.end());
		//std::cout << "heap\n";

	/*	for (const auto &i : data_core)
		{
			std::cout << i.left<< ' ';
		}*/

		std::sort_heap(data_core.begin(), data_core.end());

	//	std::cout << "\nsorted:\t";
		/*for (const auto &i : data_core)
		{
			std::cout << i.left << ' ';
		}*/
		//std::cout << '\n';
		
		int element = data_core[0].left;
		int size_element = 0;

		for (int i = 0; i < data_core.size(); i++)
		{
			size_element++;
			if (data_core[i].left != element)
			{
				element = data_core[i].left;
				element_.push_back(size_element);
				size_element = 0;
			}
		
		}
		
	}
	std::vector<int> get_vec_element()
	{
		return element_;
	}
	int get_hren()
	{
		return hren.size();
	}

	int currentclass(int index)
	{
		int iter = 0;
		for (int i = 0; i < hren.size(); i++)
		{
			if (hren[i] == index)
				break;
			iter++;
		}

		return iter;
	}

	int countclass() {

	
		hren.push_back((data_core[0].left));
		int j = 0;
		for (int i = 0; i < data_core.size(); i++)
		{
			if (hren[j] != data_core[i].left)
			{
				hren.push_back(data_core[i].left);
				j++;
			}
		}
		return hren.size();
	}
	//void CountClass()
	//{
	//	for (int i = 0; i < data_core.size(); i++)
	//	{
	//		int g = data_core[i].left;
	//		for (int j = 0; j < data_core.size(); j++)
	//		{

	//			if (data_core[j].left == g)
	//			{
	//				if (countdata.size() == 0)
	//				{
	//					countdata.push_back(g);
	//					countdata[0].value++;
	//					auto iter = data_core.begin();
	//					data_core.erase(iter);
	//					break;
	//				}
	//				bool ok = false;


	//					/*if(data_coreClass.[]*/
	//					for (int y = 0; y < countdata.size(); y++)
	//					{
	//						if (ok) break;
	//						if (countdata[y].value != g)
	//						{
	//							countdata.push_back(g);
	//							countdata[countdata.size() - 1].value++;
	//							auto iter = data_core.begin();
	//							data_core.erase(iter + j);
	//							ok = true;

	//						}
	//						else
	//						{
	//							countdata[y].value++;
	//							auto iter = data_core.begin();
	//							data_core.erase(iter + j);
	//						}

	//					}
	//					ok = false;
	//			}
	//		}
	//	}
	//}
	int size_input_layer() {
		int max = 0;
		for (int i = 0; i < data_core.size(); i++) {
			if (data_core[i].right_vec.size() > max) {
				max = data_core[i].right_vec.size();
			}
		}
		std::vector<float> support;
		for (int i = 0; i < data_core.size(); i++)
		{
			support.resize(max);
			for (int j = 0; j < data_core[i].right_vec.size(); j++)
			{
				support[j] = data_core[i].right_vec[j];
			}
			data_core[i].right_vec = support;
		
		}

		return max;
	}
	void size_string(std::string name_file)
	{
		std::fstream file(name_file, std::ios::in);
		std::string line;
		size_data = 0;
		if (file.is_open())
		{
			while (getline(file, line))
			{
				size_data++;
			}
		}
		else
			std::cout<<"can't open file" << std::endl;
		file.close();
		std::cout << size_data << std::endl;
	}
	void pars(const std::string& name_file)
	{
		data_core.resize(size_data);
		std::fstream file(name_file, std::ios::in);
		std::string line;
		int i = 0;
		if (file.is_open())
		{
			getline(file, line);
			while (getline(file, line))
			{
				seporation_string(line, "\";\"", i);
				//std::cout << data_core[i].left << std::endl;
				i++;

			}
		}
		else
			std::cout << "can't open file" << std::endl;
		file.close();
		std::cout << data_core[0].left << std::endl;

	}
private:
	bool identity_string(std::string левая_строка, std::string правая_строка)
	{
		bool флаг = true;
		for (int i = 0; i < правая_строка.size(); i++)
		{
			if (левая_строка[i] != правая_строка[i])
			{
				флаг = false;
				break;
			}
		}

		return флаг;
	}


	void seporation_string(/*std::string& left_str, std::string& right_str,*/ std::string input,  std::string condition, int index)
	{
		int iterator = 0;
		bool flag = false;
		for (int i = 0; i < input.size() - condition.size(); i++)
		{
			std::string помогающая_строка;
			for (int j = 0; j < condition.size(); j++)
				помогающая_строка.push_back(input[i + j]);
		//	std::cout << помогающая_строка << std::endl;
			flag = identity_string(помогающая_строка, condition);

			помогающая_строка.clear();
			if (flag == true)
				break;
			iterator++;
		}
		if (flag == false)
			return;
		std::string str_left;
		std::string str_right;
		//int size_left_vec = m
		for (int i = 1; i < iterator; i++)
		{
			str_left.push_back(input[i]);

			//data0[i].left_vec.push_back();
		}
		data_core[index].left = std::stoi(str_left);
		//std::cout << std::stoi(str_left) << std::endl;
		//std::cout << data0[index].left << std::endl;
		for (int i = iterator + condition.size(); i < input.size() - 14; i++)
		{
			str_right.push_back(input[i]);
		}
		int size_vector_byte = input.size() - 14 - iterator - condition.size();
		if (size_vector_byte % 4 == 0)
		{
			int size_vector = size_vector_byte / 4;
			float* vector = (float*)(str_right.c_str());
			data_core[index].right_vec.resize(size_vector);
			for (int i = 0; i < size_vector; i++)
			{
				data_core[index].right_vec[i] = vector[i];
			}
		}
		else
		{
			int size_vector = size_vector_byte / 4 + 1;
			float* vector = (float*)(str_right.c_str());
			data_core[index].right_vec.resize(size_vector); 
			for (int i = 0; i < size_vector; i++)
			{
				data_core[index].right_vec[i] = vector[i];
			}
		}
		

		//const char* txt = (const char*)(data_core[index].right_vec.data());
 		//std::cout << txt << std::endl;
		

	}


};