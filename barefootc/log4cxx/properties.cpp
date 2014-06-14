
#include <log4cxx/logstring.h>
#include <log4cxx/helpers/properties.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

Properties::Properties() : properties(0) {
}

Properties::~Properties() {
}

LogString Properties::setProperty(const LogString& key, const LogString& value) {
	LogString x(value);
	return x;
}

LogString Properties::put(const LogString& key, const LogString& value)
{
	LogString x(value);
	return x;
}

LogString Properties::getProperty(const LogString& key) const {
	LogString x;
	return x;
}

LogString Properties::get(const LogString& key) const
{
	LogString x;
	return x;
}

void Properties::load(InputStreamPtr inStream) {
}

std::vector<LogString> Properties::propertyNames() const
{
        std::vector<LogString> names;
        return names;
}

