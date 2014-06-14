
#include <log4cxx/logstring.h>
#include <log4cxx/propertyconfigurator.h>

#include "log4stdc.h"

using namespace log4cxx;
using namespace log4cxx::spi;
using namespace log4cxx::helpers;


IMPLEMENT_LOG4CXX_OBJECT(PropertyConfigurator)

PropertyConfigurator::PropertyConfigurator()
: registry(0), loggerFactory(0)
{
}

PropertyConfigurator::~PropertyConfigurator() {
}

void PropertyConfigurator::addRef() const {
   ObjectImpl::addRef();
}

void PropertyConfigurator::releaseRef() const {
   ObjectImpl::releaseRef();
}

void PropertyConfigurator::doConfigure(const File& configFileName,
        spi::LoggerRepositoryPtr& hierarchy)
{
	l4sc_configure_from_property_file(configFileName.getPath().c_str());
}

void PropertyConfigurator::configure(const File& configFilename)
{
	l4sc_configure_from_property_file(configFilename.getPath().c_str());
}


#if 0 // APR_HAS_THREADS
void PropertyConfigurator::configureAndWatch(const File& configFilename)
{
	l4sc_configure_from_property_file(configFilename.getPath().c_str());
}



void PropertyConfigurator::configureAndWatch(
        const File& configFilename, long delay)
{
	l4sc_configure_from_property_file(configFilename.getPath().c_str());
}
#endif


