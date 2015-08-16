#include <jsonHelper.h>

// constructor
JsonHelper::JsonHelper(void)
{
	bVerboseMode		= false;
}

// destructor
JsonHelper::~JsonHelper(void)
{
	// nothing for now
}

//// public functions
void JsonHelper::Read(char* json)
{
	rapidjson::Document 	jsonDoc;

	// parse the json
	jsonDoc.Parse(json);

	// loop through the json object
	if ( jsonDoc.IsObject() )	{
		for (rapidjson::Value::ConstMemberIterator itr = jsonDoc.MemberBegin(); itr != jsonDoc.MemberEnd(); ++itr)	{
			// find the element name
			std::string elementName( itr->name.GetString() );

			// find element type
			if (itr->value.IsString() )	{
				// insert element name and type into overall map
				elements.insert ( std::pair<std::string,std::string>(elementName,"string") );
				
				// insert element name and value into specific type map
				std::string elementValue( itr->value.GetString() );
				stringElements.insert ( std::pair<std::string,std::string>(elementName, elementValue) );

		    	if (bVerboseMode) printf( "Parsed member '%s' with string value '%s'\n", itr->name.GetString(), itr->value.GetString() );
		    }
		    /* TO DO: Implement the test of the types */
	    }
	}
}

// returns 1 if element is found
int JsonHelper::FindElement(std::string key)
{
	int status	= 0;

	std::map<std::string,std::string>::iterator it;

	// look for the key
	it 	= elements.find(key);

	// check if anything was found
	if ( it != elements.end() )	{
		status = 1;
	}

	return (status);
}

std::string JsonHelper::GetElement(std::string key)
{
	std::string 	ret;

	std::map<std::string,std::string>::iterator it;

	// look for the key
	it 	= stringElements.find(key);

	// check if anything was found
	if ( it != elements.end() )	{
		ret = it->second;
	}

	return (ret);
}





