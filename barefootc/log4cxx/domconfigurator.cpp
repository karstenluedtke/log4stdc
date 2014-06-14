
#include <log4cxx/logstring.h>
#include <log4cxx/xml/domconfigurator.h>

#include "log4stdc.h"

using namespace log4cxx;
using namespace log4cxx::spi;
using namespace log4cxx::xml;

IMPLEMENT_LOG4CXX_OBJECT(DOMConfigurator)

DOMConfigurator::DOMConfigurator()
   : props(), repository() {
}

void DOMConfigurator::addRef() const {
   ObjectImpl::addRef();
}

void DOMConfigurator::releaseRef() const {
   ObjectImpl::releaseRef();
}

#if 0
void DOMConfigurator::setParameter(log4cxx::helpers::Pool& p,
                                log4cxx::helpers::CharsetDecoderPtr& utf8Decoder,
                                apr_xml_elem* elem, 
                                PropertySetter& propSetter)
{
        LogString name(subst(getAttribute(utf8Decoder, elem, NAME_ATTR)));
        LogString value(subst(getAttribute(utf8Decoder, elem, VALUE_ATTR)));
        value = subst(value);
        propSetter.setProperty(name, value, p);
}
#endif

void DOMConfigurator::doConfigure(const File& filename, spi::LoggerRepositoryPtr& repository1)
{
	l4sc_configure_from_xml_file(filename.getPath().c_str());
}

void DOMConfigurator::configure(const std::string& filename)
{
	l4sc_configure_from_xml_file(filename.c_str());
}

#if LOG4CXX_WCHAR_T_API
void DOMConfigurator::configure(const std::wstring& filename)
{
	File file(filename);
	l4sc_configure_from_xml_file(file.getPath().c_str());
}
#endif

#if LOG4CXX_UNICHAR_API
void DOMConfigurator::configure(const std::basic_string<UniChar>& filename)
{
	File file(filename);
	l4sc_configure_from_xml_file(file.getPath().c_str());
}
#endif

#if LOG4CXX_CFSTRING_API
void DOMConfigurator::configure(const CFStringRef& filename)
{
	File file(filename);
	l4sc_configure_from_xml_file(file.getPath().c_str());
}
#endif


void DOMConfigurator::configureAndWatch(const std::string& filename)
{
	l4sc_configure_from_xml_file(filename.c_str());
}

#if LOG4CXX_WCHAR_T_API
void DOMConfigurator::configureAndWatch(const std::wstring& filename)
{
	File file(filename);
	l4sc_configure_from_xml_file(file.getPath().c_str());
}
#endif

#if LOG4CXX_UNICHAR_API
void DOMConfigurator::configureAndWatch(const std::basic_string<UniChar>& filename)
{
	File file(filename);
	l4sc_configure_from_xml_file(file.getPath().c_str());
}
#endif

#if LOG4CXX_CFSTRING_API
void DOMConfigurator::configureAndWatch(const CFStringRef& filename)
{
	File file(filename);
	l4sc_configure_from_xml_file(file.getPath().c_str());
}
#endif

void DOMConfigurator::configureAndWatch(const std::string& filename, long delay)
{
	l4sc_configure_from_xml_file(filename.c_str());
}

#if LOG4CXX_WCHAR_T_API
void DOMConfigurator::configureAndWatch(const std::wstring& filename, long delay)
{
	File file(filename);
	l4sc_configure_from_xml_file(file.getPath().c_str());
}
#endif

#if LOG4CXX_UNICHAR_API
void DOMConfigurator::configureAndWatch(const std::basic_string<UniChar>& filename, long delay)
{
	File file(filename);
	l4sc_configure_from_xml_file(file.getPath().c_str());
}
#endif

#if LOG4CXX_CFSTRING_API
void DOMConfigurator::configureAndWatch(const CFStringRef& filename, long delay)
{
	File file(filename);
	l4sc_configure_from_xml_file(file.getPath().c_str());
}
#endif

