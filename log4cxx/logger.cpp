
#include <stddef.h>
#include <string.h>

#include "log4stdc.h"
#include "logobjects.h"

#include <log4cxx/logstring.h>
#include <log4cxx/logger.h>
#include <log4cxx/spi/loggingevent.h>
#include <log4cxx/helpers/transcoder.h>
// #include <log4cxx/logmanager.h>
// #include <log4cxx/spi/loggerfactory.h>
// #include <log4cxx/appender.h>
// #include <log4cxx/level.h>
// #include <log4cxx/helpers/loglog.h>
// #include <log4cxx/spi/loggerrepository.h>
// #include <log4cxx/helpers/stringhelper.h>
// #include <log4cxx/helpers/synchronized.h>
// #include <log4cxx/helpers/appenderattachableimpl.h>
// #include <log4cxx/helpers/exception.h>
// #if !defined(LOG4CXX)
// #define LOG4CXX 1
// #endif
// #include <log4cxx/private/log4cxx_private.h>
// #include <log4cxx/helpers/aprinitializer.h>

#include "repository.h"

#include "barefootc/utf8.h"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace log4cxx::spi;

static union reposbuf {
	void *p[(sizeof(Log4stdcRepository) / sizeof(void *)) + 1];
	long  l[(sizeof(Log4stdcRepository) / sizeof(long))   + 1];
	char  b[ sizeof(Log4stdcRepository) ];
} defaultreposbuf;
static void **defaultrepos_initialized = 0;

#define l4scbycxxlogger(lg) \
	(l4sc_logger_ptr_t) ((char *)lg - offsetof(struct l4sc_logger, cxxbuf))

IMPLEMENT_LOG4CXX_OBJECT(Logger)

Logger::Logger(Pool& p, const LogString& name1)
: pool(&p), name(), level(), parent(), resourceBundle(),
repository(), aai(), mutex(p)
{
    // synchronized sync(mutex);
    name = name1;
    additive = true;
}

Logger::~Logger()
{
}

void Logger::addRef() const {
    // Our loggers are placed inside the log4stdc logger structure,
    // we do neither allocate nor delete them separately.
    // ObjectImpl::addRef();
}

void Logger::releaseRef() const {
    // Our loggers are placed inside the log4stdc logger structure,
    // we do neither allocate nor delete them separately.
    // ObjectImpl::releaseRef();
}

void Logger::addAppender(const AppenderPtr& newAppender)
{
}


void Logger::callAppenders(const spi::LoggingEventPtr& event, Pool& p) const
{
}

void Logger::closeNestedAppenders()
{
}


void Logger::forcedLog(const LevelPtr& level1, const std::string& message,
        const LocationInfo& location) const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	l4sc_log(logger, level1->toInt(), message.data(), message.length(),
		 location.getFileName(), location.getLineNumber(),
		 location.getMethodName().c_str());
}


void Logger::forcedLog(const LevelPtr& level1, const std::string& message) const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	l4sc_log(logger, level1->toInt(), message.data(), message.length(),
		 "unknown file", 0, "unknown method");
}

void Logger::forcedLogLS(const LevelPtr& level1, const LogString& message,
        const LocationInfo& location) const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	l4sc_log(logger, level1->toInt(), message.data(), message.length(),
		 location.getFileName(), location.getLineNumber(),
		 location.getMethodName().c_str());
}


bool Logger::getAdditivity() const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	return logger->additivity;
}

AppenderList Logger::getAllAppenders() const
{
        return AppenderList();
}

AppenderPtr Logger::getAppender(const LogString& name1) const
{
        return 0;
}

const LevelPtr& Logger::getEffectiveLevel() const
{
        return this->level;
}

LoggerRepositoryPtr Logger::getLoggerRepository() const
{
	Log4stdcRepository *repos;
	if (defaultrepos_initialized == defaultreposbuf.p) {
   		repos = (Log4stdcRepository *) defaultreposbuf.b;
		return repos;
	}
   	repos = new(defaultreposbuf.b) Log4stdcRepository();
	for (int i=0; i < 100; i++) {
		repos->addRef();
	}
	defaultrepos_initialized = defaultreposbuf.p;
        return repos;
}

ResourceBundlePtr Logger::getResourceBundle() const
{
        return 0;
}


LogString Logger::getResourceBundleString(const LogString& key) const 
{
	return LogString();
}


