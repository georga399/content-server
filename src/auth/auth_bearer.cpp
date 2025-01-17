#include "auth_bearer.hpp"
#include "../utils/jwt.hpp"

#include <algorithm>

#include <userver/http/common_headers.hpp>

namespace content_service::auth {

class AuthCheckerBearer final
    : public userver::server::handlers::auth::AuthCheckerBase {
 public:
  using AuthCheckResult = userver::server::handlers::auth::AuthCheckResult;

  AuthCheckerBearer(
      const userver::components::ComponentContext& component_context,
      bool is_required)
      : is_required_(is_required) {}

  [[nodiscard]] AuthCheckResult CheckAuth(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext& request_context) const override;

  [[nodiscard]] bool SupportsUserAuth() const noexcept override { return true; }

 private:
  bool is_required_;
};

AuthCheckerBearer::AuthCheckResult AuthCheckerBearer::CheckAuth(
    const userver::server::http::HttpRequest& request,
    userver::server::request::RequestContext& request_context) const {
  const auto& auth_value =
      request.GetHeader(userver::http::headers::kAuthorization);
  if (auth_value.empty()) {
    if (!is_required_) {
      request_context.SetData("id", std::optional<std::string>(std::nullopt));
      return {};
    }
    return AuthCheckResult{
        AuthCheckResult::Status::kTokenNotFound,
        {},
        "Empty 'Authorization' header",
        userver::server::handlers::HandlerErrorCode::kUnauthorized};
  }

  const auto bearer_sep_pos = auth_value.find(' ');
  if (bearer_sep_pos == std::string::npos ||
      std::string_view{auth_value.data(), bearer_sep_pos} != "Token") {
    return AuthCheckResult{
        AuthCheckResult::Status::kTokenNotFound,
        {},
        "'Authorization' header should have 'Bearer some-token' format",
        userver::server::handlers::HandlerErrorCode::kUnauthorized};
  }
  std::string_view token{auth_value.data() + bearer_sep_pos + 1};
  jwt::jwt_payload payload;
  try {
    payload = utils::jwt::DecodeJWT(token);
  } catch (...) {
    return AuthCheckResult{
        AuthCheckResult::Status::kTokenNotFound,
        {},
        "Token verification error",
        userver::server::handlers::HandlerErrorCode::kUnauthorized};
  }
  auto id = payload.get_claim_value<std::string>("id");

  request_context.SetData("id", std::optional<std::string>(id));
  return {};
}

userver::server::handlers::auth::AuthCheckerBasePtr CheckerFactory::operator()(
    const userver::components::ComponentContext& context,
    const userver::server::handlers::auth::HandlerAuthConfig& auth_config,
    const userver::server::handlers::auth::AuthCheckerSettings&) const {
  auto is_required = auth_config["required"].As<bool>(false);
  return std::make_shared<AuthCheckerBearer>(context, is_required);
}

}  // namespace content_service::auth
