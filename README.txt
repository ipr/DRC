
DRC - Decentralized Relay Chat

Replacement for IRC to work without central servers:
all communication is peer-to-peer where each peer is node in network
to relay messages to all other known peers.

Primary idea is that connection to any node can also relay to every other node in the network
thus making servers entirely unnecessary: all you need is connection to any node in the network.

Purpose:
Freedom of speech.

Author:
Ilkka Prusi, ilkka.prusi@gmail.com (2011)


Advantages:
- each node in network can be both server and client or just either
-> flexibility, able to use from behind NAT/firewalls 
- anonymous users: each node has only hash-string transmitted for each node
-> cannot identify user
-> no collisions of names
- each node can relay to all other (connected) nodes in network
-> the more nodes you know, more reliability 
- zero-configuration
-> quick&easy to setup

Bugs/limitations:
- message size&hashing is missing -> may cause infinite retransmissions
- other nodes are not added automatically from other known nodes yet..
- secure socket support missing 
- bugs.. 
- !!very early in development!!


This program and it's source code is in public domain, use how you wish.

Note: this is prototype using Qt, Qt GPL/LGPL restrictions apply.

