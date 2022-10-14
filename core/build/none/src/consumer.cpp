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
#include <ndn-cxx/security/validator-config.hpp>
#include <ndn-cxx/util/random.hpp>

#include <iostream>
#include <fstream>

using namespace std;

std::ofstream fp("/home/named-data-networking/core/build/aes/src/result-300");
ndn::time::steady_clock::time_point binaryExecTime;
ndn::time::nanoseconds runTime;

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
    time::steady_clock::time_point sendTime;
    time::nanoseconds rtt;

    // Additional nested namespaces should be used to prevent/limit name conflicts
    namespace examples {

        class Consumer {
        public:
            Consumer() {
                m_validator.load("trust-schema.conf");
            }

            void
            run() {
                Name interestName("/sch.ac.kr/calab/research.file");
                interestName.appendVersion();

                Interest interest(interestName);
                interest.setMustBeFresh(true);
                interest.setInterestLifetime(6_s); // The default is 4 seconds

                sendTime = time::steady_clock::now();

                std::cout << "Sending Interest " << interest << std::endl;

                m_face.expressInterest(interest,
                                       std::bind(&Consumer::onData, this, _1, _2),
                                       std::bind(&Consumer::onNack, this, _1, _2),
                                       std::bind(&Consumer::onTimeout, this, _1));

                // processEvents will block until the requested data is received or a timeout occurs
                m_face.processEvents();
            }

        private:
            void
            onData(const Interest &, const Data &data) {
                rtt = time::steady_clock::now() - sendTime;

                runTime = time::steady_clock::now() - binaryExecTime;

                std::cout << "Received Data " << data << std::endl;
                std::cout << "\n" << std::endl;
                std::cout << "Content Data " << data.getContent() << std::endl;
                std::cout << "Round Trim Time " << rtt << std::endl;


                fp << "runtime: " << runTime << "; rtt: " << rtt << endl;

                std::cout << "\n" << std::endl;

//                m_validator.validate(data,
//                                     [](const Data &) {
//                                         std::cout << "Data conforms to trust schema" << std::endl;
//                                     },
//                                     [](const Data &, const security::ValidationError &error) {
//                                         std::cout << "Error authenticating data: " << error << std::endl;
//                                     });
            }

            void
            onNack(const Interest &, const lp::Nack &nack) const {
                std::cout << "Received Nack with reason " << nack.getReason() << std::endl;
            }

            void
            onTimeout(const Interest &interest) const {
                std::cout << "Timeout for " << interest << std::endl;
            }

        private:
            Face m_face;
            ValidatorConfig m_validator{m_face};
        };

    } // namespace examples
} // namespace ndn

int
main(int argc, char **argv) {
    try {
        ndn::examples::Consumer consumer;

        binaryExecTime = ndn::time::steady_clock::now();

        for (int i = 0; i < 100; i++)
            consumer.run();

        return 0;
    }
    catch (const std::exception &e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }
}
