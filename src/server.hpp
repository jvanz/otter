#ifndef __SERVER_HPP__
#define __SERVER_HPP__

#include <list>
#include <thread>
#include <unordered_map>

#include <boost/asio.hpp>
#include "common/message_queue.hpp"

#include "session.hpp"
#include "topic.hpp"

using boost::asio::ip::tcp;

class Session;

/**
 * Class to store all the open connection and wait for more connections
 */
class Server {
 public:
  Server(boost::asio::io_context& io_context, short port);

  /**
   * Member function used to process the messages from the clients
   */
  void ProcessPendingMessage(Message&& message);

 private:
  /**
   * Register the callback for new connection requests
   */
  void Accept();
  /**
   * Add the given topic in the topic list managed by the server
   * This is a thread safe function
   */
  void AddTopic(Topic&& topic);
  /**
   * Search for the topic previous created
   */
  Topic* FindTopic(std::string topicName);

  tcp::acceptor _acceptor;
  std::list<std::unique_ptr<Session>> _sessions;
  std::mutex _topicsMtx;
  std::unordered_map<std::string, std::shared_ptr<Topic>> _topics;
};

#endif
