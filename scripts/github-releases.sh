#!/bin/bash

set +e

version=$(git describe --abbrev=0 --tags)

which github-release || echo 'please install the tool github-releases'

github-release info \
    --user remijouannet \
    --repo vpn_proxy \
    --tag $version

if [ $? != 0 ]
then
    github-release release \
        --user remijouannet \
        --repo vpn_proxy \
        --tag $version \
        --pre-release \
        --name "$version-hyper-alpha-yolo-experimental" \
        --description "risks of explosion" || echo "failed to create release for $version"
fi

cd pkg/
rm -rf *.tar.gz
tar cvfz vpn_proxy.tar.gz vpn_proxy
echo "upload vpn_proxy.apk"
github-release upload \
    --user remijouannet \
    --name "$version-hyper-alpha-yolo-experimental" \
    --repo vpn_proxy \
    --file "vpn_proxy.tar.gz" \
    --replace \
    --tag $version || echo "failed to upload vpn_proxy.tar.gz"
