# -*- coding: utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

def address_collision_prob(maxaddr, n):
    if (n > maxaddr):
        return 1.0
    
    non_coll_prob = 1
    for i in range(0, n):
        non_coll_prob *= (maxaddr-i)/maxaddr
        
    return 1.0 - non_coll_prob

def answer_of_only_one_prob(count, answer_prob):
    return count*answer_prob*(1-answer_prob)**(count-1), (1-answer_prob)**count

devices = 10
addr_bits = list(range(4, 24+1))

probs = [ address_collision_prob(2 ** b, devices) for b in addr_bits ]
collision_devices_count = [ 1 / p for p in probs ]

#plt.figure(1)
#plt.subplot(211)
#plt.xlabel("Address bits")
#plt.ylabel("Collision probability")
#plt.yscale('log')
#plt.plot(addr_bits, probs, 'bo-')
#
#
#plt.subplot(212)
plt.xlabel("Address bits")
plt.ylabel("One collision per complects")
plt.yscale('log')
plt.plot(addr_bits, collision_devices_count, 'bo-')

plt.show()