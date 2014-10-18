
#include <stddef.h>
#include <stdlib.h>

#include "log4stdc.h"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LOG4CXX_LOG4CXX_H
#include <log4cxx/logger.h>
#include "log4cxx/logmanager.h"
#include "log4cxx/basicconfigurator.h"
#include "log4cxx/propertyconfigurator.h"
#include "log4cxx/xml/domconfigurator.h"
#include "log4cxx/helpers/exception.h"
#endif

int main (int argc, char *argv[])
{
	l4sc_logger_ptr_t logger;

#ifdef HAVE_LIBEXPAT
	l4sc_configure_from_xml_file("log4j.xml");
#endif
	l4sc_get_root_logger();

	logger = l4sc_get_logger("pal", 3);
	l4sc_log(logger, INFO_LEVEL, "a single line from main", 23,
				__FILE__, __LINE__, __FUNCTION__);

	l4sc_configure_from_property_file("log4j.properties");

	logger = l4sc_get_logger("pal", 3);
	l4sc_log(logger, INFO_LEVEL, "another line from main", 22,
				__FILE__, __LINE__, __FUNCTION__);

#ifdef HAVE_LOG4CXX_LOG4CXX_H
	try {
		log4cxx::BasicConfigurator::configure();

		log4cxx::xml::DOMConfigurator::configure("log4j.xml");
		log4cxx::LoggerPtr log = log4cxx::Logger::getLogger("pal.api");
		LOG4CXX_INFO(log, "a third line from main");

		log4cxx::PropertyConfigurator::configure("log4j.properties");
		log4cxx::LoggerPtr lo2 = log4cxx::Logger::getLogger("pal.api");
		LOG4CXX_WARN(lo2, "a forth line from main");
	} catch(log4cxx::helpers::Exception&) {
		return (1);
	}
#endif

	L4SC_DEBUG(logger, "logger(\"pal\") = %p, %d, %d", logger, 1, 2);

	return (0);
}