LoggerPtr Logger::getParent() const
{
        return parent;
}

LevelPtr Logger::getLevel() const
{
        return level;
}


bool Logger::isAttached(const AppenderPtr& appender) const
{
	return false;
}

bool Logger::isTraceEnabled() const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	return IS_LEVEL_ENABLED(TRACE_LEVEL, logger->level);
}

bool Logger::isDebugEnabled() const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	return IS_LEVEL_ENABLED(DEBUG_LEVEL, logger->level);
}

bool Logger::isEnabledFor(const LevelPtr& level1) const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	return IS_LEVEL_ENABLED(level1->toInt(), logger->level);
}


bool Logger::isInfoEnabled() const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	return IS_LEVEL_ENABLED(INFO_LEVEL, logger->level);
}

bool Logger::isErrorEnabled() const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	return IS_LEVEL_ENABLED(ERROR_LEVEL, logger->level);
}

bool Logger::isWarnEnabled() const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	return IS_LEVEL_ENABLED(WARN_LEVEL, logger->level);
}

bool Logger::isFatalEnabled() const
{
	l4sc_logger_ptr_t logger = l4scbycxxlogger(this);

	return IS_LEVEL_ENABLED(FATAL_LEVEL, logger->level);
}



void Logger::l7dlog(const LevelPtr& level1, const LogString& key,
                    const LocationInfo& location, const std::vector<LogString>& params) const
{
}

void Logger::l7dlog(const LevelPtr& level1, const std::string& key,
                    const LocationInfo& location) const {
}

void Logger::l7dlog(const LevelPtr& level1, const std::string& key,
                    const LocationInfo& location, const std::string& val1) const {
}

void Logger::l7dlog(const LevelPtr& level1, const std::string& key,
                    const LocationInfo& location, 
                    const std::string& val1, const std::string& val2) const {
}

void Logger::l7dlog(const LevelPtr& level1, const std::string& key,
                    const LocationInfo& location, 
                    const std::string& val1, const std::string& val2, const std::string& val3) const {
}



void Logger::removeAllAppenders()
{
}

void Logger::removeAppender(const AppenderPtr& appender)
{
}

void Logger::removeAppender(const LogString& name1)
{
}

void Logger::setAdditivity(bool additive1)
{
}

void Logger::setHierarchy(spi::LoggerRepository * repository1)
{
}

void Logger::setLevel(const LevelPtr& level1)
{
}


#define RETURN_LOG4CXX_LOGGER(logger) \
	if (logger && (sizeof(Logger) < sizeof(logger->cxxbuf))) {	      \
		Pool pool;						      \
		Logger *cxxlog = (Logger *) &logger->cxxbuf;		      \
		if (logger->cxxbuf.p[0] && (cxxlog->ref > 0)) {		      \
			LOGINFO(("%s: existing Logger %p/%p, ref %ld",	      \
			    __FUNCTION__, logger, cxxlog, (long)cxxlog->ref));\
			return cxxlog;					      \
		}							      \
		LogString nameLS(logger->name);				      \
		cxxlog = new(&logger->cxxbuf) Logger(pool,nameLS);	      \
		cxxlog->ref = 20000; /* never release */		      \
		LOGINFO(("%s: Logger %p %d < cxxbuf %d",__FUNCTION__, cxxlog, \
			(int)sizeof(Logger), (int)sizeof(logger->cxxbuf)));   \
		LOGINFO(("%s: %p %p %p %p %p %p", __FUNCTION__,		      \
			logger->cxxbuf.p[0], logger->cxxbuf.p[1],	      \
			logger->cxxbuf.p[2], logger->cxxbuf.p[3],	      \
			logger->cxxbuf.p[4], logger->cxxbuf.p[5]));	      \
		return cxxlog;						      \
	}								      \
	return 0

LoggerPtr Logger::getLogger(const std::string& name)
{
	l4sc_logger_ptr_t logger;

	logger = l4sc_get_logger(name.data(), name.length());

	RETURN_LOG4CXX_LOGGER(logger);
}

LoggerPtr Logger::getLogger(const char* const name)
{
	l4sc_logger_ptr_t logger;

	logger = l4sc_get_logger(name, strlen(name));

	RETURN_LOG4CXX_LOGGER(logger);
}

