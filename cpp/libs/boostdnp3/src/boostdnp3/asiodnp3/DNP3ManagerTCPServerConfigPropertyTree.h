#ifndef __DNP3_MANAGER_TCP_SERVER_CONFIG_PROPERTY_TREE_H_
#define __DNP3_MANAGER_TCP_SERVER_CONFIG_PROPERTY_TREE_H_


#include <asiodnp3/DNP3ManagerTCPServerConfig.h>
#include <boost/property_tree/ptree.hpp>


namespace boostdnp3 { namespace asiodnp3 {


/// Stream property tree into DatabaseTemplate
::asiodnp3::DNP3ManagerTCPServerConfig& operator<<(::asiodnp3::DNP3ManagerTCPServerConfig&, const boost::property_tree::ptree&);



}}


#endif
