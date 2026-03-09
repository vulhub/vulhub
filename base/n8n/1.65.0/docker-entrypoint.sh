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

    echo "[*] Creating workflow..."
    RESP=$(curl -sf "$BASE_URL/rest/workflows" -X POST \
        -H "Content-Type: application/json" \
        -b /tmp/cookies.txt \
        -d '{
            "name": "Document Submission Portal",
            "nodes": [
                {
                    "parameters": {
                        "formTitle": "Document Submission",
                        "formDescription": "Please fill out the form below and upload your document.",
                        "responseMode": "responseNode",
                        "formFields": {
                            "values": [
                                {
                                    "fieldLabel": "Full Name",
                                    "fieldType": "text",
                                    "requiredField": true,
                                    "placeholder": "John Doe"
                                },
                                {
                                    "fieldLabel": "Email",
                                    "fieldType": "email",
                                    "requiredField": true,
                                    "placeholder": "john@example.com"
                                },
                                {
                                    "fieldLabel": "document",
                                    "fieldType": "file",
                                    "requiredField": true,
                                    "acceptFileTypes": ".pdf,.doc,.docx,.txt,.png,.jpg",
                                    "multipleFiles": false
                                }
                            ]
                        }
                    },
                    "id": "d4a7c1e2-8f3b-4a5c-9d6e-7f8a0b1c2d3e",
                    "name": "Form Trigger",
                    "type": "n8n-nodes-base.formTrigger",
                    "typeVersion": 2.2,
                    "position": [260, 340],
                    "webhookId": "vulnerable-form"
                },
                {
                    "parameters": {
                        "values": {
                            "string": [
                                {"name": "status", "value": "received"},
                                {"name": "processedAt", "value": "={{ new Date().toISOString() }}"}
                            ]
                        },
                        "options": {}
                    },
                    "id": "b5e9f2a3-6c4d-4e7f-8a1b-9c0d2e3f4a5b",
                    "name": "Log Submission",
                    "type": "n8n-nodes-base.set",
                    "typeVersion": 2,
                    "position": [520, 340]
                },
                {
                    "parameters": {
                        "respondWith": "binary",
                        "inputDataFieldName": "document"
                    },
                    "id": "c6f0a3b4-7d5e-4f8a-9b2c-0d1e3f4a5b6c",
                    "name": "Return Document",
                    "type": "n8n-nodes-base.respondToWebhook",
                    "typeVersion": 1.1,
                    "position": [780, 340]
                }
            ],
            "connections": {
                "Form Trigger": {
                    "main": [[{"node": "Log Submission", "type": "main", "index": 0}]]
                },
                "Log Submission": {
                    "main": [[{"node": "Return Document", "type": "main", "index": 0}]]
                }
            },
            "active": false,
            "settings": {"executionOrder": "v1"},
            "pinData": {}
        }')

    ID=$(echo "$RESP" | grep -o '"id":"[^"]*"' | head -1 | cut -d'"' -f4)
    echo "[+] Workflow ID: $ID"

    echo "[*] Activating workflow..."
    curl -sf "$BASE_URL/rest/workflows/$ID" -X PATCH \
        -H "Content-Type: application/json" \
        -b /tmp/cookies.txt \
        -d '{"active": true}'

    rm -f /tmp/cookies.txt
    echo "[+] Workflow activated"
fi

echo "[+] Lab ready! Form at: http://localhost:5678/form/vulnerable-form"
wait $N8N_PID
