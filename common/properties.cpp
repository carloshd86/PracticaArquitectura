#include "properties.h"
#include <fstream>
#include <algorithm>
#include <assert.h>


Properties *  Properties::mInstance = nullptr;

// *************************************************
//
// *************************************************

Properties::Properties(const P_Language lang) :
	mLang (lang) {}

// *************************************************
//
// *************************************************

Properties::~Properties()
{
}

// *************************************************
//
// *************************************************

Properties * Properties::Instance(const char * file, const P_Language lang)
{
	if (mInstance && lang == mInstance->mLang) return mInstance;

	delete mInstance;
	mInstance = nullptr;

	assert(file);
	const char * languageSuffix = GetLanguageSuffix(lang);
	assert(languageSuffix);

	mInstance = new Properties(lang);

	std::string propertiesSrc = "../data/";
	propertiesSrc.append(file);
	propertiesSrc.append("_");
	propertiesSrc.append(languageSuffix);
	propertiesSrc.append(".properties");

	std::ifstream propertiesFile(propertiesSrc, std::ios::binary);
	assert(propertiesFile.is_open());

	std::string line;
	std::string key;
	std::string value;
	while (!propertiesFile.eof())
	{
		std::getline(propertiesFile, line);
		int index = line.find('=', 0);
		key = line.substr(0, index - 1);
		value = line.substr(index);
		value.erase(std::remove(value.begin(), value.end(), '\r'), value.end());
		value.erase(std::remove(value.begin(), value.end(), '\n'), value.end());
		mInstance->mTexts.insert(make_pair(key, value));
	}

	return mInstance;
}

// *************************************************
//
// *************************************************

void Properties::RemoveInstance()
{
 delete	mInstance;
 mInstance = nullptr;
}

// *************************************************
//
// *************************************************

const std::string& Properties::GetProperty(const std::string& key) 
{
	if (mTexts.end() != mTexts.find(key)) return mTexts[key];
	else                                  return key;
}

// *************************************************
//
// *************************************************

const char * Properties::GetLanguageSuffix(const P_Language lang)
{
	switch (lang)
	{
		case Spanish: return "es";
		case English: return "en";
	}

	return nullptr;
}