#!/usr/bin/env bash
# 主机：向命令主题发 on/off。先 export BROKER_HOST=你的主机局域网IP
# 用法：./host-pub-cmd.sh on|off
set -euo pipefail
: "${BROKER_HOST:?set BROKER_HOST to the broker LAN IP}"
msg="${1:-}"
if [[ "$msg" != "on" && "$msg" != "off" ]]; then
  echo "usage: $0 on|off" >&2
  exit 1
fi
exec /usr/bin/docker run --rm --network host eclipse-mosquitto:2 \
  mosquitto_pub -h "$BROKER_HOST" -t 'course/led/cmd' -m "$msg"
