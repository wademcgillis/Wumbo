//
//  GM-Ini.cpp
//  Wumbo
//
//  Created by Wade McGillis on 11/22/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#include "GM-Ini.h"
#include "Wumbo-Functions.h"
namespace GM {

	bool wrotestuff;

	std::string __INI_PATH;

	class __GM_VARIABLE_TYPE
	{
	public:
		std::string stringValue;
		double realValue;
		bool isReal;
		__GM_VARIABLE_TYPE(double val)
		{
			isReal = true;
			realValue = val;
			stringValue = "";
		}
		__GM_VARIABLE_TYPE(std::string &val)
		{
			isReal = false;
			realValue = 0;
			stringValue = val;
		}
		__GM_VARIABLE_TYPE()
		{
			isReal = true;
			realValue = 0;
			stringValue = "";
		}
	};
	
	bool file_opened = false;
	
	const unsigned int INI_MAX = 128;
	
	class INIKey
	{
	public:
		INIKey(std::string _name, __GM_VARIABLE_TYPE _value)
		{
			name = _name;
			value = _value;
			isPlaintext = false;
		}
		INIKey(std::string _name)
		{
			name = _name;
			isPlaintext = true;
		}
		bool isPlaintext;	// if this is set to true then only the name variable matters. this allows the ini to keep lines of text
							// like the ones kyle puts in readme.ini
		std::string name;
		__GM_VARIABLE_TYPE value;
	};
	
	class INISection
	{
	public:
		INISection(std::string _name)
		{
			name = _name;
		}
		std::string name;
		std::vector<INIKey*> keys;
	};
	
	std::vector<INISection*> sections;
	
	INISection *INIsection_from_name(std::string name)
	{
		for(unsigned int i=0;i<sections.size();i++)
			if (strcmp(sections.at(i)->name.c_str(),name.c_str()) == 0)
				return sections.at(i);
		return NULL;
	}
	
	INIKey *INIkey_from_name(INISection *section, std::string name)
	{
		for(unsigned int kI = 0; kI < section->keys.size();kI++)
		{
			INIKey *key = section->keys.at(kI);
			if (strcmp(key->name.c_str(),name.c_str()) == 0)
				return key;
		}
		return NULL;
	}
	
	INISection *INIsection_addorget(std::string name)
	{
		for(unsigned int i=0;i<sections.size();i++)
			if (strcmp(sections.at(i)->name.c_str(),name.c_str()) == 0)
				return sections.at(i);
		//printf(("new section: %s\n",name.c_str());
		INISection *olga = new INISection(name);
		sections.push_back(olga);
		return olga;
	}
	
	INIKey *INIsection_set_keyvalue(INISection *sec, std::string name, __GM_VARIABLE_TYPE value)
	{
		INIKey *key = NULL;
		for(int i=0;(i < sec->keys.size() && key == NULL);i++)
		{
			if (strcmp(sec->keys.at(i)->name.c_str(),name.c_str()) == 0)
				key = sec->keys.at(i);
		}
		if (key != NULL)
			key->value = value;
		else
		{
			key = new INIKey(name,value);
			sec->keys.push_back(key);
		}
		return key;
	}
	
	INIKey *INIsection_set_keyvalue(std::string section, std::string name, __GM_VARIABLE_TYPE value)
	{
		return INIsection_set_keyvalue(INIsection_addorget(section),name,value);
	}
	
	std::string readline(FILE *f, bool *failed)
	{
		std::string s = "";
		while(!feof(f))
		{
			char c = fgetc(f);
			if (c == '\r')
				continue;
			else if (c == '\n')
				return s;
			else if (c >= 32)
				s += c;
		}
		if (s.length() == 0)
		{
			*failed = true;
		}
		else
		{
			//for(int i=0;i<s.length();i++)
				//printf(("%i",s.at(i));
		}
		return s;
	}
	
