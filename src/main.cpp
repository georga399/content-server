#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "auth/auth_bearer.hpp"
#include "handlers/upload/upload_del.hpp"
#include "handlers/upload/upload_post.hpp"

int main(int argc, char* argv[]) {
  userver::server::handlers::auth::RegisterAuthCheckerFactory(
      "bearer", std::make_unique<content_service::auth::CheckerFactory>());
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()
                            .Append<userver::clients::dns::Component>()
                            .Append<userver::server::handlers::TestsControl>();

  content_service::handlers::upload::post::AppendUpload(component_list);
  content_service::handlers::upload::del::AppendUpload(component_list);

  return userver::utils::DaemonMain(argc, argv, component_list);
}