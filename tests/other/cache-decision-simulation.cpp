//#include <ns3/command-line.h>

#include <boost/assert.hpp>
#include <ns3/command-line.h>
#include <ns3/config.h>
#include <ns3/names.h>
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
#include "../../helper/ndn-scenario-helper.hpp"
#include "../../helper/ndn-stack-helper.hpp"
#include "../../model/ndn-common.hpp"
#include "../../model/ndn-l3-protocol.hpp"
#include "../../NFD/daemon/face/face.hpp"
#include "../../NFD/daemon/face/face-counters.hpp"
#include "../../NFD/daemon/fw/forwarder.hpp"
#include "../../NFD/daemon/table/cs-decision-policy.hpp"
#include "../../NFD/daemon/table/cs-lcd-decision-policy.hpp"
#include "../../NFD/daemon/table/cs-uniform-decision-policy.hpp"
#include "../../utils/tracers/l2-rate-tracer.hpp"
#include "../../utils/tracers/ndn-app-delay-tracer.hpp"
#include "../../utils/tracers/ndn-cs-tracer.hpp"
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
  cmd.AddValue("params", "Parameters", params);
  cmd.Parse(argc, argv);
  std::cout <<  "Parameters: " << params << "\n";


  int ACCEPT_RATIO = std::stoi(params);
  std::string SIM_NAME = "ar" + std::to_string(ACCEPT_RATIO);

  Config::SetDefault("ns3::PointToPointNetDevice::DataRate", StringValue("1000Mbps"));
  Config::SetDefault("ns3::PointToPointChannel::Delay", StringValue("10ms"));
  Config::SetDefault("ns3::DropTailQueue::MaxPackets", StringValue("20"));

// Creating nodes
  NodeContainer nodes;
  nodes.Create(5);

  // Connecting nodes using two links
  PointToPointHelper p2p;
  p2p.Install(nodes.Get(0), nodes.Get(1));
  p2p.Install(nodes.Get(1), nodes.Get(2));
  p2p.Install(nodes.Get(2), nodes.Get(3));
  p2p.Install(nodes.Get(3), nodes.Get(4));

  // Install NDN stack on all nodes
  ndn::StackHelper ndnHelper;
  ndnHelper.SetOldContentStore("ns3::ndn::cs::Lru", "MaxSize", "100"); // default ContentStore parameters
  ndnHelper.SetDefaultRoutes(true);
  ndnHelper.InstallAll();

  Ptr<Node> client = nodes.Get(0);
  Ptr<Node> r1 = nodes.Get(1);
  Ptr<Node> r2 = nodes.Get(2);
  Ptr<Node> r3 = nodes.Get(3);
  Ptr<Node> producer = nodes.Get(4);

  // Set Policy for all nodes
  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    auto forwarder = (*node)->GetObject<ndn::L3Protocol>()->getForwarder();
    auto policy = make_shared<nfd::cs::UniformDecisionPolicy>(
        nfd::cs::UniformDecisionPolicy(ACCEPT_RATIO));
    forwarder->setDecisionPolicy(policy);
  }

// Installing applications
  AppHelper consumerHelper1("ns3::ndn::ConsumerZipfMandelbrot");
  consumerHelper1.SetPrefix("/prefix/A");
  consumerHelper1.SetAttribute("Frequency", StringValue("100"));
  consumerHelper1.SetAttribute("StopTime", StringValue("10"));
  consumerHelper1.SetAttribute("NumberOfContents", StringValue("10000"));
  consumerHelper1.Install(client);

// Producer
  AppHelper producerHelper("ns3::ndn::Producer");
  producerHelper.SetPrefix("/prefix");
  producerHelper.SetAttribute("PayloadSize", StringValue("1024"));
  producerHelper.Install(producer);

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
