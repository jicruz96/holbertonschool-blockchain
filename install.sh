#!/usr/bin/env bash

cd blockchain/v0.3/
make libhblk_blockchain.a
cd ../../crypto
make libhblk_crypto.a
cd ../llist
./install.bash
if [ "$?" == "1" ]
then
    cd ..
    exit 1
fi
cd ../cli
rm -rf cli
make cli
mv cli ../blockchain_cli
cd ..
echo "Done"
