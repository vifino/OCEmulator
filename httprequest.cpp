/*
 * Copyright © 2014 Altenius
 * This work is free. You can redistribute it and/or modify it under the
 * terms of the Do What The Fuck You Want To Public License, Version 2,
 * as published by Sam Hocevar. See http://www.wtfpl.net/ for more details.
 */

#include "httprequest.h"
#include <iostream>

HttpRequest::HttpRequest() : resolver(service), socket(service), request_stream(&request), response_stream(&response)
{

}

HttpRequest::~HttpRequest()
{

}

void HttpRequest::get(std::string address, std::string path)
{
  boost::asio::ip::tcp::resolver::query query(address, "http");
  boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);
  boost::asio::connect(socket, iterator, error);
  
  error = boost::asio::error::host_not_found;
  while (error && iterator != end_iterator)
  {
    socket.close();
    socket.connect(*iterator++, error);
  }
  
  if (error)
      throw boost::system::system_error(error);
  
  request_stream.clear();
  request.consume(request.size());
  response_stream.clear();
  response.consume(response.size());
  request_stream << "GET " << path << " HTTP/1.0\r\n";
  request_stream << "Host: " << address << "\r\n";
  request_stream << "Accept: " << "*/*\r\n";
  request_stream << "Connection: close\r\n\r\n";
  
  boost::asio::write(socket, request);
  
  boost::asio::read_until(socket, response, "\r\n");
  
  std::string http_version;
  response_stream >> http_version;
  unsigned int status_code;
  response_stream >> status_code;
  std::string status_message;
  std::getline(response_stream, status_message);
  
  if (!response_stream || http_version.substr(0, 5) != "HTTP/")
  {
    return;
  }
  
  boost::asio::read_until(socket, response, "\r\n\r\n");
  
  std::string header;
  while (std::getline(response_stream, header) && header != "\r")
  {
  }
}

std::string HttpRequest::readLine()
{
  boost::asio::read_until(socket, response, "\n", error);
  std::string res;
  std::getline(response_stream, res);
  
  if (error)
    throw boost::system::system_error(error);
  
  return res;
}

std::string HttpRequest::readAll()
{
  std::string data;
  
  while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
  {
    int size = response.size();
    char dat[size];
    response_stream.read(dat, size);
    data.append(dat, size);
  }
  
  if (error != boost::asio::error::eof)
    throw boost::system::system_error(error);
  
  return data;
}