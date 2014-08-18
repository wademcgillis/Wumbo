//
//  GM.cpp
//  Wumbo
//
//  Created by Wade McGillis on 11/14/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#include "GM.h"
#include <cmath>
namespace GM
{
	Wumbo::Program *__GM_WUMBO_PROGRAM;
	Wumbo::Renderer *__GM_WUMBO_RENDERER;
	Wumbo::InputManager *__GM_WUMBO_INPUTMANAGER;
	
	void gm_wumbo_init(Wumbo::Program *prog)
	{
		//printf("occurrences of o in the sentence: The moose likes potatoes: %i\n",GM::string_count("The moose likes potatoes","o"));
		__GM_WUMBO_PROGRAM = prog;
		__GM_WUMBO_INPUTMANAGER = prog->getInputManager();
		__GM_WUMBO_RENDERER = prog->getRenderer();
	}

	float point_distance(float x1, float y1, float x2, float y2)
	{
		return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	}
	
	unsigned int string_token_count(std::string arg, std::string token)
	{
		std::string str = "";
		str.append(arg);
		unsigned int length = str.length();
		unsigned int count = 0;
		for(int i=0;i<length;i++)
			if (str.at(i) == token.at(0))
				count++;
		return count;
	}
	
	std::string string_tokenize(std::string arg, std::string token, unsigned int number)
	{
		std::string str = "";
		str.append(arg);
		std::string retval = "";
		unsigned int length = str.length();
		unsigned int count = 0;
		for(int i=0;(i < length && count <= number);i++)
		{
			if (str.at(i) == token.at(0))
				count++;
			else if (count == number)
				retval += str.at(i);
		}
		return retval;
	}

	
	std::string string_char_at(std::string str, unsigned int index)
	{
		if (index > str.length())
			return " ";
		if (index == 0)
			index = 1;
		std::string r;
		r.push_back(str.at(index-1));
		return r;
	}
	std::string string_delete(std::string str, unsigned int index, unsigned int count)
	{
		//int before = clock();
		unsigned int originalLength = str.length();
		while(str.length() > originalLength-count)
			str.erase(str.begin()+(index-1));
		//printf("time taken for string_delete: %i\n",clock()-before);
		return str;
	}
	std::string string_lower(std::string str)
	{
		for(int i=0;i<str.length();i++)
		{
			char c = str[i];
			if (c >= 'A' && c <= 'Z')
				c = c-'A'+'a';
			str[i] = c;
		}
		//printf("string_lower = %s\n",str.c_str());
		return str;
	}
	std::string string_upper(std::string str)
	{
		for(int i=0;i<str.length();i++)
		{
			char c = str[i];
			if (c >= 'a' && c <= 'z')
				c = c-'a'+'A';
			str[i] = c;
		}
		return str;
	}
	unsigned int string_count(std::string substr, std::string str)
	{
		unsigned int occurrences = 0;
		std::string::size_type offset = 0;
		while((offset = str.find(substr,offset)) != std::string::npos)
		{
			//printf("COUNT: %i\n\tFound %s at %lu\n",occurrences,substr.c_str(),offset);
			offset += substr.length();
			occurrences++;
		}
		return occurrences;
	}
};