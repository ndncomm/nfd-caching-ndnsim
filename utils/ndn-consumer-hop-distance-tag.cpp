
#include "ndn-consumer-hop-distance-tag.hpp"

namespace ns3 {
namespace ndn {

TypeId
ConsumerHopDistanceTag::GetTypeId()
{
  static TypeId tid =
    TypeId("ns3::ndn::ConsumerHopDistanceTag").SetParent<Tag>().AddConstructor<ConsumerHopDistanceTag>();
  return tid;
}

TypeId
ConsumerHopDistanceTag::GetInstanceTypeId() const
{
  return ConsumerHopDistanceTag::GetTypeId();
}

uint32_t
ConsumerHopDistanceTag::GetSerializedSize() const
{
  return sizeof(uint32_t);
}

void
ConsumerHopDistanceTag::Serialize(TagBuffer i) const
{
  i.WriteU32(m_hopDistance);
}

void
ConsumerHopDistanceTag::Deserialize(TagBuffer i)
{
  m_hopDistance = i.ReadU32();
}

void
ConsumerHopDistanceTag::Print(std::ostream& os) const
{
  os << m_hopDistance;
}

} // namespace ndn
} // namespace ns3
