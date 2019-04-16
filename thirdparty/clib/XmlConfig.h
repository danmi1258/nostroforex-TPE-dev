#pragma once
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
namespace aaa
{
	class XmlConfig
	{
	public:
		using ConfigType = boost::property_tree::ptree;
		XmlConfig(const std::string & fileName) : _config(fileName)
		{
			boost::property_tree::read_xml(fileName, _config);
		}
		const ConfigType & getConfig() const
		{
			return _config;
		}
		//Throwing
		template <typename T>
		T getValue(const std::string & key) const
		{
			return _config.get<T>(key);
		}
		//Not throwing
		template <typename T>
		void getValue(const std::string & key, T & value, T defaultValue = T()) const
		{
			value = _config.get(key, defaultValue);
		}
	private:
		ConfigType _config;
	};
}
