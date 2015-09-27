#ifndef NFD_TEST_HELPER_HPP
#define NFD_TEST_HELPER_HPP

#include <ns3/channel.h>
#include <ns3/error-model.h>
#include <ns3/net-device.h>
#include <ns3/nstime.h>
#include <ns3/object.h>
#include <ns3/pointer.h>
#include <ns3/ptr.h>
#include <string>

#include <iostream>
#include <memory>
#include "../../helper/ndn-global-routing-helper.hpp"
#include "../../model/ndn-common.hpp"
#include "../../NFD/daemon/face/face.hpp"
#include "../../NFD/daemon/face/face-counters.hpp"

namespace ns3 {

using namespace ndn;

class TestHelper
{
public:

  static void setDelay(Ptr<NetDevice> nd, std::string timeString)
  {
    // Example: Time time1 = Time("500ms");
    Ptr < Channel > chan = nd->GetChannel();
    chan->SetAttribute("Delay", TimeValue(Time(timeString)));
  }

  static void setLinkError(Ptr<NetDevice> nd, double errorRate)
  {
    Ptr < RateErrorModel > error = CreateObject<RateErrorModel>();
    error->SetUnit(RateErrorModel::ERROR_UNIT_PACKET);
    error->SetRate(errorRate);
    nd->SetAttribute("ReceiveErrorModel", PointerValue(error));
  }

  static void print(std::string string)
  {
    std::cout << string << "\n";
  }

  static void printFaceCounter(std::vector<std::shared_ptr<nfd::Face>> faceVector)
  {
    for (std::shared_ptr<nfd::Face> face : faceVector) {

      const nfd::FaceCounters & counter = face->getCounters();
      std::cout << "FaceId: " << face->getId() << " Out: " << counter.getNOutInterests() << ", In: "
          << counter.getNInDatas() << "\n";
    }

  }

private:
};

}

#endif // NFD_TESTS_TEST_COMMON_HPP
