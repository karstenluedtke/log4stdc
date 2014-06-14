
#include <log4cxx/logstring.h>
#include <log4cxx/helpers/class.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/helpers/object.h>

using namespace log4cxx;
using namespace log4cxx::helpers;

#if 0
class Dummyclass: public Class {
	public:
	LogString getName() const {
		return LogString();
	}
};
#endif

Class::Class() {
}

Class::~Class()
{
}

LogString Class::toString() const
{
        return getName();
}

ObjectPtr Class::newInstance() const
{
        return 0;
}



Class::ClassMap& Class::getRegistry() {
    //Class::ClassMap *mp = new Class::ClassMap();
    Class::ClassMap *mp = 0;
    return *mp;
}

const Class& Class::forName(const LogString& className)
{
    //Class *cp = new Dummyclass();
    Class *cp = 0;
    return *cp;
}

bool Class::registerClass(const Class& newClass)
{
        return true;
}

void Class::registerClasses() {
}

