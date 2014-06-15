#ifndef _LOG4STDC_LOG4CXX_REPOSITORY_H
#define _LOG4STDC_LOG4CXX_REPOSITORY_H

#if defined(_MSC_VER)
#pragma warning ( push )
#pragma warning ( disable: 4231 4251 4275 4786 )
#endif

#include <log4cxx/spi/loggerrepository.h>
#include <log4cxx/helpers/class.h>

namespace log4cxx
{
    namespace spi
        {
            class LOG4CXX_EXPORT Log4stdcRepository :
		public virtual spi::LoggerRepository,
                public virtual helpers::ObjectImpl
            {
            public:
            	DECLARE_ABSTRACT_LOG4CXX_OBJECT(Log4stdcRepository)

                Log4stdcRepository();
                virtual ~Log4stdcRepository();

                virtual void addRef() const;
                virtual void releaseRef() const;
                virtual bool instanceof(const log4cxx::helpers::Class& clazz) const;
                virtual const void * cast(const log4cxx::helpers::Class& clazz) const;

                /**
                Add a {@link spi::HierarchyEventListener HierarchyEventListener}
                            event to the repository.
                */
                virtual void addHierarchyEventListener(const HierarchyEventListenerPtr&
                                    listener);
                /**
                Is the repository disabled for a given level? The answer depends
                on the repository threshold and the <code>level</code>
                parameter. See also #setThreshold method.  */
                virtual bool isDisabled(int level) const;

                /**
                Set the repository-wide threshold. All logging requests below the
                threshold are immediately dropped. By default, the threshold is
                set to <code>Level::getAll()</code> which has the lowest possible rank.  */
                virtual void setThreshold(const LevelPtr& level);

                /**
                Another form of {@link #setThreshold(const LevelPtr&)
                            setThreshold} accepting a string
                parameter instead of a <code>Level</code>. */
                virtual void setThreshold(const LogString& val);

                virtual void emitNoAppenderWarning(const LoggerPtr& logger);

                /**
                Get the repository-wide threshold. See {@link
                #setThreshold(const LevelPtr&) setThreshold}
                            for an explanation. */
                virtual const LevelPtr& getThreshold() const;

                virtual LoggerPtr getLogger(const LogString& name);

                virtual LoggerPtr getLogger(const LogString& name,
                     const spi::LoggerFactoryPtr& factory);

                virtual LoggerPtr getRootLogger() const;

                virtual LoggerPtr exists(const LogString& name);

                virtual void shutdown();

                virtual LoggerList getCurrentLoggers() const;

                virtual void fireAddAppenderEvent(const LoggerPtr& logger,
                                    const AppenderPtr& appender);

                virtual void resetConfiguration();

                virtual bool isConfigured();
                virtual void setConfigured(bool configured);

	    private:
		LevelPtr current_threshold;
		bool isconfigured;
		
            }; // class Log4stdcRepository
        }  // namespace spi
} // namespace log4cxx

#endif //_LOG4STDC_LOG4CXX_REPOSITORY_H
