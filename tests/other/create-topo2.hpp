#include <ns3/names.h>
#include <ns3/node.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/ptr.h>
#include <ns3/names.h>
#include <ns3/node.h>
#include <ns3/point-to-point-helper.h>
#include <ns3/ptr.h>
#include <ns3/point-to-point-grid.h>

#include "../../utils/topology/annotated-topology-reader.hpp"

/**
   examples:

   CreateFatTreeTopo fatTree;
   fatTree.setUp();
   Ptr<Node> client = fatTree.getClient();
   Ptr<Node> producer = fatTree.getProducer();

 */



namespace ns3 {


class CreateTopo
{
public:
  virtual void setUp() = 0;
  virtual Ptr<Node> getClient() = 0;
  virtual Ptr<Node> getProducer() = 0;
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
    return Names::Find<Node>("Node0");
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
