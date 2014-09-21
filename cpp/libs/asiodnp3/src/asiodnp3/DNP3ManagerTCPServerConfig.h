#ifndef __DNP3_MANAGER_TCPSERVER_CONFIG_H_
#define __DNP3_MANAGER_TCPSERVER_CONFIG_H_



#include <openpal/executor/TimeDuration.h>
#include <opendnp3/link/IOpenDelayStrategy.h>
#include <string>


namespace asiodnp3
{




struct DNP3ManagerTCPServerConfig
{
    DNP3ManagerTCPServerConfig(opendnp3::IOpenDelayStrategy& strategy = opendnp3::ExponentialBackoffStrategy::Instance());

    std::string id;
    uint32_t levels;
    openpal::TimeDuration minOpenRetry;
    openpal::TimeDuration maxOpenRetry;
    std::string endpoint;
    uint16_t port;
    opendnp3::IOpenDelayStrategy& openDelayStrategy;
};




}


#endif
