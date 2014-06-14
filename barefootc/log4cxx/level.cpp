
#include <log4cxx/logstring.h>
#include <log4cxx/level.h>

#include "log4stdc.h"

using namespace log4cxx;
using namespace log4cxx::helpers;

static union levelbuf {
	void *p[(sizeof(Level) / sizeof(void *)) + 1];
	long  l[(sizeof(Level) / sizeof(long))   + 1];
	char  b[ sizeof(Level) ];
} levelbufs[8];

static void **levels_initialized[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

IMPLEMENT_LOG4CXX_OBJECT_WITH_CUSTOM_CLASS(Level, LevelClass)

#define GETLEVELOBJECT(idx,name,intlevel,sysloglevel) \
   	Level *lvl;							    \
	if (levels_initialized[idx] == levelbufs[idx].p) {		    \
   		lvl = (Level *) levelbufs[idx].b;			    \
		if (lvl->ref < 10000) {					    \
			lvl->ref = 20000; /* do never delete */		    \
		}							    \
		return lvl;						    \
	}								    \
	char *buf = levelbufs[idx].b;					    \
   	lvl = new(buf) Level(intlevel, LOG4CXX_STR(name), sysloglevel);	    \
	lvl->ref = 20000; /* do never delete */				    \
	levels_initialized[idx] = levelbufs[idx].p;			    \
	return lvl;

LevelPtr Level::getOff() {
	GETLEVELOBJECT(0, "OFF", Level::OFF_INT, 0)
}

LevelPtr Level::getFatal() {
	GETLEVELOBJECT(1, "FATAL", Level::FATAL_INT, 0)
}

LevelPtr Level::getError() {
	GETLEVELOBJECT(2, "ERROR", Level::ERROR_INT, 3)
}

LevelPtr Level::getWarn() {
	GETLEVELOBJECT(3, "WARN", Level::WARN_INT, 4)
}

LevelPtr Level::getInfo() {
	GETLEVELOBJECT(4, "INFO", Level::INFO_INT, 6)
}

LevelPtr Level::getDebug() {
	GETLEVELOBJECT(5, "DEBUG", Level::DEBUG_INT, 7)
}

LevelPtr Level::getTrace() {
	GETLEVELOBJECT(6, "TRACE", Level::TRACE_INT, 7)
}

LevelPtr Level::getAll() {
	GETLEVELOBJECT(7, "ALL", Level::ALL_INT, 7)
}



Level::Level(int level1,
    const LogString& name1, int syslogEquivalent1)
: level(level1), name(name1), syslogEquivalent(syslogEquivalent1)
{
}


LevelPtr Level::toLevelLS(const LogString& sArg)
{
    return toLevelLS(sArg, Level::getDebug());
}

LogString Level::toString() const {
    return name;
}


LevelPtr Level::toLevel(int val)
{
    return toLevel(val, Level::getDebug());
}

LevelPtr Level::toLevel(int val, const LevelPtr& defaultLevel)
{
    switch(val)
    {
    case ALL_INT: return getAll();
    case TRACE_LEVEL: return getTrace();
    case DEBUG_LEVEL: return getDebug();
    case INFO_LEVEL:  return getInfo();
    case WARN_LEVEL:  return getWarn();
    case ERROR_LEVEL: return getError();
    case FATAL_LEVEL: return getFatal();
    case OFF_INT: return getOff();
    default: return defaultLevel;
    }
}

LevelPtr Level::toLevel(const std::string& sArg)
{
    return toLevel(sArg, Level::getDebug());
}

LevelPtr Level::toLevel(const std::string& sArg, const LevelPtr& defaultLevel)
{
    //LOG4CXX_DECODE_CHAR(s, sArg);
    LogString s(sArg);
    return toLevelLS(s, defaultLevel);
}

void Level::toString(std::string& dst) const {
}

#if LOG4CXX_WCHAR_T_API
LevelPtr Level::toLevel(const std::wstring& sArg)
{
    return toLevel(sArg, Level::getDebug());
}

LevelPtr Level::toLevel(const std::wstring& sArg, const LevelPtr& defaultLevel)
{
    // LOG4CXX_DECODE_WCHAR(s, sArg);
    // return toLevelLS(s, defaultLevel);
    return defaultLevel;
}

void Level::toString(std::wstring& dst) const {
    // Transcoder::encode(name, dst);
}

#endif

#if LOG4CXX_UNICHAR_API
LevelPtr Level::toLevel(const std::basic_string<UniChar>& sArg)
{
    return toLevel(sArg, Level::getDebug());
}

LevelPtr Level::toLevel(const std::basic_string<UniChar>& sArg, const LevelPtr& defaultLevel)
{
    // LOG4CXX_DECODE_UNICHAR(s, sArg);
    LogString s(sArg);
    return toLevelLS(s, defaultLevel);
}

void Level::toString(std::basic_string<UniChar>& dst) const {
    Transcoder::encode(name, dst);
}

#endif

#if LOG4CXX_CFSTRING_API
LevelPtr Level::toLevel(const CFStringRef& sArg)
{
    return toLevel(sArg, Level::getDebug());
}

LevelPtr Level::toLevel(const CFStringRef& sArg, const LevelPtr& defaultLevel)
{
    LogString s;
    Transcoder::decode(sArg, s);
    return toLevelLS(s, defaultLevel);
}

void Level::toString(CFStringRef& dst) const {
    dst = Transcoder::encode(name);
}
#endif


LevelPtr Level::toLevelLS(const LogString& sArg, const LevelPtr& defaultLevel)
{
	int level = l4sc_to_level(sArg.data(), sArg.length(), 0);
	return (toLevel(level, defaultLevel));
}


bool Level::equals(const LevelPtr& level1) const
{
        return (this->level == level1->level);
}

bool Level::isGreaterOrEqual(const LevelPtr& level1) const
{
    return this->level >= level1->level;
}

