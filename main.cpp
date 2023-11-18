#include <iostream>
#include "RelayService.h"
using boost::asio::ip::udp;
int main() {
    std::cout << "Relay Server Starting" << std::endl;

    RelayService service = RelayService();
    service.Start();

    return 0;
}