	bool ini_open(std::string name)
	{
		if (file_opened)
		{
			//printf(("INI FILE IS ALREADY OPEN!\n");
			return false;
		}
		file_opened = true;
		__INI_PATH = name;
		//if (__INI_PATH.substr(__INI_PATH.length()-7) == "o_o.ini")
		//	//printf(("TAPE DECK!\n");
		FILE *f = Wumbo::fopen_safe4ios(__INI_PATH.c_str(),"rb");
		if (f == NULL)
		{
			////printf(("ini_open() : Unable to open .ini at location %s.\n",__INI_PATH.c_str());
			return false;
		}
		std::string s;
		INISection *currentsection = INIsection_addorget("");
		wrotestuff = false;
		//printf(("~~~~~~~~~~~~~~~~~~~~~READING INI FILE~~~~~~~~~~~~~~~~~~~~~\n");
		while(!feof(f))
		{
			bool failed = false;
			s = readline(f, &failed);
			//printf(("read a line: %s\n",s.c_str());
			if (failed)
			{
			}
			else if (s == "")
			{
				//printf(("PURE EMPTY LINE\n");
				//printf(("\n");
				currentsection->keys.push_back(new INIKey(""));
			}
			else
			{
				// if the line matches a section header
				if (s.at(0) == '[')
					if (s.at(s.length()-1) == ']')
					{
						std::string sectionname = s.substr(1,s.length()-2);
						//printf(("BEGIN READING NEW SECTION: %s -> %s\n",s.c_str(),sectionname.c_str());
						currentsection = INIsection_addorget(sectionname);
						//printf(("[%s]\n",sectionname.c_str());
						continue;//read next line
					}
				int equalsPos = s.find("=",0);
				if (equalsPos == s.npos || equalsPos == 0) // if "=" isn't found anywhere in the string -> If it's just a line of text
				{
					//printf(("%s\n",s.c_str());
					currentsection->keys.push_back(new INIKey(s));
				}
				else
				{
					std::string key_name = s.substr(0,equalsPos);
					if (s.at(equalsPos-1) == ' ')
						key_name = s.substr(0,equalsPos-1);
					////printf(("\t%s = ",s1.c_str());
					std::string key_value_str = s.substr(equalsPos+1,s.length()-equalsPos-1);
					if (key_value_str.at(0) == ' ')
						key_value_str = s.substr(equalsPos+2,s.length()-equalsPos-1);
					__GM_VARIABLE_TYPE key_value_var;
					key_value_var.isReal = true;
					for(int i=0;i<key_value_str.length();i++)
					{
						char c = key_value_str.at(i);
						if ((c < '0' || c > '9') && c != '.')
						{
							////printf(("[ERROR PARSE NUMBER: %c]",c);
							key_value_var.isReal = false;
							key_value_var.stringValue = key_value_str;
							//printf(("%s = %s\n",key_name.c_str(),key_value_var.stringValue.c_str());
							i = key_value_str.length();
						}
					}
					if (key_value_var.isReal)
					{
						key_value_var.realValue = atof(key_value_str.c_str());
						//printf(("%s = %f\n",key_name.c_str(),key_value_var.realValue);
					}
					// search current section for the key
					// if found, replace value, if not, add.
					INIsection_set_keyvalue(currentsection,key_name,key_value_var);
				}
			}
		}
		fclose(f);
		//printf(("~~~~~~~~~~~~~~~~~~~~~DONE READING~~~~~~~~~~~~~~~~~~~~~\n");
		return true;
	}
	void ini_close()
	{
		if (file_opened)
		{
#pragma mark need to check to see if there is an ini file open in the first place
			FILE *f = Wumbo::fopen_safe4ios(__INI_PATH.c_str(),"wb");
			if (f == NULL)
			{
				////printf(("ini_close() : Unable to open .ini at location %s.\n",__INI_PATH.c_str());
				return;
			}
			//printf(("~~~~~~~~~~~~~~~~~~~~~WRITING INI FILE~~~~~~~~~~~~~~~~~~~~~\n");
			for(unsigned int i=0;i<sections.size();i++)
			{
				INISection *section = sections.at(i);
				//printf(("section #%i \"%s\"\n",i,section->name.c_str());
				if (i > 0 || (i == 0 && section->name.length() > 0))
				{
					//printf(("[%s]\n",section->name.c_str());//
					fprintf(f,"[%s]\r\n",section->name.c_str());
				}
				for(unsigned int kI = 0; kI < section->keys.size();kI++)
				{
					INIKey *key = section->keys.at(kI);
					if (key->isPlaintext)
					{
						if (key->name.length() == 0)
						{
							//printf(("\n");//
							fprintf(f,"\r\n");
						}
						else
						{
							//printf(("%s\n",key->name.c_str());//
							fprintf(f,"%s\r\n",key->name.c_str());
						}
					}
					else
					{
						if (key->value.isReal)
						{
							if (key->value.realValue-(float)((int)key->value.realValue) == 0)
							{
								//printf(("%s = %i\n",key->name.c_str(),(int)key->value.realValue);//
								fprintf(f,"%s = %i\r\n",key->name.c_str(),(int)key->value.realValue);
							}
							else
							{
								//printf(("%s = %f\n",key->name.c_str(),key->value.realValue);//
								fprintf(f,"%s = %f\r\n",key->name.c_str(),key->value.realValue);
							}
						}
						else
						{
							//printf(("%s = %s\n",key->name.c_str(),key->value.stringValue.c_str());//
							fprintf(f,"%s = %s\r\n",key->name.c_str(),key->value.stringValue.c_str());
						}
					}
				}
			}
			fclose(f);
			file_opened = false;
			// delete all sections and keys
			while(sections.size() > 0)
			{
				INISection *secPtr = sections.at(0);
				while(secPtr->keys.size() > 0)
				{
					delete secPtr->keys.at(0);
					secPtr->keys.erase(secPtr->keys.begin());
				}
				delete secPtr;
				sections.erase(sections.begin());
			}
		}
	}
	
