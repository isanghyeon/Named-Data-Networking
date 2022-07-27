#!/bin/bash
# Copyright (c) 2022 Sang-hyeon Lee All Rights Reserved.
# Email: cpd4268@sch.ac.kr
# Web: http://logos.sch.ac.kr


echo "============== Image Build ================="

docker build -t ndn_test:0.5 .
sleep 2

echo "============== Image checker ================="

echo "ndn_test:: " $(docker images | grep "ndn_test:0.5")
sleep 2

echo "============== Container Running ================="

docker stop "test_ndn-0.1-consumer"
docker stop "test_ndn-0.1-producer"

docker rm "test_ndn-0.1-consumer"
docker rm "test_ndn-0.1-producer"

docker run -d -it --name "test_ndn-0.1-consumer" -p 50001:6363 -p 50002:8080 -v $(pwd):/home ndn_test:0.5
docker run -d -it --name "test_ndn-0.1-producer" -p 50003:6363 -p 50004:8080 -v $(pwd):/home ndn_test:0.5

sleep 2

echo "============== Container checker ================="

echo "consumer:: " $(docker ps -a | grep "test_ndn-0.1-consumer")
echo "producer:: " $(docker ps -a | grep "test_ndn-0.1-producer")
sleep 2


echo "============== All Done ================="

