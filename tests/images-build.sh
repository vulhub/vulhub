#!/bin/bash

set -ex

runTest() {
    local image_path=$1
    local image_name=$2
    if [[ ! -d $image_path ]]; then
        echo "error message: image path \"$image_path\" not exists"
        exit 1
    fi

    tag=$(date +%s)
    docker build "$image_path" -t "$image_name:$tag" 
}

for path in "$@"; do
    image_path=${path%%/}
    image_name=${image_path//[\/\-.]/_}
    runTest "$image_path" "$image_name"
    # echo "Image Name $image_name"
done

