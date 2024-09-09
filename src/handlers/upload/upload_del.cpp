#include "upload_del.hpp"

#include <fmt/format.h>

#include <userver/fs/blocking/write.hpp>
#include <userver/http/common_headers.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utils/text.hpp>

namespace content_service::handlers::upload::del {

namespace {

class Upload final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-upload-del";
  Upload(const userver::components::ComponentConfig& config,
         const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context) {
    upload_path = "/uploads";
  }

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto filename = request.GetPathArg("slug");
    if (!filename.empty()) {
      userver::fs::blocking::RemoveSingleFile("/uploads/" + filename);
    }
    return "File was deleted";
  }

 private:
  std::string upload_path;
};

}  // namespace

void AppendUpload(userver::components::ComponentList& component_list) {
  component_list.Append<Upload>();
}

}  // namespace content_service::handlers::upload::del
