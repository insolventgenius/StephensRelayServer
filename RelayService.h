//
// Created by Stephen Satsky on 11/9/23.
//

#ifndef STEPHENSRELAYSERVER_RELAYSERVICE_H
#define STEPHENSRELAYSERVER_RELAYSERVICE_H

#include <iostream>
#include "lib/boost_1_83_0/boost/asio.hpp"
#include <array>
#include <thread>
#include <future>
#include "lib/json-develop/include/nlohmann/json.hpp"
using namespace std;

class RelayService {

public:
    static void Start();
};


#endif //STEPHENSRELAYSERVER_RELAYSERVICE_H
