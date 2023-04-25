#! /usr/bin/env sh

echo > ./package/export

./bin/smile -p package -e add-@float@float && \
./bin/smile -p package -e add-@int@int && \
./bin/smile -p package -e circle && \
./bin/smile -p package -e False && \
./bin/smile -p package -e func01 && \
./bin/smile -p package -e func01a && \
./bin/smile -p package -e func01b && \
./bin/smile -p package -e func02 && \
./bin/smile -p package -e func03 && \
./bin/smile -p package -e func04 && \
./bin/smile -p package -e FuncAdd01 && \
./bin/smile -p package -e FuncInc01 && \
./bin/smile -p package -e FuncSenSignal01 && \
./bin/smile -p package -e FuncSenSignal02 && \
./bin/smile -p package -e FuncSenSignal03 && \
./bin/smile -p package -e iAmVaryOld && \
./bin/smile -p package -e identity-function && \
./bin/smile -p package -e IgorWasBorn && \
./bin/smile -p package -e iWasBorn && \
./bin/smile -p package -e LudaWasBorn && \
./bin/smile -p package -e multiline-function && \
./bin/smile -p package -e newBool && \
./bin/smile -p package -e newFloat && \
./bin/smile -p package -e newInt && \
./bin/smile -p package -e newSignal && \
./bin/smile -p package -e pi && \
./bin/smile -p package -e pi2 && \
./bin/smile -p package -e Quote1 && \
./bin/smile -p package -e Quote2 && \
./bin/smile -p package -e Real01 && \
./bin/smile -p package -e Signal && \
./bin/smile -p package -e Tab && \
./bin/smile -p package -e True && \
./bin/smile -p package -e tuple01
# ./bin/smile -p package -e Z
