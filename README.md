# CALab_Named-Data
### Cryptography Application Lab. 
### Carry Out Apply Format-Preserving Encryption on NDN Research Projects

----
##  Specification
- [NDN-GitHub-ndn-cxx]
- [NDN-GitHub-NFD]
- [NDN-GitHub-ndn-tools]
- [NDN-GitHub-NLSR]
- [OpenSSL] - For EVP functions
- [Format-Preserving-Encryption]

## Hosted Information
[Named-Data] - Official NDN  
[Named-Data-GitHub] - Official NDN GitHub Repository

----
## Server Environment
| Type | Server Info | IP Addr | Environment | README |
|:-----|:--------------------------|:-----|:------------|:------------:|  
| Consumer | ndn-consrouter:ndn | 192.168.0.106 | Ubuntu 20.04.3 LTS, Docker version 20.10.12, build e91ed57 | [src/ndn_dev-ConsumerA][Consumer-A] [src/ndn_dev-ConsumerB][Consumer-B] |
| Router | ndn-router1:router | 192.168.0.99 | Ubuntu 20.04.3 LTS | [src/ndn_dev-router][router] |
| Router | ndn-router2:router | 192.168.0.100 | Ubuntu 20.04.3 LTS | [src/ndn_dev-router][router] |
| Router | ndn-router3:router | 192.168.0.101 | Ubuntu 20.04.3 LTS | [src/ndn_dev-router][router] |
| Producer | ndn-producer:producer | 192.168.0.104 | Ubuntu 20.04.3 LTS, Docker version 20.10.12, build e91ed57 | [src/ndn_dev-ProducerC][Producer-C] [src/ndn_dev-ProducerD][Producer-D] |


----

[Named-Data]: <https://named-data.net/>
[Named-Data-GitHub]: <https://github.com/named-data>

[NDN-GitHub-ndn-cxx]: <https://github.com/named-data/ndn-cxx>
[NDN-GitHub-NFD]: <https://github.com/named-data/NFD>
[NDN-GitHub-ndn-tools]: <https://github.com/named-data/ndn-tools>
[NDN-GitHub-NLSR]: <https://github.com/named-data/NLSR>
[OpenSSL]: <https://openssl.org/>
[Format-Preserving-Encryption]: <https://github.com/0NG/Format-Preserving-Encryption>