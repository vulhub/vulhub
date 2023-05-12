#!/bin/bash

set -ex

cleanupImages() {
    docker images | grep 'auto-' | awk '{print $3}' | xargs docker rmi 
}

cleanupImages || echo "some images is not cleanup"

echo "finish images cleanup"

exit 0
