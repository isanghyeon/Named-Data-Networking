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
## Cipher Types
| Type | Docker Hub | Base OS Img | PATH |
|:------------|:--------------------------|:------------|:-----------------:|  
| AES | cpd9957/named_data_networking:aes-0.2 | Ubuntu 20.04 LTS  | [enc-evp_aes/] |
|    | cpd9957/named_data_networking:aes-0.2 | Ubuntu 20.04 LTS  | [enc-evp_aes/] |
| Format Preserving Encryption | cpd9957/named_data_networking:fpe-0.9 | Ubuntu 20.04 LTS | [enc-fpe_ff3/]|
| None | cpd9957/named_data_networking:0.1 | Ubuntu 20.04 LTS | [enc-none/]|

<div class=col-12>
<table>
    <thead>
        <tr>
            <th>Layer 1</th>
            <th>Layer 2</th>
            <th>Layer 3</th>
        </tr>
    </thead>
    <tbody>
        <tr>
            <td rowspan=4>cpd9957/named_data_networking:0.1 </td>
            <td rowspan=2>L2 Name A</td>
            <td>L3 Name A</td>
        </tr>
        <tr>
            <td>L3 Name B</td>
        </tr>
        <tr>
            <td rowspan=2>L2 Name B</td>
            <td>L3 Name C</td>
        </tr>
        <tr>
            <td>L3 Name D</td>
        </tr>
    </tbody>
</table>
</div>

## Server Environment
| Dir Name | Description 
|:------------|:--------------------------|
| core | core |
| enc-evp_aes | enc-evp_aes |
| enc-fpe_ff3 | enc-fpe_ff3 | 
| enc-none | enc-none |
| tests | test |

----

[Named-Data]: <https://named-data.net/>
[Named-Data-GitHub]: <https://github.com/named-data>

[NDN-GitHub-ndn-cxx]: <https://github.com/named-data/ndn-cxx>
[NDN-GitHub-NFD]: <https://github.com/named-data/NFD>
[NDN-GitHub-ndn-tools]: <https://github.com/named-data/ndn-tools>
[NDN-GitHub-NLSR]: <https://github.com/named-data/NLSR>
[OpenSSL]: <https://openssl.org/>
[Format-Preserving-Encryption]: <https://github.com/0NG/Format-Preserving-Encryption>

[enc-evp_aes/]: <https://github.com/isanghyeon/CALab_Named-Data/blob/main/enc-evp_aes/README.md>
[enc-fpe_ff3/]: <https://github.com/isanghyeon/CALab_Named-Data/blob/main/enc-fpe_ff3/README.md>
[enc-none/]: <https://github.com/isanghyeon/CALab_Named-Data/blob/main/enc-none/README.md>