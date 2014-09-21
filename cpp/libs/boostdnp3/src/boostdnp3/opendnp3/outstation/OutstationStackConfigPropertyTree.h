#ifndef __OUTSTATION_STACK_CONFIG_PROPERTY_TREE_H_
#define __OUTSTATION_STACK_CONFIG_PROPERTY_TREE_H_


#include <opendnp3/outstation/OutstationStackConfig.h>
#include <boost/property_tree/ptree.hpp>


namespace boostdnp3 { namespace opendnp3 {


::opendnp3::OutstationStackConfig& operator<<(::opendnp3::OutstationStackConfig&, const boost::property_tree::ptree&);


}}


#endif//__OUTSTATION_STACK_CONFIG_PROPERTY_TREE_H_
