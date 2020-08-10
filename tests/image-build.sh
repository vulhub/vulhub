#!/bin/bash

image_path="${1%%/}"
image_name="$2"

if [[ ! "$image_name" =~ ":" ]]; then 
    image_name=${image_name}:$(date +%s)
fi

echo "Preparing test image $image_name"

cd "$image_path"
docker build -t "$image_name" .
