#!/usr/bin/env bash
# 主机：订阅 LED 状态主题。先 export BROKER_HOST=你的主机局域网IP
set -euo pipefail
: "${BROKER_HOST:?set BROKER_HOST to the broker LAN IP}"
exec /usr/bin/docker run --rm -it --network host eclipse-mosquitto:2 \
  mosquitto_sub -h "$BROKER_HOST" -t 'course/led/status' -v
