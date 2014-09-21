#import "DNP3ManagerTCPServerConfigPropertyTree.h"

#import <string>
#import <opendnp3/LogLevels.h>
#import <openpal/executor/TimeDuration.h>


using namespace openpal;
using namespace std;


namespace boostdnp3 { namespace asiodnp3 {


/// Stream property tree into DatabaseTemplate
::asiodnp3::DNP3ManagerTCPServerConfig& operator<<(
    ::asiodnp3::DNP3ManagerTCPServerConfig& lhs,
    const boost::property_tree::ptree& rhs)
{
    lhs.id = rhs.get("Id", "server");


    string levels_str = rhs.get("Levels", "ALL");

    if (0 == levels_str.compare("ALL"))
    {
        lhs.levels = opendnp3::levels::ALL;
    }
    else if (0 == levels_str.compare("NORMAL"))
    {
        lhs.levels = opendnp3::levels::NORMAL;
    }
    else if (0 == levels_str.compare("ALL_COMMS"))
    {
        lhs.levels = opendnp3::levels::ALL_COMMS;
    }
    else if (0 == levels_str.compare("NOTHING"))
    {
        lhs.levels = opendnp3::levels::NOTHING;
    }


    lhs.minOpenRetry = TimeDuration::Seconds(rhs.get("MinOpenRetry", 5));


    lhs.maxOpenRetry = TimeDuration::Seconds(rhs.get("MaxOpenRetry", 5));


    lhs.endpoint = rhs.get("Endpoint", "0.0.0.0");


    lhs.port = rhs.get("Port", 20000);


    string open_delay_strategy_str = rhs.get("OpenDelayStrategy", "ExponentialBackoffStrategy");

    if (0 == open_delay_strategy_str.compare("ExponentialBackoffStrategy"))
    {
        lhs.openDelayStrategy = opendnp3::ExponentialBackoffStrategy::Instance();
    }
    else
    {
        lhs.openDelayStrategy = opendnp3::ExponentialBackoffStrategy::Instance();
    }


    return lhs;
}

}}