	std::string ini_read_string(std::string section, std::string key, std::string defult)
	{
		INISection *sectionPtr = NULL;
		if ((sectionPtr = INIsection_from_name(section)) != NULL)
		{
			INIKey *keyPtr = NULL;
			if ((keyPtr = INIkey_from_name(sectionPtr, key)) != NULL)
			{
				if (!keyPtr->value.isReal)
					return keyPtr->value.stringValue;
				else
					return defult;
			}
			else
				return defult;
		}
		else
			return defult;
	}
	double ini_read_real(std::string section, std::string key, double defult)
	{
		INISection *sectionPtr = NULL;
		if ((sectionPtr = INIsection_from_name(section)) != NULL)
		{
			INIKey *keyPtr = NULL;
			if ((keyPtr = INIkey_from_name(sectionPtr, key)) != NULL)
			{
				if (keyPtr->value.isReal)
					return keyPtr->value.realValue;
				else
					return defult;
			}
			else
				return defult;
		}
		else
			return defult;
	}
	
	void ini_write_string(std::string section, std::string key, std::string value)
	{
		INIsection_set_keyvalue(INIsection_addorget(section),key,__GM_VARIABLE_TYPE(value));
		wrotestuff = true;
	}
	void ini_write_real(std::string section, std::string key, double value)
	{
		INIsection_set_keyvalue(INIsection_addorget(section),key,__GM_VARIABLE_TYPE(value));
		wrotestuff = true;
	}
	
	bool ini_key_exists(std::string section, std::string key)
	{
		INISection *sec = NULL;
		sec = INIsection_from_name(section);
		if (sec == NULL)
			return false;
		return INIkey_from_name(sec, key);
	}
	bool ini_section_exists(std::string section)
	{
		return (INIsection_from_name(section) != NULL);
	}
	
	bool ini_key_delete(std::string section, std::string key)
	{
		INISection *secPtr = INIsection_from_name(section);
		if (secPtr == NULL)
			return false;
		for(int i=0;i < secPtr->keys.size();i++)
		{
			if (strcmp(secPtr->keys.at(i)->name.c_str(),key.c_str()) == 0)
			{
				delete secPtr->keys.at(i);
				secPtr->keys.erase(secPtr->keys.begin()+i);
				return true;
			}
		}
		return false;
	}
	bool ini_section_delete(std::string section)
	{
		for(unsigned int i=0;i<sections.size();i++)
			if (strcmp(sections.at(i)->name.c_str(),section.c_str()) == 0)
			{
				INISection *secPtr = sections.at(i);
				while(secPtr->keys.size() > 0)
				{
					delete secPtr->keys.at(0);
					secPtr->keys.erase(secPtr->keys.begin());
				}
				delete secPtr;
				sections.erase(sections.begin()+i);
				return true;
			}
		return false;
	}
};