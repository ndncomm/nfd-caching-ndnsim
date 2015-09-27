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
#include <cassert>
#include <iterator>
#include "../../NFD/daemon/table/cs-decision-policy.hpp"
#include "../../utils/tracers/ndn-app-delay-tracer.hpp"
#include "../../utils/tracers/ndn-cs-tracer.hpp"
#include <ns3-dev/ns3/node-container.h>
#include <ns3-dev/ns3/point-to-point-helper.h>

namespace ns3 {

void run(int argc, char* argv[])
{

//  ScenarioHelper helper;
//  helper.createTopology(
//      { { "client", "r1" }, { "r1", "r2" }, { "r2", "r3" }, { "r3", "producer" } });
//  helper.addRoutes( { { "client", "r1", "/", 1 }, { "r1", "r2", "/", 1 }, { "r2", "r3", "/", 1 }, {
//      "r3", "producer", "/", 1 }, });

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

//  nfd::Forwarder clientForwarder
  shared_ptr<nfd::Forwarder> clientForwarder = client->GetObject<ndn::L3Protocol>()->getForwarder();

  int ACCEPT_RATIO = 50;
  std::string SIM_NAME = "ar" + std::to_string(ACCEPT_RATIO);

  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    shared_ptr<nfd::Forwarder> forwarder = (*node)->GetObject<ndn::L3Protocol>()->getForwarder();
    auto policy = make_shared<nfd::cs::UniformDecisionPolicy>(
        nfd::cs::UniformDecisionPolicy(ACCEPT_RATIO));

    forwarder->setDecisionPolicy(policy);
    std::cout << forwarder->getDecisionPolicy()->getName() << ": ";
    auto pol = forwarder->getDecisionPolicy();
    auto unifPol = std::static_pointer_cast<nfd::cs::UniformDecisionPolicy>(pol);
    std::cout << unifPol->getAcceptRatio() << "\n";

    // Correct acceptance rate:
    assert(unifPol->getAcceptRatio() == ACCEPT_RATIO);
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

  std::string folder = "src/ndnSIM/results/";
  AppDelayTracer::InstallAll(folder + SIM_NAME + "app-delays-trace.txt");
  CsTracer::InstallAll(folder + SIM_NAME + "cs-trace.txt", Seconds(1));

  Simulator::Stop(Seconds(15.0));
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
