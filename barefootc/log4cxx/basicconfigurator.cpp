
#include <log4cxx/logstring.h>
#include <log4cxx/basicconfigurator.h>

using namespace log4cxx;

void BasicConfigurator::configure()
{
	LoggerPtr root = Logger::getRootLogger();
	/* no action needed */
#if 0
        static const LogString TTCC_CONVERSION_PATTERN(LOG4CXX_STR("%r [%t] %p %c %x - %m%n"));
	LayoutPtr layout(new PatternLayout(TTCC_CONVERSION_PATTERN));
	AppenderPtr appender(new ConsoleAppender(layout));
	root->addAppender(appender);
#endif
}

void BasicConfigurator::configure(const AppenderPtr& appender)
{
}

void BasicConfigurator::resetConfiguration()
{
}
