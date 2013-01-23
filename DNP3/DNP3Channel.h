
//
// Licensed to Green Energy Corp (www.greenenergycorp.com) under one or
// more contributor license agreements. See the NOTICE file distributed
// with this work for additional information regarding copyright ownership.
// Green Energy Corp licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except in
// compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// This file was forked on 01/01/2013 by Automatak, LLC and modifications
// have been made to this file. Automatak, LLC licenses these modifications to
// you under the GNU Affero General Public License Version 3.0
// (the "Additional License"). You may not use these modifications except in
// compliance with the additional license. You may obtain a copy of the Additional
// License at
//
// http://www.gnu.org/licenses/agpl.html
//
// Contact Automatak, LLC for a commercial license to these modifications
//
#ifndef __DNP3_CHANNEL_H_
#define __DNP3_CHANNEL_H_

#include "IChannel.h"
#include "LinkLayerRouter.h"

#include <APL/Loggable.h>
#include <APL/AsyncTaskGroup.h>

#include <memory>
#include <functional>

namespace apl
{

class IPhysicalLayerAsync;
class ITimeSource;

namespace dnp
{

class IStack;

class DNP3Channel: public IChannel, private Loggable
{
	public:
		DNP3Channel(Logger* apLogger, millis_t aOpenRetry, IPhysicalLayerAsync* apPhys, ITimeSource* apTimerSource, std::function<void (DNP3Channel*)> aOnShutdown);
		~DNP3Channel();

		// Implement IChannel - these are exposed to clients

		void Shutdown();

		IMaster* AddMaster(		const std::string& arLoggerId,
	                            FilterLevel aLevel,
	                            IDataObserver* apPublisher,
	                            const MasterStackConfig& arCfg);

		// Helper functions only available inside DNP3Manager		

	private:	

		void Cleanup();

		void OnStackShutdown(IStack* apStack, LinkRoute aRoute);

		std::auto_ptr<IPhysicalLayerAsync> mpPhys;
		std::function<void (DNP3Channel*)> mOnShutdown;
		LinkLayerRouter mRouter;
		AsyncTaskGroup mGroup;
		std::set<IStack*> mStacks;
		
};

}
}

#endif