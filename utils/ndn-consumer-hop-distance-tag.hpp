
#ifndef NDN_FW_CONSUMER_DISTANCE_TAG_H
#define NDN_FW_CONSUMER_DISTANCE_TAG_H

#include "ns3/tag.h"

namespace ns3 {
namespace ndn {

class ConsumerHopDistanceTag : public Tag {
public:
  static TypeId
  GetTypeId(void);

  TypeId
  GetInstanceTypeId() const;


  /**
   * @brief Default constructor
   */
  ConsumerHopDistanceTag(uint32_t hopDistance = 0)
    : m_hopDistance(hopDistance)
  {};

  void
  Set(uint32_t distance)
  {
    m_hopDistance = distance;
  }

  uint32_t
  Get() const
  {
    return m_hopDistance;
  }

  ////////////////////////////////////////////////////////
  // from Tag
  ////////////////////////////////////////////////////////

  virtual uint32_t
  GetSerializedSize() const;

  virtual void
  Serialize(TagBuffer i) const;

  virtual void
  Deserialize(TagBuffer i);

  virtual void
  Print(std::ostream& os) const;

private:
  uint32_t m_hopDistance;
};

} // namespace ndn
} // namespace ns3

#endif // NDN_FW_HOP_COUNT_TAG_H
