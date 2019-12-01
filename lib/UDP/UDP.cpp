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
  if (expression.size() < 4) {
    throw Exception("Wrong number of parameters for udp.send");
  }

  boost::asio::io_context io_service;
  boost::asio::ip::udp::socket socket(io_service);
  boost::system::error_code ec;

  boost::asio::ip::address ip_address =
      boost::asio::ip::address::from_string(expression[1], ec);

  if (ec) {
    throw Exception(
        "Invalid IP address received in first parameter of udp.send");
  }

  int port = evalint(expression[2]);
  if (port < 0) {
    throw Exception(
        "Invalid port number received in second papression[1rameter of udp.send");
  }

  boost::asio::ip::udp::endpoint ep(ip_address, port);
  
  try {
    socket.connect(ep);
  } catch (boost::system::system_error &e) {
    throw Exception("udp.send failed to connect socket to endpoint " +
                    expression[1] + ":" + std::to_string(port));
  }
  
  std::string message = expression[3];
  if (expression.size() == 4) {
    message = evalstring(message);
  } else {
  message = std::accumulate(expression.begin() + 4, expression.end(), message,
                            [](const std::string &m1, const std::string &m2) {
                              return m1 + " " + m2;
                            }) +
            "\n";
  }
  boost::system::error_code error;

  socket.send(boost::asio::buffer(message), 0, error);

  if (error) {
    throw Exception("udp.send failed to write to socket");
  }

  return "";
}

std::string UDP::rec(const std::vector<std::string> &expression) {
  if (expression.size() != 2) {
    throw Exception("Wrong number of parameters for udp.receive");
  }

  boost::asio::io_context io_service;
  int port = evalint(expression[1]);
  if (port < 0) {
    throw Exception(
        "Invalid port number received in first parameter of udp.send");
  }

  std::string buf;
  try {
    boost::asio::ip::udp::socket socket(
        io_service,
        boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), port));

    boost::asio::ip::udp::endpoint sender_endpoint;

    boost::system::error_code ec;
    socket.receive_from(boost::asio::buffer(buf), sender_endpoint,
                        decltype(socket)::message_peek, ec);

    if (ec) {
      throw Exception("udp.receive failed to read from the socket");
    }

    decltype(socket)::bytes_readable readable(true);
    socket.io_control(readable);
    auto length = readable.get();
    buf.resize(length);

    socket.receive_from(boost::asio::buffer(buf.data(), buf.size()),
                        sender_endpoint, 0, ec);

    if (ec) {
      throw Exception("udp.receive failed to read from the socket");
    }

  } catch (boost::system::system_error &e) {
    throw Exception("udp.receive failed to open socket on port " +
                    std::to_string(port));
  }
  return buf.substr(0, buf.find('\n') + 1);
}
