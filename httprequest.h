/*
 * Copyright © 2014 Altenius
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 */

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <boost/asio.hpp>
#include <sstream>
#include <boost/smart_ptr.hpp>

// TODO: Asynchronous calls

class HttpRequest
{
public:
HttpRequest();
~HttpRequest();

void get(std::string address, std::string path);
boost::system::error_code error;

std::string readLine();
std::string readAll();

boost::asio::streambuf request;
boost::asio::streambuf response;
std::ostream request_stream;
std::istream response_stream;
std::stringstream data;
private:
  boost::asio::io_service service;
  boost::asio::ip::tcp::resolver::iterator end_iterator;
  boost::asio::ip::tcp::resolver resolver;
  boost::asio::ip::tcp::socket socket;
};

#endif // HTTPREQUEST_H
