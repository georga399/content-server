#include "upload_post.hpp"

#include <fmt/format.h>

#include <userver/fs/blocking/write.hpp>
#include <userver/http/common_headers.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/utils/text.hpp>

namespace content_service::handlers::upload::post {

namespace {

class Upload final : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-upload-post";
  Upload(const userver::components::ComponentConfig& config,
         const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context) {
    upload_path = "/uploads";
  }

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    const auto content_type = userver::http::ContentType(
        request.GetHeader(userver::http::headers::kContentType));
    if (content_type != "multipart/form-data") {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return "Expected 'multipart/form-data' content type";
    }
    if (!request.HasFormDataArg("image")) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return "Expected a file with key 'image'";
    }
    const auto& image = request.GetFormDataArg("image");
    if (!image.filename.has_value()) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return "Expected a file";
    }
    userver::fs::blocking::RewriteFileContents(
        "/uploads/" + image.filename.value(), image.value);
    userver::fs::blocking::Chmod("/uploads/" + image.filename.value(),
                                 boost::filesystem::perms::others_read);
    return request.GetHost() + "/uploads/" + image.filename.value();
  }

 private:
  std::string upload_path;
};

}  // namespace

void AppendUpload(userver::components::ComponentList& component_list) {
  component_list.Append<Upload>();
}

}  // namespace content_service::handlers::upload::post
