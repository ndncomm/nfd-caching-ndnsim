//#include <ns3/command-line.h>

#include <boost/assert.hpp>
#include <boost/lexical_cast.hpp>
#include <ns3/command-line.h>
#include <ns3/config.h>
#include <ns3/names.h>
#include <ns3/ndnSIM/helper/ndn-app-helper.hpp>
#include <ns3/ndnSIM/helper/ndn-global-routing-helper.hpp>
#include <ns3/ndnSIM/model/ndn-common.hpp>
#include <ns3/ndnSIM/model/ndn-l3-protocol.hpp>
#include <ns3/ndnSIM/NFD/daemon/fw/forwarder.hpp>
#include <ns3/ndnSIM/utils/tracers/l2-rate-tracer.hpp>
#include <ns3/ndnSIM/utils/tracers/ndn-app-delay-tracer.hpp>
#include <ns3/ndnSIM/utils/tracers/ndn-cs-tracer.hpp>
#include <ns3/node.h>
#include <ns3/node-container.h>
#include <ns3/node-list.h>
#include <ns3/nstime.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/ptr.h>
#include <ns3/simulator.h>
#include <ns3/string.h>
#include <cassert>
#include <cstdbool>
#include <iostream>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "../../helper/ndn-app-helper.hpp"
#include "../../helper/ndn-global-routing-helper.hpp"
#include "../../helper/ndn-scenario-helper.hpp"
#include "../../helper/ndn-stack-helper.hpp"
#include "../../NFD/daemon/face/face.hpp"
#include "../../NFD/daemon/face/face-counters.hpp"
#include "../../NFD/daemon/fw/forwarder.hpp"
#include "../../NFD/daemon/table/cs-consumer-closest-decision-policy.hpp"
#include "../../NFD/daemon/table/cs-decision-policy.hpp"
#include "../../NFD/daemon/table/cs-lcd-decision-policy.hpp"
#include "../../NFD/daemon/table/cs-producer-closest-decision-policy.hpp"
#include "../../NFD/daemon/table/cs-uniform-decision-policy.hpp"
#include "../../utils/tracers/l2-rate-tracer.hpp"
#include "../../utils/tracers/ndn-app-delay-tracer.hpp"
#include "../../utils/tracers/ndn-cs-tracer.hpp"
#include "create-topo2.hpp"
#include "helper/ndn-fib-helper.hpp"
#include "helper/ndn-strategy-choice-helper.hpp"
#include "test-helper.hpp"
#include "utils/topology/annotated-topology-reader.hpp"

namespace ns3 {

void run(int argc, char* argv[])
{

  // Read optional command-line parameters (e.g., ./waf --run=<> --visualize)
  CommandLine cmd;
  std::string params;
  std::string policyStr;
  cmd.AddValue("policy", "Policy", policyStr);
  cmd.AddValue("params", "Parameters", params);
  cmd.Parse(argc, argv);
  std::cout << "Policy: " << policyStr << "\n";
  std::cout << "Parameters: " << params << "\n";

  int ACCEPT_RATIO = std::stoi(params);
  std::string SIM_NAME = "tree_unif" + std::to_string(ACCEPT_RATIO);

  shared_ptr<nfd::cs::DecisionPolicy> decPolicy;
  if (policyStr == "lcd") {
    decPolicy = make_shared<nfd::cs::LcdDecisionPolicy>(nfd::cs::LcdDecisionPolicy());
  }
  else if (policyStr == "prodclose") {
    decPolicy = make_shared<nfd::cs::ProducerClosestDecisionPolicy>(
        nfd::cs::ProducerClosestDecisionPolicy());
  }
  else if (policyStr == "consclose") {
    decPolicy = make_shared<nfd::cs::ConsumerClosestDecisionPolicy>(
        nfd::cs::ConsumerClosestDecisionPolicy());
  }
  else {
    policyStr = "unif";
    decPolicy = make_shared<nfd::cs::UniformDecisionPolicy>(
        nfd::cs::UniformDecisionPolicy(ACCEPT_RATIO));
  }

//  auto policy = make_shared<nfd::cs::UniformDecisionPolicy>(
//      nfd::cs::UniformDecisionPolicy(ACCEPT_RATIO));

  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1000Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

  CreateFatTreeTopo fatTree;
  fatTree.setUp();
  // Ptr<Node> client = fatTree.getClient();
  Ptr<Node> producer = fatTree.getProducer();

  // Set BestRoute strategy
  // ndn::StrategyChoiceHelper::InstallAll("/", "/localhost/nfd/strategy/best-route");

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "100"); // default ContentStore parameters
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  // Set Policy for all nodes
  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    auto forwarder = (*node)->GetObject<ndn::L3Protocol>()->getForwarder();

    forwarder->setDecisionPolicy(decPolicy);
  }

// Installing applications
  AppHelper consumerHelper1("ns3::ndn::ConsumerZipfMandelbrot");
  consumerHelper1.SetPrefix("/prefix");
  consumerHelper1.SetAttribute("Frequency", StringValue("100"));
  consumerHelper1.SetAttribute("StopTime", StringValue("10"));
  consumerHelper1.SetAttribute("NumberOfContents", StringValue("10000"));
  // consumerHelper1.Install("Node7");
  // consumerHelper1.Install("Node8");
  // consumerHelper1.Install("Node9");
  for (int i = 7; i <= 14; i++) {
    consumerHelper1.Install(std::string("Node") + boost::lexical_cast<std::string>(i));
  }

// Producer
  AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(producer);

  // Installing global routing interface on all nodes
  ndn::GlobalRoutingHelper ndnGlobalRoutingHelper;
  ndnGlobalRoutingHelper.InstallAll();

  // Add /prefix origins to ndn::GlobalRouter
  ndnGlobalRoutingHelper.AddOrigins("/prefix", producer);

  // Calculate and install FIBs
  ndn::GlobalRoutingHelper::CalculateRoutes();

  std::string folder = "src/ndnSIM/results/";
  AppDelayTracer::InstallAll(folder + SIM_NAME + "app-trace.txt");
  CsTracer::InstallAll(folder + SIM_NAME + "cs-trace.txt", Seconds(1));
  L2RateTracer::InstallAll(folder + SIM_NAME + "drop-trace.txt", Seconds(1));

  Simulator::Stop(Seconds(20.0));
  Simulator::Run();
  Simulator::Destroy();
}

}
// namespace ns3

int main(int argc, char* argv[])
{

  ns3::run(argc, argv);
  return 0;
}
