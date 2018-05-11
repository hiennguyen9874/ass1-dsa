/*
 * =========================================================================================
 * Name        : eventLib.cpp
 * Author      : Duc Dung Nguyen, Nguyen Hoang Minh
 * Email       : nddung@hcmut.edu.vn
 * Copyright   : Faculty of Computer Science and Engineering - Bach Khoa University
 * Description : library for Assignment 1 - Data structures and Algorithms - Fall 2017
 *               This library contains functions used for event management
 * =========================================================================================
 */

#include "eventLib.h"


 /// NOTE: each event will be separated by spaces, or endline character
void loadEvents(char* fName, L1List<ninjaEvent_t> &eList) {
	//TODO    
	fstream file(fName);
	if (!file)
	{
		cout << "The file is not found!";
		return;
	}
	string str;
	while (file >> str)
	{
		if (str[str.length() - 1] == ';')
			str.pop_back();
		ninjaEvent_t ninja(str);
		eList.push_back(ninja);
	}
}
