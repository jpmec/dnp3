
#include "OutstationStackConfigPropertyTree.h"


namespace boostdnp3 { namespace opendnp3 {


::opendnp3::OutstationStackConfig& operator<<(::opendnp3::OutstationStackConfig& lhs, const boost::property_tree::ptree& rhs)
{
    string type_str = rhs.get<string>("DatabaseTemplate.Type");
    uint32_t count = rhs.get<uint32_t>("DatabaseTemplate.Count");

    if (0 == type_str.compare("AllTypes"))
    {
        lhs.dbTemplate = DatabaseTemplate::AllTypes(count);
    }
    else if (0 == type_str.compare("BinaryOnly"))
    {
        lhs.dbTemplate = DatabaseTemplate::BinaryOnly(count);
    }
    else if (0 == type_str.compare("DoubleBinaryOnly"))
    {
        lhs.dbTemplate = DatabaseTemplate::DoubleBinaryOnly(count);
    }
    else if (0 == type_str.compare("AnalogOnly"))
    {
        lhs.dbTemplate = DatabaseTemplate::AnalogOnly(count);
    }
    else if (0 == type_str.compare("CounterOnly"))
    {
        lhs.dbTemplate = DatabaseTemplate::CounterOnly(count);
    }
    else if (0 == type_str.compare("FrozenCounterOnly"))
    {
        lhs.dbTemplate = DatabaseTemplate::FrozenCounterOnly(count);
    }
    else if (0 == type_str.compare("BinaryOutputStatusOnly"))
    {
        lhs.dbTemplate = DatabaseTemplate::BinaryOutputStatusOnly(count);
    }
    else if (0 == type_str.compare("AnalogOutputStatusOnly"))
    {
        lhs.dbTemplate = DatabaseTemplate::AnalogOutputStatusOnly(count);
    }

    config.outstation.params.allowUnsolicited = pt.get<bool>("OustationConfig.OustationParams.AllowUnsolicited");

    config.link.LocalAddr = pt.get<uint32_t>("LinkConfig.LocalAddr");
    config.link.RemoteAddr = pt.get<uint32_t>("LinkConfig.RemoteAddr");

    return lhs;
}


}}
