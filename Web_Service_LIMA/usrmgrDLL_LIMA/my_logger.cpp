#include "StdAfx.h"
#include "my_logger.h"


namespace attrs   = boost::log::attributes;
namespace expr    = boost::log::expressions;
namespace logging = boost::log;
namespace keywords = boost::log::keywords;
namespace sinks = boost::log::sinks;

//Defines a global logger initialization routine
BOOST_LOG_GLOBAL_LOGGER_INIT(my_logger, logger_t)
{
    logger_t lg;
    logging::add_common_attributes();

    logging::add_file_log(
			//keywords::file_name = "Logs_dll/log_dll_%N.log",
			keywords::file_name = "Logs_dll/LOG_%Y-%m-%d %H-%M-%S.log",
			keywords::rotation_size = 20 * 1024 * 1024,
			//keywords::time_based_rotation = sinks::file::rotation_at_time_point(0, 0, 0),
			keywords::auto_flush = true, 
            keywords::format = (
                    expr::stream << expr::format_date_time<     boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
                    << " [" << expr::attr<     boost::log::trivial::severity_level >("Severity") << "]: "
                    << expr::smessage
            )
    );

    logging::add_console_log(
            std::cout,
            boost::log::keywords::format = (
                    expr::stream << expr::format_date_time<     boost::posix_time::ptime >("TimeStamp", "%Y-%m-%d %H:%M:%S.%f")
                    << " [" << expr::attr<     boost::log::trivial::severity_level >("Severity") << "]: "
                    << expr::smessage
            )
    );

    logging::core::get()->set_filter
    (
        logging::trivial::severity >= logging::trivial::trace
    );


    return lg;
}