LoggerPtr Logger::getRootLogger()
{
	l4sc_logger_ptr_t logger;

	logger = l4sc_get_root_logger();

	RETURN_LOG4CXX_LOGGER(logger);
}

LoggerPtr Logger::getLoggerLS(const LogString& name,
        const spi::LoggerFactoryPtr& factory)
{
	l4sc_logger_ptr_t logger;

	logger = l4sc_get_logger(name.data(), name.length());

	RETURN_LOG4CXX_LOGGER(logger);
}

void Logger::getName(std::string& rv) const {
}



void Logger::trace(const std::string& msg, const log4cxx::spi::LocationInfo& location) const { 
  if (isTraceEnabled()) { 
    forcedLog(log4cxx::Level::getTrace(), msg, location); 
  } 
} 


void Logger::trace(const std::string& msg) const { 
  if (isTraceEnabled()) { 
    forcedLog(log4cxx::Level::getTrace(), msg); 
  } 
} 

void Logger::debug(const std::string& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isDebugEnabled()) {
    forcedLog(log4cxx::Level::getDebug(), msg, location);
  }
}

void Logger::debug(const std::string& msg) const {
  if (isDebugEnabled()) {
    forcedLog(log4cxx::Level::getDebug(), msg);
  }
}


void Logger::error(const std::string& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isErrorEnabled()) {
     forcedLog(log4cxx::Level::getError(), msg, location);
  }
}


void Logger::error(const std::string& msg) const {
  if (isErrorEnabled()) {
     forcedLog(log4cxx::Level::getError(), msg);
  }
}

void Logger::fatal(const std::string& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isFatalEnabled()) {
    forcedLog(log4cxx::Level::getFatal(), msg, location);
  }
}

void Logger::fatal(const std::string& msg) const {
  if (isFatalEnabled()) {
    forcedLog(log4cxx::Level::getFatal(), msg);
  }
}

void Logger::info(const std::string& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isInfoEnabled()) {
    forcedLog(log4cxx::Level::getInfo(), msg, location);
  }
}

void Logger::info(const std::string& msg) const {
  if (isInfoEnabled()) {
    forcedLog(log4cxx::Level::getInfo(), msg);
  }
}

void Logger::log(const LevelPtr& level1, const std::string& message,
    const log4cxx::spi::LocationInfo& location) const {
    if (isEnabledFor(level1)) {
      forcedLog(level1, message, location);
    }
}

void Logger::log(const LevelPtr& level1, const std::string& message) const {
    if (isEnabledFor(level1)) {
      forcedLog(level1, message);
    }
}

void Logger::logLS(const LevelPtr& level1, const LogString& message,
    const log4cxx::spi::LocationInfo& location) const {
    if (isEnabledFor(level1)) {
      forcedLogLS(level1, message, location);
    }
}

void Logger::warn(const std::string& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isWarnEnabled()) {
    forcedLog(log4cxx::Level::getWarn(), msg, location);
  }
}

void Logger::warn(const std::string& msg) const {
  if (isWarnEnabled()) {
    forcedLog(log4cxx::Level::getWarn(), msg);
  }
}

LoggerPtr Logger::getLoggerLS(const LogString& name) {
	return 0;
}




#if LOG4CXX_WCHAR_T_API
void Logger::forcedLog(const LevelPtr& level1, const std::wstring& message,
        const LocationInfo& location) const
{
	const size_t wlen = message.length();
	std::string s;
	char ubuf[8];

	for (size_t i=0; i < wlen; i++) {
		char *cp = ubuf;
		unsigned codept = (unsigned) message[i];
		BFC_PUT_UTF8(cp, ubuf+sizeof(ubuf), codept);
		s.append(ubuf, cp - ubuf);
	}

	this->forcedLog(level1, s, location);
}

void Logger::forcedLog(const LevelPtr& level1,const std::wstring& message) const
{
	const size_t wlen = message.length();
	std::string s;
	char ubuf[8];

	for (size_t i=0; i < wlen; i++) {
		char *cp = ubuf;
		unsigned codept = (unsigned) message[i];
		BFC_PUT_UTF8(cp, ubuf+sizeof(ubuf), codept);
		s.append(ubuf, cp - ubuf);
	}

	this->forcedLog(level1, s);
}

void Logger::getName(std::wstring& rv) const {
}

LoggerPtr Logger::getLogger(const std::wstring& name)
{
        return 0;
}

