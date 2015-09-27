//#include <ns3/command-line.h>
#include <ns3/names.h>
#include <ns3/node.h>
#include <ns3/nstime.h>
#include <ns3/ptr.h>
#include <ns3/simulator.h>
#include <ns3/string.h>
#include <iostream>
#include <string>

#include "utils/topology/annotated-topology-reader.hpp"
#include "helper/ndn-app-helper.hpp"
#include "helper/ndn-fib-helper.hpp"
#include "helper/ndn-stack-helper.hpp"
#include "helper/ndn-strategy-choice-helper.hpp"
#include "../../model/ndn-l3-protocol.hpp"
#include "test-helper.hpp"

#include <memory>
#include <vector>
#include <boost/assert.hpp>
#include <cstdbool>
#include "../../model/ndn-common.hpp"
#include "../../NFD/daemon/face/face.hpp"
#include "../../NFD/daemon/face/face-counters.hpp"
#include <map>
#include "../../helper/ndn-scenario-helper.hpp"
#include "../../NFD/daemon/fw/forwarder.hpp"
#include "../../NFD/daemon/table/cs-uniform-decision-policy.hpp"
#include "../../NFD/daemon/table/cs-lcd-decision-policy.hpp"

#include <ns3/node-list.h>
#include "ns3/core-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"
#include "ns3/point-to-point-layout-module.h"
#include "ns3/ndnSIM-module.h"

namespace ns3 {

void run(int argc, char* argv[]) {
//  CommandLine cmd;
//  std::string params = "";
//  std::string strategy = "lowest-cost";
//  cmd.AddValue("params", "Number of packets to echo", params);
//  cmd.AddValue("strategy", "Forwarding strategy to use", strategy);
//  cmd.Parse(argc, argv);

// Setting default parameters for PointToPoint links and channels
	Config::SetDefault("ns3::PointToPointNetDevice::DataRate",
			StringValue("1Mbps"));
	Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
	Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("10"));

	// Read optional command-line parameters (e.g., enable visualizer with ./waf --run=<> --visualize
	CommandLine cmd;
	cmd.Parse(argc, argv);

	// Creating 3x3 topology
	PointToPointHelper p2p;
	PointToPointGridHelper grid(4, 4, p2p);
	grid.BoundingBox(100, 100, 200, 200);

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
	Ptr<Node> client = grid.GetNode(3, 3);
	NodeContainer consumerNodes;
	Ptr<Node> consumer = grid.GetNode(0, 0);

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
	producerHelper.Install(client);

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
