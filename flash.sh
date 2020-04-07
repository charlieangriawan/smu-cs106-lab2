challenge=$1
module=$2
microbitPath=$3

if [ "$challenge" = "challenge1" ]; then
    if [ "$module" != "gateway" ] && [ "$module" != "wearable" ] && [ "$module" != "lightsensor" ]; then
        echo "FlashError: invalid module"
        exit 1
    fi
elif [ "$challenge" = "challenge2" ]; then
    if [ "$module" != "motionsensor" ]; then
        echo "FlashError: invalid module"
        exit 1
    fi
else 
    echo "FlashError: invalid challenge"
    exit 1
fi

cd $challenge
jq ".bin=\"./source/${module}\"" settings.json > module.json
yt target bbc-microbit-classic-gcc
yt build
cp build/bbc-microbit-classic-gcc/source/$module/cs106-lab2-$challenge-combined.hex $microbitPath
