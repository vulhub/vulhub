#!/bin/bash
set -e

BASE_URL="http://127.0.0.1:5678"

n8n start &
N8N_PID=$!

echo "[*] Waiting for n8n to start..."
until curl -sf "$BASE_URL/rest/settings" | grep -q versionCli; do
    sleep 2
done
echo "[+] n8n is ready"

# Check if admin already exists
if curl -sf "$BASE_URL/rest/login" -X POST \
    -H "Content-Type: application/json" \
    -d '{"email":"admin@vulhub.org","password":"Vulhub123"}' | grep -q id; then
    echo "[+] Admin user already exists"
else
    echo "[*] Creating admin user..."
    curl -sf "$BASE_URL/rest/owner/setup" -X POST \
        -H "Content-Type: application/json" \
        -d '{"email":"admin@vulhub.org","firstName":"Admin","lastName":"Vulhub","password":"Vulhub123"}'

    echo "[*] Logging in..."
    curl -sf "$BASE_URL/rest/login" -X POST \
        -H "Content-Type: application/json" \
        -c /tmp/cookies.txt \
        -d '{"email":"admin@vulhub.org","password":"Vulhub123"}'

    echo "[*] Creating vulnerable workflow..."
    RESP=$(curl -sf "$BASE_URL/rest/workflows" -X POST \
        -H "Content-Type: application/json" \
        -b /tmp/cookies.txt \
        -d '{
            "name": "Vulnerable Form",
            "nodes": [
                {
                    "parameters": {
                        "formTitle": "Upload",
                        "responseMode": "responseNode",
                        "formFields": {"values": [{"fieldLabel": "document", "fieldType": "file", "requiredField": false}]}
                    },
                    "id": "trigger",
                    "name": "Form Trigger",
                    "type": "n8n-nodes-base.formTrigger",
                    "typeVersion": 2.2,
                    "position": [0, 0],
                    "webhookId": "vulnerable-form"
                },
                {
                    "parameters": {"respondWith": "binary", "inputDataFieldName": "document"},
                    "id": "respond",
                    "name": "Respond",
                    "type": "n8n-nodes-base.respondToWebhook",
                    "typeVersion": 1.1,
                    "position": [300, 0]
                }
            ],
            "connections": {"Form Trigger": {"main": [[{"node": "Respond", "type": "main", "index": 0}]]}},
            "active": false,
            "settings": {"executionOrder": "v1"}
        }')

    ID=$(echo "$RESP" | grep -o '"id":"[^"]*"' | head -1 | cut -d'"' -f4)
    echo "[+] Workflow ID: $ID"

    echo "[*] Activating workflow..."
    curl -sf "$BASE_URL/rest/workflows/$ID" -X PATCH \
        -H "Content-Type: application/json" \
        -b /tmp/cookies.txt \
        -d '{"active": true}'

    rm -f /tmp/cookies.txt
    echo "[+] Vulnerable workflow activated"
fi

echo "[+] Lab ready! Form at: http://localhost:5678/form/vulnerable-form"
wait $N8N_PID
