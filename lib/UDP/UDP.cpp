/**
 * Stephen Hunter Barbella, Baibhav Vatsa
 * GitHub: hman523, baibhavatsa
 * Email: shbarbella@gmail.com, baibhavvatsa@gmail.com
 * Licence: MIT
 * File: UDP.cpp
 */

#include "UDP.h"
#include "../../include/Interpreter.h"
#include <boost/asio.hpp>
#include <numeric>

UDP::UDP() : functions({"udp.send", "udp.receive"}) {}

std::set<std::string> UDP::getFunctions() { return functions; }

std::string UDP::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "udp.send") {
    return send(words);
  } else if (words[0] == "udp.receive") {
    return rec(words);
  }
  throw Exception("Fatal implementation error in UDP library in UDP.cpp");
}

int UDP::evalint(const std::string &val) {
  return Interpreter::Instance()->strToInt(val);
}

std::string UDP::evalstring(const std::string &val) {
  if (Interpreter::Instance()->isParens(val)) {
    return Interpreter::Instance()->eval(val);
  } else if (Interpreter::Instance()->isString(val)) {
    return Interpreter::Instance()->removequotes(val);
  } else {
    return Interpreter::Instance()->strtostr(val);
  }
}

std::string UDP::send(const std::vector<std::string> &expression) {
  // Ensure that ip, port, and message is present
  if (expression.size() < 4) {
    throw Exception("Wrong number of parameters for udp.send");
  }

  // Get the IP Address from the given string
  boost::system::error_code ec;
  boost::asio::ip::address ip_address =
      boost::asio::ip::address::from_string(evalstring(expression[1]), ec);

  // Throw exception if the given string is not valid
  if (ec) {
    throw Exception(
        "Invalid IP address received in first parameter of udp.send");
  }

  // Get the port number from the parameter
  int port = evalint(expression[2]);

  // If the port is invalid, throw exception
  if (port < 0) {
    throw Exception(
        "Invalid port number received in second parameter of udp.send");
  }

  // Initialise the socket
  boost::asio::io_context io_service;
  boost::asio::ip::udp::socket socket(io_service);

  // Open the socket, set the option and  bind it to the local endpoint
  socket.open(boost::asio::ip::udp::v4()); 
  auto e = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port);
  socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
  socket.bind(e);

  // Generate remote endpoint bound to the receiver's ip address and receiving port
  boost::asio::ip::udp::endpoint ep(ip_address, port);

  // Connect socket to the remote endpoint
  try {
    socket.connect(ep);
  } catch (boost::system::system_error &e) {
    throw Exception("udp.send failed to connect socket to endpoint " +
                    expression[1] + ":" + std::to_string(port) + " as " + e.what());
  }

  // Get the message parameter
  std::string message = expression[3];

  // If there is only one message parameter, evaluate it. Else append them all to 
  // make the message string
  if (expression.size() == 4) {
    message = evalstring(message) + "\n";
  } else {
    message = std::accumulate(expression.begin() + 4, expression.end(), message,
                              [](const std::string &m1, const std::string &m2) {
                                return m1 + " " + m2;
                              }) +
              "\n";
  }

  // Write the message to the socket to be sent to the remote endpoint
  boost::system::error_code error;
  socket.send(boost::asio::buffer(message), 0, error);

  if (error) {
    throw Exception("udp.send failed to write to socket");
  }

  return "";
}

std::string UDP::rec(const std::vector<std::string> &expression) {
  // Ensure that port is present
  if (expression.size() != 2) {
    throw Exception("Wrong number of parameters for udp.receive");
  }

  // Evaluate the first parameter to get the value of the port 
  int port = evalint(expression[1]);
  // Throw exception if invalid port
  if (port < 0) {
    throw Exception(
        "Invalid port number received in first parameter of udp.send");
  }

  boost::asio::io_context io_service;
  std::string buf;
  try {
    // Initialise a socket
    boost::asio::ip::udp::socket socket(io_service);
    auto e = boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port);

    // Open the socket, set the option and  bind it to the local endpoint
    socket.open(boost::asio::ip::udp::v4());
    socket.set_option(boost::asio::ip::udp::socket::reuse_address(true));
    socket.bind(e);

    // Peek to see if socket recceived data from a sender_endpoint 
    boost::asio::ip::udp::endpoint sender_endpoint;
    boost::system::error_code ec;
    socket.receive_from(boost::asio::buffer(buf), sender_endpoint,
                        decltype(socket)::message_peek, ec);

    if (ec) {
      throw Exception("udp.receive failed to read from the socket");
    }

    // Resize buffer to the number of readable bytes in the socket
    decltype(socket)::bytes_readable readable(true);
    socket.io_control(readable);
    auto length = readable.get();
    buf.resize(length);

    // Read the data
    socket.receive_from(boost::asio::buffer(buf.data(), buf.size()),
                        sender_endpoint, 0, ec);

    if (ec) {
      throw Exception("udp.receive failed to read from the socket");
    }

    // Close the socket
    socket.close();
  } catch (boost::system::system_error &e) {
    throw Exception("udp.receive failed to open socket on port " +
                    std::to_string(port) + " as " + e.what());
  }

  // Return message upto "\n"
  return buf.substr(0, buf.find('\n') + 1);
}
