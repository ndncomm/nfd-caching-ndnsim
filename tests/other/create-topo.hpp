#ifndef TESTS_OTHERS_CREATE_TOPO
#define TESTS_OTHERS_CREATE_TOPO

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

class CreateTopo
{
public:
  virtual void setUp();
  virtual Ptr<Node> getClient();
  virtual Prt<Node> getProducer();
};

class CreateFatTreeTopo : public CreateTopo
{
public:
  virtual void setUp()
  {
    AnnotatedTopologyReader topologyReader("", 25);
    topologyReader.SetFileName("src/ndnSIM/examples/topologies/topo-fattree.txt");
    topologyReader.Read();
  }

  virtual Ptr<Node> getClient()
  {
    return Names::Find<Node>("Node7");
  }

  virtual Ptr<Node> getProducer()
  {
    return Names::Find<Node>("Node14");
  }
};

class CreateGridTopo
{
public:
  CreateGridTopo(int nCol, int nRow):
    m_nCol(nCol),m_nRow(nRow)
  {}

  virtual void setUp()
  {
    PointToPointHelper p2p;
    PointToPointGridHelper grid(m_nCol, m_nRow, p2p);
    m_client = grid.GetNode(3, 3);
    m_producer = grid.GetNode(0, 0);
  }

  virtual Ptr<Node> getClient()
  {
    return m_client;
  }

  virtual Ptr<Node> getProducer()
  {
    return m_producer;
  }
private:
  int m_nCol;
  int m_nRow;
  Ptr<Node> m_client;
  Ptr<Node> m_producer;
};

}

#endif