//
// Created by Stephen Satsky on 11/9/23.
//

#include "RelayService.h"

//<access cd, vec<endpoint>>
map<string, vector<boost::asio::ip::udp::endpoint>> connected_clients;
//<access cd, endpoint>
map<string, boost::asio::ip::udp::endpoint> connected_hosts;


void Log(string msg){
    std::cout << "\n [RelayService] " + msg;
}

//void Listen(boost::asio::io_context &io_context, boost::asio::ip::udp::socket &socket, std::array<char, 10000> recv_buffer, boost::asio::ip::udp::endpoint &remote_endpoint){
//}

void RelayService::Start() {
    for(int i = 0; i < 2; i++) {
        try {
            boost::asio::io_context io_context;

            // Replace 'port_number' with your desired port number
            int port_number = 2456;
            boost::asio::ip::udp::socket socket(io_context,boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(),port_number));

            boost::asio::ip::udp::endpoint remote_endpoint;
            std::array<char, 125000> recv_buffer;

            socket.async_receive_from(
                    boost::asio::buffer(recv_buffer), remote_endpoint,
                    [&](const boost::system::error_code &error, std::size_t bytes_transferred) {

                        //static int call_Count = 0;
                        //call_Count++;
                        //Log(std::to_string(call_Count));
                        //Log("json size: ");
                        //Log(std::to_string(bytes_transferred));
                        if (!error) {
                            // Convert bytes to string
                            std::string json_str(recv_buffer.data(), bytes_transferred);

                            Log("json string: ");
                            Log(json_str);

                            // Parse the string as JSON
                            try {
                                //Log("parsing...");
                                nlohmann::json json_obj = nlohmann::json::parse(json_str);
                                // Work with the JSON object
                                Log(json_obj["access_code"]);
                                string access_cd = json_obj["access_code"];
                                Log(json_obj["isHost"]);
                                if(json_obj["isHost"] == "true"){
                                    //check to see if this host has been added to map
                                    if(connected_hosts[access_cd].size() < 1){
                                        //add
                                        connected_hosts.insert(std::make_pair(access_cd, remote_endpoint));
                                    }

                                    //----------------Logging ----------------
                                    /*if (json_obj.find("data") != json_obj.end() && json_obj["data"].is_array()) {
                                        const auto &data_array = json_obj["data"];
                                        for (const auto &data_item: data_array) {
                                            if (data_item.is_array()) {
                                                // If the data_item is an array, iterate through its elements
                                                for (const auto &nested_data_item: data_item) {
                                                    if (nested_data_item.is_string()) {
                                                        std::string data_str = nested_data_item.get<std::string>();
                                                        Log("data item: " + data_str);
                                                    } else {
                                                        Log("nested data item is not a string");
                                                    }
                                                }
                                            } else {
                                                Log("data item is not an array");
                                            }
                                        }
                                    } else {
                                        Log("data is missing or not an array");
                                    }*/
                                    //----------------Logging ----------------

                                    //----------------Logic ----------------
                                    // Lambda function to handle the send completion
                                    auto send_handler = [](const boost::system::error_code& error, std::size_t bytes_transferred) {
                                        if (!error) {
                                            std::cout << "Successfully sent " << bytes_transferred << " bytes." << std::endl;
                                        } else {
                                            std::cerr << "Error during send: " << error.message() << std::endl;
                                        }
                                    };
                                    //send to clients
                                    vector clients = connected_clients[access_cd];
                                    for(int x = 0; x < clients.size(); x++){
                                        //send json
                                        socket.async_send_to(boost::asio::buffer(json_str), clients[x], send_handler);
                                    }
                                }
                                else{

                                    //this was from a client- must be input data
                                    vector<boost::asio::ip::udp::endpoint> client_vec;
                                    if(connected_clients[access_cd].size() < 1){
                                        //add
                                        client_vec = {remote_endpoint};
                                        connected_clients.insert(std::make_pair(access_cd, client_vec));
                                    }
                                    else{
                                        client_vec = connected_clients[access_cd];
                                    }
                                    bool client_exists = false;
                                    for(int y = 0; y < client_vec.size(); y++){
                                        if(remote_endpoint == client_vec[y]){
                                            client_exists = true;
                                        }
                                    }
                                    if(client_exists == false){
                                        client_vec.push_back(remote_endpoint);
                                    }
                                    //send data to host
                                    // Lambda function to handle the send completion
                                    auto send_handler = [](const boost::system::error_code& error, std::size_t bytes_transferred) {
                                        if (!error) {
                                            std::cout << "Successfully sent " << bytes_transferred << " bytes." << std::endl;
                                        } else {
                                            std::cerr << "Error during send: " << error.message() << std::endl;
                                        }
                                    };
                                    //send the input data to the host based on the packet's access cd
                                    socket.async_send_to(boost::asio::buffer(json_str), connected_hosts[access_cd], send_handler);
                                }


                            }
                            catch (const exception &e) {
                                Log("json error: " + std::string(e.what()));
                            }
                        } else {
                            Log("There was an error");
                            Log("json size: ");
                            Log(std::to_string(bytes_transferred));
                        }
                    }
            );

            io_context.run();

        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }

        i = 0;
    }
}