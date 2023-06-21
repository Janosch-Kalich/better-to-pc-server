#include "Communication.h"
#include "com.pb.h"
#include <chrono>
#include <zmq.h>
#include <zmq.hpp>

namespace Communication
{
  std::map<std::string, std::string> shared_variables;
  std::map<std::string, std::function<void()>> shared_functions;
  bool running = false;

  to_pc_server::ResponseMessage handle_msg(to_pc_server::RequestMessage msg);

  zmq::context_t ctx;
  zmq::socket_t sck(ctx, zmq::socket_type::rep);

  void listen()
  {
    zmq::socket_t stop_sck(ctx, ZMQ_PAIR);

    sck.bind("tcp://127.0.0.1:3334");
    sck.set(zmq::sockopt::linger, 0);
    stop_sck.set(zmq::sockopt::linger, 0);
    stop_sck.bind("inproc://stop");
    running = true;

    zmq::message_t rec;
    to_pc_server::RequestMessage req;
    to_pc_server::ResponseMessage res;
    zmq::message_t res_msg;

    zmq_pollitem_t items[] = {
      { sck, 0, ZMQ_POLLIN, 0},
      { stop_sck, 0, ZMQ_POLLIN, 0}
    };

    while (running)
    {
      int rc = 0;
      rc = zmq::poll(items, 2, std::chrono::milliseconds(100));
      if (rc > 0)
      {
        if (items[0].revents &ZMQ_POLLIN)
        {
          sck.recv(rec, zmq::recv_flags::none);
          std::string rec_str = rec.to_string();
          if (rec_str.compare("SHUTDOWN") == 0)
            break;
          req.ParseFromString(rec_str);
          res = handle_msg(req);
          std::string res_str;
          res.SerializeToString(&res_str);
          res_msg.empty();
          res_msg.rebuild(res_str.size());
          memcpy((void*) res_msg.data(), res_str.c_str(), res_str.size());
          sck.send(res_msg, zmq::send_flags::dontwait);
        }
        if (items[1].revents &ZMQ_POLLIN)
          break;
      }
    }

    sck.unbind("tcp://127.0.0.1:3334");
    sck.close();
    stop_sck.unbind("inproc://stop");
    stop_sck.close();
    ctx.shutdown();
    ctx.close();

    return;
  }

  void shutdown()
  {
    zmq::socket_t stop_sck(ctx, ZMQ_PAIR);
    stop_sck.connect("inproc://stop");
    stop_sck.send(zmq::str_buffer("STOP"));
    stop_sck.unbind("inproc://stop");
    stop_sck.close();
  }

  to_pc_server::ResponseMessage handle_msg(to_pc_server::RequestMessage msg)
  {
    to_pc_server::ResponseMessage res;
    res.set_status(to_pc_server::Status::SUCCESS);

    switch (msg.type())
    {
      case (to_pc_server::MessageType::GET):
        if (shared_functions.contains(msg.get_data().var() + "_get"))
          shared_functions[msg.get_data().var() + "_get"]();
        res.set_data(shared_variables[msg.get_data().var()]);
        break;
      case (to_pc_server::MessageType::SET):
        shared_variables[msg.set_data().var()] = msg.set_data().data();
        if (shared_functions.contains(msg.set_data().var() + "_update"))
          shared_functions[msg.set_data().var() + "_update"]();
        break;
    case (to_pc_server::MessageType::DO):
        shared_functions[msg.do_data().function_id()]();
        break;
      default:
        res.set_status(to_pc_server::Status::ERROR);
        res.set_data("No such method");
        break;
    }

    return res;
  }

  void add_var(std::string id, std::string value, std::function<void()> update_function, std::function<void()> get_function)
  {
    shared_variables.insert(std::pair<std::string, std::string>(id, value));
    if (update_function != nullptr)
      shared_functions.insert(std::pair<std::string, std::function<void()>>(id + "_update", update_function));
    if (get_function != nullptr)
      shared_functions.insert(std::pair<std::string, std::function<void()>>(id + "_get", get_function));
  }

  void delete_var(std::string id)
  {
    shared_variables.erase(id);
    if (shared_functions.contains(id))
      shared_functions.erase(id);
  }

  void add_function(std::string id, std::function<void()> function)
  {
    shared_functions.insert(std::pair<std::string, std::function<void()>>(id, function));
  }

  void delete_function(std::string id)
  {
    shared_functions.erase(id);
  }
}
