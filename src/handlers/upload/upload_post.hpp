#pragma once

#include <string>
#include <string_view>

#include <userver/components/component_list.hpp>

namespace content_service::handlers::upload::post {

void AppendUpload(userver::components::ComponentList& component_list);

}  // namespace content_service::handlers::upload::post
