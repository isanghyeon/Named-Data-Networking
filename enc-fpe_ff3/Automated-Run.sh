#!/bin/bash
# Copyright (c) 2022 Sang Hyeon Lee All Rights Reserved.
# Email: cpd4268@sch.ac.kr
# Web: http://logos.sch.ac.kr

export version=0.6
export name=ndn_test


echo "============== Image Build ================="

docker build -t $name:$version .
sleep 2

echo "============== Image checker ================="

echo "$name"
echo $(docker images | awk '{print $1":"$2}' | grep $name)
sleep 2

echo "============== Container Running ================="

docker run -d -it --name "test_ndn-$version-consumer" -p 50001:6363 -p 50002:8080 -v $(pwd):/home $name:$version
docker run -d -it --name "test_ndn-$version-producer" -p 50003:6363 -p 50004:8080 -v $(pwd):/home $name:$version

sleep 2

echo "============== Container checker ================="

echo "consumer:: " $(docker ps -a | grep "test_ndn-$version-consumer")
echo "producer:: " $(docker ps -a | grep "test_ndn-$version-producer")
sleep 2


echo "============== All Done ================="

