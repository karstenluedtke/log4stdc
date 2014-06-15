
#if defined(_MSC_VER)
#pragma warning ( push )
#pragma warning ( disable: 4231 4251 4275 4786 )
#endif

#include "log4stdc.h"

#include <log4cxx/appender.h>
#include <log4cxx/spi/loggerrepository.h>
#include <log4cxx/spi/loggerfactory.h>
#include <log4cxx/level.h>
#include <log4cxx/spi/hierarchyeventlistener.h>

#include "repository.h"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace log4cxx::spi;


IMPLEMENT_LOG4CXX_OBJECT(Log4stdcRepository)

Log4stdcRepository::Log4stdcRepository():
	current_threshold(Level::getTrace()), isconfigured(false)
{
}

Log4stdcRepository::~Log4stdcRepository()
{
}

void
Log4stdcRepository::addRef() const
{
	// Our repository lives in some static buffer and is never deleted.
	// return ObjectImpl::addRef();
}

void
Log4stdcRepository::releaseRef() const
{
	// Our repository lives in some static buffer and is never deleted.
	// return ObjectImpl::releaseRef();
}

bool
Log4stdcRepository::instanceof(const Class& clazz) const
{
	// return ObjectImpl::instanceof(clazz);
	return (false);
}

const void *
Log4stdcRepository::cast(const Class& clazz) const
{
	// return ObjectImpl::cast(clazz);
	return 0;
}

void
Log4stdcRepository::addHierarchyEventListener(
			const HierarchyEventListenerPtr& listener)
{
}

bool
Log4stdcRepository::isDisabled(int level) const
{
	return (false);
}

void
Log4stdcRepository::setThreshold(const LevelPtr& level)
{
}

void
Log4stdcRepository::setThreshold(const LogString& val)
{
}

void
Log4stdcRepository::emitNoAppenderWarning(const LoggerPtr& logger)
{
}

const LevelPtr&
Log4stdcRepository::getThreshold() const
{
	return current_threshold;
}


LoggerPtr
Log4stdcRepository::getLogger(const LogString& name)
{
	return Logger::getLogger(name);
}

LoggerPtr
Log4stdcRepository::getLogger(const LogString& name,
                     const spi::LoggerFactoryPtr& factory)
{
	return Logger::getLogger(name);
}

LoggerPtr
Log4stdcRepository::getRootLogger() const
{
	return Logger::getRootLogger();
}

LoggerPtr
Log4stdcRepository::exists(const LogString& name)
{
	// return Logger::exists(name);
	return Logger::getLogger(name);
}

void
Log4stdcRepository::shutdown()
{
}

LoggerList
Log4stdcRepository::getCurrentLoggers() const
{
	LoggerList loggers;
	return loggers;
}

void
Log4stdcRepository::fireAddAppenderEvent(const LoggerPtr& logger,
                                    const AppenderPtr& appender)
{
}

void
Log4stdcRepository::resetConfiguration()
{
}

bool
Log4stdcRepository::isConfigured()
{
	return (l4sc_is_configured());
}

void
Log4stdcRepository::setConfigured(bool configured)
{
	isconfigured = configured;
	l4sc_set_configured(configured);
}

