//
//  GM-Ini.h
//  Wumbo
//
//  Created by Wade McGillis on 11/22/13.
//  Copyright (c) 2013 Wade McGillis. All rights reserved.
//

#ifndef __GM_Ini_h__
#define __GM_Ini_h__

#include "GM.h"

#include <map>
#include <string>

namespace GM
{
	extern bool ini_open(std::string name);
	extern void ini_close();
	
	extern std::string ini_read_string(std::string section, std::string key, std::string defult);
	extern double ini_read_real(std::string section, std::string key, double defult);
	
	extern void ini_write_string(std::string section, std::string key, std::string value);
	extern void ini_write_real(std::string section, std::string key, double value);
	
	extern bool ini_key_exists(std::string section, std::string key);
	extern bool ini_section_exists(std::string section);
	
	extern bool ini_key_delete(std::string section, std::string key);
	extern bool ini_section_delete(std::string section);
};

#endif /* defined(__Wumbo__GM_Ini__) */
