//#include <ns3/command-line.h>

#include <boost/assert.hpp>
#include <ns3/command-line.h>
#include <ns3/names.h>
#include <ns3/ndnSIM/helper/ndn-app-helper.hpp>
#include <ns3/ndnSIM/helper/ndn-global-routing-helper.hpp>
#include <ns3/ndnSIM/helper/ndn-stack-helper.hpp>
#include <ns3/ndnSIM/helper/ndn-strategy-choice-helper.hpp>
#include <ns3/ndnSIM/model/ndn-common.hpp>
#include <ns3/ndnSIM/model/ndn-l3-protocol.hpp>
#include <ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp>
#include <ns3/ndnSIM/utils/topology/annotated-topology-reader.hpp>
#include <ns3/node.h>
#include <ns3/node-list.h>
#include <ns3/nstime.h>
#include <ns3/ptr.h>
#include <ns3/simulator.h>
#include <ns3/string.h>
#include "ns3/core-module.h"
#include "ns3/ndnSIM-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/point-to-point-module.h"
#include <cassert>
#include <cstdbool>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../helper/ndn-app-helper.hpp"
#include "../../helper/ndn-scenario-helper.hpp"
#include "../../helper/ndn-stack-helper.hpp"
#include "../../helper/ndn-strategy-choice-helper.hpp"
#include "../../NFD/daemon/face/face.hpp"
#include "../../NFD/daemon/face/face-counters.hpp"
#include "../../NFD/daemon/fw/forwarder.hpp"
#include "../../NFD/daemon/table/cs-decision-policy.hpp"
#include "../../NFD/daemon/table/cs-lcd-decision-policy.hpp"
#include "../../NFD/daemon/table/cs-uniform-decision-policy.hpp"
#include "../../NFD/model/ndn-l3-protocol.hpp"
#include "../../utils/topology/annotated-topology-reader.hpp"
#include "helper/ndn-fib-helper.hpp"
#include "test-helper.hpp"

namespace ns3 {

void run(int argc, char* argv[]) {
//  CommandLine cmd;
//  std::string params = "";
//  std::string strategy = "lowest-cost";
//  cmd.AddValue("params", "Number of packets to echo", params);
//  cmd.AddValue("strategy", "Forwarding strategy to use", strategy);
//  cmd.Parse(argc, argv);



	// Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
	CommandLine cmd;
	cmd.Parse(argc, argv);

	// Creating fat tree topology
	AnnotatedTopologyReader topologyReader("", 25);
	topologyReader.SetFileName("src/ndnSIM/examples/topologies/topo-fattree.txt");
	topologyReader.Read();


	// Install NDN stack on all nodes
	ndn::StackHelper ndnHelper;
	ndnHelper.InstallAll();

	// Set BestRoute strategy
	ndn::StrategyChoiceHelper::InstallAll("/",
			"/localhost/nfd/strategy/best-route");

	// Installing global routing interface on all nodes
	ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
	ndnGlobalRoutingHelper.InstallAll();

	// Getting containers for the consumer/producer
	Ptr<Node> client = Names::Find<Node>("Node7");
	NodeContainer consumerNodes;
	Ptr<Node> producer = Names::Find<Node>("Node14");

//  nfd::Forwarder clientForwarder
	shared_ptr<nfd::Forwarder> clientForwarder = client->GetObject<
			ndn::L3Protocol>()->getForwarder();

	for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End();
			node++) {
		shared_ptr<nfd::Forwarder> forwarder = (*node)->GetObject<
				ndn::L3Protocol>()->getForwarder();
		auto policy = make_shared < nfd::cs::UniformDecisionPolicy
				> (nfd::cs::UniformDecisionPolicy(95));

		forwarder->setDecisionPolicy(policy);
		std::cout << forwarder->getDecisionPolicy()->getName() << ": ";
		auto pol = forwarder->getDecisionPolicy();
		auto unifPol = std::static_pointer_cast < nfd::cs::UniformDecisionPolicy
				> (pol);
		std::cout << unifPol->getAcceptRatio() << "\n";

		// Correct acceptance rate:
		assert(unifPol->getAcceptRatio() == 95);

	}

// Installing applications //
	AppHelper consumerHelper1("ns3::ndn::ConsumerCbr");
	consumerHelper1.SetPrefix("/prefix/A");
	consumerHelper1.SetAttribute("Frequency", StringValue("50"));
	consumerHelper1.SetAttribute("StopTime", StringValue("10"));
	consumerHelper1.Install(client);

// Producer
	AppHelper producerHelper("ns3::ndn::Producer");
	producerHelper.SetPrefix("/prefix");
	producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
	producerHelper.Install(producer);
//  TestHelper::setLinkError(client->GetDevice(0), 0.1);

//  Simulator::Schedule(Seconds(13.0), TestHelper::printFaceCounter, faceVector);

	Simulator::Stop(Seconds(15.0));
	Simulator::Run();
	Simulator::Destroy();

}

}
// namespace ns3

int main(int argc, char* argv[]) {
	ns3::run(argc, argv);
	return 1;
}
