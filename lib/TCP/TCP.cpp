/**
 * Stephen Hunter Barbella, Baibhav Vatsa
 * GitHub: hman523, baibhavatsa
 * Email: shbarbella@gmail.com, baibhavvatsa@gmail.com
 * Licence: MIT
 * File: TCP.cpp
 */

#include "TCP.h"
#include "../../include/Interpreter.h"
#include <numeric>

// Common IO context shared by the functions
boost::asio::io_context io_service;

// Initialise the socket 
TCP::TCP() : functions({"tcp.send", "tcp.receive"}), socket(io_service) {}

std::set<std::string> TCP::getFunctions() { return functions; }

std::string TCP::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "tcp.send") {
    return send(words);
  } else if (words[0] == "tcp.receive") {
    return rec(words);
  }
  throw Exception("Fatal implementation error in TCP library in TCP.cpp");
}

int TCP::evalint(const std::string &val) {
  return Interpreter::Instance()->strToInt(val);
}

std::string TCP::evalstring(const std::string &val) {
  if (Interpreter::Instance()->isParens(val)) {
    return Interpreter::Instance()->eval(val);
  } else if (Interpreter::Instance()->isString(val)) {
    return Interpreter::Instance()->removequotes(val);
  } else {
    return Interpreter::Instance()->strtostr(val);
  }
}

std::string TCP::send(const std::vector<std::string> &expression) {
  // Ensure that ip, port, and message is present
  if (expression.size() < 4) {
    throw Exception("Wrong number of parameters for tcp.send");
  }

  // Get the IP Address from the given string
  boost::system::error_code ec;
  boost::asio::ip::address ip_address =
      boost::asio::ip::address::from_string(evalstring(expression[1]), ec);

  // Throw exception if the given string is not valid
  if (ec) {
    throw Exception("Invalid IP Address in first parameter of tcp.send");
  }

  // Get the port number from the parameter
  int port = evalint(expression[2]);
  
  // If the port is invalid, throw exception
  if (port < 0) {
    throw Exception(
        "Invalid port number received in second parameter of tcp.send");
  }

  // Generate remote endpoint bound to the receiver's ip address and receiving port
  boost::asio::ip::tcp::endpoint ep(ip_address, port);
  // Generate local endpoint bound to the port passed in parameter
  boost::asio::ip::tcp::endpoint loc_ep(boost::asio::ip::tcp::v4(), port);

  // If socket is already open, just set the option, else open it, set the option and 
  // bind it to the local endpoint
  if (!socket.is_open()) {
    socket.open(boost::asio::ip::tcp::v4());
    socket.set_option(boost::asio::ip::tcp::socket::reuse_address(true));
    socket.bind(loc_ep);
  } else {
    socket.set_option(boost::asio::ip::tcp::socket::reuse_address(true));
  }

  // Connect socket to the remote endpoint
  try {
    socket.connect(ep);
  } catch (boost::system::system_error &e) {
    throw Exception("tcp.send failed to connect socket to endpoint " +
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

  // Transfer all from message to the TCP connection to be sent to the remote endpoint
  boost::system::error_code error;
  boost::asio::write(socket, boost::asio::buffer(message),
                     boost::asio::transfer_all(), error);
  if (error) {
    throw Exception("tcp.send failed to write to socket");
  }

  return "";
}

std::string TCP::rec(const std::vector<std::string> &expression) {
  // Ensure that port is present
  if (expression.size() != 2) {
    throw Exception("Wrong number of parameters for tcp.receive");
  }

  // Evaluate the first parameter to get the value of the port 
  int port = evalint(expression[1]);
  // Throw exception if invalid port
  if (port < 0) {
    throw Exception(
        "Invalid port number received in first parameter of tcp.send");
  }

  // Create a local endpoint with port "port" for the socket to be bound to
  boost::asio::ip::tcp::endpoint ep(boost::asio::ip::tcp::v4(), port);
  boost::asio::streambuf buf;

  try {
    // Initialise a TCP acceptor
    boost::asio::ip::tcp::acceptor acceptor(io_service, ep);
    // Set the socket options such that they allow connections to it
    int one = 1;
    setsockopt(acceptor.native_handle(), SOL_SOCKET, SO_REUSEPORT, &one, sizeof(one));
    setsockopt(acceptor.native_handle(), SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one));
    // Wait till the data is available to read
    socket.wait(boost::asio::ip::tcp::socket::wait_read);
    // Read from the socket to know if it is still alive
    boost::system::error_code error2;
    boost::asio::read_until(socket, buf, '\n', error2);
    if (error2) {
      // If it is not, wait for a connection to accept and read data from it
      acceptor.accept(socket);
      boost::system::error_code error;
      boost::asio::read_until(socket, buf, '\n', error);
      if (error) {
        throw Exception("tcp.receive failed to read");
      }
    }
  } catch (boost::system::system_error &e) {
    throw Exception("tcp.receive failed to accept connection on port " +
                    std::to_string(port) + " as " + e.what());
  }
  // Close the socket
  socket.close();
  return boost::asio::buffer_cast<const char *>(buf.data());
}
