#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/validator-config.hpp>
#include <ndn-cxx/util/time.hpp>
#include <iostream>
#include <fstream>
using namespace std;
// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
ofstream fp("/usr/src/app/result.txt");
namespace ndn
{
  // Additional nested namespaces should be used to prevent/limit name conflicts
  namespace examples
  {
    time::steady_clock::time_point now;
    time::nanoseconds rtt;
    class Consumer
    {
    public:

      void
      run()
      {
        Name interestName("/sch.ac.kr/calab/research.file");
        interestName.appendVersion();

        Interest interest(interestName);
        interest.setMustBeFresh(true);
        interest.setInterestLifetime(6_s); // The default is 4 seconds
        now = time::steady_clock::now();
        //std::cout << "Sending Interest " << interest << std::endl;
        m_face.expressInterest(interest,
                               std::bind(&Consumer::onData, this, _1, _2),
                               std::bind(&Consumer::onNack, this, _1, _2),
                               std::bind(&Consumer::onTimeout, this, _1));

        // processEvents will block until the requested data is received or a timeout occurs
        m_face.processEvents();
      }

    private:
      void
      onData(const Interest &, const Data &data)
      {
        //std::cout << "Received Data " << data << std::endl;
        data.getContent();
        
	      rtt = time::steady_clock::now() - now;
	      fp << rtt << endl;
	      std::cout << "RTT: " << rtt << std::endl;
      }

      void
      onNack(const Interest &, const lp::Nack &nack) const
      {
        std::cout << "Received Nack with reason " << nack.getReason() << std::endl;
      }

      void
      onTimeout(const Interest &interest) const
      {
        std::cout << "Timeout for " << interest << std::endl;
      }

    private:
      Face m_face;
    };

  } // namespace examples
} // namespace ndn

int main(int argc, char **argv)
{
  try
  {
    ndn::examples::Consumer consumer;
    for(int i = 0; i < 1000; i++) {
	    consumer.run();
    }
    fp.close();
    return 0;
  }
  catch (const std::exception &e)
  {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }
}
