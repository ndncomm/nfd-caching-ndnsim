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

namespace ns3 {

void run(int argc, char* argv[])
{
//  CommandLine cmd;
//  std::string params = "";
//  std::string strategy = "lowest-cost";
//  cmd.AddValue("params", "Number of packets to echo", params);
//  cmd.AddValue("strategy", "Forwarding strategy to use", strategy);
//  cmd.Parse(argc, argv);

  ScenarioHelper helper;

  helper.createTopology(
      { { "client", "r1" }, { "r1", "r2" }, { "r2", "r3" }, { "r3", "producer" } });

  helper.addRoutes( { { "client", "r1", "/", 1 }, { "r1", "r2", "/", 1 }, { "r2", "r3", "/", 1 }, {
      "r3", "producer", "/", 1 }, });

  Ptr<Node> client = Names::Find<Node>("client");
  Ptr<Node> r1 = Names::Find<Node>("r1");
  Ptr<Node> r2 = Names::Find<Node>("r2");
  Ptr<Node> r3 = Names::Find<Node>("r3");
  Ptr<Node> producer = Names::Find<Node>("producer");

//  nfd::Forwarder clientForwarder
  shared_ptr<nfd::Forwarder> clientForwarder = client->GetObject<ndn::L3Protocol>()->getForwarder();

  for (NodeList::Iterator node = NodeList::Begin(); node != NodeList::End(); node++) {
    shared_ptr<nfd::Forwarder> forwarder = (*node)->GetObject<ndn::L3Protocol>()->getForwarder();
    auto policy = make_shared<nfd::cs::UniformDecisionPolicy>(
        nfd::cs::UniformDecisionPolicy(90));

    forwarder->setDecisionPolicy(policy);
    std::cout << forwarder->getDecisionPolicy()->getName() << "\n";
    std::cout << static_cast<nfd::cs::UniformDecisionPolicy>forwarder->getDecisionPolicy()->getName() << "\n";
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

int main(int argc, char* argv[])
{
  ns3::run(argc, argv);
  return 0;
}
