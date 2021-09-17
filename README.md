# holbertonschool-blockchain

Main repository for the Holberton School Blockchain Curriculum

## _**Now with multithreading!**_

This Holberton-made Blockchain is designed so that it could mine blocks 40% faster than all other Holberton-made Blockchains, despite utilizing the exact same consensus protocol mechanism, _proof of work_. How could this be achieved? Easy. This blockchain attempts to take full advantage of the multiple cores in your computer to compute hashes at up to 4 times the speed of a regular Holberton Blockchain.

## Install

### Requirements
* Unix-like machine. (sorry Windows)
* GNU Make --version >=4.2.1


1. Download this repository
```bash
$ git clone https://github.com/jicruz96/holbertonschool-blockchain
```

2. Run the install script **as root user**
```bash
$ sudo ./install.sh
```

3. Run the blockchain CLI
```bash
$ ./blockchain_cli
```

4. Use the CLI to create wallets, send transactions, and mine blocks.
```bash
$ ./blockchain_cli
>>> wallet_load
ec_load: No public key in folder [.]
No wallet in folder [.]
Create new key? [y/n]: y
Creating new wallet
Wallet loaded
Wallet address: 04a4588d37c2816dc34996926db39589a360cec89796279d1e9e19005bdc9d1bd5cb8959f9cdc574c2599ab568fd53af67d25d819a74acdff27c0e2a29c44a16e2
>>> 
```