LoggerPtr Logger::getLogger(const wchar_t* const name)
{
        return 0;
}

void Logger::trace(const std::wstring& msg, const log4cxx::spi::LocationInfo& location) const { 
  if (isTraceEnabled()) { 
    forcedLog(log4cxx::Level::getTrace(), msg, location); 
  } 
} 


void Logger::trace(const std::wstring& msg) const { 
  if (isTraceEnabled()) { 
    forcedLog(log4cxx::Level::getTrace(), msg); 
  } 
} 

void Logger::debug(const std::wstring& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isDebugEnabled()) {
    forcedLog(log4cxx::Level::getDebug(), msg, location);
  }
}

void Logger::debug(const std::wstring& msg) const {
  if (isDebugEnabled()) {
    forcedLog(log4cxx::Level::getDebug(), msg);
  }
}

void Logger::error(const std::wstring& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isErrorEnabled()) {
     forcedLog(log4cxx::Level::getError(), msg, location);
  }
}

void Logger::error(const std::wstring& msg) const {
  if (isErrorEnabled()) {
     forcedLog(log4cxx::Level::getError(), msg);
  }
}

void Logger::fatal(const std::wstring& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isFatalEnabled()) {
    forcedLog(log4cxx::Level::getFatal(), msg, location);
  }
}

void Logger::fatal(const std::wstring& msg) const {
  if (isFatalEnabled()) {
    forcedLog(log4cxx::Level::getFatal(), msg);
  }
}

void Logger::info(const std::wstring& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isInfoEnabled()) {
    forcedLog(log4cxx::Level::getInfo(), msg, location);
  }
}

void Logger::info(const std::wstring& msg) const {
  if (isInfoEnabled()) {
    forcedLog(log4cxx::Level::getInfo(), msg);
  }
}

void Logger::log(const LevelPtr& level1, const std::wstring& message,
    const log4cxx::spi::LocationInfo& location) const {
    if (isEnabledFor(level1)) {
      forcedLog(level1, message, location);
    }
}

void Logger::log(const LevelPtr& level1, const std::wstring& message) const {
    if (isEnabledFor(level1)) {
      forcedLog(level1, message);
    }
}

void Logger::warn(const std::wstring& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isWarnEnabled()) {
    forcedLog(log4cxx::Level::getWarn(), msg, location);
  }
}

void Logger::warn(const std::wstring& msg) const {
  if (isWarnEnabled()) {
    forcedLog(log4cxx::Level::getWarn(), msg);
  }
}

#endif


#if LOG4CXX_UNICHAR_API || LOG4CXX_CFSTRING_API
void Logger::forcedLog(const LevelPtr& level1, const std::basic_string<UniChar>& message,
        const LocationInfo& location) const
{
}

void Logger::forcedLog(const LevelPtr& level1, const std::basic_string<UniChar>& message) const
{
}
#endif

#if LOG4CXX_UNICHAR_API
void Logger::getName(std::basic_string<UniChar>& rv) const {
}

LoggerPtr Logger::getLogger(const std::basic_string<UniChar>& name)
{
        return 0;
}

void Logger::trace(const std::basic_string<UniChar>& msg, const log4cxx::spi::LocationInfo& location) const { 
  if (isTraceEnabled()) { 
    forcedLog(log4cxx::Level::getTrace(), msg, location); 
  } 
} 


void Logger::trace(const std::basic_string<UniChar>& msg) const { 
  if (isTraceEnabled()) { 
    forcedLog(log4cxx::Level::getTrace(), msg); 
  } 
} 

void Logger::debug(const std::basic_string<UniChar>& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isDebugEnabled()) {
    forcedLog(log4cxx::Level::getDebug(), msg, location);
  }
}

void Logger::debug(const std::basic_string<UniChar>& msg) const {
  if (isDebugEnabled()) {
    forcedLog(log4cxx::Level::getDebug(), msg);
  }
}

void Logger::error(const std::basic_string<UniChar>& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isErrorEnabled()) {
     forcedLog(log4cxx::Level::getError(), msg, location);
  }
}

void Logger::error(const std::basic_string<UniChar>& msg) const {
  if (isErrorEnabled()) {
     forcedLog(log4cxx::Level::getError(), msg);
  }
}

void Logger::fatal(const std::basic_string<UniChar>& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isFatalEnabled()) {
    forcedLog(log4cxx::Level::getFatal(), msg, location);
  }
}

