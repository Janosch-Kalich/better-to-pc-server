#include "Server.h"

#include <utility>
#include "QR.h"
#include "FileBatch.h"
#include <regex>
#include <boost/algorithm/string/replace.hpp>
#include <restinio/helpers/file_upload.hpp>
#include "Trys.h"
#include "Resources.h"

Server::Server(std::string host, std::uint16_t port, std::string password, fs::path templates_path)
{
  this->templates.templates_path = std::move(templates_path);
  this->host = std::move(host);
  this->port = port;
  this->password = std::move(password);
  this->running = false;
}

Json::Value parse_req(std::shared_ptr<restinio::generic_request_t<restinio::no_extra_data_factory_t::data_t>> req)
{
  Json::Value json;

  std::stringstream ss;
  ss << req->body();

  ss >> json;

  return json;
}

auto Server::server_handler()
{
  auto router = std::make_unique<restinio::router::express_router_t<>>();

  //RECEIVE LINK
  router->http_post("/receive-link", [this](auto req, auto){
    restinio::http_status_line_t status = this->auth(req);
    if (status.status_code().raw_code() != restinio::status_code::ok.raw_code())
      return req->create_response(status).done();

    Json::Value json = parse_req(req);

    std::string data = json["data"].asString();

    last_link = data;
    ShellExecute(0, "open", std::format("http://127.0.0.1:{}/link", this->port).c_str(), 0, 0, SW_SHOW);

    auto res = req->create_response();
    res.append_header("Server", "to-pc RESTionio server");
    res.append_header("Content-Type", "application/json");
    res.set_body("{\"status\": 200}");
    return res.done();
  });

  //RECEIVE IMAGE
  router->http_post("/receive-image", [this](auto req, auto){
    restinio::http_status_line_t status = this->auth(req);
    if (status.status_code().raw_code() != restinio::status_code::ok.raw_code())
      return req->create_response(status).done();

    Json::Value json = parse_req(req);

    std::string data = json["data"].asString();

    last_image = data;
    ShellExecute(0, "open", std::format("http://127.0.0.1:{}/image", this->port).c_str(), 0, 0, SW_SHOW);

    auto res = req->create_response();
    res.append_header("Server", "to-pc RESTionio server");
    res.append_header("Content-Type", "application/json");
    res.set_body("{\"status\": 200}");
    return res.done();
  });

  //RECEIVE PLAIN
  router->http_post("/receive-plain", [this](auto req, auto){
    restinio::http_status_line_t status = this->auth(req);
    if (status.status_code().raw_code() != restinio::status_code::ok.raw_code())
      return req->create_response(status).done();

    Json::Value json = parse_req(req);

    std::string data = json["data"].asString();

    last_plain = data;
    ShellExecute(0, "open", std::format("http://127.0.0.1:{}/plain", this->port).c_str(), 0, 0, SW_SHOW);

    auto res = req->create_response();
    res.append_header("Server", "to-pc RESTionio server");
    res.append_header("Content-Type", "application/json");
    res.set_body("{\"status\": 200}");
    return res.done();
  });

  //LINK REDIRECT
  router->http_get("/link", [this](auto req, auto){
    if (this->last_link.empty())
      return req->create_response(restinio::status_not_found()).done();

    auto res = req->create_response();
    std::string res_body = "<head>"
                           "<meta http-equiv=\"Refresh\" content=\"0; URL={}\" />"
                           "</head>";
    boost::replace_all(res_body, "{}", this->last_link);
    res.set_body(res_body);
    return res.done();
  });

  //IMAGE VIEW
  router->http_get("/image", [this](auto req, auto){
    if (this->last_image.empty())
      return req->create_response(restinio::status_not_found()).done();

    auto res = req->create_response();
    std::string res_body = this->templates.image;
    boost::replace_all(res_body, "{}", this->last_image);
    res.set_body(res_body);
    return res.done();
  });

  //PLAIN VIEW
  router->http_get("/plain", [this](auto req, auto){
    if (this->last_plain.empty())
      return req->create_response(restinio::status_not_found()).done();

    auto res = req->create_response();
    std::string res_body = this->templates.plain;
    boost::replace_all(res_body, "{}", this->last_plain);
    res.set_body(res_body);
    return res.done();
  });

  //QR-CODE VIEW
  router->http_get("/qr", [this](auto req, auto){
    if (req->remote_endpoint().address().to_string().compare(std::string("127.0.0.1")) == 0)
    {
      auto res = req->create_response();
      std::string res_body = this->templates.qr;
      boost::replace_all(res_body, "{}", gen_qr_code(this->port, this->password));
      res.set_body(res_body);

      return res.done();
    }

    return req->create_response(restinio::status_unauthorized()).done();
  });

  //QR-CODE RAW
  router->http_get("/qr-raw", [this](auto req, auto){
    if (req->remote_endpoint().address().to_string().compare(std::string("127.0.0.1")) == 0)
    {
      auto res = req->create_response();
      res.set_body(gen_qr_code(this->port, this->password));
      res.append_header("Content-type", "image/svg+xml");

      return res.done();
    }

    return req->create_response(restinio::status_unauthorized()).done();
  });

  router->http_post("/upload", [this](auto req, auto){
    restinio::http_status_line_t status = this->auth(req);
    if (status.status_code().raw_code() != restinio::status_code::ok.raw_code())
      return req->create_response(status).done();

    fs::path batch_id = fs::unique_path();
    FileBatch batch(batch_id);

    const auto res = restinio::file_upload::enumerate_parts_with_files(*req, [&batch, batch_id](const restinio::file_upload::part_description_t &part) {

      if (part.name.starts_with("file"))
      {
        if (part.filename)
        {
          batch.files.push_back(TempFile(batch_id, part.filename, part.body));
        }
      }

      return restinio::file_upload::handling_result_t ::continue_enumeration;
    });

    batch.default_action();
    this->batches.push_back(batch);

    return req->create_response().done();
  });

  //Verify
  router->http_post("/verify", [this](auto req, auto){
    restinio::http_status_line_t status = this->auth(req);
    if (status.status_code().raw_code() != restinio::status_code::ok.raw_code())
      return req->create_response(status).done();

    Json::Value json = parse_req(req);

    show_verify_toast(json["device_name"].asString());

    return req->create_response().done();
  });

  //TEST ENDPOINT
  router->http_post("/test", [this](auto req, auto){
    restinio::http_status_line_t status = this->auth(req);
    if (status.status_code().raw_code() != restinio::status_code::ok.raw_code())
      return req->create_response(status).done();

    ShellExecute(0, "open", "https://janosch-kalich.com", 0, 0, SW_SHOW);
    return req->create_response().done();
  });

  return router;
}

