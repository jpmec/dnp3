/**
 * Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
 * more contributor license agreements. See the NOTICE file distributed
 * with this work for additional information regarding copyright ownership.
 * Green Energy Corp licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except in
 * compliance with the License.  You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * This project was forked on 01/01/2013 by Automatak, LLC and modifications
 * may have been made to this file. Automatak, LLC licenses these modifications
 * to you under the terms of the License.
 */

#include <asiodnp3/DNP3Manager.h>
#include <asiodnp3/PrintingSOEHandler.h>
#include <asiodnp3/ConsoleLogger.h>
#include <asiodnp3/DNP3ManagerTCPServerConfig.h>

#include <asiopal/UTCTimeSource.h>

#include <opendnp3/outstation/TimeTransaction.h>
#include <opendnp3/outstation/SimpleCommandHandler.h>
#include <opendnp3/outstation/Database.h>
#include <opendnp3/LogLevels.h>

#include <boostdnp3/opendnp3/outstation/OutstationStackConfigPropertyTree.h>
#include <boostdnp3/asiodnp3/DNP3ManagerTCPServerConfigPropertyTree.h>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include <string>
#include <thread>
#include <iostream>




using namespace std;
using namespace opendnp3;
using namespace openpal;
using namespace asiopal;
using namespace asiodnp3;
using namespace boostdnp3;

using namespace boost::property_tree;

using namespace boostdnp3::asiodnp3;
using namespace boostdnp3::opendnp3;


int main(int argc, char* argv[])
{
	string options_filename;

	if (1 < argc)
	{
		options_filename = argv[1];

		cout << "options filename: " << options_filename << endl;
	}
	else
	{
		cerr << "must provide an options file" << endl;
		return 1;
	}

	ptree pt;
	try
    {
        read_json(options_filename, pt);
    }
    catch (json_parser_error& error)
    {
        cerr << error.what() << endl;
        return 1;
    }

	// This is the main point of interaction with the stack
	// Allocate a single thread to the pool since this is a single outstation
	DNP3Manager manager(1);

	// send log messages to the console
	manager.AddLogSubscriber(&ConsoleLogger::Instance());

	// Get TCP server configuration
    ptree tcp_server_config_ptree = pt.get_child("DNP3Manager.DNP3Channel.DNP3ManagerTCPServerConfig");

    DNP3ManagerTCPServerConfig tcp_server_config;

    tcp_server_config << tcp_server_config_ptree;

	// Create a TCP server (listener)
	auto pChannel= manager.AddTCPServer( tcp_server_config.id.c_str()
		                               , tcp_server_config.levels
		                               , tcp_server_config.minOpenRetry
		                               , tcp_server_config.maxOpenRetry
		                               , tcp_server_config.endpoint
		                               , tcp_server_config.port
		                               , tcp_server_config.openDelayStrategy);


	// Optionally, you can bind listeners to the channel to get state change notifications
	// This listener just prints the changes to the console
	pChannel->AddStateListener([](ChannelState state)
	{
		cout << "channel state: " << ChannelStateToString(state) << endl;
	});


	ptree outstation_stack_config_ptree = pt.get_child("DNP3Manager.DNP3Channel.Outstation.OutstationStackConfig");

	// The main object for a outstation. The defaults are useable,
	// but understanding the options are important.
	OutstationStackConfig stack_config;

	stack_config << outstation_stack_config_ptree;

	// Create a new outstation with a log level, command handler, and
	// config info this	returns a thread-safe interface used for
	// updating the outstation's database.
	auto pOutstation = pChannel->AddOutstation( "outstation"
		                                      , SuccessCommandHandler::Instance()
		                                      , DefaultOutstationApplication::Instance()
		                                      , stack_config);

	// Enable the outstation and start communications
	pOutstation->Enable();

	// variables used in example loop
	string input;
	uint32_t count = 0;
	double value = 0;
	bool binary = false;
	DoubleBit dbit = DoubleBit::DETERMINED_OFF;

	while (true)
	{
		cout << "Enter one or more measurement changes then press <enter>" << endl;
		cout << "c = counter, b = binary, d = doublebit, a = analog, x = exit" << endl;
		cin >> input;

		TimeTransaction tx(pOutstation->GetDatabase(), UTCTimeSource::Instance().Now());
		for (char& c : input)
		{
			switch (c)
			{
				case('c') :
				{
					tx.Update(Counter(count), 0);
					++count;
					break;
				}
				case('a') :
				{
					tx.Update(Analog(value), 0);
					value += 1;
					break;
				}
				case('b') :
				{
					tx.Update(Binary(binary), 0);
					binary = !binary;
					break;
				}
				case('d') :
				{
					tx.Update(DoubleBitBinary(dbit), 0);
					dbit = (dbit == DoubleBit::DETERMINED_OFF) ? DoubleBit::DETERMINED_ON : DoubleBit::DETERMINED_OFF;
					break;
				}
				case('x') :

					// DNP3Manager destructor cleanups up everything automagically
					return 0;

				default:
					cout << "No action registered for: " << c << endl;
					break;
			}
		}

	}

	return 0;
}