void Logger::fatal(const std::basic_string<UniChar>& msg) const {
  if (isFatalEnabled()) {
    forcedLog(log4cxx::Level::getFatal(), msg);
  }
}

void Logger::info(const std::basic_string<UniChar>& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isInfoEnabled()) {
    forcedLog(log4cxx::Level::getInfo(), msg, location);
  }
}

void Logger::info(const std::basic_string<UniChar>& msg) const {
  if (isInfoEnabled()) {
    forcedLog(log4cxx::Level::getInfo(), msg);
  }
}

void Logger::log(const LevelPtr& level1, const std::basic_string<UniChar>& message,
    const log4cxx::spi::LocationInfo& location) const {
    if (isEnabledFor(level1)) {
      forcedLog(level1, message, location);
    }
}

void Logger::log(const LevelPtr& level1, const std::basic_string<UniChar>& message) const {
    if (isEnabledFor(level1)) {
      forcedLog(level1, message);
    }
}

void Logger::warn(const std::basic_string<UniChar>& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isWarnEnabled()) {
    forcedLog(log4cxx::Level::getWarn(), msg, location);
  }
}

void Logger::warn(const std::basic_string<UniChar>& msg) const {
  if (isWarnEnabled()) {
    forcedLog(log4cxx::Level::getWarn(), msg);
  }
}

#endif


#if LOG4CXX_CFSTRING_API
void Logger::forcedLog(const LevelPtr& level1, const CFStringRef& message,
        const LocationInfo& location) const
{
}

void Logger::forcedLog(const LevelPtr& level1, const CFStringRef& message) const
{
}

void Logger::getName(CFStringRef& rv) const {
}

LoggerPtr Logger::getLogger(const CFStringRef& name)
{
        return 0;
}

void Logger::trace(const CFStringRef& msg, const log4cxx::spi::LocationInfo& location) const { 
  if (isTraceEnabled()) { 
    forcedLog(log4cxx::Level::getTrace(), msg, location); 
  } 
} 


void Logger::trace(const CFStringRef& msg) const { 
  if (isTraceEnabled()) { 
    forcedLog(log4cxx::Level::getTrace(), msg); 
  } 
} 

void Logger::debug(const CFStringRef& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isDebugEnabled()) {
    forcedLog(log4cxx::Level::getDebug(), msg, location);
  }
}

void Logger::debug(const CFStringRef& msg) const {
  if (isDebugEnabled()) {
    forcedLog(log4cxx::Level::getDebug(), msg);
  }
}

void Logger::error(const CFStringRef& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isErrorEnabled()) {
     forcedLog(log4cxx::Level::getError(), msg, location);
  }
}

void Logger::error(const CFStringRef& msg) const {
  if (isErrorEnabled()) {
     forcedLog(log4cxx::Level::getError(), msg);
  }
}

void Logger::fatal(const CFStringRef& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isFatalEnabled()) {
    forcedLog(log4cxx::Level::getFatal(), msg, location);
  }
}

void Logger::fatal(const CFStringRef& msg) const {
  if (isFatalEnabled()) {
    forcedLog(log4cxx::Level::getFatal(), msg);
  }
}

void Logger::info(const CFStringRef& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isInfoEnabled()) {
    forcedLog(log4cxx::Level::getInfo(), msg, location);
  }
}

void Logger::info(const CFStringRef& msg) const {
  if (isInfoEnabled()) {
    forcedLog(log4cxx::Level::getInfo(), msg);
  }
}

void Logger::log(const LevelPtr& level1, const CFStringRef& message,
    const log4cxx::spi::LocationInfo& location) const {
    if (isEnabledFor(level1)) {
      forcedLog(level1, message, location);
    }
}

void Logger::log(const LevelPtr& level1, const CFStringRef& message) const {
    if (isEnabledFor(level1)) {
      forcedLog(level1, message);
    }
}

void Logger::warn(const CFStringRef& msg, const log4cxx::spi::LocationInfo& location) const {
  if (isWarnEnabled()) {
    forcedLog(log4cxx::Level::getWarn(), msg, location);
  }
}

void Logger::warn(const CFStringRef& msg) const {
  if (isWarnEnabled()) {
    forcedLog(log4cxx::Level::getWarn(), msg);
  }
}

#endif