int Server::listen()
{
  std::ifstream image_template_file;
  std::ifstream qr_template_file;
  std::ifstream plain_template_file;

  image_template_file.open(Resources::templates().append("image").append("index.html").c_str());
  qr_template_file.open(Resources::templates().append("qr").append("index.html").c_str());
  plain_template_file.open(Resources::templates().append("plain").append("index.html").c_str());

  if (image_template_file.is_open())
  {
    std::string line;
    while (std::getline(image_template_file, line))
    {
      this->templates.image += line;
    }

    image_template_file.close();
  }
  else
    this->templates.image = "{}";

  if (qr_template_file.is_open())
  {
    std::string line;
    while (std::getline(qr_template_file, line))
    {
      this->templates.qr += line;
    }

    qr_template_file.close();
  }
  else
    this->templates.qr = "{}";

  if (plain_template_file.is_open())
  {
    std::string line;
    while (std::getline(plain_template_file, line))
    {
      this->templates.plain += line;
    }

    plain_template_file.close();
  }
  else
    this->templates.plain = "{}";

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

restinio::http_status_line_t Server::auth(std::shared_ptr<restinio::generic_request_t<restinio::no_extra_data_factory_t::data_t>> req)
{
  if (Trys::locked)
    return restinio::status_forbidden();

  if (!req->header().has_field("X-Password"))
  {
    Trys::fail();
    return restinio::status_unauthorized();
  }

  if (req->header().get_field("X-Password").compare(this->password) != 0)
  {
    Trys::fail();
    return restinio::status_unauthorized();
  }

  Trys::reset();

  return restinio::status_ok();
}
