#include "Server.h"
#include "QR.h"

Server::Server(std::string host, std::uint16_t port, std::string password)
{
  this->host = host;
  this->port = port;
  this->password = password;
  this->running = false;
  this->last_url = "https://janosch-kalich.com";
}

auto Server::server_handler()
{
  auto router = std::make_unique<restinio::router::express_router_t<>>();

  //INCOMING DATA HANDLING
  router->http_post("/receive", [this](auto req, auto){
    Json::Value json;

    std::stringstream ss;
    ss << req->body();

    ss >> json;
    std::string password = json["pwd"].asString();
    std::string data = json["data"].asString();

    OutputDebugStringA(password.c_str());
    OutputDebugStringA(data.c_str());

    if (password.compare(this->password) == 0)
    {
      if (data.length() < 1024)
        ShellExecute(0, 0, data.c_str(), 0, 0, SW_SHOW);
      else {
        last_url = data;
        ShellExecute(0, 0, std::format("http://127.0.0.1:{}/image", this->port).c_str(), 0, 0, SW_SHOW);
      }

      auto res = req->create_response();
      res.append_header("Server", "to-pc RESTionio server");
      res.append_header("Content-Type", "application/json");
      res.set_body("{\"status\": 200}");
      return res.done();
    }
    else
    {
      auto res = req->create_response(restinio::status_forbidden());
      return res.done();
    }
  });

  //IMAGE VIEW
  router->http_get("/image", [this](auto req, auto){
    auto res = req->create_response();
    res.set_body(std::format(
                             "<body style=\"background-color: black;\">"
                             "<div style=\"justify-content: center; display: flex;\">"
                             "<img src=\"{}\"/>"
                             "</div>"
                             "</body>"
                             , this->last_url));
    return res.done();
  });

  //QR-CODE VIEW
  router->http_get("/qr", [this](auto req, auto){
    if (req->remote_endpoint().address().to_string().compare(std::string("127.0.0.1")) == 0)
    {
      auto res = req->create_response();
      res.set_body(
                   "<head>"
                   "<style>"
                   "svg {"
                   "background-color: white;"
                   "height: 700px;"

                   "</style>"
                   "</head>"
                   "<body style=\"background-color: black;\">"
                   "<div style=\"justify-content: center; align-items: center; display: flex; max-height: 60%;\">"
                   +
                   gen_qr_code(this->port, this->password)
                   +
                   "</div>"
                   "</body>");

      return res.done();
    }

    return req->create_response(restinio::status_forbidden()).done();
  });

  //TEST ENDPOINT
  router->http_post("/test", [this](auto req, auto){
    Json::Value json;

    std::stringstream ss;
    ss << req->body();

    ss >> json;
    std::string password = json["pwd"].asString();
    std::string data = json["data"].asString();

    OutputDebugStringA(password.c_str());
    OutputDebugStringA(data.c_str());

    if (password.compare(this->password) == 0)
    {
      ShellExecute(0, 0, "https://janosch-kalich.com", 0, 0, SW_SHOW);
      return req->create_response().done();
    }
    else
    {
      auto res = req->create_response(restinio::status_forbidden());
      return res.done();
    }
  });

  return router;
}

int Server::listen()
{
  try {
    using namespace std::chrono;

     this->server = std::unique_ptr<restinio::http_server_t<traits_t>>(new restinio::http_server_t<traits_t> {
        restinio::own_io_context(),
        [this](auto & settings) {
          settings.port(this->port);
          settings.address(this->host);
          settings.request_handler(this->server_handler());
          settings.read_next_http_message_timelimit(10s);
          settings.write_http_response_timelimit(1s);
          settings.handle_request_timeout(1s);
        }
    });

    this->server_thread = std::unique_ptr<std::thread>(new std::thread{ [this] {
      restinio::run(
        restinio::on_thread_pool(4, restinio::skip_break_signal_handling(), *this->server)
      );
    }});

    this->running = true;
  }
  catch (const std::exception &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

void Server::shutdown()
{
  restinio::initiate_shutdown(*this->server);
  this->server_thread->join();
  this->running = false;
}