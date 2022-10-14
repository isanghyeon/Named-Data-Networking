/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2013-2022 Regents of the University of California.
 *
 * This file is part of ndn-cxx library (NDN C++ library with eXperimental eXtensions).
 *
 * ndn-cxx library is free software: you can redistribute it and/or modify it under the
 * terms of the GNU Lesser General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later version.
 *
 * ndn-cxx library is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more details.
 *
 * You should have received copies of the GNU General Public License and GNU Lesser
 * General Public License along with ndn-cxx, e.g., in COPYING.md file.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * See AUTHORS.md for complete list of ndn-cxx authors and contributors.
 */

#include <ndn-cxx/face.hpp>
#include <ndn-cxx/security/key-chain.hpp>
#include <ndn-cxx/security/signing-helpers.hpp>

#include <iostream>

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespaces should be used to prevent/limit name conflicts
namespace examples {

class Producer
{
public:
  void
  run()
  {
    m_face.setInterestFilter(Name("/sch.ac.kr/calab/research.file", 0),
                             std::bind(&Producer::onInterest, this, _1, _2),
                             nullptr, // RegisterPrefixSuccessCallback is optional
                             std::bind(&Producer::onRegisterFailed, this, _1, _2));

    auto cert = m_keyChain.getPib().getDefaultIdentity().getDefaultKey().getDefaultCertificate();
    m_certServeHandle = m_face.setInterestFilter(security::extractIdentityFromCertName(cert.getName()),
                             [this, cert] (auto&&...) {
                               m_face.put(cert);
                             },
                             std::bind(&Producer::onRegisterFailed, this, _1, _2));
    m_face.processEvents();
  }

private:
  void
  onInterest(const InterestFilter&, const Interest& interest)
  {
    std::cout << ">> I: " << interest << std::endl;

    static const std::string content("m9sUXkyY2hfQhurPTbmH5yYxOhyMQYwh915YINlnMAq3GrqHVo6aZPEPVp3MCKVxOXzOD0GlMjDlOOtDtUdcEIzUAAX2X66mgypXqmO4zr3n0m6O1NmVNU2f7rp5iYUiRU2MndiRFa4vXbFdQKZZIJw5JLpFTHMXq0bIirnwRgPPYxbndoNPbIbxZbp0tk1MFlf0zwG8WDEIrCiARe4DegCUZMLsaEQqL72o8j31yVPtyrT06OmTouv5LKnlYcLs4d6lMgxJh7njqP9ko8TV9gTwwlwsaPY5y7bHeQfAExdHhUVS5RW5VZY0NzuJv53QQZPaV5ChKdvqzkr6HBKTP9KvNMpcWpyZy8uETgtojeInso013CYXDPIFfOxcItLKuxRzHgaUfb2MhfrorB9DvWDXvd2e6RKnKr7pOhsr9OjbvWIXJd0NXZmygo4WKCiRY9tL1zeyEG4no7mgHsPttFGlTdbvLcYwp9MFAyqBayACrDGvUEH58Hk4UDpprPpxIvfMb0bihdH10S7cut8hbxutSFwhVLzQihJxn4YlxiQwshbXPTqA6yNSlzcuKFWGA3YqQFeXVFtVgsujfKKjkF4pE05ONFbQyBEvFklVeiyXM406RJNJEo7g0muWOFeUAC6HoWeeZBHzT12jhnog2uCmBllHhI2W1l4KPyHeLVm8HO19PtLgNa3xUYnS2eIx9ZGTqAyU5g2QHCAkOSK3qhwtrNUl7RiVq1P47dau570A2fRmi5F6C2GjFjrTwODhsUtnsozjHtzu3GCKezLkrWmBpXrLJzHvOJsdhaDk6yt5xKF8F6iPXL065XUc1daZhMhIxa6XFtiDvqI74jF026lGQgMzYQawE1p81rMmq735VCzsEtdN7mpfZTU78uuenBKOEwE32kFgDxAVbTDyyc3Yrr0gH3gu4IcqlBxz7WNvFdyFkaK34vb0S8sCteOXMQMN3I2hMLaiM6lWzpyGqQ05rRxQHTPy5gSza5VF");

    // Create Data packet
    auto data = make_shared<Data>(interest.getName());
    data->setFreshnessPeriod(10_s);
    data->setContent(make_span(reinterpret_cast<const uint8_t*>(content.data()), content.size()));

    // in order for the consumer application to be able to validate the packet, you need to setup
    // the following keys:
    // 1. Generate example trust anchor
    //
    //         ndnsec key-gen /example
    //         ndnsec cert-dump -i /example > example-trust-anchor.cert
    //
    // 2. Create a key for the producer and sign it with the example trust anchor
    //
    //         ndnsec key-gen /example/testApp
    //         ndnsec sign-req /example/testApp | ndnsec cert-gen -s /example -i example | ndnsec cert-install -

    // Sign Data packet with default identity
    m_keyChain.sign(*data);
    // m_keyChain.sign(*data, signingByIdentity(<identityName>));
    // m_keyChain.sign(*data, signingByKey(<keyName>));
    // m_keyChain.sign(*data, signingByCertificate(<certName>));
    // m_keyChain.sign(*data, signingWithSha256());

    // Return Data packet to the requester
    std::cout << "<< D: " << *data << std::endl;
    m_face.put(*data);
  }

  void
  onRegisterFailed(const Name& prefix, const std::string& reason)
  {
    std::cerr << "ERROR: Failed to register prefix '" << prefix
              << "' with the local forwarder (" << reason << ")" << std::endl;
    m_face.shutdown();
  }

private:
  Face m_face;
  KeyChain m_keyChain;
  ScopedRegisteredPrefixHandle m_certServeHandle;
};

} // namespace examples
} // namespace ndn

int
main(int argc, char** argv)
{
  try {
    ndn::examples::Producer producer;
    producer.run();
    return 0;
  }
  catch (const std::exception& e) {
    std::cerr << "ERROR: " << e.what() << std::endl;
    return 1;
  }
}
