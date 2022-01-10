#!/bin/sh
# A simple command to start CoinGreen network node
# under very strong assumptions (as follows)
# If for some reason you did not put coingreen.service to /etc/systemd/system  or did not run systemctl enable coingreen           
/home/coingreen/coingreend -conf=/home/coingreen/conf/coingreen.conf -datadir=/home/coingreen/data -daemon -disablewallet=true -txindex=0
