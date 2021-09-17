#!/bin/bash

if [ "$(id -u)" != "0" ]
then
	>&2 echo "Must be executed as root"
	exit 1
fi

echo "Installing llist library and headers ..."
cp libllist.so /usr/local/lib/
cp llist.h /usr/local/include/
ldconfig

echo "Installing llist man pages ..."
mkdir /usr/local/man/man3
cp man/*.3 /usr/local/man/man3/
mandb

echo "Llist library installed!"
