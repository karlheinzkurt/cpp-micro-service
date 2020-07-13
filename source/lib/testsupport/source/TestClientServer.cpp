
#include "../include/TestClientServer.h"
#include "../include/SocketUtility.h"

#include "pplx/threadpool.h"

/** One thread for client and one for server because
    they share the thread pool.
    Just 1 would be not enough because client waits
    for response and server cannot pick a thread.
  */
static bool initialized = []() {
  crossplat::threadpool::initialize_with_threads(2);
  return true;
}();

namespace TestSupport
{
  TestClientServer::TestClientServer()
      : m_baseUri(web::uri_builder("http://localhost")
                      .set_port(TestSupport::getFreeTCPPort("127.0.0.1"))
                      .to_uri())
      , m_listener()
      , m_client(m_baseUri)
  {}

  web::http::http_response
  TestClientServer::request(web::uri requestUri)
  {
    web::http::http_request request(web::http::methods::GET);
    request.set_request_uri(requestUri);
    auto response = m_client.request(request);
    response.wait();
    return response.get();
  }
} // namespace TestSupport