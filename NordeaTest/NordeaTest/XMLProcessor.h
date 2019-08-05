#pragma once
#include "stdafx.h"
#include <vector>
#include <iostream>
#include <string>
#include <filesystem>
#include "boost/property_tree/ptree.hpp"
#include "boost/property_tree/xml_parser.hpp"
#include "boost/foreach.hpp"
#include <boost/algorithm/string.hpp>

struct result
{
	std::string file;
	std::string resultfile;
};



class XMLProcessor
{
private:
	std::string						_xmlfile;// = "data001.xml";
	std::string						_xmlfile_result;
	boost::property_tree::ptree		_pt;
	boost::property_tree::ptree		_ptResult;
	std::vector<result>				_fileList;
public:
	XMLProcessor();
	~XMLProcessor();
	void init(const std::string & xmlfile, const std::string & xmlfileresult);
	void deinit();
	void processXML();
	void findfiles(const std::string& path);
	void processFile();

private:
	int callType(const boost::property_tree::ptree subtree);
	int processAdd(boost::property_tree::ptree & subtree);
	int processSub(boost::property_tree::ptree & subtree);
	int processMul(boost::property_tree::ptree & subtree);
	int processDiv(boost::property_tree::ptree & subtree);
};

void XMLProcessor::findfiles(const std::string& path)
{
	for (auto & p : std::experimental::filesystem::directory_iterator(path))
	{
		result r;
		r.file = p.path().u8string();
		std::string s = p.path().filename().u8string().substr(0,p.path().filename().u8string().find_first_of("."));
		s = s + "_result.xml";
		r.resultfile = s;
		_fileList.push_back(r);
		std::cout << s << std::endl;
	}
}

void XMLProcessor::processFile()
{
	while (_fileList.size() > 0)
	{		
		init(_fileList.back().file,_fileList.back().resultfile);
		processXML();
		deinit();
		_fileList.pop_back();
	}
}

XMLProcessor::XMLProcessor()
{
}

XMLProcessor::~XMLProcessor()
{
}

void XMLProcessor::init(const std::string & xmlfile,const std::string & xmlfileresult)
{
	_xmlfile = xmlfile;
	_xmlfile_result = xmlfileresult;
	read_xml(xmlfile, _pt);
}

void XMLProcessor::deinit()
{
	_pt.clear();
	_ptResult.clear();
	_xmlfile.clear();
	_xmlfile_result.clear();
}

int XMLProcessor::callType(const boost::property_tree::ptree subtree)
{
	int result = 0;
	BOOST_FOREACH(const auto & v, subtree.get_child(""))
	{
		if (0 == v.first.compare("addition"))
		{
			boost::property_tree::ptree p = v.second;
			result = processAdd(p);
		}
		else if (0 == v.first.compare("subtraction"))
		{
			boost::property_tree::ptree p = v.second;
			result = processSub(p);
		}
		else if (0 == v.first.compare("multiplication"))
		{
			boost::property_tree::ptree p = v.second;
			result = processMul(p);
		}
		else if (0 == v.first.compare("division"))
		{
			boost::property_tree::ptree p = v.second;
			result = processDiv(p);
		}
	}
	return result;
}

void XMLProcessor::processXML()
{
	BOOST_FOREACH(const boost::property_tree::ptree::value_type & type, _pt.get_child("expressions"))
	{
		BOOST_FOREACH(const auto & v, type.second.get_child("<xmlattr>"))
		{
			if (0 == v.first.compare("complex"))
				continue;

			if (0 == type.first.compare("addition"))
			{
				boost::property_tree::ptree p = type.second;
				/*boost::property_tree::ptree fieldid;
				boost::property_tree::ptree result;
				fieldid.put("<xmlattr>.ID", v.second.data());
				result.put("result", fieldid);
				result.put_value(processAdd(p));
				_ptResult.put("expressions.result", result);*/
				std::cout << v.first.data() << "=" << v.second.data() << "\t" << processAdd(p) << std::endl;
			}
			else if (0 == type.first.compare("subtraction"))
			{
				boost::property_tree::ptree p = type.second;

				/*boost::property_tree::ptree fieldid;
				boost::property_tree::ptree result;
				fieldid.put("<xmlattr>.ID", v.second.data());
				result.put("result", fieldid);
				result.put_value(processSub(p));
				_ptResult.put("expressions.result", result);*/
				std::cout << v.first.data() << "=" << v.second.data() << "\t" << processSub(p) << std::endl;
			}
			else if (0 == type.first.compare("multiplication"))
			{
				boost::property_tree::ptree p = type.second;
				/*boost::property_tree::ptree fieldid;
				boost::property_tree::ptree result;
				fieldid.put("<xmlattr>.ID", v.second.data());
				result.put("result", fieldid);
				result.put_value(processMul(p));
				_ptResult.put("expressions.result", result);*/
				std::cout << v.first.data() << "=" << v.second.data() << "\t" << processMul(p) << std::endl;
			}
			else if (0 == type.first.compare("division"))
			{
				boost::property_tree::ptree p = type.second;
				/*boost::property_tree::ptree fieldid;
				boost::property_tree::ptree result;
				fieldid.put("<xmlattr>.ID", v.second.data());
				result.put("result", fieldid);
				result.put_value(processDiv(p));
				_ptResult.put("expressions.result", result);*/
				//_ptResult.put("expressions.result", processDiv(p));
				std::cout << v.first.data() << "=" << v.second.data() << "\t" << processDiv(p) << std::endl;
			}
		}
	}
}


int XMLProcessor::processAdd(boost::property_tree::ptree & subtree)
{
	int add_result = 0;
	BOOST_FOREACH(const auto& addition, subtree.get_child(""))
	{
		if (0 == addition.first.compare("item"))
		{
			try
			{
				add_result += std::stoi(addition.second.data());
			}
			catch(...)
			{ 
				add_result += callType(addition.second);
			}
		}	
	}
	return add_result;
}

int XMLProcessor::processMul(boost::property_tree::ptree & subtree)
{
	int mul_result = 1;
	BOOST_FOREACH(const auto& multiplication, subtree.get_child(""))
	{
		if (0 == multiplication.first.compare("factor"))
		{
			try
			{
				mul_result *= std::stoi(multiplication.second.data());
			}
			catch(...)
			{
				mul_result *= callType(multiplication.second);
			}
		}
	}
	return mul_result;
}

int XMLProcessor::processSub(boost::property_tree::ptree & subtree)
{
	int sub_result = 0;
	BOOST_FOREACH(const auto& subtraction, subtree.get_child(""))
	{
		if (0 == subtraction.first.compare( "minuend"))
		{ 
			try
			{
				sub_result += std::stoi(subtraction.second.data());
			}
			catch (...)
			{
				sub_result += callType(subtraction.second);
			}
		}
		else if (0 == subtraction.first.compare("subtrahend"))
		{
			try
			{
				sub_result -= std::stoi(subtraction.second.data());
			}
			catch (...)
			{
				sub_result -= callType(subtraction.second);
			}
		}
	}
	return sub_result;
}

int XMLProcessor::processDiv(boost::property_tree::ptree & subtree)
{
	int div_result = 0;
	BOOST_FOREACH(const auto& division, subtree.get_child(""))
	{
		if (0 == division.first.compare("dividend"))
		{
			try
			{
				div_result += std::stoi(division.second.data());
			}
			catch (...)
			{
				div_result += callType(division.second);
			}
		}
		else if (0 == division.first.compare("divisor"))
		{
			try
			{
				div_result /= std::stoi(division.second.data());
			}
			catch (...)
			{
				div_result /= callType(division.second);
			}
		}
	}
	return div_result;
}