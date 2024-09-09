#!/bin/bash

nginx

# run userver app
./build_release/content_service --config configs/static_config.yaml --config_vars configs/config_vars.docker.yaml

